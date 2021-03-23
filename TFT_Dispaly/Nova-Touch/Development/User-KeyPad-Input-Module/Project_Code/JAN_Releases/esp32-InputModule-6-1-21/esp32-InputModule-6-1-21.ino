
#include "UserInputKeyBoard.h"
#include "keyBoardImage.h"


extern char findChar( int8_t idx );

TFT_eSPI tft = TFT_eSPI();

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

char userInputArray[20];
int8_t entIdx = 0;
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
  memset(userInputArray, '\0', 20);
}

int8_t inc=0;

void loop() {
  tft.setFreeFont( mplus_B20 );
  tft.setTextSize(1);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  int8_t temp = findChar( idx );
//  Serial.println("Temp : " + String(temp) );
  if (temp != -20)
  {
    ++idx;
    if ( entIdx < 20 )
    {
      userInputArray[entIdx++] = temp;
    }
    else
    {
      entIdx = 0;
      userInputArray[entIdx++] = temp;
    }    
    Serial.println("--->>> " + String( userInputArray ) );
//    tft.fillRect(12 , 18, 100, 50, TFT_WHITE);
    if( strlen( userInputArray ) > 9 )
      tft.drawString( &userInputArray[++inc], 12,18,1);  
    else
     tft.drawString( userInputArray, 12,18,1); 
  }
}
