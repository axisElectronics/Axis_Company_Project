
#include "UserInputKeyBoard.h"
#include "keyBoardImage.h"

TFT_eSPI tft = TFT_eSPI();
extern int findChar( int8_t idx );

struct _userInput userInput;

char keyArray[10][4] = { 
  {'0', ' ', '/', '@' },
  {'1', 'A', 'B', 'C' },
  {'2', 'D', 'E', 'F' },
  {'3', 'G', 'H', 'I' },
  {'4', 'J', 'K', 'L' },
  {'5', 'M', 'N', 'O' },
  {'6', 'P', 'Q', 'R' },
  {'7', 'S', 'T', 'U' },
  {'8', 'V', 'W', 'X' },
  {'9', 'Y', 'Z', '-' },
};


bool testingImage()
{
  tft.setSwapBytes(true);
  tft.pushImage(0, 0, PLAX_IT_KEYSImage_Width, PLAX_IT_KEYSImage_Hight, PLAX_IT_KEYSArray );
}

char userInputArray[50];
int8_t entIdx = 15;
int8_t idx = 0;

void setup() {

  Serial.begin(9600);
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  uint16_t calData[5] = { 329, 3444, 448, 3193, 2 };
  tft.setTouch(calData);
  tft.fillScreen(TFT_BLACK);

  testingImage();
  
  userInput.userInputArray = userInputArray;
  userInput.userInputArray_Size = 50;
  userInput.entIdx = 15;
  userInput.text_xAxis   = 12;
  userInput.text_yAxis   = 25;
  userInput.text_FGcolor   = TFT_GREEN;
  userInput.text_BGColor =  TFT_BLACK;
  userInput.customFont = mplus_R13;
   
  memset( userInput.userInputArray, '\0', userInput.userInputArray_Size);
  memset( userInput.userInputArray, ' ',  15 );
  userInput.userInputArray[15] = '_';

  tft.setFreeFont( userInput.customFont );
  tft.setTextSize(1);
  tft.setTextColor(userInput.text_FGcolor,  userInput.text_BGColor =  TFT_BLACK);

  tft.drawString(userInput.userInputArray, 12,21,1);
  Serial.println("--->>> " + String( userInput.userInputArray ) );
}


void loop() {
  
  takeUserInput( &userInput );
}
