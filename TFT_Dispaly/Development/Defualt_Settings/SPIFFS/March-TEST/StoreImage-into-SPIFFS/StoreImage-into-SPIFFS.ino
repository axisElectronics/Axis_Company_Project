
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <SPI.h>
#include <TFT_eSPI.h>
#include <String.h>
#include <vector>
#include "Arduino.h"
#include <iostream>
// Include the jpeg decoder library
#include <TJpg_Decoder.h>
using namespace std;
     
TFT_eSPI tft = TFT_eSPI();         // Invoke custom library


#define SPL Serial.println
#define SP Serial.print

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

  Serial.begin(9600); // DEBUG WINDOW
  //  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1); // Keypad interface
  //  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Weight Mechine
  SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));

  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  uint16_t calData[5] = { 329, 3444, 448, 3193, 2 };
  tft.setTouch(calData);
  tft.fillScreen(TFT_BLACK);

  // Initialise SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }
  //SPIFFS.flush();
  Serial.println("\r\nInitialisation done."); 
  tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)

  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);

  // Draw the image, top left at 0,0
  TJpgDec.drawFsJpg(0, 0, "/jpg-100.jpg");
//   TJpgDec.drawFsJpg(0, 0, "/ABC.jpg");
    delay(2000);
}

void loop() {
//  TJpgDec.drawFsJpg(0, 0, "/AA.jpg");
}
