void reg_screen()
{
  if(char(EEPROMr.read(DATA_OFFSET) == NULL))
  {
    display.clearDisplay();
    display.setCursor(5,2);
    display.println("One Time Device Reg Req");
    display.display();    
            
    display.setCursor(5,30);
    display.println("Hold button A");
    display.display();  
    Serial.println("in reg screen");
        
   }

}

void device_id_reg()  
{
    http.begin("http://"+readString(90)+"/routers/device_registration");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header     
    //mac = WiFi.macAddress();
    doc["mac_address"]= "AW:WD:EF:DE:ER:F1";
     
   char macID[512];
   serializeJson(doc, macID);
   Serial.print("mac:");
   Serial.println(macID);
    int httpCode = http.POST(macID);   //Send the request
    String payload = http.getString();                  //Get the response payload     
    //String payload = macID;
    Serial.print("HTTP code: ");
    Serial.println(httpCode);
    Serial.print("Payload: ");
    Serial.println(payload);
    void clear();
     DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, payload);
     if (error) 
      {
          Serial.print(F("deserializeJson() failed with code "));
          Serial.println(error.c_str());
          return;
      }
      else
      {
          JsonObject root = doc.as<JsonObject>();              
          const char* device_id = root["device_id"];
          Serial.print("device_id:");
          Serial.println(device_id);
          Serial.print("httpCode:");
          Serial.println(httpCode);   //Print HTTP return code
          Serial.println(payload);    //Print request response payload
          String id = device_id;
        //  Save string to flash memory (EEPROM library). Comment this line after the first run.
          Serial.print("Writing string: ");
          String input_string = id;
          Serial.println(input_string);
          writeString(input_string,0);
          delay(2000);
          http.end();  //Close connection
          buff_var[0] == 0;
      }
   
}

void writeString(String str,int pos){
  for (int i=0; i<str.length(); i++) {
    int s = str[i];
    EEPROMr.write(i+pos+DATA_OFFSET, s);
  }
  EEPROMr.write(str.length()+pos+DATA_OFFSET,'\0');
  EEPROMr.commit();
}

//Reading device id from EEPROM
String readString(int pos){
  char data[100]; // Max 100 Bytes
  int len = 0;
  unsigned char k;
  while(k != '\0' && len <= 100){
    k = char(EEPROMr.read(len+pos+DATA_OFFSET));
    data[len] = k;
    len++;
  }  
   return data;
  
}
