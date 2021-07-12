
#ifndef COUNTING_H
#define COUNTING_H

#include <math.h>
#include "commonGlobal.h"
#include "InterruptHandler.h"

enum {
  COUNT_UnitWeight = 0,
  COUNT,
  COUNT_NetWeight
};


class countingMachine: public WeighingHandle {
  private :
    String temp =  "";

    void _calculateCounts();
    String _readbuf( );
    void onScreen(uint8_t showMeAt );
    bool printStringCOUNT( );
    bool countStripImage();


  public :
    bool flag = 0;
    class WeighingHandle commFun;
    class userKeyBoard kbd;

    char asciiTemp[15];
    int8_t Dotpos;
    double tempCountFlag;
    char asciiValues[3][15];
    char preAsciiValues[3][15];
    double numericValues[3];
    uint8_t updateField = COUNT_NetWeight;

    void  _start();
    void countHandler();
    void _rawCalculation();

    int8_t activityBasedOnTouch();
    void showCount();
    void updateTotalPcsWindow();
    void  updateUnitWindow();
};



int8_t countingMachine:: activityBasedOnTouch() {

  switch (  getactiveMachineKeyMapping() ){
    case map_CmdESC   : pressed = 0; return -1;
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
    else if ( Field_Two_Touch  ){
      STOP_SERIAL2
      START_SERIAL1

      kbd.userInput.userInputArray_Size = 25;
      kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];
      kbd.init(  );
      kbd.userInput.numericSwitchFlag = 1;

      kbd.takeUserInput( NULL );
      SPL("1-keyboard : " + String( kbd.userInput.userInputArray ) );

      // update unite weight fields
      updateField = COUNT_UnitWeight;
      strcpy( asciiTemp,  kbd.userInput.userInputArray );
      
      _rawCalculation();
      numericValues[updateField] /=  1000;
      _rawCalculation();
      
      numericValues[COUNT] = 0.0;
      // Load screen images again.
      STOP_SERIAL1
      START_SERIAL2

      initTFTHandler();
      printStringCOUNT( );

      // SHOW DATA ON SCREEN
      onScreen(COUNT_UnitWeight);
      onScreen(COUNT_NetWeight);
      onScreen(COUNT);
      updateField = COUNT_NetWeight;
      delete[]  kbd.userInput.userInputArray;
      START_SERIAL1
      START_SERIAL2
    }
    else  if ( Field_One_Touch  ) {
      SPL("X : " + String(xAxis) + ", Y : " + String( yAxis ) );
      STOP_SERIAL2
      START_SERIAL1

      kbd.userInput.userInputArray_Size = 25;
      kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];
      kbd.init(  );
      kbd.userInput.numericSwitchFlag = 1;

      kbd.takeUserInput( NULL );
      SPL("1-keyboard : " + String( kbd.userInput.userInputArray ) );

      updateField = COUNT;
      numericValues[COUNT_UnitWeight] = 0.0;
      strcpy( asciiTemp,  kbd.userInput.userInputArray );
      _rawCalculation();
      updateField = COUNT_NetWeight;

      // Load screen images again.
      STOP_SERIAL1
      START_SERIAL2

      initTFTHandler();
      printStringCOUNT( );

      // SHOW DATA ON SCREEN
       strcpy( showDigits.preValue[1], "ABCDEFGH" );
      onScreen(COUNT_UnitWeight);
      onScreen(COUNT_NetWeight);
      onScreen(COUNT);

      delete[]  kbd.userInput.userInputArray;
      START_SERIAL1
      START_SERIAL2
    }
  }//end-if
}//end-



void countingMachine::  showCount() {

  // main code for counting
  if ( ( numericValues[COUNT_UnitWeight] == 0.0 ) && ( numericValues[COUNT] == 0.0 ) ) {
    for (int8_t i = 0; i < 8; i++ ) {
      // if ( !strcmp( preAsciiValues[updateField] , asciiTemp) ) break;;
      if ( i == showDigits.dotPosition ) {
        asciiValues[COUNT_UnitWeight][i] =  '.';
        asciiValues[COUNT][i] =  '.';
      }
      else {
        asciiValues[COUNT_UnitWeight][i] =  '0';
        asciiValues[COUNT][i] =  '0';
      }
      asciiValues[COUNT_UnitWeight][7] =  '\0';
      asciiValues[COUNT][7] =  '\0';
    }
    // SPL("unite weight is Zero");
  }
  else {
    // calculate count when we have unite weight is not zero.
    if ( ( numericValues[COUNT_UnitWeight] > 0.0 ) && ( numericValues[COUNT] == 0.0 )) {
      numericValues[COUNT] = ( numericValues[COUNT_NetWeight] / numericValues[COUNT_UnitWeight] );
      // now update count fields
      updateField = COUNT;
      memset( asciiTemp, '\0', 15);
      sprintf( asciiTemp, "%lf", numericValues[updateField]);
      _rawCalculation();
      updateField = COUNT_NetWeight;
    }
    else {
      // calculate unite weight when we have count field is not zero.
      if ( ( numericValues[COUNT_UnitWeight] == 0.0 ) && ( numericValues[COUNT] > 0.0 ) ) {
        numericValues[COUNT_UnitWeight] = ( numericValues[COUNT_NetWeight] /  numericValues[COUNT] );
        // now update count fields
        updateField = COUNT_UnitWeight;
        memset( asciiTemp, '\0', 15);
        sprintf( asciiTemp, "%lf", numericValues[updateField]);
        _rawCalculation();
        onScreen(updateField);
        updateField = COUNT_NetWeight;
        // by doing so, now whole count calculation process will be done by above part of code.
        numericValues[COUNT] = 0.00;
      }
      else {
        // when both unite and count field are not equals to zero.
        // check if preValue of unite weight has been chenged or NOT.
        // if unite weight has been changed then count update accordingly.
        // calculate count when we have unite weight is not zero.
        if ( ( numericValues[COUNT_UnitWeight] > 0.0 ) && ( numericValues[COUNT] > 0.0 )) {
          numericValues[COUNT] = ( numericValues[COUNT_NetWeight] / numericValues[COUNT_UnitWeight] );
          // now update count fields
          updateField = COUNT;
          memset( asciiTemp, '\0', 15);
          sprintf( asciiTemp, "%lf", numericValues[updateField]);
          _rawCalculation();
          updateField = COUNT_NetWeight;
        }
      }
    }
  }
}


void countingMachine:: onScreen(uint8_t showMeAt ) {
  bufferWithoutDot( showDigits.currentValue,  asciiValues[showMeAt] );
  showDigits.currentValue[6] = '\0';

  switch ( showMeAt ) {
    case COUNT            :  updateTotalPcsWindow(); break;
    case COUNT_UnitWeight :   updateUnitWindow(); /* windowTwo(); */  break;
    case COUNT_NetWeight  :  windowThree(); break;
  }
}//end-onScreen


/*
   NOte : There are two parameter which is need to be care.Before calling this function these two field must be updated.
          1 >> updateField -->> this is the field which you want to update
          2 >> asciiTemp -->> this the temprary Array variable which you want to update.
*/
void countingMachine:: _rawCalculation( ) {
  uint8_t dotpos;
  if ( !strcmp( preAsciiValues[updateField] , asciiTemp) ) return;
  dotpos = 0;
  numericValues[updateField] = strtod( asciiTemp, NULL);
  // convert double values into equivalent ASCII values
  memset( asciiValues[updateField], '\0', 15);
  sprintf( asciiValues[updateField], "%lf", numericValues[updateField]);

  if ( updateField == COUNT ) {
    asciiValues[updateField][7] = '\0';
    //save current value for later use
    strcpy( preAsciiValues[updateField] , asciiValues[updateField]);
    return;
  }
  else {
    // Adjust dot postion in Array
    findDotPosition( asciiValues[updateField], dotpos);
    // check valid Dot position.[user set this during calibration]
    // Bug Fix : showDigits.dotPosition was corrupting again & again.
    showDigits.dotPosition = _getDecimal().c_str()[0] - '0';
    memset(asciiTemp, '0', 15);
    for (int8_t i = 0, j = ( 6 - showDigits.dotPosition - dotpos); ( i < 7 ) && ( j < 7 ) ; i++, j++ ) {
      asciiTemp[ j ] = asciiValues[updateField][i];
    }
    asciiTemp[7] = '\0';

    strcpy( asciiValues[updateField], asciiTemp );
  }//end-else

  asciiValues[updateField][7] = '\0';
  //save current value for later use
  strcpy( preAsciiValues[updateField] , asciiValues[updateField]);
}//end-_rawClaculation





void countingMachine:: countHandler() {

  memset(asciiTemp, '\0', 10);
  strcpy( asciiTemp, _readbuf( ).c_str() );  asciiTemp[7] = '\0';
  if ( strlen(asciiTemp) > 5 ) {
    _rawCalculation();
    showCount();
    onScreen( COUNT_NetWeight );
    onScreen( COUNT );
  }
  DateNTimeHandler();
}//end-countHandler





String  countingMachine:: _readbuf( ) {
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
    SP("Response :>> "); SPL(temp);
    switch (ch)
    {
      case '1'://zero command
        SPL("Zero response received....!!!");
        numericValues[COUNT_NetWeight] = numericValues[COUNT] = 0.00;
        strcpy( preAsciiValues[COUNT_NetWeight], "ABCDEFG");
        strcpy( preAsciiValues[COUNT], "ABCDEFG");
        break;
      case 'T':
        // handleTareCommand( (char *)temp.c_str() );
        break;

      default : break;
    }
    temp = "";
    goto HERE;
  }
  // handle flages
  handleFlags( (char *)temp.c_str() );
  return temp;
}//end-_readCount



void countingMachine:: _start() {
  char src[10];
  char timeArray[10];
  char preTime[10];

  initTFTHandler ( );
  printStringCOUNT( );
  Dotpos = showDigits.dotPosition = _getDecimal().c_str()[0] - 48;

  numericValues[COUNT] = numericValues[COUNT_NetWeight] = numericValues[COUNT_UnitWeight] = 0.00;
  strcpy(preAsciiValues[COUNT], "ABCDEFGH");
  strcpy(preAsciiValues[COUNT_NetWeight], "ABCDEFGH");
  strcpy(preAsciiValues[COUNT_UnitWeight], "ABCDEFGH");

  updateField = COUNT_UnitWeight;
  strcpy(asciiTemp, "0.00");
  _rawCalculation();
  onScreen(COUNT_UnitWeight);


  pressed = 0;
  TouchInterrupt_ptr = myfastfunction;

#ifdef ATTACH_INTER
  ATTACH_TOUCH_INTERRUPT
#elif
  ISR_Called = 1;
#endif

  ISR_dtech = 0;

  updateField = COUNT_NetWeight;
}






bool  countingMachine :: countStripImage() {
  tft.setSwapBytes(true);
  tft.pushImage( 10, 0, CountingModeStripWidth, CountingModeStripHeight, CountingModeStrip );
}


bool  countingMachine :: printStringCOUNT( ) {
  countStripImage();
  String weightUnit = _getWeighingUnit();
  tft.setTextSize( 1 );
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  char dateArray[10];
  parshDateformat(dateArray, getRTCDate().c_str() );

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


void  countingMachine :: updateTotalPcsWindow() {
  temp = String( asciiValues[COUNT] );
  uint8_t tempDotPos = temp.indexOf('.');
  char tempArray[15];
  int8_t leadingZero = 0;

  if ( tempDotPos < 7 ) {
    // tempArray have data without dot.
    memset(tempArray, '\0', 15);
    memset(tempArray, '0', 7 - tempDotPos);
    asciiValues[COUNT][tempDotPos] = '\0';
    strcat(tempArray, asciiValues[COUNT]);
  } else return;
#if 0
  SPL("ascii : " + String( asciiValues[COUNT] ) );
  SPL("numeric : " + String( numericValues[COUNT] ) );
  SPL();
  SPL("unite : " + String(asciiValues[COUNT_UnitWeight]) );
  SPL("unite : " + String(numericValues[COUNT_UnitWeight], 5) );
  SPL();
  SP("tempDotPos "); SPL(tempDotPos);
  SP("tempArray "); SPL(tempArray);
  SPL("===================");
#endif

  strcpy( showDigits.currentValue,  tempArray );
  showDigits.currentValue[7] = '\0';


  //1. check with previous Value
  if ( !strcmp( showDigits.preValue[0] , showDigits.currentValue ) )  return;
  //   SPL("currentValue : " + String(showDigits.currentValue) );
  //   SPL("preValue[0] : " + String(showDigits.preValue[0]) );

  leadingZero = 7 - tempDotPos;

  // 2. Set Font Size & Style
  showDigits.digitFontSize  = 1;
  showDigits.digitFontStyle = LED7SEG_STD40;
  tft.setTextSize( showDigits.digitFontSize );
  tft.setFreeFont( (const GFXfont *)showDigits.digitFontStyle );


  //3. draw blank rectangle only those digits which is different from previous Value.

  static int8_t cnt_1 = 0;
  if ( ( cnt_1++) > 20 )
  {
    for (uint8_t idx = 0; idx < 7; ++idx )
    {
      if ( showDigits.preValue[0][idx] !=  showDigits.currentValue[idx] )
      {
        tft.fillRect( 15  + ( idx * 65),  58,   62,  90,  TFT_BLACK);
        if ( cnt_1 > 22 ) cnt_1 = 0;
      }

    }
  }

  //4.draw blank digits
  tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
  // IN PLACE OF leading Zeros we are drawing blank digits.
  for (uint8_t idx = 0; idx < leadingZero; ++idx )
  {
    tft.drawChar( '8', 15 + ( idx * 65), 130,  1);
  }
  for (uint8_t idx = leadingZero; idx < 7; ++idx )
  {
    if ( showDigits.preValue[0][idx] !=  showDigits.currentValue[idx] )
    {
      tft.drawChar( '8', 15 + ( idx * 65), 130,  1);
    }
  }


  //5. draw Digits only, Those digits which is different from previous Value.
  // front digit
  tft.setTextColor(showDigits.showDigitColorWindowOne, TFT_BLACK);
  for (uint8_t idx = leadingZero ; idx < 7; ++idx )
  {
    if ( showDigits.preValue[0][idx] !=  showDigits.currentValue[idx] )
    {
      tft.drawChar( showDigits.currentValue[idx], 15 + ( idx * 65), 130,  1);
    }
  }

  // end : update preVlaue
  strcpy( showDigits.preValue[0], showDigits.currentValue );

}//end




void  countingMachine :: updateUnitWindow() {
 
  // we need to see unit weight in grams always, modified by Sir.
  sprintf( asciiValues[COUNT_UnitWeight], "%lf",  ( numericValues[COUNT_UnitWeight] ) );

  temp = String( asciiValues[COUNT_UnitWeight] );
  uint8_t tempDotPos = temp.indexOf('.');
   uint8_t starttempDotPos= tempDotPos;
  char tempArray[15], modified[15];
  int8_t leadingZero = 0;
  
  if ( tempDotPos < 7 ) {
    memset(tempArray, '0', 10);
   strcpy(tempArray, asciiValues[COUNT_UnitWeight]);  
   // eliminating tariling Zero
   for(int8_t idx= strlen(tempArray)-1 ; idx > tempDotPos+1; --idx ){
    if( tempArray[idx] == '0' ) tempArray[idx] = '\0'; 
    else break;
   }
   
   // find dot postion from back side
   tempDotPos = 0;
   for(int8_t idx = strlen(tempArray)-1; idx >= 0; --idx ){
    if( tempArray[idx] != '.' ) tempDotPos++; 
    else if( tempArray[idx] == '.' ) break;
   }
  
  //set dot new position
   tempDotPos = 6 - tempDotPos;

  //eliminating dot & storing data in reverse order
   memset(showDigits.currentValue, '0', 10);
   int8_t len = strlen(tempArray)-1;   
   for(int8_t idx = 0, j=7-len; idx <= len; ++idx ){
    if( tempArray[idx] != '.' ) showDigits.currentValue[j++] = tempArray[idx];; 
   }
    showDigits.currentValue[7] = '\0';
  } else return;

////===========================================================//  

  //1. check with previous Value
  if ( !strcmp( showDigits.preValue[1] , showDigits.currentValue ) )  return;

  // find Leading Zeros
  for(uint8_t i = 0; i < tempDotPos; i++ ){
    if( showDigits.currentValue[i] == '0' ) leadingZero++;
    else break;
  }
  
   if( ( numericValues[COUNT_UnitWeight] ) == 0.0 ){
    tempDotPos = leadingZero = 5;
  }

 
  // 1. Set Font Size
  showDigits.digitFontSize  = 1;
  showDigits.digitFontStyle = LED7SEG_STD18;
  tft.setTextSize( showDigits.digitFontSize );
  tft.setFreeFont( (const GFXfont *)showDigits.digitFontStyle );
  
    //eliminate leading Zeros.
    tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
    for (uint8_t idx = 0; idx < leadingZero; ++idx ){
        tft.drawChar(  '8', 15 + ( idx * 31) , 210,  1);
    }
  

  tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
  for (uint8_t idx = 0; idx < 7; ++idx ){
    if ( showDigits.preValue[1][idx] !=  showDigits.currentValue[idx] ){
      tft.fillRect( 15  + ( idx * 31),  180,   30,  40,  TFT_BLACK );
      tft.drawChar(  '8', 15 + ( idx * 31) , 210,  1);
    }
  }


  //5. remove all previous dots and redraw at given position;
  for (uint8_t idx = 0; idx < 6; ++idx ){
    tft.fillCircle( 41 + ( idx * 31), 216, 3, TFT_BLACK);
  }
  
  int8_t dotPosition = tempDotPos;
  tft.fillCircle( 41 + ( dotPosition * 31), 216, 3, TFT_RED );

  //6. draw Digits only,, Those digits which is different from previous Value.
  tft.setTextColor( TFT_GREEN, TFT_BLACK );
  for (uint8_t idx = leadingZero; idx < 7; ++idx ){
    if ( showDigits.preValue[1][idx] !=  showDigits.currentValue[idx] ){
      tft.drawChar(  showDigits.currentValue[idx], 15 + ( idx * 31) , 210,  1);
    }
  }

#if 0
  SPL("ascii : " + String( asciiValues[COUNT] ) );
  SPL("numeric : " + String( numericValues[COUNT] ) );
  SPL();
  SPL("unite : " + String(asciiValues[COUNT_UnitWeight]) );
  SPL("unite : " + String( numericValues[COUNT_UnitWeight], 5) );
  SPL();
  SP("tempDotPos "); SPL(tempDotPos);
  SP("tempArray "); SPL(tempArray);
  SP("modified "); SPL(modified);
  SPL("currentValue : " + String(showDigits.currentValue) );
  SPL("preValue[1] : " + String(showDigits.preValue[1]) );
  SPL("===================");
#endif

  // end : update preVlaue
 // strcpy( showDigits.preValue[1], showDigits.currentValue );


}




#endif
