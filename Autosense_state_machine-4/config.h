
//I2C Port Expander Address
#define I2CADDR 0x38

// OLED Address
#define OLED_ADDR 0x3C

// DEVICE Baud Rate
#define DEVICE_BAUD 9600

// GPS Baud Rate
#define GPS_BAUD 9600

// WiFi SSID
#define ssid readString(32)

// WiFi Password
#define password readString(64)

// MQTT Server IP
#define mqtt_server "192.168.43.95" //18.225.10.48//13.235.243.43



// HTTP Routes //

// Start_trip Route
String start_http_route = "http://protected-mountain-04200.herokuapp.com/api/deviceId/driverId/start";
// End_trip Route
String end_http_route = "http://protected-mountain-04200.herokuapp.com/api/deviceId/driverId/end";
