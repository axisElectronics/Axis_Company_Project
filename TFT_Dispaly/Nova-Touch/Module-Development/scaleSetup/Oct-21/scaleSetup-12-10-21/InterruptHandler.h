
#ifndef INTER_HANDLER
#define INTER_HANDLER

#include "commonGlobal.h"

 #define ATTACH_INTER 0


int8_t findTouch();
bool checkTouch(uint8_t touchMode );
void  myfastfunction();
void IRAM_ATTR INTER_resetPage();

extern int Mode;
extern class  WeighingHandle weightClass;

extern boolean ISR_Called;
extern boolean ISR_dtech;
extern volatile boolean pressed;
extern volatile uint32_t TimeOut;
extern  uint16_t xAxis, yAxis;
extern volatile int *int_status;
extern volatile int *int_clear;
extern volatile int *int_SET;
extern void (*TouchInterrupt_ptr)(void ) ;


// The TIRQ interrupt signal must be used for this example.
#define GPIOPin_TIRQ  27
#define INT_STATUS  0x3FF44044
#define INT_CLEAR 0x3FF4404C
#define INT_SET 0x3FF44048

#define CLEAR_INTERRUPT_BIT   if( *int_status ) (*int_clear) |= ( 1 << GPIOPin_TIRQ );    //   SPL("INT_READ : " + String( *int_status, HEX ) );      
                                         
 
#define ATTACH_TOUCH_INTERRUPT {                               \
    CLEAR_INTERRUPT_BIT                                        \
    attachInterrupt(digitalPinToInterrupt(GPIOPin_TIRQ), TouchInterrupt_ptr, FALLING); \
  }

#define touchHandler(Mode) if( checkTouch(Mode) ) return -1;

#define Weight_CheckOut(Mode)  if( weightClass.checkTouch(Mode) ) return -1;
#define Count_CheckOut(Mode)   if( weightClass.checkTouch(Mode) ) return -1;
#define Price_CheckOut(Mode)   if( weightClass.checkTouch(Mode) ) return -1;
#define Check_CheckOut(Mode)   if( weightClass.checkTouch(Mode) ) return -1;

#endif
