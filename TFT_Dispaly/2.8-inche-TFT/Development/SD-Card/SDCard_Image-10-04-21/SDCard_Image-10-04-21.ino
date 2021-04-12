

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

#include "FS.h"
#include "SD.h"

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

  SPIClass SPI1 = SPIClass(HSPI);
  SPI1.begin(); 
   
  if (!SD.begin(15, SPI1)) {
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

 // Include the jpeg decoder library
#include <TJpg_Decoder.h> 

}

void loop() {
  tft.setRotation(1);  // landscape
  tft.fillScreen(TFT_BLACK);
  drawSdJpeg("/horse.jpg", 0, 0);     // This draws a jpeg pulled off the SD Card
  
  delay(2000);

}
