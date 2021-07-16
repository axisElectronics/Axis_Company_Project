




#include "headerFiles.h"

extern volatile boolean pressed;
extern uint16_t xAxis, yAxis;
//extern class price priceObj;


// thread handlers
TaskHandle_t xPriceHandler;
TaskHandle_t xTouchHandler;

void findtouch_oper( void * pvParameters );

void create_TouchThread() {

  xTaskCreate(    findtouch_oper,
                  "find touch",
                  10000,
                  NULL,
                  2,
                  &xTouchHandler );
}




// touch handler
void findtouch_oper( void * pvParameters ) {
  bool temp = 0;

  for (;;) {
    if (pressed) {
    
      vTaskDelay(1 / portTICK_RATE_MS);
      yield();
    }
    else {
      yield();
      pressed = tft.getTouch(&xAxis, &yAxis, 20);
      if ( pressed ) {
        SP("temp : "); SP(xAxis); SP(" , "); SPL(yAxis);
      }
    }
    vTaskDelay(1 / portTICK_RATE_MS);
    yield();
  }
}
