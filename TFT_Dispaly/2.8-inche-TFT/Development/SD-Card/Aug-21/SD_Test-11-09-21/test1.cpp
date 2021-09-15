
#include "header.h"


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
