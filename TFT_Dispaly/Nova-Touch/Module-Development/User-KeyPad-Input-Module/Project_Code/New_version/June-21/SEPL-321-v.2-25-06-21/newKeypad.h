

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <SPI.h>
#include <TFT_eSPI.h>
#include <String.h>
#include <vector>

#include "keyBoardImage.h"
#include "MACROs.h"





class GenericKeypad {

  private :
    TFT_eSPI tft = TFT_eSPI();

    int8_t _interruptPin = 27; //for Rpi, set accordingly
    int8_t _key;
    uint16_t xAxis, yAxis;
    boolean pressed;

    boolean ISR_dtech;
    uint32_t TimeOut;

    volatile int *int_status = (int *)( INT_STATUS ) ;
    volatile int *int_clear = (int *)( INT_CLEAR ) ;
    volatile int *int_SET = (int *)( INT_SET  );

    char _keyArray[10][4] = {
      {'0', ' ', '/', '@' },
      {'1', 'A', 'B', 'C' },
      {'2', 'D', 'E', 'F' },
      {'3', 'G', 'H', 'I' },
      {'4', 'J', 'K', 'L' },
      {'5', 'M', 'N', 'O' },
      {'6', 'P', 'Q', 'R' },
      {'7', 'S', 'T', 'U' },
      {'8', 'V', 'W', 'X' },
      {'9', 'Y', 'Z', '-' },
    };

    int8_t _initkeypad();
    void _getPhysicalKey();
    int8_t _getVirtualKey();
   
    
  public :
    boolean ISR_Called;
    friend void ISR_Keypad_Touch();
    void enableInterrupt( void (*UserISR_fptr)(void), int8_t interruptPin, int interruptEdge );
    void disableInterrupt();
     void showKeypadImage();
    int8_t getInput();
    
    GenericKeypad() {
      _initkeypad();
    }

    ~GenericKeypad() {
      disableInterrupt();
    }
    
};



//<<===========================================================>>//
//++            Defination                                       ++
//<<===========================================================>>//

class GenericKeypad *kbd_ptr;
void ISR_Keypad_Touch() {
 // kbd_ptr->disableInterrupt();
  noInterrupts();  
  kbd_ptr->ISR_Called = 1;
  interrupts();
}

void GenericKeypad :: enableInterrupt( void (*UserISR_fptr)(void)=NULL, int8_t interruptPin=27, int interruptEdge=FALLING ) {

  _interruptPin = interruptPin;
  pinMode(_interruptPin, INPUT);
  
  if ( *int_status ) (*int_clear) |= ( 1 << _interruptPin );

  if ( UserISR_fptr )
    attachInterrupt(digitalPinToInterrupt(_interruptPin), UserISR_fptr, interruptEdge);
   else
    attachInterrupt(digitalPinToInterrupt(_interruptPin), ISR_Keypad_Touch, interruptEdge);
  
  ISR_Called = 0;
}



int8_t GenericKeypad ::getInput() {
  _key = xAxis = yAxis = 0;
  _getPhysicalKey();

  if ( ISR_Called ) {
    ISR_Called = 0;
    tft.getTouch(&xAxis, &yAxis, 20);
    enableInterrupt();
    return findkey;
  } else if ( _key  ) {
    return _key;
  }
  return keepRunning;
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






void  GenericKeypad :: disableInterrupt() {
  detachInterrupt(digitalPinToInterrupt(_interruptPin));
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

  enableInterrupt();
}


void GenericKeypad :: showKeypadImage() {
  tft.setSwapBytes(true);
  tft.pushImage(0, 0, PLAX_IT_KEYSImage_Width, PLAX_IT_KEYSImage_Hight, PLAX_IT_KEYSArray );
}
