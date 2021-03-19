
#include "defaultSetting.h"

class defaultSettings dftSett;

void setup() {
  Serial.begin(9600); 
  dftSett.initDefaultSettings( );

  Serial.println("0 -->> " + String( dftSett._getApplication() ));
  dftSett._setApplication( 3 );

}

void loop() {
  Serial.println("1 -->> " + String( dftSett._getApplication() ));
  delay(1000);
}
