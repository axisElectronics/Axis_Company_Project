
#include "commonGlobal.h"

TFT_eSPI tft = TFT_eSPI();
class WeighingHandle Wtft;


void WeighingHandle :: readyAxisScales()
{
  int Mode = 0;
  uint16_t xAxis = 0, yAxis = 0;

  initWeighingTFT( );

HERE :
  settingPageInit( ); // import default settings

  uint8_t tempdot = _getDecimal().c_str()[0];
  showDigits.dotPosition = tempdot - 48;

  String temp = _getApplication();

  if (  temp.indexOf('0') != -1  ) userSetting.weighingMode =   1; else userSetting.weighingMode =    0;
  if (  temp.indexOf('1') != -1 ) userSetting.countingMode =    1; else userSetting.countingMode =    0;
  if (  temp.indexOf('2') != -1 ) userSetting.checkWeighing =   1; else userSetting.checkWeighing =   0;
  if (  temp.indexOf('3') != -1 ) userSetting.pricecomputing =  1; else userSetting.pricecomputing =  0;

  uint32_t tout = millis();
  boolean pressed;
  startUPImage();
  STOP_SERIAL2
   
  while (1)
  {
    pressed = tft.getTouch( &xAxis, &yAxis);
    
    if ( pressed )
    {
      if ( SettingTouchEnable() )
      {
        Mode = 1;
        break;
      }
    else if ( WeighingModeTouchEnable()  )
      {
        if ( userSetting.weighingMode ) {
          Mode = 2;
          START_SERIAL2
          CMD_STARTDATA
          break;
        }
      }
    else if (  CountingModeTouchEnable()  )
      {
        if ( userSetting.countingMode ) {
          Mode = 3;
          START_SERIAL2
          CMD_STARTDATA
          break;
        }
      }
    else if (  PriceingModeTouchEnable() )
      {
        if ( userSetting.pricecomputing )
        {
          Mode = 4;
          START_SERIAL2
          CMD_STARTDATA
          break;
        }
      }
    else if (  CheckingModeTouchEnable() )
      {
        Mode = 5;
        START_SERIAL2
        CMD_STARTDATA
        break;
      }
    }//end-if( getTouch() )

    yield();
  }//end-while()

  switch (Mode)
  {
    case 1:
      {
        tft.fillScreen(TFT_BLACK);
        settingPageHandler( );
        SPL("I am Back my friends \n" );
        tft.fillScreen(TFT_BLACK);
        Mode = 0;
        goto HERE;
      }
      break;
    case 2:
      if ( startWeighing()  == -1 )
      {
        Mode = 0;
        goto HERE;
      }
      break;
    case 3:
      if ( userSetting.countingMode )
      {
        if ( startCountComputing() == -1 )
        {
          Mode = 0;
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
          goto HERE;
        }
      } break;
    case 5:
      CMD_STARTDATA
      if (  startCheckWeighing() == -1 )
      {
        Mode = 0;
        goto HERE;
      }
      break;
  }//end- switch mode

}//end- readyAsixScales


void setup() {

  Wtft.readyAxisScales();

}

void loop() {

}
