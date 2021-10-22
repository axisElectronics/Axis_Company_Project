
#include "commonGlobal.h"
#include "InterruptHandler.h"


//#define FEEDBACK

TFT_eSPI tft = TFT_eSPI();

class WeighingHandle Wtft;
class headerInput headerStr;

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
   if( tft.getTouch(&xAxis, &yAxis, 20)  )  pressed = 1;
   yield();
 }
//    SPL("hello");
    //==============================================
    //        STOP this Thread for some Time
    //==============================================
    vTaskDelay(1 / portTICK_RATE_MS);
    yield();
  }

void setup() {
  Wtft.initWeighingTFT( );  
  pinMode(GPIOPin_TIRQ, INPUT_PULLUP);
  TimeOut = millis();
}//end-setup


void loop() {
  
if ( ISR_dtech > 10 ) {
    ATTACH_TOUCH_INTERRUPT
    ISR_dtech = 0;
  }

  Wtft.readyAxisScales();
#ifdef FEEDBACK
  Wtft.batterySymbl(UPPER);
  Wtft.batteryAnimation(-1);
  Wtft.batteryAnimation(10);
  Wtft.batteryAnimation(40);
  Wtft.batteryAnimation(60);
#endif  
  yield();

}//end-loop



void WeighingHandle :: readyAxisScales(){
//  SP("pressed : ");SPL( pressed = tft.getTouch( &xAxis, &yAxis, 20) );
//if( !pressed )
// pressed = tft.getTouch( &xAxis, &yAxis, 20);


  if ( !flag && !pressed ) return;

HERE :
  if (flag) {

    // import default settings
    settingPageInit( ); 
    // check 2 main parameters which are critical fot this
    // 1. Dot 2. Capacity
       
    uint8_t tempdot = _getDecimal().c_str()[0];
    showDigits.dotPosition = tempdot - 48;

    String temp = _getApplication();

    if (  temp.indexOf('0') != -1 ) userSetting.weighingMode =    1; else userSetting.weighingMode   = 0;
    if (  temp.indexOf('1') != -1 ) userSetting.countingMode =    1; else userSetting.countingMode   = 0;
    if (  temp.indexOf('2') != -1 ) userSetting.checkWeighing =   1; else userSetting.checkWeighing  = 0;
    if (  temp.indexOf('3') != -1 ) userSetting.pricecomputing =  1; else userSetting.pricecomputing = 0;

    startUPImage();
    STOP_SERIAL2
    #ifdef FEEDBACK
      popupButton(0); 
    #endif
    TouchInterrupt_ptr = INTER_resetPage;

    flag = 0;
    pressed = 0;

   ATTACH_TOUCH_INTERRUPT
   yield();


  }//end-if(flag)
  

  
// If any touch has been detected then take action according to that
  if ( pressed ){  
    if ( SettingTouchEnable() ){
       Mode = 1;
      // Setting Mode has been touched. So draw rectangle on it.
      #ifdef FEEDBACK
        popupButton(SettingMode, TFT_GREEN);
      #endif  
        pressed =  0;   
        ATTACH_TOUCH_INTERRUPT 
        Serial.printf(" total number of bytes used by the SPIFFS file system : %ld\n", SPIFFS.totalBytes() );
        Serial.printf("total Bytes used by SPIFFS : %ld\n", SPIFFS.usedBytes() );
           
    }
    else if (  WeighingModeTouchEnable()  ) {
      // Weighing Mode has been touched. So draw rectangle on it.
      #ifdef FEEDBACK
        popupButton(WeightMode, TFT_GREEN);
      #endif  
      if ( userSetting.weighingMode ) {
        Mode = 2;
        START_SERIAL2
        CMD_STARTDATA
      } else {
        pressed =  0;   
        ATTACH_TOUCH_INTERRUPT     
      }
    }
    else if (  CountingModeTouchEnable()  ) {
      // Count Mode has been touched. So draw rectangle on it.
      #ifdef FEEDBACK
        popupButton(CountMode, TFT_GREEN);
      #endif  
      if ( userSetting.countingMode ) {
        Mode = 3;
        START_SERIAL2
        CMD_STARTDATA
      } else {
        pressed =  0;
       
        ATTACH_TOUCH_INTERRUPT
       
      }
    }
    else if (  PriceingModeTouchEnable()  ) {
     // Price Mode has been touched. So draw rectangle on it.
     #ifdef FEEDBACK
        popupButton(PriceMode, TFT_GREEN);
     #endif   
      if ( userSetting.pricecomputing ){
        Mode = 4;
        START_SERIAL2
        CMD_STARTDATA
      } else {
        pressed =  0;      
        ATTACH_TOUCH_INTERRUPT
        
      }
    }
    else if (  CheckingModeTouchEnable()  ) {
     // Check Mode has been touched. So draw rectangle on it.
     #ifdef FEEDBACK
      popupButton(CheckMode, TFT_GREEN); 
     #endif 
      Serial.printf("checking Mode : %d\n", CheckMode);  
      if (  userSetting.checkWeighing ){          
      Mode = 5;
      pressed =  0; 
      START_SERIAL2
      CMD_STARTDATA
    }else {
      pressed =  0;       
      ATTACH_TOUCH_INTERRUPT
    }
  }//end-if(  CheckingModeTouchEnable() )
    else{
      pressed = 0;
      ATTACH_TOUCH_INTERRUPT
    }
  }//end-if(pressed)



  switch (Mode){
    case 1:{
        tft.fillScreen(TFT_BLACK);

        settingPageHandler( );
   //     SPL("I am Back my friends \n" );
        tft.fillScreen(TFT_BLACK);
        Mode = 0;
        flag = 1;
        pressed = 0;
        delay(100);
        goto HERE;
      }
      break;

    case 2:
      if ( startWeighing()  == -1 ){
        Mode = 0;
        flag = 1;
        pressed = 0;
        delay(100);
        goto HERE;
      }
      break;


    case 3:
      if ( userSetting.countingMode ){
        if ( startCountComputing() == -1 ){
          Mode = 0;
          flag = 1;
          pressed = 0;
          delay(100);
          goto HERE;
        }//end-if
      }//end-if
      break;


    case 4:
      if ( userSetting.pricecomputing ){
        if ( startPriceComputing() == -1 ){
          Mode = 0;
          flag = 1;
          pressed = 0;
          delay(100);
          goto HERE;
        }//end-if
      }//end-if
      break;

    case 5:    
    if ( userSetting.checkWeighing ){
      CMD_STARTDATA
      if (  startCheckWeighing() == -1 ){
        Mode = 0;
        flag = 1;
        pressed = 0;
        delay(100);
        goto HERE;
      }//end-if 
    }//end-if      
      break;
  }//end- switch mode



  yield();
}//end- readyAsixScales
