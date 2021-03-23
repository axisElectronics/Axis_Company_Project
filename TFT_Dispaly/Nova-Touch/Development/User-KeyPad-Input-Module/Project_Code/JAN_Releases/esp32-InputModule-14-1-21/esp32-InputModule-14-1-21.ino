

#include "UserInputKeyBoard.h"


TFT_eSPI tft = TFT_eSPI();
class userKeyBoard kbd;



struct _userInput userInput;
struct _fixFormat userFixFormat;

char userInputArray[50];
/*
 * @ customKeypad :
 *    ++ This function is responsible for making customization in Keypad attributes.
 *    ++ These keypad attributes will take effect after default values.
 *    ++ It is user resposiblity to take care this Functions.
 */
void customKeypad( struct _userInput *userInput )
{
//  userInput->invalidKey[0] = dot;
//  userInput->invalidKey[1] = one;
  userInput->noticBoard_FGColor = TFT_RED;

  /* Fix Format data */
  userInput->fixFormatstruct = &userFixFormat;
  if( userInput->fixFormatstruct )
  {
    userInput->fixFormatstruct->fixFormatArraySize = 50;
    userInput->fixFormatstruct->fixFormatFlag = 1;
    userInput->fixFormatstruct->fieldType = Password;
  }

   
  // Don't Erase this line it is call back function used with other functionality.
  userInput->userCustomKeypad = customKeypad;
}


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
   *  
   *  + call "keypadParamInit" function with appropriate structure, 
   *    ++  User Array and Its Size must be assign to structure. Don't call "keypadParamInit" function without assigning array.
   *    ++  User Array size must be greater then 16 Bytes.
   *  + Call "takeUserInput" function with appropriate structure and get user input by pressing Enter button.
   *  +      
   *  
   *  # para[0] = Gloabal Array which User want to get data
   *  # para[1] = Size of This given User Array.
   *  # para[2] = invalid keys. This arrays has 11 spaces to accomadate keys. It is user resposiblity to 
   *  # 
   * 
   *  @@ button hide[ Done ]
   *  @@ field Lable [ Done ]
   *  
   *  @@ min and max
   *  @@ Default value by develpoer
   *  @@ Format DD/MM/YY only cursor
   *  
   *  
   *  
   */

  userInput.userInputArray_Size = 50;
  userInput.userInputArray = new char[50];
 
  keypadParamInit( &userInput );
  customKeypad( &userInput );

  kbd.tag.stepUser = step_del;
  
}


void loop() {
  
  
  takeUserInput( &userInput );

//  userInputFixFormat( &userInput );
  
}
