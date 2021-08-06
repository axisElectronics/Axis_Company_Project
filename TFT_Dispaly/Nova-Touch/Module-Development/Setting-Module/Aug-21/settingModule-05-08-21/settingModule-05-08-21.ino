#include "settings.h"

#include "SPI.h"
#include "XPT2046_Touchscreen.h"

#define TOUCH_CS_PIN       5
#define TOUCH_IRQ_PIN      27

XPT2046_Touchscreen touch( TOUCH_CS_PIN, TOUCH_IRQ_PIN );


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


SPIClass SPI_H = SPIClass(HSPI);

TS_Point rawLocation;
uint16_t x;
uint16_t y; 
uint8_t z;




void setup() {

  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Weight Mechine
  SPI.beginTransaction(SPISettings(80000000, MSBFIRST, SPI_MODE0));

    
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  uint16_t calData[5] = { 329, 3444, 448, 3193, 2 };
  tft.setTouch(calData);

//  touch.begin();
//  Serial.println( "Touch screen ready." );
//  delay(2000);
//  tft.fillScreen(TFT_RED);

                 
  //  set.info();
  set.settingPageInit(  );
  set.settingPageHandler( );

}




void loop() {
  
}
