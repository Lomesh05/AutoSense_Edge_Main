
bool start_trip_call(String driverId)
{
    display.clearDisplay();
    display.setCursor(0, 30);
    display.println(String(gps.location.lng(), 6));
    display.setCursor(0, 50);
    display.println(String(gps.location.lat(), 6));
    display.display();
  Serial.println(gps.location.lat());
  Serial.println(gps.location.lng());

  doc["driver_id"] = driverId.toInt();
  doc["start_lat"] = String(gps.location.lat(), 6);
  doc["start_lon"] = String(gps.location.lng(), 6);
  doc["device_id"] = readString(0);

  char buffer1[512];
  serializeJson(doc, buffer1);

  //Declare object of class HTTPClient
  //start_http_route Trip Start Sequence
  http.begin(start_http_route);
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(buffer1);   //Send the request
  Serial.print("Http: ");
  Serial.println(httpCode);
  Serial.print("http post: ");
    Serial.println(buffer1);
  if ((httpCode == 200) || (httpCode == 201))
  {
    doc.clear();
    memset(buffer1, 0, sizeof buffer1);

    String payload = http.getString();  //Get the response payload
    Serial.print("Start Trip Response: ");
    Serial.println(httpCode);   //Print HTTP return code
    Serial.print("payload: ");
    Serial.println(payload);

    auto error = deserializeJson(doc, payload);
    if (error)
    {
      display.clearDisplay();
      display.setCursor(0, 15);
      display.println("Bad Response");
      display.display();
      Serial.print(F("deserializeJson() failed with code "));
          Serial.println(error.c_str());
      //return 0;
    }
    else
    {
      JsonObject obj = doc.as<JsonObject>();
      bool d_auth = obj["driver_auth"];
      if (d_auth)
      {
        driver_name = obj["driver_name"];
        trip_id = obj["trip_id"];
        trip_status = obj["trip_status"];
        Serial.println(driver_name);
        Serial.println(trip_id);
        //Serial.println(d_a);
        doc.clear();
        http.end();
        return 1;
      }
      else
      {
        display.clearDisplay();
        display.setCursor(0, 15);
        display.println("Not Authenticated");
        display.display();
        delay(1000);
        return 0;
      }

    }
  }
  else
  {
    display.clearDisplay();
    display.setCursor(0, 15);
    display.println("Bad Connection!");
    display.display();
    delay(1000);
    return 0;
  }

}


bool end_trip_call()
{
  Serial.println("In End trip call");
  doc["end_lat"] = String(gps.location.lat(), 6);
  doc["end_lon"] = String(gps.location.lng(), 6);
  doc["trip_id"] = trip_id;
  char buffer1[512];
  serializeJson(doc, buffer1);

  //end_http_route Trip Start Sequence
  http.begin(end_http_route);
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(buffer1);   //Send the request
  Serial.print("End Trip Response: ");
  Serial.println(httpCode);   //Print HTTP return code
  String payload = http.getString();  //Get the response payload
  Serial.println(payload);
  if ((httpCode == 200) || (httpCode == 201))
  {
    doc.clear();
    memset(buffer1, 0, sizeof buffer1);

    String payload = http.getString();  //Get the response payload
    Serial.println(payload);

    auto error = deserializeJson(doc, payload);
    if (error)
    {
      display.clearDisplay();
      display.setCursor(0, 15);
      display.println("Bad Response");
      display.display();
      //return 0;
    }
    else
    {
      JsonObject obj = doc.as<JsonObject>();
      trip_status = obj["trip_status"];
      http.end();
      return 1;
    }
  }
  else
  {
    Serial.println("Bad Exit HTTP Code");
    return 0;
  }
}

void start_trip()
{
   if (buff_string.length() == max_len)
          {
            display.clearDisplay();
            display.setCursor(0, 15);
            display.println("Auth..");
            display.display();
            if (start_trip_call(buff_string))
            {
              display.clearDisplay();
              display.setCursor(0, 15);
              display.println(driver_name);
              display.setCursor(0, 35);
              display.println(trip_id);
              display.display();
              delay(1000);
              buff_var[0] = 1;
              display.clearDisplay();
            }
            else
            {
              // If not authenticated Get back to the Password Screen
              display.clearDisplay();
              display.setCursor(6, 15);
              display.println(buff_string);
              display.display();
            }
          }
          else
          {
            // Error message when less than 4 Digits are available
            display.clearDisplay();
            display.setCursor(6, 15);
            display.println("4 Digits!");
            display.display();
            delay(1000);
            display.clearDisplay();
            display.setCursor(6, 15);
            display.println(buff_string);
            display.display();
          }
          
}

void end_trip()
{
   if(end_trip_call())
     {
        // display.clearDisplay();
         display.setCursor(6, 15);
         display.println("Trip Ended");
         display.display();
         buff_var[0] = 0;
         delay(1000);
//         display.clearDisplay();
//         display.display();
         buff_string = "";
         
      }
      else
         {
           display.clearDisplay();
           display.setCursor(6, 15);
           display.println("Not ended");
           display.display();
           delay(1000);
           display.clearDisplay();
           display.display();
         }
          
}
