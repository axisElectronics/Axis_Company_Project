

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

#include "FS.h"
#include "SD.h"

 // Include the jpeg decoder library
#include <TJpg_Decoder.h> 

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}


// This next function will be called during decoding of the jpeg file to
// render each block to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
   // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // This might work instead if you adapt the sketch to use the Adafruit_GFX library
  // tft.drawRGBBitmap(x, y, bitmap, w, h);

  // Return 1 to decode next block
  return 1;
}






void setup() {
  //========================================================
  //         Serial Begin
  //========================================================
  Serial.begin(9600);

  //========================================================
  //         TFT Begin
  //========================================================
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  //========================================================
  //        SD card Begin
  //  we are using HSPI for fetching data from SD card.
  //========================================================

  //SPIClass SPI1 = SPIClass(HSPI);
 // SPI1.begin(); 
   
  if (!SD.begin(4)) { // using D4 GPIO for SD_card
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  //========================================================
  //       List SD card Files
  //========================================================
 listDir(SD, "/", 0);
 
  File root = SD.open("/sun1.png", "r+");
  Serial.println("Size : " + String( root.size() ) );
  root = SD.open("/horse.jpg", "r+");
  Serial.println("Size horse : " + String( root.size() ) );

  //========================================================
  //       Start TJpg Decoder
  //========================================================
 tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)

  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);
 tft.setRotation(1);  // landscape
}

void loop() {
 
  tft.fillScreen(TFT_BLACK);
  
// Get the width and height in pixels of the jpeg if you wish
  uint16_t w = 0, h = 0;
  TJpgDec.getSdJpgSize(&w, &h, "/flower-1.jpg");
//  Serial.print("Width = "); Serial.print(w); Serial.print(", height = "); Serial.println(h);

  // Draw the image, top left at 0,0
  TJpgDec.drawSdJpg(0, 0, "/flower-1.jpg");

  
  delay(2000);

  // Get the width and height in pixels of the jpeg if you wish
  TJpgDec.getSdJpgSize(&w, &h, "/try-2.jpg");
//  Serial.print("Width = "); Serial.print(w); Serial.print(", height = "); Serial.println(h);

  // Draw the image, top left at 0,0
  TJpgDec.drawSdJpg(0, 0, "/try-2.jpg");

  delay(2000);

  // Get the width and height in pixels of the jpeg if you wish
  TJpgDec.getSdJpgSize(&w, &h, "/test-1F.jpg");
//  Serial.print("Width = "); Serial.print(w); Serial.print(", height = "); Serial.println(h);

  // Draw the image, top left at 0,0
  TJpgDec.drawSdJpg(0, 0, "/test-1F.jpg");

  delay(2000);

}
