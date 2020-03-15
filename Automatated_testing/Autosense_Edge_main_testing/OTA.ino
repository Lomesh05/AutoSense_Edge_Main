
  void update_started() {
    Serial.println("CALLBACK:  HTTP update process started");
     display.setCursor(20,20);
            display.setTextSize(1);
            display.println("Updating Firmware");
            display.display();
            display.startscrollright(0x00, 0x00);
  }

  void update_finished() {
    Serial.println("CALLBACK:  HTTP update process finished");
  }

  void update_progress(int cur, int total) {
    Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
           Serial.println("Updating Firmware");

  }

  void update_error(int err) {
    Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
  }


void ota_update()
{
  // wait for WiFi connection
  if (WiFi.status() == WL_CONNECTED) {

    //WiFiClient client;

    // The line below is optional. It can be used to blink the LED on the board during flashing
    // The LED will be on during download of one buffer of data from the network. The LED will
    // be off during writing that buffer to flash
    // On a good connection the LED should flash regularly. On a bad connection the LED will be
    // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
    // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);

    // Add optional callback notifiers
    ESPhttpUpdate.onStart(update_started);
    ESPhttpUpdate.onEnd(update_finished);
    ESPhttpUpdate.onProgress(update_progress);
    ESPhttpUpdate.onError(update_error);

    t_httpUpdate_return ret = ESPhttpUpdate.update(espClient, "http://esp8266otaflux.s3.ap-south-1.amazonaws.com/FA1_3/Autosense_Edge_main_FA1_3.bin");
    // Or:
    //t_httpUpdate_return ret = ESPhttpUpdate.update(client, "server", 80, "file.bin");

    
    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;

      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

      case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
    }
  }
 
}


void ota_screen()
{     
      display.clearDisplay();
      display.display();
      display.setCursor(0,0);
      display.println("Do you want to Update?");
      display.display();

      display.setCursor(0,30);
      display.println("YES:PRESS A");
      display.display();

      display.setCursor(0,50);
      display.println("NO:PRESS B");
      display.display();

      display.clearDisplay();
           
}

void ota_pin(char key)
{
  enterKey(key, 6, 0, 36);
     if(buff_string.length()== 6)
      {           
        if(buff_string == OTA_PIN)
        {
          ota_update();         
        }
        else
        {
           display.clearDisplay();
           display.setCursor(20, 20);
           display.println("Wrong Pin");
           display.display();
           delay(2000);
           display.clearDisplay(); 
           display.display();                    
           buff_string = "";
           buff_var[0] = 0;
           change_scn = 0; 
                   
        }
        
      }
           
 }
