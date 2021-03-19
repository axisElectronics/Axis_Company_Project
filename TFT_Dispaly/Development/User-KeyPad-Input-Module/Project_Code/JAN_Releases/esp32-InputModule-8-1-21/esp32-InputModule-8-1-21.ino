
#include "UserInputKeyBoard.h"


TFT_eSPI tft = TFT_eSPI();


struct _userInput userInput;


char userInputArray[50];


void setup() {

  Serial.begin(9600);
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  uint16_t calData[5] = { 329, 3444, 448, 3193, 2 };
  tft.setTouch(calData);
  
  /* User can give following parameters :
   *  - userInputArray [ must ]
   *  - text_FGcolor
   *  - customFont
   *  + Call "takeUserInput" function with appropriate structure and get user input by pressing Enter button.
   *  + 
   *  
   *  @@ min and max
   *  @  button hide
   *  @@ default value by develpoer
   *  @@ field Lable
   *  @@ Format DD/MM/YY only cursor
   */
  userInput.userInputArray = userInputArray;
  userInput.userInputArray_Size = sizeof( userInputArray );
  
}


void loop() {
  
  takeUserInput( &userInput );
}
