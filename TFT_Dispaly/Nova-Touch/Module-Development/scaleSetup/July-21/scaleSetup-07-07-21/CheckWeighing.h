

#ifndef __CHECK_H
#define __CHECK_H


#include "commonGlobal.h"
#include "InterruptHandler.h"


enum {
  Nor = 0,
  Hi = 1,
  Low = 2,
  MAX = 0,
  CHECK_NetWeight = 1,
  MIN = 2
};


class checkWeighing: public WeighingHandle
{
  private :
    int8_t flag = Nor;
    uint8_t updateField = CHECK_NetWeight;
    String temp;
    char asciiTemp[10];
    char asciiValues[3][10];
    char preAsciiValues[3][10];
    double numericValues[3], tareWeight = 0.0;

    String _readbuf( );
    void onScreen(uint8_t showMeAt );
    void _rawCalculation();
    void DateNTimeHandler();
    bool printStringCHECK( );
    bool checkStripImage();

  public :
    class WeighingHandle commFun;
    class userKeyBoard kbd;

    void  _start();
    void checkHandler();
    int8_t activityBasedOnTouch();
};



void  checkWeighing :: DateNTimeHandler() {
  char timeArray[10];
  char preTime[10];

  //======================================================
    // write only when pre and current values of Time are different
    // memset(timeArray, '\0', 10);
    parshTimeFormat(timeArray, getRTCTime().c_str() )
    if ( strcmp( preTime, timeArray ) ) {
      tft.setFreeFont( (const GFXfont *)EUROSTILE_B7 );
      tft.setTextColor(TFT_BLACK, TFT_BLACK);
      tft.fillRect(400, 295, 50, 20, TFT_WHITE);
      tft.setCursor(400, 310);
      tft.print("  " + String(timeArray) + String("pm") );
      //    SPL("  " + String(timeArray) + String("pm") );
      strcpy( preTime, timeArray );
    }
    //======================================================
}


int8_t  checkWeighing ::  activityBasedOnTouch() {
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

  if ( ( pressed = tft.getTouch(&xAxis, &yAxis) ) == 0 ) return 0;
  //  if ( pressed == 0 ) return 0;
  if ( pressed )
  {
    pressed = 0;
    if ( Field_Two_Touch  ) {
      updateField = MAX;


    } else if ( Field_three_Touch ) {
      updateField = MIN;
    }
    else if ( Zerotouch )     {
      CMD_ZERODATA
    }
    else if ( Taretouch_Auto ) {
      CMD_AUTOTARE
    }
    else if ( ESC_Touch ) {
      SPL("ESCTouch...\n");
      return -1;
    }
    else
      SPL("xAxis : " + String(xAxis ) + "\nyAxis : " + String(yAxis) );
  }//end-if( TFT_Touch )

  if ( updateField != CHECK_NetWeight ) {

    STOP_SERIAL2
    START_SERIAL1
    kbd.userInput.userInputArray_Size = 25;
    kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];
    kbd.init(  );
    kbd.userInput.numericSwitchFlag = 1;
    kbd.takeUserInput( NULL );
    SPL("keyboard : " + String( kbd.userInput.userInputArray ) );

    if ( strlen( kbd.userInput.userInputArray ) > 0 ) {
      strcpy(asciiTemp, kbd.userInput.userInputArray );
      _rawCalculation();
    }
    STOP_SERIAL1
    START_SERIAL2

    initTFTHandler();
    printStringCHECK( );

    onScreen(CHECK_NetWeight);
    onScreen(MAX);
    onScreen(MIN);

    delete[] kbd.userInput.userInputArray;
    updateField = CHECK_NetWeight;
    START_SERIAL1
  }//end-if( updateField )
  return 0;
}


void   checkWeighing :: onScreen(uint8_t showMeAt ) {
  bufferWithoutDot( showDigits.currentValue,  asciiValues[showMeAt] );
  showDigits.currentValue[6] = '\0';

  switch ( showMeAt ) {
    case CHECK_NetWeight:  windowOne(); break;
    case MAX :  windowTwo(); break;
    case MIN  : windowThree(); break;
  }
}


/*
   NOte : There are two parameter which is need to be care.Before calling this function these two field must be updated.
          1 >> updateField -->> this is the field which you want to update
          2 >> asciiTemp -->> this the temprary Array variable which you want to update.
*/
void   checkWeighing ::_rawCalculation() {

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

  if ( (numericValues[updateField] >  numericValues[MIN]) && ( numericValues[updateField] <  numericValues[MAX] ) ) {
    if (  showDigits.showDigitColorWindowOne != TFT_GREEN ) {
      strcpy( showDigits.preValue[0], "ABCDEFGH" );
      showDigits.showDigitColorWindowOne = TFT_GREEN;
    }
  }
  else if (  numericValues[updateField] >=  numericValues[MAX] ) {
    if (  showDigits.showDigitColorWindowOne != TFT_RED ) {
      strcpy( showDigits.preValue[0], "ABCDEFGH" );
      showDigits.showDigitColorWindowOne = TFT_RED;
    }
  } else if ( numericValues[updateField] <=  numericValues[MIN] ) {
    if (  showDigits.showDigitColorWindowOne != TFT_YELLOW ) {
      strcpy( showDigits.preValue[0], "ABCDEFGH" );
      showDigits.showDigitColorWindowOne = TFT_YELLOW;
    }
  }

}//end-_rawCalcualtion()



void  checkWeighing :: checkHandler() {
  memset(asciiTemp, '\0', 10);
  strcpy( asciiTemp, _readbuf( ).c_str() );  asciiTemp[7] = '\0';
  if ( strlen(asciiTemp) > 5 ) {
    _rawCalculation();

    onScreen( CHECK_NetWeight );
  }

   DateNTimeHandler();
}

String checkWeighing :: _readbuf( ) {
  temp =  "";
HERE :
  // 1. Get data from weighing machine
  //if( Serial2.available() > 0 )
  temp = Serial2.readStringUntil('=');

  //2. check, is String temp contains <STX> <ETX> ??
  int8_t stx = temp.indexOf(2);
  if ( stx >= 0 )
  {
    char ch = temp[stx + 1];
    switch (ch)
    {
      case '1'://Zero Command Response
        tareWeight = 0.0;
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

void  checkWeighing :: _start() {
   char timeArray[10];
  char preTime[10];

  initTFTHandler ( );
  printStringCHECK( );
  parshTimeFormat(timeArray, getRTCTime().c_str() )
  strcpy( preTime, timeArray );
  showDigits.dotPosition = _getDecimal().c_str()[0] - 48;

  updateField = MAX;
  strcpy(asciiTemp, "0.0" );
  _rawCalculation();

  updateField = MIN;
  strcpy(asciiTemp, "0.0" );
  _rawCalculation();

  onScreen(MAX);
  onScreen(MIN);

  updateField = CHECK_NetWeight;
  pressed = 0;
  TouchInterrupt_ptr = myfastfunction;
  ATTACH_TOUCH_INTERRUPT
  ISR_dtech = 0;

}


bool  checkWeighing :: checkStripImage(){
  tft.setSwapBytes(true);
  tft.pushImage( 10, 0, checkingModeStripWidth, checkingModeStripHeight, checkingModeStrip );
}



bool  checkWeighing :: printStringCHECK( ){
  checkStripImage();
  String weightUnit = _getWeighingUnit();
  tft.setTextSize( 1 );
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  char dateArray[10];
  parshDateformat(dateArray, getRTCDate().c_str() );

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

#endif
