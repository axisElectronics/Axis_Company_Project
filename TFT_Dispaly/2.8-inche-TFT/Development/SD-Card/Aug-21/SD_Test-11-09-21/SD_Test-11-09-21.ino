
#include "header.h"

SPIClass spiSD(HSPI);
#define SD_CS 15

TFT_eSPI tft = TFT_eSPI();





/*<! ========================================================== !>*/




void setup() {
  //========================================================
  //         Serial Begin
  //========================================================  
Serial.begin(115200);
Serial.setRxBufferSize(2048);

 //========================================================
  //         TFT Begin
  //========================================================
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  //========================================================
  //        SD card Begin
  //  we are using HSPI for fetching data from SD card.
  //========================================================

/* SPI settings */
uint32_t freq = 80*1000000; //80MHZ
SPISettings settingsA(8000000, MSBFIRST, SPI_MODE1);
spiSD.begin(14, 33, 13, 15); //SCK,MISO,MOSI,SS //HSPI1
SPI.beginTransaction(settingsA);
//spiSD.setFrequency(freq);

/* SD card Settings */
//if (!SD.begin( SD_CS, spiSD  )) {
  Serial.println("Card Mount Failed");
}else{
  Serial.println("Card Mount Successed ....!!!");
}

File myFile = SD.open("/");
myFile.close();

myFile = SD.open("/test.txt", FILE_WRITE);
  if (myFile) { 
    Serial.println("Writing to test.txt...");
//    myFile.print("HELLO WORLD"); 
for(uint32_t i=0; i < 1000; i++ ){
   myFile.print(i);
   myFile.print(',');
}
    myFile.print('\n');
}
myFile.close();


 //========================================================
  //       Start TJpg Decoder
  //========================================================
 tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)

  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);
 tft.setRotation(1);  // landscape

}//end-setup


void loop(){
  camImage_DispalyTFT();

  yield();
}
