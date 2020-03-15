
void mqtt_publish()
{
  
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      if (gps.location.isValid() && gps.time.isValid())
      {
        doc["lat"] = String(gps.location.lat(), 6);
        doc["lon"] = String(gps.location.lng(), 6);
        char buffer1[512];
        serializeJson(doc, buffer1);
        delay(2000);
        client.publish("ring", buffer1);
        doc.clear();
        memset(buffer1, 0, sizeof buffer1);
        display.clearDisplay();
      }
  display.setCursor(0, 24);
  display.println(String(gps.location.lat(), 6));
  display.setCursor(0, 36);
  display.println(String(gps.location.lng(), 6));
  display.display();
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    //Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
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
      delay(5000);
    }
  }
}


void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  //Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(ssid);

  WiFi.begin(ssid, password);
  int count = 0;
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    display.setCursor(count, 8);
    display.println(".");
    display.display();
    count++;
    delay(500);
  }
  randomSeed(micros());

  //  Serial.println("");
  //  Serial.println("WiFi connected");
  //  Serial.println("IP address: ");
  //Serial.println(WiFi.localIP().toString());

  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("IP: ");
  display.setCursor(16, 0);
  display.println(WiFi.localIP().toString());
  display.display();
  delay(1000);
  
}
