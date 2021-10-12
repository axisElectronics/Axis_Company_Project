
#include "HeaderInput.h"

class headerInput headerStr;
TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  char buff[70] = "0123456789ABCDEFG";
  headerStr.tftInit(); 
  Serial.println("===### " + headerStr.takeStringInput(buff, 70) );

}

void loop() {
  

}
