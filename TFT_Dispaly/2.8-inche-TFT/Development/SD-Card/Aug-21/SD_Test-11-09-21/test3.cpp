
#include "header.h"


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
