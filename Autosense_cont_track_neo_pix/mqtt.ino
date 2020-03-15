
void mqtt_publish()
{
  Serial.println("In mqtt_publish");
   //Belows 4 lines to convert integer into const char*
  String topicstring = String(vehicle_name);
  char topicchar[20];
  topicstring.toCharArray(topicchar,sizeof(topicstring));
  const char *topic = topicchar;
 
  Serial.println("");
  Serial.println("");
  Serial.print("topic:  ");
  Serial.println(topic);
  
  while (ss.available() > 0){

    if (gps.encode(ss.read()))
    {
      if (gps.location.isValid() && gps.time.isValid())
      {
        pixels.setPixelColor(0, 0, 206, 209);
        pixels.show(); 
    
        Serial.println("gps Signal valid");
        Serial.print("lat: ");
        Serial.println(String(gps.location.lat(), 6));
        Serial.print("lang: ");
        Serial.println(String(gps.location.lng(), 6));
        Serial.print("speed: ");
        Serial.println(gps.speed.kmph());
        doc["lat"] = String(gps.location.lat(), 6);
        doc["lon"] = String(gps.location.lng(), 6);
        doc["speed"] = String(gps.speed.kmph());
        char buffer1[512];
        serializeJson(doc, buffer1);
        Serial.print("Buffer1");
        Serial.println(buffer1);
        client.publish(topic, buffer1);  //Publishing to AWS mosquitto server
        Serial.println("");
        Serial.println("");
        Serial.print("topic:  ");
        Serial.println(topic);
        doc.clear();
        memset(buffer1, 0, sizeof buffer1);

      }
    }
  }
   
//         pixels.setPixelColor(0, 0, 106, 109);
//         pixels.show(); 
//         client.publish("outTopic", "Hello");
   
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if(client.state() < 0)
    {
      count = 0;
      pixels.setPixelColor(0, 139, 0, 0);
      pixels.show(); 
      delay(50);

      pixels.setPixelColor(0, 0, 0, 0);
      pixels.show(); 
      delay(50);  
    }
    
    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(100);
    }
  }
}

   
