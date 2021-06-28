
#ifndef GKEYPAD
#define GKEYPAD

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <SPI.h>
#include <TFT_eSPI.h>
#include <String.h>
#include <vector>


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
    static boolean ISR_Called;
    static void IRAM_ATTR ISR_Keypad_Touch(void);
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

#endif
