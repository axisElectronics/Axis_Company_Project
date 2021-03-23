
#include "commonGlobal.h"

TFT_eSPI tft = TFT_eSPI();
class WeighingHandle Wtft;

char src[12];
char dest[10];


void setup() {

  Wtft.initWeighingTFT( );
  Wtft.initTFTHandler ( );

  strcpy( Wtft.showDigits.preValue[0], "ABCDEFGH" );
  strcpy( Wtft.showDigits.preValue[1], "ABCDEFGH" );
  strcpy( Wtft.showDigits.preValue[2], "ABCDEFGH" );
  Wtft.showDigits.dotPosition = 3;





}

void loop() {

  strcpy(src, Wtft._readbuf( ).c_str() );
  strcpy(src, Wtft.handleFlags( src ).c_str() );
  bufferWithoutDot(dest, src);

  if ( strlen(dest) > 5 )
  {
    SPL("--->>> " + String( dest ) );
    strcpy( Wtft.showDigits.currentValue, dest );
    Wtft.windowOne( );
    strcpy( Wtft.showDigits.currentValue, dest );
    Wtft.windowTwo( );
    strcpy( Wtft.showDigits.currentValue, dest );
    Wtft.windowThree( );
  }
}
