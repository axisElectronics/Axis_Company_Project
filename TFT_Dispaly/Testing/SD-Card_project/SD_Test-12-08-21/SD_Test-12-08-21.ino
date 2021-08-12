
#include "headerFiles.h"
#include "examples.h"


TFT_eSPI tft = TFT_eSPI();

void setup() {
  //========================================================
  //         Serial Begin
  //========================================================  
  Serial.begin(115200);

  TFTinit();
  SDCardInit();
  startJPEGDecoder();

}//end-setup



void loop() {
//example_3();
showSdImageOnTFT( "/images/flower.jpg" );
delay(2000);
yield();
}
