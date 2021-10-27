#ifndef _Display_H
#define _Display_H
#include "TFT_eSPI.h"

typedef enum e_Display {
  typeOne,
  typeTwo
};

extern TFT_eSPI tft;

class DisplayInit {

private:

// Code to run a screen calibration, not needed when calibration values set in setup()
void touch_calibrate(){
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // Calibrate
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.println("Touch corners as indicated");

  tft.setTextFont(1);
  tft.println();

  tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

  Serial.println(); Serial.println();
  Serial.println("// Use this calibration code in setup():");
  Serial.print("  uint16_t calData[5] = ");
  Serial.print("{ ");

  for (uint8_t i = 0; i < 5; i++)
  {
    Serial.print(calData[i]);
    if (i < 4) Serial.print(", ");
  }

  Serial.println(" };");
  Serial.print("  tft.setTouch(calData);");
  Serial.println(); Serial.println();

  tft.fillScreen(TFT_BLACK);
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Calibration complete!");
  tft.println("Calibration code sent to Serial port.");

  delay(4000);
}




public:

void RpiDisplayInit(e_Display num){
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  
  switch(num){
    case typeOne :
     {
      uint16_t calData[5] = { 262, 3565, 1319, 2532, 2 }; 
      tft.setTouch(calData);
     }
    break;

    case typeTwo :
     {
      uint16_t calData[5] = { 292, 3504, 467, 3298, 2 };
      tft.setTouch(calData);
     }
    break;
  }//end-switch

  tft.fillScreen(TFT_BLACK);
  
}//end-RpiDisplayInit

void DisplayCalibration(){
    // Use serial port
  Serial.begin(115200);
  // Initialise the TFT screen
  tft.init();

  // Set the rotation to the orientation you wish to use in your project before calibration
  // (the touch coordinates returned then correspond to that rotation only)
  tft.setRotation(1);

  // Calibrate the touch screen and retrieve the scaling factors
  touch_calibrate();
}//end-DisplayCalibration

};//end-DispalyInit
#endif
