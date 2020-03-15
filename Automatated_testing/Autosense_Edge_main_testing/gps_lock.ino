void gps_lock()
{
  Serial.println("Searching for GPS..");
  //Device lock Achived
  display.setCursor(6, 18);
  display.println("Searching for GPS..");
  display.display();
  int cnt = 6;
  // Wait until proper GPS lock is achieved
  while (1)
  {
    if (ss.available() > 0)
    {
      if (gps.encode(ss.read()))
      {
        if (gps.location.isValid() && gps.time.isValid())
        {
          //state = DRIVE_ID;
          Serial.println("GPS Lock Acquired!");
          display.clearDisplay();
          display.setCursor(6, 28);
          display.println("GPS Lock Acquired!");
          display.display();
          delay(1000);
          display.clearDisplay();
          
          break;
        }
      }
    }
    else
    {
      Serial.println("Waiting for GPS lock");
      display.clearDisplay();
      display.setCursor(6, 15);
      display.println("Waiting for GPS lock");
      display.display();
      delay(50);
      display.clearDisplay();
    }
  }
}
