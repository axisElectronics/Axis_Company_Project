
#include "Gkeypad.h"

class GenericKeypad kbd;

void setup() {
  /* Initialise Serial */
  Serial.begin(9600);

  /* it will just show image */
  kbd.showKeypadImage();

}

void loop() {
  //Serial.print("==>>> ");
  Serial.println( String( ( kbd.getInput() ) ) );
}
