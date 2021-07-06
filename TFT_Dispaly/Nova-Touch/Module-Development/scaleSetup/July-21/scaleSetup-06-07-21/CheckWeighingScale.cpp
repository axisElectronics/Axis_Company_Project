

#include "commonGlobal.h"
#include "InterruptHandler.h"

#include "checkWeighing.h"


extern class userKeyBoard kbd;

#define CHECK_NetWeight 1
#define MAX   0
#define MIN   2

/****************************************************************************
                      ---->> Check Weighing <<----


****************************************************************************/

class checkWeighing checkObj; 

static uint32_t readSkipCount = 1;
int8_t WeighingHandle :: startCheckWeighing()
{
 
checkObj._start();
while (1) {
   if( checkObj.activityBasedOnTouch() == -1 ) return -1;
    checkObj.checkHandler();
    yield();
  }
}


bool  WeighingHandle :: checkStripImage()
{
  tft.setSwapBytes(true);
  tft.pushImage( 10, 0, checkingModeStripWidth, checkingModeStripHeight, checkingModeStrip );
}



bool  WeighingHandle :: printStringCHECK( )
{
  checkStripImage();
  String weightUnit = _getWeighingUnit();
  tft.setTextSize( 1 );
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  char dateArray[10];
  parshDateformat(dateArray, _getDate().c_str() );

  // Window-1
  tft.setFreeFont( (const GFXfont *)EUROSTILE_B13 );
  tft.setCursor(18, 50);  tft.print("TOTAL WT");
  tft.setCursor(400, 50);  tft.print(weightUnit);

  // Window-2
  tft.setFreeFont( (const GFXfont *)EUROSTILE_B10 );
  tft.setCursor(18, 173);  tft.print("MAX WT");
  tft.setCursor(180, 173);   tft.print(weightUnit);

  // window-3
  tft.setCursor(260, 173);  tft.print("MIN WT");
  tft.setCursor(420, 173);  tft.print(weightUnit);

  // Bottom strip flags.
  tft.setFreeFont( (const GFXfont *)EUROSTILE_B7 );
  tft.setTextColor(TFT_BLACK, TFT_BLACK);
  tft.setCursor(5, 310);
  tft.print( "Model: " + String("TAX")\
             + " Max: " + String( 30 ) + String( weightUnit) \
             + " Min: " + String( 10 ) + String( weightUnit) \
             + " e: " + String(1) + String("g") \
             + " class: " + String("ll") \
             + "  " + String( dateArray  ) );

  char timeArray[10];
  memset(timeArray, '\0', 10);
  parshTimeFormat(timeArray, getRTCTime() )
  tft.setCursor(400, 310);
  tft.print("  " + String(timeArray) + String("pm") );
}
