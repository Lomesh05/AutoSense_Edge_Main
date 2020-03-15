
  void update_started() {
    Serial.println("CALLBACK:  HTTP update process started");
  }

  void update_finished() {
    Serial.println("CALLBACK:  HTTP update process finished");
  }

  void update_progress(int cur, int total) {
       Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
    
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
//    ESPhttpUpdate.setLedPin(14, LOW);
      
     analogWrite(RED, 75);
     analogWrite(GREEN, 0);
     analogWrite(BLUE, 130);
    // Add optional callback notifiers
    ESPhttpUpdate.onStart(update_started);
    ESPhttpUpdate.onEnd(update_finished);
    ESPhttpUpdate.onProgress(update_progress);
    ESPhttpUpdate.onError(update_error);

    Serial.println("updating ota");
    t_httpUpdate_return ret = ESPhttpUpdate.update(espClient, "http://esp8266otaflux.s3.ap-south-1.amazonaws.com/continuousTracking/Autosense_continuous_track.ino.nodemcu.bin");
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
