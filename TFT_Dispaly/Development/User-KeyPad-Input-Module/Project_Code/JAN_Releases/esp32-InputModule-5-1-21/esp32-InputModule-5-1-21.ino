
#include "UserInputKeyBoard.h"
#include "keyBoardImage.h"
#include "EurostileBold20pt7b.h"

#define EUROSTILE_B20   &EurostileBold20pt7b
#define BlankRect()  tft.fillRect(40, 10, 60, 60, TFT_BLACK);

TFT_eSPI tft = TFT_eSPI();

bool testingImage()
{
   tft.setSwapBytes(true);  
   tft.pushImage(0, 0, PLAX_IT_KEYSImage_Width, PLAX_IT_KEYSImage_Hight, PLAX_IT_KEYSArray ); 
}


void setup() {
  
 Serial.begin(9600);
 tft.init();
 tft.fillScreen(TFT_BLACK);
 tft.setRotation(1); 
 uint16_t calData[5] = { 329, 3444, 448, 3193, 2 };
 tft.setTouch(calData);
 tft.fillScreen(TFT_BLACK); 

  testingImage();
     
}

void loop() {

uint16_t xAxis=0, yAxis=0;
uint8_t cnt = 0;
char keyArray[10][4] = { {'0',' ', '/', '@' },
                         {'1','A', 'B', 'C' },
                         {'2','D', 'E', 'F' },
                         {'3','G', 'H', 'I' },
                         {'4','J', 'K', 'L' },
                         {'5','M', 'N', 'O' },
                         {'6','P', 'Q', 'R' },
                         {'7','S', 'T', 'U' },
                         {'8','V', 'W', 'X' },
                         {'9','Y', 'Z', '-' },
                       };

    tft.setFreeFont(EUROSTILE_B20);
    tft.setTextSize(1);
    tft.setTextColor(TFT_RED, TFT_BLACK);

  while( tft.getTouch(&xAxis, &yAxis) )
  {
    int8_t keyvalue = _findkeypadTouch( xAxis, yAxis);
    
    if( keyvalue != -20  )
    {
            
      int tout = millis();
      int8_t tempKeyvalue = -20;
      BlankRect()
      tft.drawChar(keyArray[keyvalue][cnt], 50, 50, 1);
      Serial.println( String( keyArray[keyvalue][cnt] ) );
      while( ( millis() - tout ) < 1000 )
      {
        xAxis = 0;
        yAxis = 0; 
        tft.getTouch(&xAxis, &yAxis);
        tempKeyvalue = _findkeypadTouch( xAxis, yAxis);
             
        if( keyvalue == tempKeyvalue )
        {
          cnt++;
          tout = millis();
          while( cnt >= 4) cnt = cnt-4;
          BlankRect()
          tft.drawChar(keyArray[keyvalue][cnt], 50, 50, 1);
          Serial.println( String( keyArray[keyvalue][cnt] ) );
          delay(200);       
        }
      }
      
    }
    Serial.println( String( "IN WHILE LOOP : " ) );
    
    
    
  
    
    yield();
  }

}
