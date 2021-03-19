#include "settings.h"

#define RXD1 32
#define TXD1 33

#define RXD2 16
#define TXD2 17

/****** Class Declaration ********/
TFT_eSPI tft = TFT_eSPI();

class settings set;
struct s_setting basesetting;

void setup() {

  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Weight Mechine
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  uint16_t calData[5] = { 329, 3444, 448, 3193, 2 };
  tft.setTouch(calData);

  //  set.info();
  settingPageInit( &basesetting, settingMenuArray, &set );

  settingPageHandler( &basesetting );

}

void loop() {



}
