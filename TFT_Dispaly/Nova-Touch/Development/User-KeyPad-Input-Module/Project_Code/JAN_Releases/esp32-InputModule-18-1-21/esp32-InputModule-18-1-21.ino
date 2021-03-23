
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
  
  kbd.userInput.userInputArray_Size = 60;
  kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];
  kbd.init(&kbd.userInput);
   
  kbd.userInput.fixFormatstruct->fixFormatFlag = 1;
  kbd.userInput.fixFormatstruct->fixFormatArraySize = 15;  
  kbd.userInput.fixFormatstruct->fieldType = Date;
 
}


void loop() {
  Serial.println("Hello :");  
  kbd.takeUserInput( &kbd.userInput );
  Serial.println("World : " + String(kbd.userInput.fixFormatstruct->fixFormatArray));
  kbd.userInput.fixFormatstruct->fixFormatFlag = 1;
}
