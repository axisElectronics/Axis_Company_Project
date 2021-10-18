
#include "defaultSetting.h"

class defaultSettings dftSett;

void setup() {
  Serial.begin(9600); 
  dftSett.initDefaultSettings( );
  strcpy( dftSett.userScaleSetup.saveHeader[0], "Hello I am vivek" );
  Serial.println("0 -->> " + String( dftSett.userScaleSetup.saveHeader[0] ));
//  dftSett._setHeader();
  dftSett.saveAsDefault();
  delay(2000);
}

void loop() {
  Serial.println("1 -->> " + String( dftSett._getHeader(0) ));
  delay(1000);
}
