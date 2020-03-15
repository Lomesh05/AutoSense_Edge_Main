void setup_wifi()
{
  Serial.println("In access point function");
  Serial.println("Disconnecting previously connected WiFi");
  WiFi.disconnect();
  int count = 0; 
  display.clearDisplay(); 
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Connecting to WiFi");
  display.display(); 
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println();
  Serial.println();
  Serial.println("Startup");
  Serial.println("Reading from EEPROM of esp8266");
  Serial.print("ssid: ");
  Serial.println(ssid);
  Serial.print("password: ");
  Serial.println(password);
  WiFi.begin(ssid, password);
  while(1)
  {
  
    if(WiFi.status() != WL_CONNECTED) 
      {
        
        display.setCursor(count, 8);
        display.println(".");
        display.display();
        count++;
        delay(500);
      }
     else if(WiFi.status() == WL_CONNECTED)
     {
        count = 0;
        break;
     }
     if(count > 20)
     {
      break;
     }
    
  }

    if(count > 20)
      {
         Serial.println("Turning the HotSpot On");
         display.clearDisplay();
         display.setCursor(0, 0);
         display.println("Setup WiFi config:");
         display.display();
         display.setCursor(0, 12);
         display.println("Turn on WiFi on your device");
         display.display();
         display.setCursor(0, 36);
         display.println("Connect to fluxAuto");
         display.display(); 
         display.setCursor(0, 48);
         display.println("Open 192.168.4.1 on  your browser");
         display.display(); 
         launchWeb();
         setupAP();// Setup HotSpot
      }
  Serial.println();
  Serial.println("Waiting.");
  
  while ((WiFi.status() != WL_CONNECTED))
  {
    Serial.print(".");
    delay(100);
    server.handleClient();
  }
delay(7000);
}

void launchWeb()
{
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED)
    Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer();
  // Start the server
  server.begin();
  Serial.println("Server started");
}

void setupAP(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);  
  WiFi.softAP("fluxauto", "");
  Serial.println("softap");
  launchWeb();
  Serial.println("ESP8266 as Web server ");
}

void createWebServer()
{
 {
    server.on("/", []() {      
      content = "<!DOCTYPE HTML>\r\n<html>";
      content += "<h1>Welcome to AutoSense </h1>";
      content += "<style></style>"; 
      content += "<p>";
      content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
      content += "</html>";
      server.send(200, "text/html", content);
      Serial.print("content: ");
      Serial.println(content);
    });
    server.on("/scan", []() {
      //setupAP();
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      server.send(200, "text/html", content);
    });

    server.on("/setting", []() {
      String wssid = server.arg("ssid");
      String wpass = server.arg("pass");
      if (wssid.length() > 0 && wpass.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 30; i < 96; ++i) {
          EEPROMr.write(i, 0);
        }
        Serial.println(wssid);
        Serial.println("");
        Serial.println(wpass);
        Serial.println("");

        Serial.println("writing eeprom ssid:");
        writeString(wssid,32);
        writeString(wpass,64);
        EEPROMr.commit();
        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
        ESP.restart();
      } else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "application/json", content);

    });
  } 
}
