
#include "header.h"


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
