
#include "UserInputKeyBoard.h"

TFT_eSPI tft = TFT_eSPI();

class userKeyBoard kbd;
char userInputArray[50];

void setup() {

  Serial.begin(9600);
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  uint16_t calData[5] = { 329, 3444, 448, 3193, 2 };
  tft.setTouch(calData);

  kbd.userInput.userInputArray_Size = 25;
  kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];

  kbd.init();
  kbd.userInput.numericSwitchFlag = ALPHANUM; 
//   kbd.userInput.fixFormatstruct->fixFormatFlag = 1;
}


void loop() {
  Serial.println("Hello :");  
  kbd.takeUserInput("30000" );
  Serial.println("World : " + String(kbd.userInput.fixFormatstruct->fixFormatArray));
  kbd.userInput.fixFormatstruct->fixFormatFlag = 1;
}
