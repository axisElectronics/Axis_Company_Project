#include "settings.h"

/****** Class Declaration ********/
TFT_eSPI tft = TFT_eSPI();

class settings set;
struct s_setting basesetting;


void setup() {

  Serial.begin(9600);
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
