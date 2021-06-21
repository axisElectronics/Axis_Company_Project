
#include "commonGlobal.h"


#include "freertos/xtensa_context.h"
#include "esp_panic.h"
#include "sdkconfig.h"
#include "soc/soc.h"
#include "soc/dport_reg.h"
#include "soc/gpio_reg.h"


unsigned long gpio_status;

volatile int *int_status = (int *)( INT_STATUS ) ;
volatile int *int_clear = (int *)( INT_CLEAR ) ;
volatile int *int_SET = (int *)( INT_SET  );



TFT_eSPI tft = TFT_eSPI();
class WeighingHandle Wtft;


int Mode = 0;
bool flag = 1;

volatile boolean pressed;
volatile uint32_t TimeOut = 0;
 uint16_t xAxis = 0, yAxis = 0;
volatile int8_t ISR_dtech = 0;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;


void IRAM_ATTR myfastfunction() {

  portENTER_CRITICAL_ISR(&mux);

  if ( millis() - TimeOut > 500 ) {
    pressed = tft.getTouch( &xAxis, &yAxis, 20);
    if ( pressed ){
      detachInterrupt(GPIOPin_TIRQ);
      TimeOut = millis();
    }
  }else  if ( ++ISR_dtech > 10 ) {
    detachInterrupt(GPIOPin_TIRQ);
    SPL("\nISR called more then 10");
  }
  // SPL("\npressed : " + String(pressed) );
  portEXIT_CRITICAL_ISR(&mux);
}//end-ISR


void setup() {
  Wtft.initWeighingTFT( );
  pinMode(GPIOPin_TIRQ, INPUT);
  TimeOut = millis();
}

void loop() {
  if( ISR_dtech > 10 ){
     ATTACH_TOUCH_INTERRUPT
     ISR_dtech = 0;
  }
  Wtft.readyAxisScales();
  yield();
}



void WeighingHandle :: readyAxisScales()
{

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
    ATTACH_TOUCH_INTERRUPT

    flag = 0;
    pressed = 0;
    yield();
  }


  if ( pressed )
  {
    if ( SettingTouchEnable() )
    {
      Mode = 1;
    }
    else if ( WeighingModeTouchEnable()  )
    {
      if ( userSetting.weighingMode ) {
        Mode = 2;
        START_SERIAL2
        CMD_STARTDATA
      }
    }
    else if (  CountingModeTouchEnable()  )
    {
      if ( userSetting.countingMode ) {
        Mode = 3;
        START_SERIAL2
        CMD_STARTDATA
      }
    }
    else if (  PriceingModeTouchEnable() )
    {
      if ( userSetting.pricecomputing )
      {
        Mode = 4;
        START_SERIAL2
        CMD_STARTDATA
      }
    }
    else if (  CheckingModeTouchEnable() )
    {
      Mode = 5;
      START_SERIAL2
      CMD_STARTDATA
    } else {
      pressed =  0;
      ATTACH_TOUCH_INTERRUPT
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
        pressed = 0;
        goto HERE;
      }
      break;


    case 2:
      if ( startWeighing()  == -1 )
      {
        Mode = 0;
        flag = 1;
        pressed = 0;
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
          detachInterrupt(digitalPinToInterrupt(GPIOPin_TIRQ));
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
          detachInterrupt(digitalPinToInterrupt(GPIOPin_TIRQ));
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
        detachInterrupt(digitalPinToInterrupt(GPIOPin_TIRQ));
        goto HERE;
      }
      break;

  }//end- switch mode
 
  yield();
}//end- readyAsixScales
