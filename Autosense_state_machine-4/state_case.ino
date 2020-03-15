int state_case(int state, char key)
{
  switch(state)
    {
      Serial.println("In switch case");
      case DEVICE_ID:
                    reg_screen();
                    break;          
                                          
      case DRIVE_ID:
                if(buff_var[0] == 0)
                {
                    Serial.println("In drive id state");
                    display.setCursor(0, 0);
                    display.println("Enter Your ID:");
                    display.display();
                    inputDriverID(key);
                }
                    break;
                    
      case Start_Trip_Call:
                  start_trip();
                  break;
                  
      case MQTT:
              mqtt_publish();
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
