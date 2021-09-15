
#include "commonGlobal.h"
#include "InterruptHandler.h"

TFT_eSPI tft = TFT_eSPI();

class WeighingHandle Wtft;


int Mode = 0;
bool flag = 1;
unsigned long gpio_status;
volatile boolean pressed;
boolean ISR_Called = 0;
boolean ISR_dtech  = 0;
volatile uint32_t TimeOut = 0;
uint16_t xAxis = 0, yAxis = 0;

volatile int *int_status = (int *)( INT_STATUS ) ;
volatile int *int_clear = (int *)( INT_CLEAR ) ;
volatile int *int_SET = (int *)( INT_SET  );

void (*TouchInterrupt_ptr)(void) ;



TaskHandle_t xHandleTouch;
SemaphoreHandle_t xSemaphore_Touch;

void TouchMonitor( void * pvParameters ) {
 
  for(;;){

   if( tft.getTouch(&xAxis, &yAxis, 20)  ){
    pressed = 1;
   }
 
 }
    SPL("hello");
    //==============================================
    //        STOP this Thread for some Time
    //==============================================
    vTaskDelay(1 / portTICK_RATE_MS);
    yield();
  }

void setup() {
  Wtft.initWeighingTFT( );  
  pinMode(GPIOPin_TIRQ, INPUT_PULLUP);
  Serial2.begin(9600);
  delay(500);
//   xTaskCreate(   TouchMonitor,
//                "TouchMonitor",
//                 1024, // if get any issue please increase stack size to 4096
//                 NULL,
//                 1,
//                 &xHandleTouch );

 TimeOut = millis();
}


void loop() {
  

  if ( ISR_dtech > 10 ) {
    ATTACH_TOUCH_INTERRUPT
    ISR_dtech = 0;
  }

  Wtft.readyAxisScales();

  Serial2.print( "Hellow\n" );
  Serial.print( (char)Serial2.read() );

yield();

}



void WeighingHandle :: readyAxisScales(){
//  SP("pressed : ");SPL( pressed = tft.getTouch( &xAxis, &yAxis, 20) );
// pressed = tft.getTouch( &xAxis, &yAxis, 20);
  if ( !flag && !pressed ) return;
  
HERE :
  if (flag) {
  
    settingPageInit( ); // import default settings

    uint8_t tempdot = _getDecimal().c_str()[0];
    showDigits.dotPosition = tempdot - 48;

    String temp = _getApplication();

    if (  temp.indexOf('0') != -1  ) userSetting.weighingMode =   1; else userSetting.weighingMode =    0;
    if (  temp.indexOf('1') != -1 ) userSetting.countingMode =    1; else userSetting.countingMode =    0;
    if (  temp.indexOf('2') != -1 ) userSetting.checkWeighing =   1; else userSetting.checkWeighing =   0;
    if (  temp.indexOf('3') != -1 ) userSetting.pricecomputing =  1; else userSetting.pricecomputing =  0;

    startUPImage();
    STOP_SERIAL2
 
    TouchInterrupt_ptr = INTER_resetPage;

    flag = 0;
    pressed = 0;
//    xAxis = yAxis = 0;
//        while ( !( xAxis || yAxis ) ) {
//          xAxis = yAxis = 0;
//          pressed = tft.getTouch(&xAxis, &yAxis);
//          SPL("Waiting : " + String(xAxis) + ", " + String(yAxis));
//          yield();
//        }

   ATTACH_TOUCH_INTERRUPT
    yield();
   
   }
  

  if ( pressed )
  {
   
    if ( SettingTouchEnable() )
    {
      Mode = 1;
    }else if ( WeighingModeTouchEnable()  ){
      if ( userSetting.weighingMode ) {
        Mode = 2;
        START_SERIAL2
        CMD_STARTDATA
      } else {
        pressed =  0;
      
        ATTACH_TOUCH_INTERRUPT
        
      }
    }else if (  CountingModeTouchEnable()  ){
      if ( userSetting.countingMode ) {
        Mode = 3;
        START_SERIAL2
        CMD_STARTDATA
      } else {
        pressed =  0;
       
        ATTACH_TOUCH_INTERRUPT
       
      }
    }else if (  PriceingModeTouchEnable() ){
      if ( userSetting.pricecomputing )
      {
        Mode = 4;
        START_SERIAL2
        CMD_STARTDATA
      } else {
        pressed =  0;
       
        ATTACH_TOUCH_INTERRUPT
        
      }
    }else if (  CheckingModeTouchEnable() ) {
      Mode = 5;
      START_SERIAL2
      CMD_STARTDATA
    }else {
      pressed =  0;
      
        ATTACH_TOUCH_INTERRUPT
        
    }
  }//end-if( pressed )


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
        pressed = 0;
        goto HERE;
      }
      break;


    case 2:
      //  SPL("startWeighing() : " + String(xAxis) + ", " + String(yAxis));
      if ( startWeighing()  == -1 )
      {
        Mode = 0;
        flag = 1;
        pressed = 0;
        delay(100);
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
          pressed = 0;
          delay(100);
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
          pressed = 0;
          delay(100);
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
        pressed = 0;
        delay(100);
        goto HERE;
      }
      break;
  }//end- switch mode
 
  yield();
}//end- readyAsixScales