
#include "UserInputKeyBoard.h"

TFT_eSPI tft = TFT_eSPI();

class userKeyBoard kbd;
char userInputArray[50];


void setup() {

  Serial.begin(115200);
  
  kbd.tftInit(); 
  Serial.println("===### " + String( kbd.takeStringInput() ) );
 
}


void loop() {
 
}
