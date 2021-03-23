
#include "defaultSettings.h"

class defaultSettings dftsett;

void setup() 
{
Serial.begin(9600);
dftsett.setDefaultSettings();

//dftsett._setWeightUnit( 2 );
Serial.println("\n\n_getBuadrate()1 --->>>> " + String( dftsett._getWeightUnit() ) );

 delay(2000);
}

void loop() {
Serial.println("_getBuadrate() --->>>> " + String( dftsett._getWeightUnit() ) );

}
