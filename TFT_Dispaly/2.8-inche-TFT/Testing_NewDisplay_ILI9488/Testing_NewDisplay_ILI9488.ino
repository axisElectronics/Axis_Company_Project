/* Map XPT2046 input to ILI9341 320x240 raster */

//#define _FLEXIO_SPI_H_

#include <SPI.h>
#include "XPT2046_Touchscreen.h" /* https://github.com/PaulStoffregen/XPT2046_Touchscreen */

#define TOUCH_CS_PIN       5
#define TOUCH_IRQ_PIN      27

SPIClass SPI1 = SPIClass(HSPI);

XPT2046_Touchscreen touch( TOUCH_CS_PIN, TOUCH_IRQ_PIN );

void setup() {
  Serial.begin( 115200 );
  touch.begin(  );
  Serial.println( "Touch screen ready." );
  delay(2000);

  
  
}

TS_Point rawLocation;

void loop() {
    while ( touch.touched() )
    {
      rawLocation = touch.getPoint();
        Serial.print("x = ");
        Serial.print(rawLocation.x);
        Serial.print(", y = ");
        Serial.print(rawLocation.y);
        Serial.print(", z = ");
        Serial.println(rawLocation.z);   
    }
}
