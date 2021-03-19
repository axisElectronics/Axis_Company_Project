
#include "commonGlobal.h"

TFT_eSPI tft = TFT_eSPI();
class WeighingHandle Wtft;
struct _showDigits showDigits;
char src[12];
char dest[10];


void setup() {

  Wtft.initWeighingTFT( );
  Wtft.initTFTHandler ( );
  Wtft.showDigits = &showDigits;

  strcpy( Wtft.showDigits->preValue[0], "ABCDEFGH" );
  strcpy( Wtft.showDigits->preValue[1], "ABCDEFGH" );
  strcpy( Wtft.showDigits->preValue[2], "ABCDEFGH" );
  Wtft.showDigits->dotPosition = 2;

  Wtft._updateTareWeight( "0.00" );
}


void loop() {

 
  strcpy(src, Wtft._readbuf( ).c_str() );
  strcpy(src, Wtft.handleFlags( src ).c_str() );
  bufferWithoutDot( dest, src);
  src[7] = '\0';

  Wtft._updateGrossWeight( src );

  if ( strlen(dest) > 5 )
  {
    //    SPL("--->>> " + String( dest ) );
    Wtft._updateNetWindow( );

  }
}
