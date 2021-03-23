/*
   EXAMPLE READING AND WRITING A C++ STRUCT TO A FILE
*/
#include "defaultSetting.h"


class defaultSettings dftSett;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dftSett.initDefaultSettings( );

  Serial.println("0 -->> " + String( dftSett._getWeighingUnit() ));
  dftSett._setWeighingUnit(1);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("1 -->> " + String( dftSett._getWeighingUnit() ));
  delay(1000);
}
