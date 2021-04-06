#include "customFontHeaders.h"

TFT_eSPI tft = TFT_eSPI();

void setup() {
  
  Serial.begin(9600);
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  uint16_t calData[5] = { 329, 3444, 448, 3193, 2 };
  tft.setTouch(calData);

  
  
}

void loop() {
  
}
