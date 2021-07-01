
#ifndef COUNTING
#define COUNTING

#include <math.h>

enum {
  COUNT_UnitWeight = 0,
  COUNT,
  COUNT_NetWeight
};


class countingMachine: public WeighingHandle {
  private :
    void _calculateCounts();
  
  public :
    class WeighingHandle commFun;
    class userKeyBoard kbd;

    String temp =  "";
    int8_t Dotpos;
    double tempCountFlag;
    char asciiValues[3][10];
    char preAsciiValues[3][10];
    
    String _readbuf( );
    char asciiTemp[10];
    uint8_t updateField = COUNT_NetWeight;
    double numericValues[3];


    void  _start();
    void countHandler();
    void   _updateOnScreen(uint8_t screenNum );
    void _rawCalculation();
    void onScreen(uint8_t showMeAt );
    int8_t activityBasedOnTouch();
};


void  countingMachine:: _calculateCounts(){
  
}

int8_t countingMachine:: activityBasedOnTouch() {

  switch (  getactiveMachineKeyMapping() )
  {
    case map_CmdESC   : pressed = 0; return -1;
    case map_CmdUnits : break; // as per requirement
    case map_CmdPrint : break; // as per requirement
    case map_CmdZero  : CMD_ZERODATA break;
    case map_CmdTare  : CMD_AUTOTARE break;
  }//end-switch

  if ( ( pressed = tft.getTouch(&xAxis, &yAxis) ) == 0 ) return 0;
  if ( pressed == 0 ) return 0;

  if ( pressed ) {
    pressed = 0;

    if ( Taretouch_Auto ) {
      CMD_AUTOTARE
    }
    else if ( Zerotouch ) {
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

      updateField = COUNT_UnitWeight;
      strcpy( asciiTemp,  kbd.userInput.userInputArray );
      _rawCalculation();

      // Load screen images again.
      START_SERIAL2
      STOP_SERIAL1

      initTFTHandler();
      printStringCOUNT( );

      // SHOW DATA ON SCREEN
      onScreen(COUNT_UnitWeight);
      onScreen(COUNT_NetWeight);
      onScreen(COUNT);

      delete[]  kbd.userInput.userInputArray;
      START_SERIAL1
    }
  }//end-if
}//end-


void countingMachine:: onScreen(uint8_t showMeAt ) {

  bufferWithoutDot( showDigits.currentValue,  asciiValues[showMeAt] );
  showDigits.currentValue[6] = '\0';

  switch ( showMeAt ) {
    case COUNT            :  FromMachine[COUNT] =  numericValues[COUNT];  updateTotalPcsWindow(); break;
    case COUNT_UnitWeight :  windowTwo(); break;
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
  numericValues[updateField] = strtod(asciiTemp, NULL);
  // convert double values into equivalent ASCII values
  memset( asciiValues[updateField], '\0', 10);
  sprintf( asciiValues[updateField], "%lf", numericValues[updateField]);
HERE:
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


  if ( numericValues[COUNT_UnitWeight] == 0.0 ) {
    for (int8_t i = 0; i < 8; i++ ) {
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
      tempCountFlag = 00.00;
    }
  }else{
    numericValues[COUNT] = ( numericValues[COUNT_NetWeight] / numericValues[COUNT_UnitWeight] );
   
    if ( numericValues[COUNT] != tempCountFlag )
    {
      tempCountFlag = numericValues[COUNT];
      updateField = COUNT;
      memset( asciiValues[updateField], '\0', 10);
      sprintf( asciiValues[updateField], "%lf", numericValues[updateField]);
      if ( strcmp( preAsciiValues[updateField] , asciiValues[updateField] ) ) {
        SPL("flag ON");
        FromMachine[updateField] =  numericValues[updateField];
        goto HERE;
      } else {
        SPL("flag OFF");
      }
    }
  }
  updateField = COUNT_NetWeight;
}//end-_rawClaculation





void countingMachine:: countHandler() {

  memset(asciiTemp, '\0', 10);
  strcpy( asciiTemp, _readbuf( ).c_str() );  asciiTemp[7] = '\0';
  if ( strlen(asciiTemp) > 5 ) {
    _rawCalculation();

    //    FromMachine[COUNT]            =  numericValues[COUNT];
    //    FromMachine[COUNT_NetWeight]  =  numericValues[COUNT_NetWeight];

    onScreen(COUNT_NetWeight );
    onScreen(COUNT );
  }
}//end-countHandler








String  countingMachine:: _readbuf( ) {
  temp =  "";
HERE :
  // 1. Get data from weighing machine
  if ( Serial2.available() > 12  ) temp = Serial2.readStringUntil('=');

  if ( temp.length() > 50 ) {
    temp = "";
    goto HERE;
  }

  //2. check, is String temp contains <STX> <ETX> ??
  int8_t stx = temp.indexOf(2);
  if ( stx >= 0 )
  {
    char ch = temp[stx + 1];
    switch (ch)
    {
      case '1'://zero command
        FromMachine[COUNT_NetWeight] = 0.00;
        FromMachine[COUNT] = 0.00;
        FromMachine[COUNT_UnitWeight] = 1.00;
        _updateWeightperCOUNT( "1.00" );
        strcpy(showDigits.preValue[COUNT_UnitWeight], "ABCDEFGH");

        _updateWindowCOUNT(COUNT);
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
}


#endif
