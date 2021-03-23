
#include "defaultSettings.h"

void setup() 
{
Serial.begin(9600);
setDefaultSettings();

 _setCapacity( "30000" );
Serial.println("_getBuadrate()1 --->>>> " + String(  _getCapacity() ) );

 _setCapacity( "500000" );

 delay(2000);
}

void loop() {

Serial.println("_getBuadrate() --->>>> " + String( _getCapacity() ) );

}
