
#ifndef _ERROR_H
#define  _ERROR_H


#include "headerFiles.h"
#include "commonGlobal.h"

extern class TFT_eSPI tft;

class classError {

private:

public :

void erashDigits(){
  //1. make setup
  tft.setTextSize( 1 );
  tft.setFreeFont( LED7SEG_STD40 );
  
  //2. erash all previous data
  for (uint8_t idx = 0; idx < 7; ++idx ){    
    tft.fillRect( 15  + ( idx * 65),  58,   62,  90,  TFT_BLACK);
 }

  //3. remove all previous dots and redraw at given position;
  for (uint8_t idx = 0; idx < 6; ++idx ){
    tft.fillCircle( 73 + ( idx * 65 ), 138, 6, TFT_BLACK);
}
  //4. draw character
  char arr[10] = "--O L--";
  for (uint8_t idx = 0; idx < 7; ++idx )
    tft.drawChar( arr[idx], 15 + ( idx * 65), 130,  1);

  delay(1000);

  //5. erash all previous data
  for (uint8_t idx = 0; idx < 7; ++idx ){    
    tft.fillRect( 15  + ( idx * 65),  58,   62,  90,  TFT_BLACK);
 }
  
}//end-erashDigits
  
};


#endif
