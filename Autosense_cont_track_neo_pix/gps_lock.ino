void gps_lock()
{
  // Wait until proper GPS lock is achieved
  while (1)
  {
    if (ss.available() > 0)
    {
      if (gps.encode(ss.read()))
      {
        if (gps.location.isValid() && gps.time.isValid())
        {
          Serial.println("GPS Lock Acquired!");
          break;
        }
      }
    }
    else
    {
      Serial.println("Waiting for GPS lock");
    }
  }
}
