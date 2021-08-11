#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <TFT_eSPI.h> // Hardware-specific library

 // Include the jpeg decoder library
#include <TJpg_Decoder.h> 

SPIClass spiSD(HSPI);
#define SD_CS 15

TFT_eSPI tft = TFT_eSPI();
/*<! ========================================================== !>*/

void checkFile(const char *path){
  if (SD.exists(path)) {
  File myFile = SD.open(path); 
  Serial.printf(" File <%s> exists and its size is <%ld>\n ", path, myFile.size() );
  myFile.close();
}
else {
  Serial.printf(" File %s doesn't exist...!!!\n" );
}

}

/*<! ========================================================== !>*/

void readFile(const char *path, char *userBuff){
File myFile;
myFile = SD.open( path, FILE_READ);
uint32_t fsize = myFile.size();
int32_t len = fsize;
Serial.println("File Size to read : " + String(fsize) );

/* Read file */
//for( uint32_t i=0 ; ( i < fsize ) && ( myFile.available() ) ; ){ 
//  userBuff[i++] = (char)myFile.read(); 
////Serial.write((char)myFile.read()); 
//  yield();
//}
int32_t spliteFrame = 0;
while(len > 0){
  if( fsize > 512 ){
    fsize = 512;
  }
  myFile.read((uint8_t *)&userBuff[spliteFrame], fsize);
  len = len - fsize;
  spliteFrame += fsize;
  yield();
}
myFile.close();
}

/*<! ========================================================== !>*/

void writeFile(const char *path, const char *userBuff){
File myFile;  
uint32_t fsize = strlen(userBuff);
int32_t len = fsize;

/* write file */
Serial.println("Buffer Size to write : " + String( fsize ) );
myFile = SD.open(path, FILE_WRITE);

//if (myFile) { 
//    for(uint32_t i=0; i < size; i++ ){
//      myFile.write( userBuff[i] );
//      yield();
//    }
//    myFile.print('\n');
//  }
int32_t spliteFrame = 0;
while(len > 0){
  if( fsize > 512 ){
    fsize = 512;
  }else{
     fsize = len;
  }
  
  myFile.write((uint8_t *)&userBuff[spliteFrame], fsize);
  len = len - fsize;
  spliteFrame += fsize;
  yield();
}
myFile.print('\n');
myFile.close(); 
}

/*<! ========================================================== !>*/
 #define FIZE_SIZE 4096
void copyFile(char *destpath, const char *srcpath){
 
  File srcFile, destFile;
  uint8_t tempbuff[FIZE_SIZE];
  memset(tempbuff, '\0', FIZE_SIZE);
  
  destFile = SD.open(destpath, FILE_WRITE);
  srcFile = SD.open(srcpath, FILE_READ);
  uint32_t fsize = srcFile.size();
  int32_t len = fsize;
  int32_t spliteFrame = 0;
  uint32_t nextblocktowrite=0;
  Serial.printf("Source File size = %ld\n", len);
  
  while( len ){
    if( len > FIZE_SIZE ){
      fsize = FIZE_SIZE;
    }else{
      fsize = len;
    }
    srcFile.read(tempbuff, fsize);
    destFile.write(tempbuff, fsize );
    len -= fsize;
    //SD.seek(nextblocktowrite);
    yield();
  }
  destFile.close();
  srcFile.close();
  Serial.println("File writing completed ...!!!");
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
if (!SD.begin( SD_CS, spiSD  )) {
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
 tft.setRotation(2);  // landscape

}//end-setup

void loop() {
//char str[4096];
//memset(str, '\0', 4096);
//
//checkFile("/test.txt");
//uint32_t timeout = millis();
////readFile("/test.txt", str );
////writeFile("/test_1.txt", str);
//copyFile("/test_1.txt", "/walk.h");
//Serial.printf("Timout : %ld\n", millis()-timeout);
//delay(1000);
 uint16_t w = 0, h = 0;
  char imagebuff[50];

//  tft.fillScreen(TFT_BLACK);
//  TJpgDec.getSdJpgSize(&w, &h, "/images/flower.jpg" );
//  // Draw the image, top left at 0,0
//  TJpgDec.drawSdJpg(0, 0, "/images/flower.jpg");
// delay(2000);


// Get the width and height in pixels of the jpeg if you wish
//tft.fillScreen(TFT_BLACK); 
for(uint8_t i=1; i < 30; i++ )
{ 
  sprintf(imagebuff, "/walk/%d.jpg", i);
//  Serial.printf("==>> %s\n", imagebuff );
  TJpgDec.getSdJpgSize(&w, &h, imagebuff );
  // Draw the image, top left at 0,0
  TJpgDec.drawSdJpg(0, 0, imagebuff );
  delay(1);
yield();
}


yield();
}
