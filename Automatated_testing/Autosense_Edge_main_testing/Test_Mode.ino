void Test_Mode()
{
   Serial.println("In Test Mode");
   while(1)
   {
                 
    if (Serial.available()) 
      {
         command = Serial.readStringUntil('\n');
   
    if(command == "check_route_address")
    {
       display.setCursor(0, 12);
       display.println("Checking Route Addresses");
       display.display();
       buff_string = "5001";
       start_trip();
    }
      else if(command == "clearBuff")
            {
              buff_string = "";
              Serial.println("in arduino clearing the buffer");
              Serial.print("buff_string: ");
              Serial.println(buff_string);
            }
      else if(command == "end")
            {
              Serial.println("In trip end");
              end_trip();
            }
   
      else if(command == "OTA")
           {
              Serial.println("testing OTA arduino");
              ota_update();
           }
        else if(command == "MQTT")
        {
          Serial.println("Testing MQTT from arduino");
          if (client.connected())
             {
                Serial.println("MQTT Connected");
              }
              else if(!client.connected())
              {
                Serial.println("MQTT doesn't connected!");
                reconnect();
              }
        }
     Serial.println(command);  
  }

    if(keypad.getState() == PRESSED)
    {
      if(PRESSED == '*')
      {
      Serial.println("Test mode exit");
      buff_var[0] = 0;
      break;  
      }
    }
   }
}
