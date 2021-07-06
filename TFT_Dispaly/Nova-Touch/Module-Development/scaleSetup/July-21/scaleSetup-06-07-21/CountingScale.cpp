
#include "commonGlobal.h"
#include "InterruptHandler.h"

#include "Counting.h"

extern class userKeyBoard kbd;
char netWeight[10];




/****************************************************************************
                    >>>>> COUNT   <<<<<<

   @ startCountComputing() :
    - This funcation is main starting point for user.
       call this funcation. Leave reset to handle price
       computing. It will take care all activity. User need  not call anyother function.

    ==================================================================================


 *******************************************************************************/
  class countingMachine countM;
  
int8_t  WeighingHandle :: startCountComputing()
{

  countM._start();
  while (1) {
   if( countM.activityBasedOnTouch() == -1 ) return -1;
    countM.countHandler();
    yield();
  }
}//end-startCountingComputing




void  WeighingHandle :: updateTotalPcsWindow()
{
  static int8_t cnt_1 = 0;
  int8_t leadingZero = 0;
  uint8_t dotpos = 0;

  //3. check with previous Value
  // one index ahead due to one leading Zero.
  // Find Raw double Value and then reverse it
  // find dotposition so that you can eleminate all other
  sprintf( FromMachineArray[COUNT], "%lf", FromMachine[COUNT]);
  if ( FromMachineArray[COUNT][0] == '-' ) return;
  String temp = String( FromMachineArray[COUNT] );
  reverse(temp.begin(), temp.end());

  findDotPosition(  temp.c_str(), dotpos);

  strcpy(showDigits.currentValue, temp.c_str() );

  if ( !strcmp( showDigits.preValue[COUNT], showDigits.currentValue ) )  return;

  //check valid Dot position.
  /*
     Bug Fix : showDigits.dotPosition was corrupting again & again.
  */
  showDigits.dotPosition = _getDecimal().c_str()[0] - '0';
  if (  !( showDigits.dotPosition < 5 &&  showDigits.dotPosition > 0 ) ) {
    SPL("Error : showDigits.dotPosition : " + String( showDigits.dotPosition ));
    return;
  }

  // 1. Set Font Size & Style
  showDigits.digitFontSize  = 1;
  showDigits.digitFontStyle = LED7SEG_STD40;
  tft.setTextSize( showDigits.digitFontSize );
  tft.setFreeFont( (const GFXfont *)showDigits.digitFontStyle );

  // 2. remove all dots
  for (uint8_t idx = 0; idx < 6; ++idx )
  {
    tft.fillCircle( 73 + ( idx * 65 ), 138, 6, TFT_BLACK);
  }


  //6. draw Digits only,, Those digits which is different from previous Value.

  tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
  for (uint8_t idx = dotpos, digitpos = 7; temp[idx] != NULL || showDigits.preValue[COUNT][idx] != NULL; ++idx, --digitpos )
  {
    if ( showDigits.preValue[COUNT][idx] !=  showDigits.currentValue[idx] )
    {
      tft.drawChar( '8', 15 + ( digitpos * 65), 130,  1);
    }
  }


  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  for (uint8_t idx = dotpos, digitpos = 7; temp[idx] != NULL; ++idx, --digitpos )
  {
    if ( showDigits.preValue[COUNT][idx] !=  showDigits.currentValue[idx] )
    {
      tft.drawChar( showDigits.currentValue[idx], 15 + ( digitpos * 65), 130,  1);
    }
  }

  // end : update preVlaue
  strcpy( showDigits.preValue[COUNT], temp.c_str() );
  showDigits.preValue[COUNT][strlen( showDigits.preValue[COUNT]) ] = '\0';

}



bool  WeighingHandle :: countStripImage()
{
  tft.setSwapBytes(true);
  tft.pushImage( 10, 0, CountingModeStripWidth, CountingModeStripHeight, CountingModeStrip );
}


bool  WeighingHandle :: printStringCOUNT( )
{
  countStripImage();
  String weightUnit = _getWeighingUnit();
  tft.setTextSize( 1 );
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  char dateArray[10];
  parshDateformat(dateArray, _getDate().c_str() );

  // Window-1
  tft.setFreeFont( (const GFXfont *)EUROSTILE_B13 );
  tft.setCursor(18, 50);  tft.print("TOTAL PCs");
  //  tft.setCursor(400, 50);  tft.print(weightUnit);

  // Window-2
  tft.setFreeFont( (const GFXfont *)EUROSTILE_B10 );
  tft.setCursor(18, 173);  tft.print("unit WT");
  tft.setCursor(180, 173);   tft.print(weightUnit);

  // window-3
  tft.setCursor(260, 173);  tft.print("Net WT");
  tft.setCursor(420, 173);  tft.print(weightUnit);

  // Bottom strip flags.
  // Bottom strip flags.
  tft.setFreeFont( (const GFXfont *)EUROSTILE_B7 );
  tft.setTextColor(TFT_BLACK, TFT_BLACK);
  tft.setCursor(5, 310);
  tft.print( "Model: " + String("TAX")\
             + " Max: " + String( 30 ) + String( weightUnit) \
             + " Min: " + String( 10 ) + String( weightUnit) \
             + " e: " + String(1) + String("g") \
             + " class: " + String("ll") \
             + "  " + String(dateArray) );

}
