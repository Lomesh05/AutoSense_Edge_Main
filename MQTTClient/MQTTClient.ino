#include <ESP8266MQTTClient.h>
#include <ESP8266WiFi.h>
MQTTClient mqtt;

void setup() {
  Serial.begin(115200);
  WiFi.begin("moto x4", "qazwsxedc");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
if(WiFi.status() == WL_CONNECTED)
  Serial.println("connected");

  //topic, data, data is continuing
  mqtt.onData([](String topic, String data, bool cont) {
    Serial.println("in onData function");
    Serial.printf("Data received, topic: %s, data: %s\r\n", topic.c_str(), data.c_str());
    mqtt.unSubscribe("test");
  });

  mqtt.onSubscribe([](int sub_id) {
    Serial.println("in onSubsribe funcion");
    Serial.printf("Subscribe topic id: %d ok\r\n", sub_id);
    mqtt.publish("test", "Hello", 0, 0);
  });
  mqtt.onConnect([]() {
    Serial.println("in onConnect function");
    Serial.println("MQTT: Connected\r\n");
    Serial.printf("Subscribe id: %d\r\n", mqtt.subscribe("test", 0));
//    mqtt.subscribe("/qos1", 1);
//    mqtt.subscribe("/qos2", 2);
  });

  mqtt.begin("18.222.141.176");
//  mqtt.begin("mqtt://test.mosquitto.org:1883", {.lwtTopic = "hello", .lwtMsg = "offline", .lwtQos = 0, .lwtRetain = 0});
//  mqtt.begin("mqtt://user:pass@mosquito.org:1883");
//  mqtt.begin("mqtt://user:pass@mosquito.org:1883#clientId");

}

void loop() {
  Serial.println("in void loop");
  mqtt.handle();
 
}
