
#include "commonGlobal.h"

TFT_eSPI tft = TFT_eSPI();
class WeighingHandle Wtft;
//class settings set;

void WeighingHandle :: readyAxisScales()
{
  int Mode = 0;
  uint16_t xAxis, yAxis;

  Wtft.initWeighingTFT( );

  HERE : 
  settingPageInit( ); // import default settings
  
  SPL("\n******************************************************");
  SPL("dotp -->> 1 : " + String( _getDecimal() ) );
  uint8_t tempdot = _getDecimal().c_str()[0];
  showDigits.dotPosition = tempdot-48;
  SPL("dotp -->> 2 : " + String( tempdot ) );
  SPL("dotp -->> 3 : " + String( showDigits.dotPosition ) );
  SPL("******************************************************");  
 
  String temp = _getApplication();
  
  if ( temp == "Weighing Mode" ) userSetting.weighingMode =   1; else userSetting.weighingMode =   0;
  if ( temp == "Counting Mode" ) userSetting.countingMode =   1; else userSetting.countingMode =   0;
  if ( temp == "Check Weighing") userSetting.checkWeighing =  1; else userSetting.checkWeighing =   0;
  if ( temp == "Price Weighing") userSetting.pricecomputing = 1; else userSetting.pricecomputing =   0;

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
        if ( userSetting.weighingMode ) {
          Mode = 2;         
          break;
        }
      }
      else if ( CountingModeTouchEnable()  )
      {
        if ( userSetting.countingMode ) {
          Mode = 3; 
          break;
        }
      }
      else if ( PriceingModeTouchEnable() )
      {
        if ( userSetting.pricecomputing ) {
          Mode = 4;
          break;
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
        settingPageHandler( );
        SPL("I am Back my friends \n" );
        tft.fillScreen(TFT_BLACK);
        Mode = 0;       
        goto HERE;
      }
      break;
    case 2: 
      if ( Wtft.startWeighing()  == -1 )
      {
        Mode = 0;
        goto HERE;
      }
      break;
    case 3:
      if ( userSetting.countingMode )
      {
        if ( Wtft.startCountComputing() == -1 )
        {
          Mode = 0;
          goto HERE;
        }
      }
      break;
    case 4:
      if ( userSetting.pricecomputing )
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


void setup() {

   Wtft.readyAxisScales();

}

void loop() {
  
}
