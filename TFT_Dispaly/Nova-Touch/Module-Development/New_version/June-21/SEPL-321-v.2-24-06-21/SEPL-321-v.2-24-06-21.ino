
#include "newKeypad.h"

class GenericKeypad kbd;


void ISR_Keypad_Touch() {
  noInterrupts();
  kbd.ISR_Called = 1;
  interrupts();
}


void setup() {
  /* Initialise Serial */
  Serial.begin(9600);

  kbd.showKeypadImage();
  int8_t interruptPin = 27;
  kbd.enableInterrupt( ISR_Keypad_Touch,  27  );
}

void loop() {
  //Serial.print("==>>> ");
  Serial.println( String( ( kbd.getInput() ) ) );
}
