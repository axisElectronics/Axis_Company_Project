

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <SPI.h>
#include <TFT_eSPI.h>
#include <String.h>
#include <vector>
#include "Arduino.h"
#include <iostream>
// Include the jpeg decoder library
#include <TJpg_Decoder.h>
#include <XPT2046_Touchscreen.h>
#include <Wire.h>
#include <ErriezDS1307.h>

using namespace std; 



TFT_eSPI tft;

TaskHandle_t Task1;
uint16_t xAxis = 0, yAxis = 0;

boolean pressed=0;

void Task1code( void * parameter) {
  for (;;) {
    pressed = tft.getTouch( &xAxis, &yAxis, 30);
//    Serial.print("\nTask1code() running on core ");
//    Serial.println(xPortGetCoreID()); // core Zero
    vTaskDelay(1);
    yield();
  }
}

void setup() {
  Serial.begin(9600); 
  tft.init();
  tft.fillScreen(TFT_GREEN);
  uint16_t calData[5] = { 331, 3456, 489, 3176, 2 };
  tft.setTouch(calData);
  
  xTaskCreatePinnedToCore(
    Task1code, /* Function to implement the task */
    "Task1", /* Name of the task */
    10000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    0,  /* Priority of the task */
    &Task1 , //);  /* Task handle. */
    0    ); /* Core where the task should run */

  delay(500);
}

void loop() {

  if( pressed ){//core 1
    Serial.println("xAxis : " + String(xAxis)  + " yAxis : " + String(yAxis) );
    xAxis = 0;
    yAxis = 0;
    pressed=0;
  }

}
