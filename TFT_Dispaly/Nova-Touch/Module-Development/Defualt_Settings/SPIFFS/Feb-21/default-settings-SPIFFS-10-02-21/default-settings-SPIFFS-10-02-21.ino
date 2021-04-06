/*
   EXAMPLE READING AND WRITING A C++ STRUCT TO A FILE
*/
#include "defaultSetting.h"


class defaultSettings dftSett;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dftSett.initDefaultSettings( );

  Serial.println("0 -->> " + String( dftSett._getDate() ));
//  dftSett._setDate("11/02/2021");

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("1 -->> " + String( dftSett._getDate() ));
  delay(1000);
}
