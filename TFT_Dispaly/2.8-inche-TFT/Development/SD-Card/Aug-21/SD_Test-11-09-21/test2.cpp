
#include "header.h"

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
