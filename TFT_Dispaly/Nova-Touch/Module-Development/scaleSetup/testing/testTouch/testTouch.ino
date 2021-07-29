#include <SPI.h>
#include <TFT_eSPI.h>
#include <String.h>
#include <vector>
#include "Arduino.h"
#include <iostream>

#include "freertos/xtensa_context.h"
#include "esp_panic.h"
#include "sdkconfig.h"
#include "soc/soc.h"
#include "soc/dport_reg.h"
#include "soc/gpio_reg.h"
#include <esp_task_wdt.h>


#define SPL Serial.println  
#define SP Serial.print

enum pin{
  RXD2 = 16,
  TXD2 = 17,
  RXD1 = 32,
  TXD1 = 33
};


TFT_eSPI tft;
TaskHandle_t xHandle;
uint16_t xAisx, yAisx; 
void TouchMonitor( void * pvParameters ) {
  for(;;){

   if( tft.getTouch(&xAisx, &yAisx, 20) ){
    SPL("Hello new task...!!!");
    SP("X: ");SPL(xAisx);
    SP("Y : ");SPL(yAisx);
   }
    
    //==============================================
    //        STOP this Thread for some Time
    //==============================================
    vTaskDelay(1 / portTICK_RATE_MS);
    yield();

  }
}
 
void setup() {
  Serial.begin(9600); // DEBUG WINDOW
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1); // Keypad interface
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Weight Mechine
  SPI.beginTransaction(SPISettings(80000000, MSBFIRST, SPI_MODE0));
  Serial2.setRxBufferSize(500);

  
  

  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);

  uint16_t calData[5] = { 331, 3456, 489, 3176, 2 };

  tft.setTouch(calData);
  tft.fillScreen(TFT_BLACK);

 xTaskCreate(   TouchMonitor,
                "TouchMonitor",
                 1024, // if get any issue please increase stack size to 4096
                 NULL,
                 1,
                 &xHandle );


}

void loop() {
  SPL("Hello from loop.......");
  delay(1);
  yield();
}
