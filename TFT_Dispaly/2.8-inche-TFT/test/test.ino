
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();



void setup() {
  Serial.begin(9600); // DEBUG WINDOW
//  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1); // Keypad interface
//  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Weight Mechine
  SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));

  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  uint16_t calData[5] = { 329, 3444, 448, 3193, 2 };
  tft.setTouch(calData);
  tft.fillScreen(TFT_BLACK);
  delay(2000);
 tft.fillScreen(TFT_GREEN);
  delay(1000);

  
}

void loop() {
 
}
