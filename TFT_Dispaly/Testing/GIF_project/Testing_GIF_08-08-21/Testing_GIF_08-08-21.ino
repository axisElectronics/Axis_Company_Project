#include <stdlib.h>
#include <stdint.h>
#include <SPI.h>
#include "TFT_eSPI.h" // Hardware-specific library
#include "walk.h"


TFT_eSPI tft = TFT_eSPI();

void setup() {
  // put your setup code here, to run once:
 tft.init();
  tft.setRotation(1);

//  tft.setSwapBytes(true);
  tft.fillScreen(TFT_WHITE);
  delay(2000);

 
}

void loop() {
 
for(int i=0;i<20;i++){
 
  tft.pushImage(60, 15,animation_width  , animation_height, walk[i]);
  }
 yield();
}
