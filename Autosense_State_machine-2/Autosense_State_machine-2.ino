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

String buff_driverid = "";
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
int set_reg=0;        //state for device registration
 int buff_arr[2]; //state variable to changes the states
int hold_state = 0;   //holding the key A state
String pin = "1234";  //flash erasing pin

double gps_speed;
int state = INITIAL;



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


void setup() {
 
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
  gps_lock();

}

void loop() {
   char key = keypad.getKey();
        if(key == 'C') 
        {
          state = CLEAR;
        }
    switch(state)
    {
      case INITIAL:
          if(char(EEPROMr.read(DATA_OFFSET) == NULL))
            {
              reg_screen();
              state = hold_A;
              Serial.print("after state");
            }
            else
            {
              state = DRIVE_ID;
            }
            break;

      case DEVICE_ID:
            if(char(EEPROMr.read(DATA_OFFSET) == NULL))
            {
              device_id_reg();
            }
            break;
      case RE_REG_SCREEN:
      case hold_A:
       
          break;
      case CLEAR:
        break;
    }

}



void keypadEvent_key(KeypadEvent key)
{
  switch (keypad.getState())
  {
    case PRESSED:
      switch (key)
      {
        case 'A':
 
         
        case 'B':  
  
        case 'C':
  
        case 'D':
        break;
      }
        


    case HOLD:
      switch (key)
      {
      case 'A':
          state = DEVICE_ID;
          Serial.println("Hold A functioning");
          break;

      case 'B':
   

      case '#':
      
     
       case '*':
       break;
        
      }
  }
     
}
