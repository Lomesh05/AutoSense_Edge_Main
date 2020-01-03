int state_case(int state, char key)
{
  switch(state)
    {
      Serial.println("In switch case");
      case DEVICE_ID:
                    reg_screen();
                    break;
          
                      
      case DRIVE_ID:
                    Serial.println("In drive id state");
                    display.setCursor(0, 0);
                    display.println("Enter Your ID:");
                    display.display();
                    inputDriverID(key);
                   break;
      case Start_Trip_Call:
                  start_trip();
                  break;
                  
      case MQTT:
              mqtt_publish();
              break;

      case RE_REG_SCREEN:
                  Serial.println("In re reg state");
                  count++;
                  if(count == 20000)
                      buff_var[0] = 0;
                     break;
      case FLASH_E:
              
                display.setCursor(0,0);
                display.println("Enter 4 Digit Pin");
                display.display();
                if(isDigit(key))
                flash_erase(key);
                break;
      case CLEAR:
                 clear_buff();
                 break;
}
}
