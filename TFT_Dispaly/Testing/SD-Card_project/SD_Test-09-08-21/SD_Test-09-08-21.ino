#include "FS.h"
#include "SD.h"
#include "SPI.h"

SPIClass spiSD(HSPI);
#define SD_CS 15


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
uint32_t size = myFile.size();
Serial.println("File Size to read : " + String(size) );

/* Read file */
for( uint32_t i=0 ; ( i < size ) && ( myFile.available() ) ; ){ 
  userBuff[i++] = myFile.read();  
  yield();
}
myFile.close();
}

/*<! ========================================================== !>*/

void writeFile(const char *path, const char *userBuff){
File myFile;  
uint32_t size = strlen(userBuff);
/* write file */
Serial.println("Buffer Size to write : " + String( size ) );
myFile = SD.open(path, FILE_WRITE);

if (myFile) { 
    for(uint32_t i=0; i < size; i++ ){
      myFile.write( userBuff[i] );
      yield();
    }
    myFile.print('\n');
  }
myFile.close(); 

}

/*<! ========================================================== !>*/

void copyFile(char *destpath, const char *srcpath){
  File srcFile, destFile;

  destFile = SD.open(destpath, FILE_WRITE);
  srcFile = SD.open(srcpath, FILE_READ);

  while( srcFile.available() ){
    destFile.print( (char )srcFile.read());
  }

  destFile.close();
  srcFile.close();
  Serial.println("File writing completed ...!!!");
}




void setup() {
Serial.begin(115200);
Serial.setRxBufferSize(2048);

/* SPI settings */
spiSD.begin(14, 33, 13, 15); //SCK,MISO,MOSI,SS //HSPI1
uint32_t freq = 80*1000000;
spiSD.setFrequency(freq);

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

}//end-setup

void loop() {
char str[3900];
memset(str, '\0', 3900);

checkFile("/walk.h");
//readFile("/walk.h", str );
//writeFile("/walk.h", str);
copyFile("/test_1.txt", "/walk.h");

delay(1000);

while(1){
yield();
}

yield();
}
