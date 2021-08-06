
#ifndef _WEIGHT_H
#define _WEIGHT_H

#include "commonGlobal.h"
#include "InterruptHandler.h"


enum {
  GROSS = 0,
  NET   = 1,
  TARE  = 2
};

class weight : public WeighingHandle {
  private :
    uint8_t digitSpeed = 0;
    uint8_t updateField = GROSS;
    String temp;
    char asciiTemp[10];
    char asciiValues[3][10];
    char preAsciiValues[3][10];
    double numericValues[3];

    String _readbuf( );
    void onScreen(uint8_t showMeAt );
    void _rawCalculation();

    bool weightStripImage();
    bool printStringWeight( );
    void  handleTareCommand( char *Weight  );

  public :
    class WeighingHandle commFun;
    class userKeyBoard kbd;

    void  _start();
    void weightHandler();
    int8_t activityBasedOnTouch();

};




void weight :: _rawCalculation() {

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
  if ( updateField == NET ) {
    numericValues[GROSS] = numericValues[NET] + numericValues[TARE];
    updateField = GROSS;
    memset( asciiTemp, '\0', 10);
    sprintf( asciiTemp, "%lf", numericValues[updateField]);
    _rawCalculation();
    updateField = NET;
  }

}


void  weight :: weightHandler() {
  memset(asciiTemp, '\0', 10);
  strcpy( asciiTemp, _readbuf( ).c_str() );  asciiTemp[7] = '\0';

  SP(">>>> "); SPL(asciiTemp);

  if ( strlen(asciiTemp) > 5 ) {
    _rawCalculation();

    onScreen( GROSS );
    if ( ++digitSpeed > 3 ) {
      onScreen(NET);
      digitSpeed = 0;
    }
  }
  DateNTimeHandler();
  esp_task_wdt_reset();
}



void weight:: _start() {

  char timeArray[10];
  char preTime[10];

  initTFTHandler ( );
  printStringWeight( );

  /* This is command which is responsibale for starting continuous data from machine */
  class command cmd;
  if ( cmd.sendCommand(STARTDATA) ) {
    SPL("Ok Recevied ....!!!");
  } else {
    SPL("Ok Not Recevied ....!!!");
  }


  uint8_t tempdot = _getDecimal().c_str()[0];
  showDigits.dotPosition = tempdot - '0';

  updateField = GROSS;
  memset( asciiTemp, '\0', 10);
  strcpy(asciiTemp , "0.0");
  _rawCalculation();
  onScreen( GROSS );

  updateField = TARE;
  _rawCalculation();
  onScreen( TARE );

  updateField = NET;
  _rawCalculation();
  onScreen( NET );

  // get TARE data from Machine
  CMD_GETTARE
  _readbuf( );

  pressed = 0;


  TouchInterrupt_ptr = myfastfunction;

#ifdef ATTACH_INTER
  ATTACH_TOUCH_INTERRUPT
#elif
  ISR_Called = 1;
#endif

  ISR_dtech = 0;

}



void   weight :: onScreen(uint8_t showMeAt ) {
  bufferWithoutDot( showDigits.currentValue,  asciiValues[showMeAt] );
  showDigits.currentValue[6] = '\0';

  switch ( showMeAt ) {
    case GROSS      :  windowTwo(); break;
    case NET        :  windowOne();  break;
    case TARE       :  windowThree(); break;
  }
}




int8_t weight :: activityBasedOnTouch() {
  char src[12] = {0};

  switch (  getactiveMachineKeyMapping() )
  {
    case map_CmdESC   : pressed = 0; return -1;
    case map_CmdUnits : break; // as per requirement
    case map_CmdPrint : break; // as per requirement
    case map_CmdZero  : CMD_ZERODATA break;
    case map_CmdTare  : CMD_AUTOTARE break;
    case open_tareWin : goto TARE_WEIGHT;
  }//end-switch

  // if ( ( pressed = tft.getTouch(&xAxis, &yAxis) ) == 0 ) return 0;

  if ( pressed == 0 ) return 0;

  if ( pressed )
  {
    SPL("pressed : " + String(pressed));
    pressed = 0;

    if ( ESC_Touch )
    {
      SPL("ESCTouch...>>X:  " + String(xAxis) + "Y : " + String(yAxis) );
      xAxis = 0;
      yAxis = 0;
      STOP_SERIAL2
      EMPTY_SERIALBUFFER
      pressed = 0;

#ifdef ATTACH_INTER
      ISR_Called = 0;
      detachInterrupt(GPIOPin_TIRQ);
      ISR_dtech = 1;
#endif

      return -1;
    }
    else if ( Taretouch_Auto )
    {
      CMD_AUTOTARE
      return 0;
    }
    else if ( Zerotouch )
    {
      CMD_ZERODATA
      return 0;
    }
    else if ( Field_three_Touch  )
    {
TARE_WEIGHT :

      STOP_SERIAL2
      START_SERIAL1
      kbd.userInput.userInputArray_Size = 25;
      kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];
      kbd.init( );
      kbd.userInput.numericSwitchFlag = 1;
      strcpy( kbd.userInput.fieldLable, "TARE" );
      strcpy( kbd.userInput.unitbuf,  _getWeighingUnit().c_str() );
      kbd.takeUserInput(  NULL );
      SPL("keyboard : " + String( kbd.userInput.userInputArray ) );
      START_SERIAL2
      char cmd[20] = "\2T#";
      strcat(cmd, kbd.userInput.userInputArray );
      cmd[strlen(cmd)] = 3;
      Serial2.print(cmd);
      //  SPL("Manual Tare : " + String(cmd) );
      initTFTHandler();
      printStringWeight( );
      CMD_GETTARE
      _readbuf( );
      delete[]  kbd.userInput.userInputArray;
    }
  }//end-if
  return 0;
}



String  weight :: _readbuf( ) {
  temp =  "";
HERE :
  // 1. Get data from weighing machine
  if ( Serial2.available() > 12  )
  {
    temp = Serial2.readStringUntil('=');
  }

  //2. check, is String temp contains <STX> <ETX> ??
  int8_t stx = temp.indexOf(2);
  if ( stx >= 0 ){
    char ch = temp[stx + 1];
    switch (ch)
    {
      case '1':
        numericValues[GROSS] = 0.00;
        numericValues[NET] = 0.00;
        numericValues[TARE] = 0.00;
        handleTareCommand( "0.00"  );
        break;
      case 'T':
        handleTareCommand( (char *)temp.c_str() );
        break;

      default : break;
    }
    temp = "";
    goto HERE;
  }
  if (  temp.length() > 7 ){
    handleFlags( (char *)temp.c_str() );
  }
  
  return temp;
}



bool   weight :: weightStripImage() {
  tft.setSwapBytes(true);
  tft.pushImage(10, 0, WeightModeStripWidth, WeightModeStripHeight, WeightModeStrip );
}

bool  weight :: printStringWeight( ) {
  String weightUnit = _getWeighingUnit();
  weightStripImage();
  SPL("weightUnit : " + String(weightUnit) );
  tft.setTextSize( 1 );

  char dateArray[10];
  SPL("getDate+  : " + getRTCDate() );
  parshDateformat(dateArray,  getRTCDate().c_str() );
  SPL("parseDate  : " + String( dateArray ) );

  // Window -1
  tft.setFreeFont( (const GFXfont *)EUROSTILE_B13 );
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(18, 50);  tft.print("NET WT");
  tft.setCursor(400, 50);  tft.print(weightUnit);

  // Window -2
  tft.setFreeFont( (const GFXfont *)EUROSTILE_B10 );
  tft.setCursor(18, 173);  tft.print("GROSS WT");
  tft.setCursor(180, 173);   tft.print(weightUnit);

  // Window -3
  tft.setCursor(260, 173);  tft.print("TARE WT");
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



void weight :: handleTareCommand( char *Weight  ) {
  String cmdBuf = "";
  String temp = Weight;

  int8_t stx = temp.indexOf(2);
  int8_t etx = temp.indexOf(3);

  for (int8_t leftIdx = stx + 3; leftIdx < etx; ++leftIdx )
  {
    cmdBuf += temp[leftIdx];
  }

  updateField = TARE;
  memset( asciiTemp, '\0', 10);
  strcpy(asciiTemp , cmdBuf.c_str());
  _rawCalculation();
  onScreen( TARE );
  updateField = NET;
}


#endif
