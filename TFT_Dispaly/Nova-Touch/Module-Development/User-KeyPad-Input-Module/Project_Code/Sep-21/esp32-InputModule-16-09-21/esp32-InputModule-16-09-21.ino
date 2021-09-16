
#include "UserInputKeyBoard.h"

TFT_eSPI tft = TFT_eSPI();

class userKeyBoard kbd;
char userInputArray[50];


void setup() {

  Serial.begin(115200);
  char buff[70];
  kbd.tftInit(); 
  Serial.println("===### " + String( kbd.takeStringInput( buff, 70 ) ) );
 
}


void loop() {
 
}
