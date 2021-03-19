
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
#include <Effortless_SPIFFS.h>
using namespace std;
     
TFT_eSPI tft = TFT_eSPI();         // Invoke custom library

eSPIFFS fileSystem = eSPIFFS ();
float myVariable;


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
  
  Serial.println("\r\nInitialisation done."); 
  tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)

  const uint16_t bootWidth = 412;
  const uint16_t bootHeight = 186;
  File fs =  SPIFFS.open("/jpg-300.raw", "r" );
  if ( fs <= 0 ){
    SPL("File doesn't open\n");
    return;
  }
  
  SPL("Size : " + String(fs.size()) );


size_t fileSize = fileSystem.getFileSize("/jpg-300.raw");


char *fileContents;  // Dont forget about the null terminator for C Strings

SPL("Size-->> : " + String(fileSize) );
fileSystem.openFile("/jpg-300.raw", fileContents, fileSize);
//fileContents[fileSize] = '\0';
Serial.print("Data stored in fileContents is: ");
//Serial.println(fileContents);
 tft.setSwapBytes(true);
tft.pushImage(0, 0, 479, 320, (uint8_t *)fileContents ); 


delete[] fileContents;
}

void loop() {
//  TJpgDec.drawFsJpg(0, 0, "/AA.jpg");
}
