
#include "Gkeypad.h"
#include "keyBoardImage.h"

//<<===========================================================>>//
//++            Defination                                       ++
//<<===========================================================>>//

char GenericKeypad :: getMappedInput() {
  int8_t temp, cnt = 0;
  Flag_alphaNum = Num;
  switch ( Flag_alphaNum ) {
    case Alpha :
      TimeOut = millis();
      if ( ( millis() - TimeOut ) > responseTime ) {
        if ( ( temp = getInput() ) != keepRunning ) {
          cnt++;
          if ( cnt >= 4 ) cnt = 0;
        }
      }
      break;
    case Num :
      if ( ( temp = getInput() ) != keepRunning ) {
        cnt = Num;        
      }
      break;
  }//end-switch

  if (temp != keepRunning )
        return _keyArray[temp][--cnt];
      else
        return temp;
}//end-getMappedInput()



  int8_t GenericKeypad ::getInput() {
    _key = xAxis = yAxis = 0;
    _getPhysicalKey();

    if ( _key  ) {
      return _key;
    }

    tft.getTouch(&xAxis, &yAxis, 20);
    return findkey;
  }


  void  GenericKeypad ::  _getPhysicalKey() {
    if (Serial1.available() > 0 ) {
      char key[2];
      int8_t buffBytes = Serial1.readBytes(key, 2);
      Serial1.read();
      Serial1.flush();
      //    if ( buffBytes != 0 ){
      //      Serial.print("rawkey : "); Serial.println(key);
      //    }
      sscanf(key, "%d", &_key);
    }
  }




  int8_t GenericKeypad :: _initkeypad() {

    /* Initialise RPi TFT(320x480) */
    tft.init();
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(1);
    uint16_t calData[5] = { 329, 3444, 448, 3193, 2 };
    tft.setTouch(calData);

    /* Serial-1 for physical Keypad */
    Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1); // keypad

    // enableInterrupt();
  }


  void GenericKeypad :: showKeypadImage() {
    tft.setSwapBytes(true);
    tft.pushImage(0, 0, PLAX_IT_KEYSImage_Width, PLAX_IT_KEYSImage_Hight, PLAX_IT_KEYSArray );
  }




  /* Interrupt is not working as of now.

    //
    //void  GenericKeypad :: disableInterrupt() {
    //  detachInterrupt(digitalPinToInterrupt(_interruptPin));
    //}


    //  if ( ISR_Called ) {
    //    ISR_Called = 0;
    //    tft.getTouch(&xAxis, &yAxis, 20);
    //    enableInterrupt();
    //    return findkey;
    //  } else if ( _key  ) {
    //    return _key;
    //  }



    ////class GenericKeypad *kbd_ptr;
    //boolean GenericKeypad :: ISR_Called = 0;
    //void GenericKeypad :: ISR_Keypad_Touch(void) {
    //  // kbd_ptr->disableInterrupt();
    //  noInterrupts();
    //  //  kbd_ptr->ISR_Called = 1;
    //  ISR_Called = 1;
    //  interrupts();
    //}

    //
    //void GenericKeypad :: enableInterrupt( void (*UserISR_fptr)(void) = NULL, int8_t interruptPin = 27, int interruptEdge = FALLING ) {
    //
    //  _interruptPin = interruptPin;
    //  pinMode(_interruptPin, INPUT);
    //
    //  if ( *int_status ) (*int_clear) |= ( 1 << _interruptPin );
    //
    //  if ( UserISR_fptr )
    //    attachInterrupt(digitalPinToInterrupt(_interruptPin), UserISR_fptr, interruptEdge);
    //  else
    //    attachInterrupt(digitalPinToInterrupt(_interruptPin), ISR_Keypad_Touch, interruptEdge);
    //
    //  ISR_Called = 0;
    //}
    return keepRunning;
    }
  */
