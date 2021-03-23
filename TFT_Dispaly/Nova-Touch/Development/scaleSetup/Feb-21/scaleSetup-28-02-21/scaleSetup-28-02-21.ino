
#include "commonGlobal.h"

TFT_eSPI tft = TFT_eSPI();
class WeighingHandle Wtft;
class settings set;

bool loaded = false;
void readyAxisScales()
{
//  int Mode = 0;
//  uint16_t xAxis, yAxis;


  Wtft.initWeighingTFT( );
  set.settingPageInit( ); // import default settings
  String temp = set._getApplication();
  
  if ( temp == "Weighing Mode" ) set.userSetting.weighingMode =   1;
  if ( temp == "Counting Mode" ) set.userSetting.countingMode =   1;
  if ( temp == "Check Weighing") set.userSetting.checkWeighing =  1;
  if ( temp == "Price Weighing") set.userSetting.pricecomputing = 1;
/*  
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
      else if ( CountingModeTouchEnable()  )
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
        Mode = 5; //break;
      }

    }
    yield();
  }//end-while()

  switch (Mode)
  {
    case 1:
      {
        set.settingPageHandler( );
        SPL("I am Back my friends \n" );
        tft.fillScreen(TFT_BLACK);
        Mode = 0;
        goto HERE;
      }
      break;
    case 2:
    Wtft.startWeighing();
      if (err_ret  == -1 )
      {
        Mode = 0;
        goto HERE;
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
  */
}

int Mode = 0;
uint16_t xAxis, yAxis;
void setup() {

  readyAxisScales();
  loaded = true;
}

void loop() {
  
  if(loaded)
  {
    Wtft.startUPImage();
    loaded = false;
  }
  if ( tft.getTouch( &xAxis, &yAxis) )
    {
      if ( SettingTouchEnable() )
      {
        Mode = 1;
        //break;
      }
      else if ( WeighingModeTouchEnable()  )
      {       
        if ( set.userSetting.weighingMode ) {
          Mode = 2;         
          //break;
        }
      }
      else if ( CountingModeTouchEnable()  )
      {
        if ( set.userSetting.countingMode ) {
          Mode = 3; //break;
        }
      }
      else if ( PriceingModeTouchEnable() )
      {
        if ( set.userSetting.pricecomputing ) {
          Mode = 4; //break;
        }
      }
      else if ( CheckingModeTouchEnable() )
      {
        Mode = 5; //break;
      }

    }
    yield();

  switch (Mode)
  {
    case 1:
      {
        set.settingPageHandler( );
        SPL("I am Back my friends \n" );
        tft.fillScreen(TFT_BLACK);
        Mode = 0;
        loaded = true;
      }
      break;
    case 2:
    
    Wtft.startWeighing();
    
      if (err_ret  == -1 )
      {
        Mode = 0;
        loaded = true;
      }
      break;
    case 3:
      if ( set.userSetting.countingMode )
      {
        if ( Wtft.startCountComputing() == -1 )
        {
          Mode = 0;
          loaded = true;
        }
      }
      break;
    case 4:
      if ( set.userSetting.pricecomputing )
      {
        if ( Wtft.startPriceComputing() == -1 )
        {
          Mode = 0;
          loaded = true;
        }
      } break;
    case 5: break;
  }
}
