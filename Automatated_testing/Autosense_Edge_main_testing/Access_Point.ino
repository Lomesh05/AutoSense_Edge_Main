void setup_wifi()
{
  Serial.println("In access point function");
  Serial.println("Disconnecting previously connected WiFi");
  WiFi.disconnect();

  String test = String(readString(90))+"/api/deviceId/driverId/start";
  Serial.println("");
  Serial.println("");
  Serial.print("test: ");
  Serial.println(test);
      Serial.print("mqtt_ip: ");
      Serial.println(readString(60));
      Serial.print("Start Route: ");
      Serial.println(readString(90));
      Serial.print("End Route: ");
      Serial.println(readString(90));
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
     if(count > 60)
     {
      break;
     }

    Serial.println("after connecting wifi");
  }

    if(count > 60)
      {
         Serial.println("Turning on the Hotspot");
         display.clearDisplay();
         display.setCursor(0, 0);
         display.println("WiFi Setup Config:");
         display.display();
         display.setCursor(0, 12);
         display.println("Turn on WiFi of your device");
         display.display();
         display.setCursor(0, 36);
         display.println("Connect to fluxAuto");
         display.display(); 
         display.setCursor(0, 48);
         display.println("Open 192.168.4.1");
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
  WiFi.softAP("fluxAuto", "");
  Serial.println("softap");
  launchWeb();
  Serial.println("ESP8266 as Web server ");
}

void createWebServer()
{
 {
    server.on("/", [](){
       html_content =   "<!DOCTYPE html>";
    html_content += "<html>";
    html_content += "<head>";
    html_content += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    html_content += "<style>";
    html_content += "body {font-family: Arial, Helvetica, sans-serif;}";
    html_content += "* {box-sizing: border-box;}";

    html_content += ".open-button {";
    html_content += "background-color: rgb(54, 51, 54);";
    html_content += "color: white;";
    html_content += "padding: 16px 20px;";
    html_content += "border: none;";
    html_content += "cursor: pointer;";
    html_content += "opacity: 0.8;";
    html_content += "position: static;";
    html_content += "bottom: 23px;";
    html_content += "right: 28px;";
    html_content += "width: auto;}";
    html_content += ".http-button {";
    html_content += "background-color: rgb(54, 51, 54);";
    html_content += "color: white;";
    html_content += "padding: 16px 20px;";
    html_content += "border: none;";
    html_content += "cursor: pointer;";
    html_content += "opacity: 0.8;";
    html_content += "position: static;";
    html_content += "bottom: 23px;";
    html_content += "right: 28px;";
    html_content += "width: auto;}";
    html_content += ".form-popup {";
    html_content += "display: none;";
    html_content += "position: absolute;";
    html_content += "border: 3px solid #f1f1f1;";
    html_content += "z-index: 9;";
    html_content += "}";

    html_content += ".form-container {";
    html_content += "width: 100%;";
    html_content += "padding: 10px;";
    html_content += "justify-content:center;";
    html_content += "background-color: white;";
    html_content += "}";

    html_content += ".form-container input[type=text], .form-container input[type=password] {";
    html_content += "width: 100%;";
    html_content += "padding: 15px;";
    html_content += "margin: 5px 0 22px 0;";
    html_content += "border: none;";
    html_content += "background: #f1f1f1;";
    html_content += "}";

    html_content += "form-container input[type=text]:focus, .form-container input[type=password]:focus {";
    html_content += "background-color: #ddd;";
    html_content += "outline: none;";
    html_content += "}";

    html_content += "h1{";
    html_content += "color:red;";
    html_content += "background-color: black;";
    html_content += "font-size: 50px;";
    html_content += "width: auto;";
    html_content += "height: auto;";
    html_content += "text-align: center;";
    html_content += "}";

    html_content += "body{";
    html_content += "background-color:GREY;";
    html_content += "}";
    
    html_content += ".form-container .btn {";
    html_content += "background-color: #4CAF50;";
    html_content += "color: white;";
    html_content += "padding: 16px 20px;";
    html_content += "border: none;";
    html_content += "cursor: pointer;";
    html_content += "width: auto;";
    html_content += "margin-bottom:10px;";
    html_content += "opacity: 0.8;";
    html_content += "}";
    html_content += ".open-button";
    html_content += ".form-container .cancel {";
    html_content += "background-color: red;";
    html_content += "}";

    html_content += ".form-container .btn:hover, .open-button:hover {";
    html_content += "opacity: 1;";
    html_content += "}";
    html_content += "ul {";
    html_content += "list-style-type: none;";
    html_content += "margin: 0;";
    html_content += "padding: 0;";
    html_content += "overflow: hidden;";
    html_content += "background-color:black;";
    html_content += "}";

    html_content += "li {";
    html_content += "float: left;";
    html_content += "}";

    html_content += "li a {";
    html_content += "display: block;";
    html_content += "color: white;";
    html_content += "background-color: #333;";
    html_content += "text-align: center;";
    html_content += "padding: 14px 16px;";
    html_content += "text-decoration: none;";
    html_content += "}";

    html_content += "li a:hover {";
    html_content += "background-color: #111;";
    html_content += "}";
    html_content += "</style>";
    html_content += "</head>";
    html_content += "<body>";

  

    html_content += "<ul>";
    html_content += "<h1> AutoSense Edge </h1> ";
    html_content += "<li><a href=\"#\"><button id=\"button1\" class=\"open-button\" onclick=\"openForm()\">Wifi Config</button></a></li>";
    html_content += "<li><a href=\"#\"><button id=\"button2\" class=\"http-button\" onclick=\"httpForm()\">Server IP Addresses</button></a></li>";
    html_content += "</ul>";

    html_content += "<div class=\"form-popup\" id=\"wifiForm\">";
    html_content += "<form method='get' action='setting' class=\"form-container\">";
    html_content += "<h2>WiFi Config</h2>";

    html_content += "<label for=\"ssid\"><b>ssid</b></label>";
    html_content += "<input type=\"text\" placeholder=\"Enter ssid\" name=\"ssid\" required>";

    html_content += "<label for=\"psw\"><b>Password</b></label>";
    html_content += "<input type=\"password\" placeholder=\"Enter Password\" name=\"psw\" required>";

    html_content += "<button type=\"submit\" class=\"btn\">Submit</button>";

    html_content += "</form>";
    html_content += "</div>";

    html_content += "<div class=\"form-popup\" id=\"ipForm\">";
    html_content += "<form method='get' action='setting' class=\"form-container\">";
    html_content += "<h2>IP Address</h2>";

    html_content += "<label for=\"MQTT-IP\"><b>MQTT-IP</b></label>";
    html_content += "<input type=\"text\" placeholder=\"Enter MQTT-IP\" name=\"MQTT-IP\">";

    html_content += "<label for=\"HTTP1-IP\"><b>Route Address</b></label>";
    html_content += "<input type=\"text\" placeholder=\"Enter Route Address\" name=\"HTTP1-IP\" >";

    html_content += "<button type=\"submit\" class=\"btn\">Submit</button>";
    html_content += "</form>";
    html_content += "</div>";
    html_content += "<script>";
    html_content += "function openForm() {console.log(\"in open forms\");";
//   
    html_content += "document.getElementById(\"wifiForm\").style.display = \"block\";";
    html_content += "document.getElementById(\"ipForm\").style.display = \"none\";";
    html_content += "document.getElementById(\"button1\").style.backgroundColor = \"black\";";
    html_content += "document.getElementById(\"button2\").style.backgroundColor = \" rgb(54, 51, 54)\";";
    html_content += "}";
    html_content += "function httpForm() {console.log(\"in http forms\");";
//    
    html_content += "document.getElementById(\"ipForm\").style.display = \"block\";";
    html_content += "document.getElementById(\"wifiForm\").style.display = \"none\";";
    html_content += "document.getElementById(\"button2\").style.backgroundColor = \"black\";";
    html_content += "document.getElementById(\"button1\").style.backgroundColor = \" rgb(54, 51, 54)\";";
    html_content += "}";

    html_content += "</script>";

    html_content += "</body>";
    html_content += "</html>";

       server.send(200, "text/html", html_content);
    });
    server.on("/setting", [](){
      String wssid = server.arg("ssid");
      String wpass = server.arg("psw");
      String mqtt_ip = server.arg("MQTT-IP");
      String route_addr = server.arg("HTTP1-IP");
      
      if(mqtt_ip.length() > 0 )
      { 
         Serial.println("");
         Serial.println("");
         Serial.println("");
         Serial.println("writing...");
         writeString(mqtt_ip,60);                    
         html_content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
         statusCode = 200;
         ESP.restart();
      }

      if(route_addr.length() > 0)
      {
         writeString(route_addr,90);
          html_content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
         statusCode = 200;
         ESP.restart();
      }
      
      if (wssid.length() > 0 && wpass.length() > 0) {
        Serial.println(wssid);
        Serial.println("");
        Serial.println(wpass);
        Serial.println("");

        Serial.println("writing eeprom ssid:");
        writeString(wssid,20);
        writeString(wpass,40);
        html_content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
        ESP.restart();
      }
      
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "application/json", html_content);

    });
  } 
}
