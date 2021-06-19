
#include "commonGlobal.h"


#include "freertos/xtensa_context.h"
#include "esp_panic.h"
#include "sdkconfig.h"
#include "soc/soc.h"
#include "soc/dport_reg.h"
#include "soc/gpio_reg.h"


unsigned long gpio_status;


TFT_eSPI tft = TFT_eSPI();
class WeighingHandle Wtft;
extern XPT2046_Touchscreen ts;

uint16_t xAxis = 0, yAxis = 0;
boolean pressed;
int Mode = 0;
bool flag = 1;
volatile int8_t GPIOPin_TIRQ = 27;
uint32_t TimeOut = 0;

 #define INT_STATUS  0x3FF44044
 #define INT_CLEAR 0x3FF4404C


void IRAM_ATTR myfastfunction() {
 
  if ( ( millis() - TimeOut ) > 500) {
    if ( ( pressed = tft.getTouch( &xAxis, &yAxis) ) ) {
      detachInterrupt(digitalPinToInterrupt(GPIOPin_TIRQ));      
      TimeOut = millis();
    } else {
      xAxis = 0;
      yAxis = 0;      
    }//end-else
  }//end-debounching

}//end-ISR



void setup() {
  Wtft.initWeighingTFT( );
  pinMode(GPIOPin_TIRQ, INPUT_PULLUP);
  TimeOut = millis();
}

void loop() {
  Wtft.readyAxisScales();
  yield();
}



void WeighingHandle :: readyAxisScales()
{

  if ( !flag && !pressed ) return;

  Serial.println("\n1: ++++++++++");
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

    startUPImage();
    STOP_SERIAL2

    flag = 0;
    pressed = 0;
    
    if ( !pressed ) {
      //Clear GPIO Interrupt Status    
      int *int_status = (int *)( INT_STATUS ) ;
      int *int_clear = (int *)( INT_CLEAR ) ;
      
     //  Serial.print("\nRead Reg : " ); SPL(*int_status, HEX);
       if( *int_status ) *int_clear |= 1 << 27;
     //  Serial.print("\nRead Reg : " ); SPL(*int_status, HEX);
       
      attachInterrupt(digitalPinToInterrupt(GPIOPin_TIRQ), myfastfunction, FALLING);     
    }
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

    }
  }//end-if( getTouch() )
  Serial.println("\n3: ++++++++++");
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
  Serial.println("\n4: ++++++++++");
  yield();
}//end- readyAsixScales
