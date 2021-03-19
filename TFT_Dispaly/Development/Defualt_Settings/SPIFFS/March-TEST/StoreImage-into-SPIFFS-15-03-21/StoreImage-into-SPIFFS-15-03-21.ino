
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
//#include <TJpg_Decoder.h>
//#include <Effortless_SPIFFS.h>
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


void drawRAW(char *filename, int16_t x, int16_t y, int16_t rawWidth, int16_t rawHeight);

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


  size_t fileSize = fileSystem.getFileSize("/jpg-300.raw");
  char fileContents[1024];  // Dont forget about the null terminator for C Strings

  SPL("Size-->> : " + String(fileSize) );
  //fileSystem.openFile("/jpg-300.raw", fileContents, fileSize);
  //fileContents[fileSize-1] = '\0';
  //Serial.print("Data stored in fileContents is: ");

  fs = SPIFFS.open("/jpg-300.raw", "rb+");

 // filesize /= 2;
  tft.setSwapBytes(true);
  if (fs)
  {
    char pix[2]= {0};
    uint32_t x=0, y=0;
    for (uint32_t i = 0; i < fileSize; i++)
    {
      pix[1] = 0xF1;
      pix[0] =  0x1A;
      tft.drawPixel( x++, y++, &pix);
    }
  }

//  tft.setSwapBytes(true);
//  tft.pushImage(0, 0, 231, 121, (uint16_t *)fileContents );
  //delete[] fileContents;



  //drawRAW("/jpg-100.raw", 0, 0, 231, 154);
}

void loop() {
//  TJpgDec.drawFsJpg(0, 0, "/jpg-300.jpg");
//  SPL("====>>>> ");
//  tft.fillScreen(TFT_BLACK);
}

// This function opens a ".raw" image file and displays it at the given coordinates.
// It is faster than plotting BMP images as the file is already in the correct
// format to pipe directly to the display.
// Uses same BUFF_SIZE as BMP function.
// The width and hsight of the image in pixels must be passed to the function
// as these parameters are not in the file
#define BUFF_SIZE 1000
void drawRAW(char *filename, int16_t x, int16_t y, int16_t rawWidth, int16_t rawHeight) {
  File     rawFile;
  uint8_t  sdbuffer[2 * BUFF_SIZE];   // SPIFFS read pixel buffer (16 bits per pixel)

  // Check file exists and open it
  if ((rawFile = SPIFFS.open(filename)) == NULL) {
    Serial.println(F("File not found"));
    return;
  }

  unsigned int drawTime = millis(); // Save current time for performance evaluation

  tft.setAddrWindow(x, y, x + rawWidth - 1, y + rawHeight - 1);

  // Work out how many whole buffers to send
  uint16_t nr = ((long)rawHeight * rawWidth) / BUFF_SIZE;
  while (nr--) {
    rawFile.read(sdbuffer, sizeof(sdbuffer));
    tft.pushColors(sdbuffer, BUFF_SIZE);
  }

  // Send any partial buffer
  nr = ((long)rawHeight * rawWidth) % BUFF_SIZE;
  if (nr) {
    rawFile.read(sdbuffer, nr << 1); // We load  2 x BUFF_SIZE bytes
    tft.pushColors(sdbuffer, nr);      // We send BUF_SIZE pixels
  }

  drawTime = millis() - drawTime;
  rawFile.close();
}
