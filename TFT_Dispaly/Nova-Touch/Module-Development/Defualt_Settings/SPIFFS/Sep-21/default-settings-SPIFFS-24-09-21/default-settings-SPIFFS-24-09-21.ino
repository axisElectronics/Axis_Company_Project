
#include "defaultSetting.h"

class defaultSettings dftSett;

void setup() {
  Serial.begin(9600); 
  dftSett.initDefaultSettings( );

  Serial.println("0 -->> " + String( dftSett._getWeighingUnit() ));
  dftSett._setWeighingUnit(2);
   delay(2000);
}

void loop() {
  Serial.println("1 -->> " + String( dftSett._getWeighingUnit() ));
  delay(1000);
}
