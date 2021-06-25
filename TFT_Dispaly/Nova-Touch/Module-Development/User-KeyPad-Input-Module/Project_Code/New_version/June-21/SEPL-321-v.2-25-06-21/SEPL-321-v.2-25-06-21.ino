
#include "newKeypad.h"

class GenericKeypad kbd;

void setup() {
  /* Initialise Serial */
  Serial.begin(9600);

  /* it will just show image */
  kbd.showKeypadImage();
//  kbd.enableInterrupt();
}

void loop() {
  //Serial.print("==>>> ");
  Serial.println( String( ( kbd.getInput() ) ) );
}
