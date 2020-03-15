//these are state, state invokes the functions

int state_case(int state, char key)
{
  switch(state)
    {
      Serial.println("In switch case");                                    
      case DRIVE_ID:
                if(buff_var[0] == 0)
                {
                  count++;
                  if(count == 1)
                      display.clearDisplay();
                    //Serial.println("In drive id state");
                    display.setCursor(0, 0);
                    display.println("Enter Your ID:");
                    display.display();
                    enterKey(key, max_len, 6, 15);
                } 
                    break;
                    
      case Start_Trip_Call:
                  start_trip();
                  break;
                  
      case MQTT:
              mqtt_publish();
              break;

      case End_Trip_Call:
                  end_trip();
                  break;

      case RE_REG_SCREEN:
              if(buff_var[1] == 1)
              {              
                display.setCursor(0,0);
                display.println("Enter 4 Digit Pin");
                display.display();
                if(isDigit(key))
                flash_erase(key);
              }
                break;
      case OTA:
                display.setCursor(0,0);
                display.println("Firmware Update:");
                display.display();
                
                display.setCursor(0,24);
                display.println("Enter Pin");
                display.display();
                if(isDigit(key))
                {
                  ota_pin(key);
                  Serial.println("in OTA state And in if loop");
                }
                break;

      case TEST:
                 Test_Mode();
                 break;
                 
      case CLEAR:
                 if(change_scn == 1)
                 {
                    clear_buff(0, 36);
                 }
                 else
                    clear_buff(6, 15);
                 break;
}
}
