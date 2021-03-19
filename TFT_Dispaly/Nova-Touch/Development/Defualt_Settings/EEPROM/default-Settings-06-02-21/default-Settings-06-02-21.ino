
#include "defaultSettings.h"

void setup() {
  
Serial.begin(9600);
setDefaultSettings();

 _setParityBits( "None" );
Serial.println("_getBuadrate()1 --->>>> " + String(  _getParityBits() ) );

 _setParityBits( "odd" );

 delay(2000);
}

void loop() {

Serial.println("_getBuadrate() --->>>> " + String( _getParityBits() ) );

}
