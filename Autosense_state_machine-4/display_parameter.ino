void OLED_param()
{
  // Set Initial Oled parameters
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  // Display Flux Logo on Boot
  display.drawBitmap(0, 0, flux_logo, 128, 64, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  display.display();
  delay(1000);

  // Display AutoSense
  display.clearDisplay();
  display.setCursor(6, 0);
  display.println("Auto-Sense");
  display.display();
  display.setTextSize(1);
  delay(800);
  display.clearDisplay();
  
}
