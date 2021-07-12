#include "commonGlobal.h"
#include "InterruptHandler.h"





void IRAM_ATTR INTER_resetPage() {
  noInterrupts();
  if ( millis() - TimeOut > 500 ) {
    if ( pressed || ( pressed = tft.getTouch( &xAxis, &yAxis, 20) ) ) {
      detachInterrupt(GPIOPin_TIRQ);
      TimeOut = millis();
      ISR_Called = 1;
    }
  } else  if ( ++ISR_dtech > 10 ) {
       detachInterrupt(GPIOPin_TIRQ);
    SPL("\nISR called more then 10");
  }
  interrupts();
  // SPL("\npressed : " + String(pressed) );
}//end-ISR




void  myfastfunction() {
  noInterrupts();
  //  detachInterrupt(GPIOPin_TIRQ);
  ISR_Called = 1;
  interrupts();
}//end-ISR


int8_t findTouch() {
  noInterrupts();
  if ( ISR_Called ) {
    if ( pressed || ( pressed = tft.getTouch(&xAxis, &yAxis, 20) ) ) {
     
      #ifdef ATTACH_INTER
      ISR_Called = 0;
      detachInterrupt(GPIOPin_TIRQ);
      ISR_dtech = 1;
      #endif
      
      if ( ESC_Touch )
      {
        //  SPL("ESCTouch...>>X:  " + String(xAxis) + " Y : " + String(yAxis) );
        xAxis = 0;
        yAxis = 0;
        STOP_SERIAL2
        EMPTY_SERIALBUFFER
        pressed = 0;
//        detachInterrupt(GPIOPin_TIRQ);
        return -1;
      }
      
    }
    interrupts();
  }//end-ISR_called
  else return ( pressed = 0 );

  return pressed;
}
