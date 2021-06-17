
#include "commonGlobal.h"

#include "esp_system.h" //This inclusion configures the peripherals in the ESP system.
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"



TFT_eSPI tft = TFT_eSPI();
class WeighingHandle Wtft;
extern XPT2046_Touchscreen ts;

TaskHandle_t Task1, Task2;
uint16_t xAxis = 0, yAxis = 0;
boolean pressed;
int Mode = 0;
bool flag = 1;

void Task1code( void * parameter) {
 //  vTaskSuspend( NULL );
   
  for (;;) {
    pressed = tft.getTouch( &xAxis, &yAxis, 30);
    if( pressed )  vTaskResume( Task2 );
   // else  vTaskSuspend( Task2 );
    
    Serial.print("\nTask1code() running on core ");
    Serial.println(xPortGetCoreID());
    vTaskDelay(10);
//    yield();
  }
}


void Task2code( void * parameter) {
  for (;;) {
    Wtft.readyAxisScales();
    vTaskDelay(1);
//    yield();
  }
}


void setup() {

  Wtft.initWeighingTFT( );


   xTaskCreatePinnedToCore(
    Task2code, /* Function to implement the task */
    "Task2", /* Name of the task */
    50000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    0,  /* Priority of the task */
    &Task2, /* Task handle. */
    0 ); /* Core where the task should run */

  delay(500);


  xTaskCreatePinnedToCore(
    Task1code, /* Function to implement the task */
    "Task1", /* Name of the task */
    50000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    5,  /* Priority of the task */
    &Task1, /* Task handle. */  
    1    ); /* Core where the task should run */

  delay(500);


}

void loop() {

}



void WeighingHandle :: readyAxisScales()
{
  
  vTaskSuspend( Task1 );
  Serial.print("setup() running on core ");
  Serial.println(xPortGetCoreID());

  
HERE :
  if ( flag ) {
    
     
    settingPageInit( ); // import default settings

    uint8_t tempdot = _getDecimal().c_str()[0];
    showDigits.dotPosition = tempdot - 48;

    String temp = _getApplication();

    if (  temp.indexOf('0') != -1  ) userSetting.weighingMode =   1; else userSetting.weighingMode =    0;
    if (  temp.indexOf('1') != -1 ) userSetting.countingMode =    1; else userSetting.countingMode =    0;
    if (  temp.indexOf('2') != -1 ) userSetting.checkWeighing =   1; else userSetting.checkWeighing =   0;
    if (  temp.indexOf('3') != -1 ) userSetting.pricecomputing =  1; else userSetting.pricecomputing =  0;

    Serial.println("\n++++++++++");
    
    startUPImage();
   
     Serial.println("\n==========");
    STOP_SERIAL2
    flag = 0;
    vTaskResume( Task1 );
  }

if( pressed ){//core 1
    Serial.println("xAxis : " + String(xAxis)  + " yAxis : " + String(yAxis) );
    xAxis = 0;
    yAxis = 0;
    pressed=0;
  }

  
/*
  if ( pressed )
  {
    if ( SettingTouchEnable() )
    {
      Mode = 1;
      //        break;
    }
    else if ( WeighingModeTouchEnable()  )
    {
      if ( userSetting.weighingMode ) {
        Mode = 2;
        START_SERIAL2
        CMD_STARTDATA
        //          break;
      }
    }
    else if (  CountingModeTouchEnable()  )
    {
      if ( userSetting.countingMode ) {
        Mode = 3;
        START_SERIAL2
        CMD_STARTDATA
        //          break;
      }
    }
    else if (  PriceingModeTouchEnable() )
    {
      if ( userSetting.pricecomputing )
      {
        Mode = 4;
        START_SERIAL2
        CMD_STARTDATA
        //          break;
      }
    }
    else if (  CheckingModeTouchEnable() )
    {
      Mode = 5;
      START_SERIAL2
      CMD_STARTDATA
      //        break;
    }
  }//end-if( getTouch() )


  switch (Mode)
  {
    case 1:
      {
        tft.fillScreen(TFT_BLACK);
        settingPageHandler( );
        SPL("I am Back my friends \n" );
        tft.fillScreen(TFT_BLACK);
        Mode = 0;
        flag = 1;
        goto HERE;
      }
      break;


    case 2:
      if ( startWeighing()  == -1 )
      {
        Mode = 0;
        flag = 1;
        goto HERE;
      }
      break;


    case 3:
      if ( userSetting.countingMode )
      {
        if ( startCountComputing() == -1 )
        {
          Mode = 0;
          flag = 1;
          goto HERE;
        }
      }
      break;


    case 4:
      if ( userSetting.pricecomputing )
      {
        if ( startPriceComputing() == -1 )
        {
          Mode = 0;
          flag = 1;
          goto HERE;
        }
      }
      break;


    case 5:
      CMD_STARTDATA
      if (  startCheckWeighing() == -1 )
      {
        Mode = 0;
        flag = 1;
        goto HERE;
      }
      break;

  }//end- switch mode
*/
}//end- readyAsixScales
