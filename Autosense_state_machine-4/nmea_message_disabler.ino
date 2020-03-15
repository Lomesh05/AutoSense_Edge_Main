//Messsages to Disable NMEA Dataframes.
uint8_t disableGSA[] = {0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x31};
uint8_t disableGLL[] = {0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A};
uint8_t disableGSV[] = {0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x38};
uint8_t disableVTG[] = {0x06, 0x01, 0x08, 0x00, 0xF0, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x46};
uint8_t utc10Hz[]    = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x64, 0x00, 0x01, 0x00, 0x00, 0x00, 0x79, 0x10};
uint8_t gps10Hz[]    = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x64, 0x00, 0x01, 0x00, 0x01, 0x00, 0x7A, 0x12};
uint8_t glo10Hz[]    = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x64, 0x00, 0x01, 0x00, 0x02, 0x00, 0x7B, 0x14};
uint8_t bds10Hz[]    = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x64, 0x00, 0x01, 0x00, 0x03, 0x00, 0x7C, 0x16};
uint8_t gal10Hz[]    = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x64, 0x00, 0x01, 0x00, 0x04, 0x00, 0x7D, 0x18};

void sendUBX(uint8_t *MSG, uint32_t len, long timeout = 3000) {

  uint32_t CK_A = 0, CK_B = 0;
  uint8_t fullPacket[len + 4];

//  Serial.println();

  fullPacket[0] = 0xB5;
  fullPacket[1] = 0x62;

  //Calculate checksum
  for (int i = 0; i < len; i++) {
    fullPacket[i + 2] = MSG[i];
    CK_A = CK_A + MSG[i];
    CK_A &= 0xFF;
    CK_B = CK_B + CK_A;
    CK_B &= 0xFF;
  }

//  Serial.println("CK_A= " + String(CK_A));
//  Serial.println("CK_B= " + String(CK_B));

  fullPacket[len + 2] = CK_A; //Add the checksums to the end of the UBX packet
  fullPacket[len + 3] = CK_B;

//  Serial.println("fullPacket is:");

  for (int i = 0; i < (len + 4); i++) {
//    Serial.print(fullPacket[i], HEX); //Print out a byte of the UBX data packet to the serial monitor
//    Serial.print(", ");
    Serial.write(fullPacket[i]);//Send a byte of the UBX data packet to the GPS unit
  }
  Serial.flush();
//  Serial.println();
}

void nmea_message_disabler()
{
//  Serial.begin(9600);
//  Serial2.begin(9600);
  uint32_t len = sizeof(disableGLL) / sizeof(uint8_t);

  //Send the NMEA Dataframe Disabling Messages to the Ublox GPS Module
  sendUBX(disableGLL, len);
  delay(200);
  len = sizeof(disableGSA) / sizeof(uint8_t);
  sendUBX(disableGSA, len);
  delay(200);
  len = sizeof(disableGSV) / sizeof(uint8_t);
  sendUBX(disableGSV, len);
  delay(200);
  len = sizeof(disableVTG) / sizeof(uint8_t);
  sendUBX(disableVTG, len);
  delay(200);
  len = sizeof(utc10Hz) / sizeof(uint8_t);
  sendUBX(utc10Hz, len);
  delay(200);
  len = sizeof(gps10Hz) / sizeof(uint8_t);
  sendUBX(gps10Hz, len);
  delay(200);
  len = sizeof(glo10Hz) / sizeof(uint8_t);
  sendUBX(glo10Hz, len);
  delay(200);
  len = sizeof(bds10Hz) / sizeof(uint8_t);
  sendUBX(bds10Hz, len);
  delay(200);
  len = sizeof(gal10Hz) / sizeof(uint8_t);
  sendUBX(gal10Hz, len);
  delay(200);

  //GPS config done.
//  Serial.println("Finished Programming GPS");
}
