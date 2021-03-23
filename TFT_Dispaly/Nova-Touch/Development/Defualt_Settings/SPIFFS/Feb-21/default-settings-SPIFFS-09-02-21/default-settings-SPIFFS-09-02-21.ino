/*
   EXAMPLE READING AND WRITING A C++ STRUCT TO A FILE
*/
#include "defaultSetting.h"


class defaultSettings dftSett;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dftSett.initDefaultSettings( );

  Serial.println("0 -->> " + String( dftSett._getApplication() ));
//  dftSett._setApplication(1);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("1 -->> " + String( dftSett._getApplication() ));
  delay(1000);
}
