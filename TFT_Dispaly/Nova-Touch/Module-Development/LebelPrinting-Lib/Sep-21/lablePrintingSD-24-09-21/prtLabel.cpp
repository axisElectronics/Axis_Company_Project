
#include "prtLabel.h"

SPIClass spiSD(HSPI);
#define SD_CS 15

/*<! ========================================================== !>*/

void prtLabel:: readLabelFile(const char *path, char *userBuff) {
  File myFile;
  myFile = SD.open( path, FILE_READ);
  uint32_t fsize = myFile.size();
  int32_t len = fsize;
  Serial.println("File Size to read : " + String(fsize) );

  /* Read file */
  while ( myFile.available() ) {
    line = myFile.readStringUntil('\n');
    String idxstr = whichVarNeedReplacementSD( );

    if ( intVar >= 0 && intVar <  PRT_MAX_VAR ) {
      line.replace( idxstr, labelVaribales[intVar] );
    }

    if (idxstr = "PRINT") {
      SPL(line);
      Serial2.print(line);
    }

  }
  myFile.close();
}



void prtLabel::startSDCard() {
  //========================================================
  //        SD card Begin
  //  we are using HSPI for fetching data from SD card.
  //========================================================
  /* SPI settings */
  uint32_t freq = 80 * 1000000; //80MHZ
  SPISettings settingsA(8000000, MSBFIRST, SPI_MODE1);
  spiSD.begin(14, 33, 13, 15); //SCK,MISO,MOSI,SS //HSPI1
  SPI.beginTransaction(settingsA);
  //spiSD.setFrequency(freq);

  /* SD card Settings */
  if (!SD.begin( SD_CS, spiSD  )) {
    Serial.println("Card Mount Failed");
  } else {
    Serial.println("Card Mount Successed ....!!!");
  }

  File myFile = SD.open("/");
  myFile.close();
}


void  prtLabel:: printLebel( ) {

  if ( printFlag ) {
    tempstr = temp;
    printFlag = 0;
  }

  for (uint8_t i = 0; i < PRT_MAX_VAR; ++i ) {
    String idxstr = whichVarNeedReplacement( );

    // it means There is not any variable which need to be replaced.
    // So Print this Label as it as.
    if ( idxstr == "PRINT" ) {
        PRINT :
           //   SPL("#####################");
              SPL(tempstr);
          //    SPL("#####################");
              //          Serial2.print(tempstr);
              printFlag = 1;
              intVar = -1;
              break;
    }
    else {
              if ( intVar >= 0 && intVar <  PRT_MAX_VAR )
                SP("len : "); SPL( strlen( labelVaribales[intVar] ) );
              if ( strlen( labelVaribales[intVar] ) ) {
                tempstr.replace( idxstr, labelVaribales[intVar] );
              }
              else {
                tempstr.replace( idxstr, idxstr );
                goto PRINT;
              }
    }
  }
}



void  prtLabel:: updateLableVar(enum_LableVar varNum, char *varData) {
  strcpy( labelVaribales[varNum], varData );
}



// This is for user to check that Varibale is stored OR not appropriatly.
char * prtLabel:: showMeLableVar(int varNum) {
  return labelVaribales[varNum];
}


 void  prtLabel:: printLebelSD( char *labelFilePath ){
   readLabelFile(labelFilePath, NULL);
 }
