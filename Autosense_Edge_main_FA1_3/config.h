//String readString(int);


//I2C Port Expander Address
#define I2CADDR 0x38

// OLED Address
#define OLED_ADDR 0x3C

// DEVICE Baud Rate
#define DEVICE_BAUD 9600

// GPS Baud Rate
#define GPS_BAUD 9600

// WiFi SSID
#define ssid readString(20)

// WiFi Password
#define password readString(40)

// MQTT Server IP
#define mqtt_server "13.59.55.180"        //readString(60)           //"18.191.5.128"  //"192.168.43.85"  "18.225.10.48" 13.235.243.43

//String topicstring = mqttIP;
//  char topicchar[30];
//  topicstring.toCharArray(topicchar,sizeof(topicstring));
//  const char *mqtt_server = topicchar;



// Start_trip Route
#define start_http_route "http://"+readString(90)+"/api/deviceId/driverId/start"   //13.58.73.166       //18.191.122.255           //"http://ec2-3-16-214-156.us-east-2.compute.amazonaws.com/api/deviceId/driverId/start";
// End_trip Route
#define end_http_route "http://"+readString(90)+"/api/deviceId/driverId/end"                     //"http://ec2-3-16-214-156.us-east-2.compute.amazonaws.com/api/deviceId/driverId/end";
