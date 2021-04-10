

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

#include "FS.h"
#include "SD.h"
#include "SD_MMC.h"

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
  //  TFT and SD_MMC card is not work on SPI pins.
  //========================================================
//  tft.init();
//  tft.setRotation(3);
//  tft.fillScreen(TFT_BLACK);
//
//  tft.setTextSize(1);
//  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  //========================================================
  //        SD card Begin
  //========================================================

  if (!SD_MMC.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD_MMC card attached");
    return;
  }

  Serial.print("SD_MMC Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
  Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);

  //========================================================
  //       List SD card Files
  //========================================================

  listDir(SD_MMC, "/", 0);

}

void loop() {
  // put your main code here, to run repeatedly:

}
