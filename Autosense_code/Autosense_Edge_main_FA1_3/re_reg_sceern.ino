//erasing the device id from EEPROM
void flash_erase(char key)
{ 
    enterKey(key, max_len);
     if(buff_string.length()== max_len)
      {           
        if(buff_string == pin)
        {
           for(int i=0; i<20; i++)
             {
                EEPROMr.write(i+DATA_OFFSET, 0);
             }
          EEPROMr.commit();
          display.clearDisplay();
          display.setCursor(6,15);
          display.println("Flash Erased!");
          display.display();
          delay(2000);
          display.clearDisplay();
          ESP.restart();
         
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
           cnt++;
           buff_string = ""; 
                   
        }
        
      }
      if(cnt == 3)
      {
        buff_var[0] = 0;
        buff_var[1] = 0;
        buff_var[2] = 0; 
        buff_string = ""; 
        cnt = 0;
        display.clearDisplay(); 
        display.display(); 
      }               
 
 }

//This function display flash erasing window
void re_reg_screen()
{ 
      display.clearDisplay();
      display.display();
      display.setCursor(0,0);
      display.println("Do you want to erase flash?");
      display.display();

      display.setCursor(0,30);
      display.println("YES:PRESS A");
      display.display();

      display.setCursor(0,50);
      display.println("NO:PRESS B");
      display.display();

      display.clearDisplay();
           
}
