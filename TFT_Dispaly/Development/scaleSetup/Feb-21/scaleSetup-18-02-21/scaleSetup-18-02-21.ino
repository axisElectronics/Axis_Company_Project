
#include "commonGlobal.h"

TFT_eSPI tft = TFT_eSPI();
class WeighingHandle Wtft;

char src[12];
char dest[10];


void setup() {

  Wtft.initWeighingTFT( );
  Wtft.initTFTHandler ( );

  Wtft.showDigits.digitFontSize  = 1;
  Wtft.showDigits.digitFontStyle = LED7SEG_STD40;
  strcpy( Wtft.showDigits.preValue, "ABCDEFGH" );
  Wtft.showDigits.cntBlankRect   = 2;
  Wtft.showDigits.fontDigitColor   = TFT_GREEN;
  Wtft.showDigits.backDigitColor   = makeCustomColor(10, 10, 10)

}

void loop() {

  strcpy(src, Wtft._readbuf( ).c_str() );
  bufferWithoutDot(dest, src);
  strcpy( Wtft.showDigits.currentValue, dest );

  if ( strlen(dest) > 5 )
  {
    memmove( &dest[1], &dest[0], strlen(dest) );
    dest[0] = '0';
    dest[7] = '\0';
    SPL("-->>> " + String( dest ) );
    Wtft. Testfun( );
  }
}

void WeighingHandle :: Testfun( )
{
  // 1. Set Font Size
  tft.setTextSize( showDigits.digitFontSize );
  tft.setFreeFont( (const GFXfont *)showDigits.digitFontStyle );

  // 2. set color
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  //3. check with previous Value
  if ( !strcmp( showDigits.preValue , showDigits.currentValue ) ) return;


  SPL("current : " + String( showDigits.currentValue ) );

  //4. draw blank rectangle only those digits which is different from previous Value.
  for (uint8_t idx = 0; idx < 7; ++idx )
  {
    if ( showDigits.preValue[idx] !=  showDigits.currentValue[idx] )
    {
      tft.fillRect( 15  + ( idx * 65),  58,   62,  90,  TFT_BLACK);
    }
  }
  //5. remove all previous dots and redraw at given position;
  for (uint8_t idx = 0; idx < 6; ++idx )
  {
    tft.fillCircle( 73 + ( idx * 65 ), 138, 8, TFT_RED);
  }
  //6. draw Digits only,, Those digits which is different from previous Value.
  for (uint8_t idx = 0; idx < 7; ++idx )
  {
    if ( showDigits.preValue[idx] !=  showDigits.currentValue[idx] )
    {
      tft.drawChar( showDigits.currentValue[idx], 15 + ( idx * 65), 130,  1);
    }
  }

  // end : update preVlaue
  strcpy( Wtft.showDigits.preValue, showDigits.currentValue );
}
