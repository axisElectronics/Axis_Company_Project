
#ifndef _PRICE_H
#define _PRICE_H


#include "commonGlobal.h"
#include "InterruptHandler.h"


enum {
  PRICE_perPcs = 0,
  PRICE_Total = 1,
  PRICE_NetWeight = 2,
};

class price : public WeighingHandle
{
  private :
    uint8_t updateField = PRICE_NetWeight;
    String temp;
    char asciiTemp[10];
    char asciiValues[3][10];
    char preAsciiValues[3][10];
    double numericValues[3];

    String _readbuf( );
    void onScreen(uint8_t showMeAt );
    void _rawCalculation();

    bool printStringPrice( );
    bool priceStripImage();

  public :
    class WeighingHandle commFun;
    class userKeyBoard kbd;

    void  _start();
    void priceHandler();
    int8_t activityBasedOnTouch();

};





int8_t  price ::  activityBasedOnTouch() {
  char src[12] = {0};
  int8_t tempDot = showDigits.dotPosition;

  switch (  getactiveMachineKeyMapping() )
  {
    case map_CmdESC   :  pressed = 0; return -1;
    case map_CmdUnits : break; // as per requirement
    case map_CmdPrint : break; // as per requirement
    case map_CmdZero  : CMD_ZERODATA break;
    case map_CmdTare  : CMD_AUTOTARE break;
  }//end-switch

 // if ( ( pressed = tft.getTouch(&xAxis, &yAxis) ) == 0 ) return 0;
    if ( pressed == 0 ) return 0;

  if ( pressed ) {
    pressed = 0;

    if ( Taretouch_Auto ) {
      START_SERIAL2
      CMD_AUTOTARE
    }
    else if ( Zerotouch ) {
      SPL("Zero Touch...!!!");
      START_SERIAL2
      CMD_ZERODATA
    }
    else if ( ESC_Touch ) {
      SPL("ESCTouch...\n");
      return -1;
    }
    else if ( Field_Two_Touch  )
    {
      STOP_SERIAL2
      START_SERIAL1

      kbd.userInput.userInputArray_Size = 25;
      kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];
      kbd.init(  );
      kbd.userInput.numericSwitchFlag = 1;

      kbd.takeUserInput( NULL );
      SPL("1-keyboard : " + String( kbd.userInput.userInputArray ) );

      updateField = PRICE_perPcs;
      strcpy( asciiTemp,  kbd.userInput.userInputArray );
      _rawCalculation();

      // Load screen images again.
      START_SERIAL2
      STOP_SERIAL1
         
      initTFTHandler();
      printStringPrice();

      // SHOW DATA ON SCREEN
      strcpy( preAsciiValues[PRICE_Total] , "ABCDEFGH" );
      strcpy( preAsciiValues[PRICE_NetWeight] , "ABCDEFGH" );
       
      onScreen(PRICE_perPcs);
      onScreen(PRICE_NetWeight);
      onScreen(PRICE_Total);    
      updateField = PRICE_NetWeight;   
              
      delete[]  kbd.userInput.userInputArray;
      START_SERIAL1
    }
  }//end-if
  return 0;
}



void   price :: onScreen(uint8_t showMeAt ) {
  bufferWithoutDot( showDigits.currentValue,  asciiValues[showMeAt] );
  showDigits.currentValue[6] = '\0';

  switch ( showMeAt ) {
    case PRICE_perPcs      :  windowTwo(); break;
    case PRICE_Total       :  windowOne();  break;
    case PRICE_NetWeight   :  windowThree(); break;
  }
}


/*
   NOte : There are two parameter which is need to be care.Before calling this function these two field must be updated.
          1 >> updateField -->> this is the field which you want to update
          2 >> asciiTemp -->> this the temprary Array variable which you want to update.
*/
void   price ::_rawCalculation() {

  uint8_t dotpos;
  if ( !strcmp( preAsciiValues[updateField] , asciiTemp) ) return;
  dotpos = 0;
  numericValues[updateField] = strtod(asciiTemp, NULL);
 
  // convert double values into equivalent ASCII values
  memset( asciiValues[updateField], '\0', 10);
  sprintf( asciiValues[updateField], "%lf", numericValues[updateField]);

  // Adjust dot postion in Array
  findDotPosition( asciiValues[updateField], dotpos);
  // check valid Dot position.[user set this during calibration]
  // Bug Fix : showDigits.dotPosition was corrupting again & again.
  showDigits.dotPosition = _getDecimal().c_str()[0] - '0';

  memset(asciiTemp, '0', 10);
  for (int8_t i = 0, j = ( 6 - showDigits.dotPosition - dotpos); ( i < 7 ) && ( j < 7 ) ; i++, j++ ) {
    asciiTemp[ j ] = asciiValues[updateField][i];
  }
  asciiTemp[7] = '\0';

  strcpy( asciiValues[updateField], asciiTemp );
  asciiValues[updateField][7] = '\0';
  //save current value for later use
  strcpy( preAsciiValues[updateField] , asciiValues[updateField]);

//main calculation
  if( updateField == PRICE_NetWeight ){
    numericValues[PRICE_Total] = numericValues[PRICE_NetWeight] * numericValues[PRICE_perPcs];
    updateField = PRICE_Total;
    memset( asciiTemp, '\0', 10);
    sprintf( asciiTemp, "%lf", numericValues[updateField]);
    _rawCalculation();
     updateField = PRICE_NetWeight;
  }
  

}//end-_rawCalcualtion()



void  price :: priceHandler() {
  memset(asciiTemp, '\0', 10);
  strcpy( asciiTemp, _readbuf( ).c_str() );  asciiTemp[7] = '\0';
  if ( strlen(asciiTemp) > 5 ) {
    _rawCalculation();

    onScreen( PRICE_NetWeight );
    onScreen(PRICE_Total);
  }

   DateNTimeHandler();
}


String price :: _readbuf( ) {
  temp =  "";
HERE :
  // 1. Get data from weighing machine
  if ( Serial2.available() > 0 )
    temp = Serial2.readStringUntil('=');

  //2. check, is String temp contains <STX> <ETX> ??
  int8_t stx = temp.indexOf(2);
  if ( stx >= 0 )
  {
    char ch = temp[stx + 1];
    switch (ch)
    {
      case '1'://Zero Command Response
      //  tareWeight = 0.0;
        break;
      case 'T':
        // Nothing to do.
        break;

      default : break;
    }
    temp = "";
    goto HERE;
  }

  handleFlags( (char *)temp.c_str() );
  return temp;
}




void  price :: _start() {
  char timeArray[10];
  char preTime[10];

  initTFTHandler ( );
  printStringPrice();
  showDigits.dotPosition = _getDecimal().c_str()[0] - 48;;

   updateField = PRICE_Total;
  strcpy(asciiTemp, "0.0" );
  _rawCalculation();

  updateField = PRICE_perPcs;
  strcpy(asciiTemp, "0.0" );
  _rawCalculation();

  onScreen(PRICE_perPcs);
  onScreen(PRICE_Total);

  updateField = PRICE_NetWeight;

  pressed = 0;
  TouchInterrupt_ptr = myfastfunction;
  ATTACH_TOUCH_INTERRUPT
  ISR_dtech = 0;


}







bool  price :: priceStripImage()
{
  tft.setSwapBytes(true);
  tft.pushImage(10, 0, PriceComputingModeStripWidth, PriceComputingModeStripHeight, PriceComputingModeStrip );
}


bool  price :: printStringPrice( ){
   String weightUnit = "kg";
  priceStripImage();
  tft.setTextSize( 1 );
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

 char dateArray[10];
  parshDateformat(dateArray, getRTCDate().c_str() );

  tft.setFreeFont( (const GFXfont *)EUROSTILE_B13 );
  tft.setCursor(18, 50);  tft.print("TOTAL Price");
  //  tft.setCursor(400, 50);  tft.print(weightUnit);

  tft.setFreeFont( (const GFXfont *)EUROSTILE_B10 );
  tft.setCursor(18, 173);  tft.print("Unit Price");
  tft.setCursor(180, 173);   tft.print(weightUnit);

  tft.setCursor(260, 173);  tft.print("Total WT");
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
            + "  " + String(dateArray)); 
}

#endif
