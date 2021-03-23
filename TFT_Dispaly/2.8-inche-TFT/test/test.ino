
#include <TFT_eSPI.h>
#include "FS.h"
#include "SPIFFS.h"
#include <SPI.h>
#include "iostream"
using namespace std;

TFT_eSPI tft = TFT_eSPI();

uint16_t xAxis, yAxis;
// This is the file name used to store the calibration data
// You can change this to create new calibration files.
// The SPIFFS file name must start with "/".
#define CALIBRATION_FILE "/TouchCalData1"

// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false
 
void touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;


  // check file system exists
  if (!SPIFFS.begin()) {
    Serial.println("Formating file system");
    SPIFFS.format();
    SPIFFS.begin();
  }

  // check if calibration file exists and size is correct
  if (SPIFFS.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL)
    {
      // Delete if we want to re-calibrate
      SPIFFS.remove(CALIBRATION_FILE);
    }
    else
    {
       fs::File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL) {
    // calibration data valid
    tft.setTouch(calData);
  } else {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
     fs::File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}



void setup() {
  Serial.begin(9600); // DEBUG WINDOW
//  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1); // Keypad interface
//  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Weight Mechine
  SPI.beginTransaction(SPISettings(80000000, MSBFIRST, SPI_MODE0));

  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);

  touch_calibrate();
  tft.fillScreen(TFT_BLACK);
  delay(2000);
 tft.fillScreen(TFT_GREEN);
  delay(5000);
   tft.fillScreen(TFT_RED);

  
}

void loop() {
xAxis = yAxis = 0;
  if( tft.getTouch( &xAxis, &yAxis) )
  {
      Serial.println("x: " + String(xAxis) + "  y : " + String(yAxis) );
  }
 else
 {
    Serial.println("hello");
 }

}
