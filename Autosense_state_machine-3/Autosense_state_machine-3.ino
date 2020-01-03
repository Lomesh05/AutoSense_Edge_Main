#include <ArduinoJson.h>
#include <TinyGPS++.h>
#include <SPI.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Keypad.h>
#include <Keypad_I2C.h>
#include <EEPROM_Rotate.h> 
#include "config.h"
#include "oled_bitmaps.h"




#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET LED_BUILTIN // Reset pin # (or -1 if sharing Arduino reset pin)
#define DATA_OFFSET 10

enum state_names{ REG_SCREEN, DEVICE_ID, MQTT, DRIVE_ID, RE_REG_SCREEN, CLEAR, Start_Trip_Call, FLASH_E};

const byte ROWS = 4; //four rows of Keypad
const byte COLS = 4; //three columns of Keypad

// Define the Keyboard map
char keys[ROWS][COLS] = {
  {'D', 'C', 'B', 'A'},
  {'#', '9', '6', '3'},
  {'0', '8', '5', '2'},
  {'*', '7', '4', '1'}
};

byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad

static const uint32_t GPSBaud = 9600;

bool auth_state = false;
bool mqtt_state = false;
bool end_state = false;
bool trip_status;

String buff_string = "";
String erase_pin="";
const int max_len = 4;
int trip_id; // Strores the Trip ID globally
const char* driver_name;

//function prototype for device_id reg.
void writeString(String, int);
String readString(int);
void device_id_reg();
void re_reg_screen();
bool start_trip_call(String driver_id);
bool end_trip_call();

String mac;           //MAC address
int pos;              //Position of writing and reading string
int buff_var[2] = {0, 0}; //state variable to changes the states
String pin = "1234";  //flash erasing pin

double gps_speed;
int state = DRIVE_ID;
int count =0;
const long interval = 1000;


// Objects Defs
HTTPClient http;
TinyGPSPlus gps;
WiFiClient espClient;
SoftwareSerial ss(0, 2);
DynamicJsonDocument doc(1024);
PubSubClient client(mqtt_server, 1883, espClient);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // oled objcet
Keypad_I2C keypad = Keypad_I2C( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR);
EEPROM_Rotate EEPROMr; ////Library for writing data into the flash memory


 static long currentMillis = millis();
void setup()
{
  
  // Initialize all the devices
  Serial.begin(DEVICE_BAUD);
  ss.begin(GPS_BAUD);
  EEPROMr.size(4);
  EEPROMr.begin(4096);
  keypad.begin( );
  keypad.addEventListener(keypadEvent);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
  {
    for (;;);
  }
  ;
  setup_wifi();

  // MQTT Server Connection
  if (!client.connected())
  {
    reconnect();
  }
  
  OLED_param();
   if(char(EEPROMr.read(DATA_OFFSET) == NULL))
    {
       Serial.println("In initial state2");
       reg_screen();
    }
  gps_lock();
 
}


void loop()
{
  client.loop();
  if(char(EEPROMr.read(DATA_OFFSET)) == NULL)
      state = DEVICE_ID;
   
  else if(buff_var[0] == 1)
      state = MQTT;
 
  else if(buff_var[1] == 1)
      state = RE_REG_SCREEN;
 
  else if(buff_var[1] == 2)
      state = FLASH_E;
 
  else if(buff_var[0] == 0)
      state = DRIVE_ID;
      
  char key = keypad.getKey();
  state_case(state, key);
  
}


void inputDriverID(char key)
{  
    if ((isDigit(key)) && (buff_string.length() < max_len))
    {
      buff_string += key;   
      Serial.println(buff_string);
      display.setCursor(6, 15);
      display.println(buff_string);
      display.display();
    }
 
}


void keypadEvent(KeypadEvent key)
{ 
  
  Serial.println("In keypad event key");
  switch (keypad.getState())
  {
    case PRESSED:
              switch (key)
                     {
                      case 'A':
                              buff_var[1] = 2;
                              clearAll();      
                              break;        
                      case 'B':  
                              break;

                      case 'C':
                              state = CLEAR;
                              break;
                      case 'D':
                              state = Start_Trip_Call;
                              break;
                     }
             break;


    case HOLD:
      switch (key)
      {
          case 'A':
                   if(char(EEPROMr.read(DATA_OFFSET)) == NULL)
                        device_id_reg();
                   break;      
         case 'B':
                    re_reg_screen();
                    buff_var[1] == 1;
                    break;

         case '#':
                  //Long press # to display Device_id and MAC address of esp8266
                  display.clearDisplay();
                  display.setCursor(15, 20);
                  display.println(readString(0));
                  display.display();
                  display.setCursor(15, 40);
                  display.println(WiFi.macAddress());
                  display.display();
                  delay(3000);
                  display.clearDisplay();
                  break;
         case '*':
                  if(state == MQTT)
                  {
                    end_trip();
                    buff_var[0] = 0;
                  }
                  break;
      }
          break;
  }
}

void clear_buff()
{
  buff_string.remove(buff_string.length() - 1);
  display.clearDisplay();
  display.setCursor(6, 15);
  display.println(buff_string);
  display.display();
}

void clearAll()
{
 display.clearDisplay(); 
 display.display();
 delay(200);
}

void start_trip()
{
   if (buff_string.length() == max_len)
          {
            display.clearDisplay();
            display.setCursor(0, 15);
            display.println("Auth..");
            display.display();
            if (start_trip_call(buff_string))
            {
              display.clearDisplay();
              display.setCursor(0, 15);
              display.println(driver_name);
              display.setCursor(0, 35);
              display.println(trip_id);
              display.display();
              delay(1000);
              buff_var[0] = 1;
             // display.clearDisplay();
            }
            else
            {
              // If not authenticated Get back to the Password Screen
              display.clearDisplay();
              display.setCursor(6, 15);
              display.println(buff_string);
              display.display();
            }
          }
          else
          {
            // Error message when less than 4 Digits are available
            display.clearDisplay();
            display.setCursor(6, 15);
            display.println("4 Digits!");
            display.display();
            delay(1000);
            display.clearDisplay();
            display.setCursor(6, 15);
            display.println(buff_string);
            display.display();
          }
          
}

void end_trip()
{
            if (end_trip_call())
            {
              display.clearDisplay();
              display.setCursor(6, 15);
              display.println("Trip Ended");
              display.display();
              delay(1000);
              display.clearDisplay();
              display.display();
              buff_string = "";
              buff_var[0] = 0;
            }
            else
            {
              display.clearDisplay();
              display.setCursor(6, 15);
              display.println("Not ended");
              display.display();
              delay(1000);
              display.clearDisplay();
              display.display();
            }
          
}
