//String readString(int);


//I2C Port Expander Address
#define I2CADDR 0x20

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
#define mqtt_server "18.191.5.128"        //readString(60)



// Start_trip Route
#define start_http_route "http://"+readString(90)+"/api/deviceId/driverId/start"
#define end_http_route "http://"+readString(90)+"/api/deviceId/driverId/end"                    
