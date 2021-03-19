
#include "commonGlobal.h"

//extern struct _showDigits showDigits;

TFT_eSPI tft = TFT_eSPI();
class WeighingHandle Wtft;

char src[12];
char dest[10];


void setup() {

  Wtft.initWeighingTFT( );
  Wtft.initTFTHandler ( );
 
}

void loop() {
  memset(dest, '\0', 10);
  memset(src, '\0', 10);

  strcpy(src, Wtft._readbuf( ).c_str() );

  bufferWithoutDot(dest, src);
  src[7] = '\0';

  if ( strlen(dest) > 5 )
  {
    Wtft._updateNetWeight( src );
   
    Wtft._updateWindow( NET );
    Wtft._updateWindow( GROSS );
  }
}
