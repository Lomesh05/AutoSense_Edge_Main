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

#define vehicle_name readString(60) 

// MQTT Server IP
#define mqtt_server "13.233.173.73"
