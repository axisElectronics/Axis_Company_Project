
#include "headerFiles.h"

#define FIZE_SIZE 4096

#define H_SCK 14
#define H_MISO 33 //<* In palce of pin 12. if we use pin 12 then keep Low while booting esp32.> <* remove SD-card module >
#define H_MOSI 13
#define SD_CS 15

extern TFT_eSPI tft;
SPIClass spiSD(HSPI);

void TFTinit(){
  //========================================================
  //         TFT Begin
  //========================================================
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

}


void SDCardInit(){
  
  //========================================================
  //        SD card Begin
  //  we are using HSPI for fetching data from SD card.
  //========================================================
/* SPI settings */
uint32_t freq = 80*1000000; //80MHZ
SPISettings settingsA(8000000, MSBFIRST, SPI_MODE1);
spiSD.begin(H_SCK, H_MISO, H_MOSI, SD_CS); //SCK,MISO,MOSI,SS //HSPI1
//spiSD.beginTransaction(settingsA); // connection speed of SD card ...!!!! 
SPI.beginTransaction(settingsA);
spiSD.setFrequency(freq);

/* SD card Settings */
if (!SD.begin( SD_CS, spiSD  )) {
  Serial.println("Card Mount Failed");
}else{
  Serial.println("Card Mount Successed ....!!!");
}

File myFile = SD.open("/");
myFile.close();
}


void startJPEGDecoder(){
  //========================================================
  //       Start TJpg Decoder
  //========================================================
 tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)

  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);
//  tft.setRotation(1);  // landscape
  tft.setRotation(2); 
}


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
int32_t spliteFrame = 0;
/* write file */
Serial.println("Buffer Size to write : " + String( fsize ) );
myFile = SD.open(path, FILE_WRITE);


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





/*<!-----------------------------------------------------------------
 * This funcation is responsibale for showing images from SD card.
 * You need to give proper path to access images from Sd card.
 * ------------------------------------------------------------------!>
 */
void showSdImageOnTFT(const char *imageSdpath ){
   uint16_t w = 0, h = 0;
  tft.fillScreen(TFT_BLACK);

  TJpgDec.getSdJpgSize(&w, &h, imageSdpath );
  // Draw the image, top left at 0,0
  TJpgDec.drawSdJpg(0, 0, imageSdpath );
  
}
