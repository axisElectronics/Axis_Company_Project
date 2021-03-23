
#include "commonGlobal.h"

TFT_eSPI tft = TFT_eSPI();
class WeighingHandle Wtft;
class settings set;


void readyAxisScales()
{
  int Mode = 0;
  uint16_t xAxis, yAxis;


  Wtft.initWeighingTFT( );
HERE :
  Wtft.startUPImage();

  while (1)
  {
    if ( tft.getTouch( &xAxis, &yAxis) )
    {
      if ( SettingTouchEnable() )
      {
        Mode = 1;
        break;
      }
      else if ( WeighingModeTouchEnable()  )
      {
        if ( set.userSetting.weighingMode ) {
          Mode = 2;
          break;
        }
      }
      else if ( CountingModeTouchEnable() )
      {
        if ( set.userSetting.countingMode ) {
          Mode = 3; break;
        }
      }
      else if ( PriceingModeTouchEnable() )
      {
        if ( set.userSetting.pricecomputing ) {
          Mode = 4; break;
        }
      }
      else if ( CheckingModeTouchEnable() )
      {
        Mode = 5; break;
      }

    }
    yield();
  }//end-while()

  switch (Mode)
  {
    case 1:
      {
        set.settingPageInit( );
        SPL("I am Back my friends \n" );
        tft.fillScreen(TFT_BLACK);
        Mode = 0;
        goto HERE;
      }
      break;
    case 2:

      { if ( set.userSetting.weighingMode )
        {
          if ( !Wtft.startWeighing() )
          {
            Mode = 0;
            SPL("You have come back my friend again \n");
            goto HERE;
          }
        }
        break;
      case 3:
        if ( set.userSetting.countingMode )
        {
          if ( Wtft.startCountComputing() == -1 )
          {
            Mode = 0;
            goto HERE;
          }
        }
        break;
      case 4:
        if ( set.userSetting.pricecomputing )
        {
          if ( Wtft.startPriceComputing() == -1 )
          {
            Mode = 0;
            goto HERE;
          }
        } break;
      case 5: break;
      }
  }
}

void setup() {

  readyAxisScales();

}

void loop() {

}
