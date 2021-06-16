#include "settings.h"

#define RXD1 32
#define TXD1 33

#define RXD2 16
#define TXD2 17


/****** Class Declaration ********/
TFT_eSPI tft = TFT_eSPI();

class settings set;

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
   // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // Return 1 to decode next block
  return 1;
}

void setup() {

  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Weight Mechine
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  uint16_t calData[5] = { 329, 3444, 448, 3193, 2 };
  tft.setTouch(calData);

  //  set.info();
  set.settingPageInit(  );
  set.settingPageHandler( );

}

void loop() {



}
