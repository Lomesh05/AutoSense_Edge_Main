#include <ArduinoJson.h>
#include <TinyGPS++.h>
#include <SPI.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include <ESP8266httpUpdate.h>
#include <EEPROM_Rotate.h> 
#include "config.h"

const int RED = 14;
const int GREEN = 12;
const int BLUE = 13;

#define DATA_OFFSET 10

static const uint32_t GPSBaud = 9600;

void writeString(String, int);
String readString(int);
//Function Decalration
bool testWifi(void);
void launchWeb(void);
void setupAP(void);



const long interval = 1000;
int otaUpdate = 0;
int statusCode;
String html_content; //taking the html code as string
int count = 0;


// Objects Defs
TinyGPSPlus gps;
WiFiClient espClient;
SoftwareSerial ss(0, 2);
PubSubClient client(mqtt_server, 1883, espClient);
ESP8266WebServer server(80);
EEPROM_Rotate EEPROMr;
DynamicJsonDocument doc(1024);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived: ");
  char otaString[3];
  for (int i = 0; i < length; i++) {
    otaString[i] = (char)payload[i];
    Serial.println(otaString[i]);
  }
  String str = String(otaString);
  Serial.println(str);

  if (str == "OTA") {
    otaUpdate = 1;
  }

}

void setup()
{
  // Initialize all the devices
  Serial.begin(DEVICE_BAUD);
  client.setServer(mqtt_server, 1883);
  ss.begin(GPS_BAUD);
  EEPROMr.size(4);
  EEPROMr.begin(4096);
  client.setCallback(callback);
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  setup_wifi();
  // MQTT Server Connection
  if (!client.connected())
  {
    reconnect();
  }
  
 nmea_message_disabler();
 //gps_lock();
 
}


void loop()
{
  Serial.println("in void loop");
  client.loop();
  mqtt_publish();
  client.subscribe("test");
  if(otaUpdate == 1)
  {
    Serial.println("in OTA update");
    ota_update();
  }
  //continuosly checking the mqtt connection
  Serial.print("client.state(): ");
  Serial.println(client.state());
  if(client.state() == 0)
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  else
    {
      
      Serial.println("turn of hotsot");
     // reconnect(mqtt_server);
     if(WiFi.status() != WL_CONNECTED)
            reconnect();
     else if(WiFi.status() == WL_CONNECTED)
              reconnect();
            
    }

}

void writeString(String str,int pos){
  for (int i=0; i<str.length(); i++) {
    int s = str[i];
    EEPROMr.write(i+pos+DATA_OFFSET, s);
  }
  EEPROMr.write(str.length()+pos+DATA_OFFSET,'\0');
  EEPROMr.commit();
}

//Reading device id from EEPROM
String readString(int pos){
  char data[100]; // Max 100 Bytes
  int len = 0;
  unsigned char k;
  while(k != '\0' && len <= 100){
    k = char(EEPROMr.read(len+pos+DATA_OFFSET));
    data[len] = k;
    len++;
  }  
   return data;
  
}
