
#include "Gkeypad.h"

class GenericKeypad kbd;

void setup() {
  /* Initialise Serial */
  Serial.begin(9600);

  /* it will just show image */
  kbd.showKeypadImage();

}
char temp;

void loop() {
  //Serial.print("==>>> ");
//  if ( ( temp = kbd.getInput() ) != -20 ) {
    if ( ( temp = kbd.getMappedInput() ) != -20 ) {
    Serial.println( String( temp ) );
  }
}
