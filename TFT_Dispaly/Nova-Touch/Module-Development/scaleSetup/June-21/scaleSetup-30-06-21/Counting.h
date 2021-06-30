
#ifndef COUNTING
#define COUNTING

#include <math.h>

enum {
  COUNT_UnitWeight = 0,
  COUNT,
  COUNT_NetWeight
};


class countingMachine: public WeighingHandle {
   public :
    class WeighingHandle commFun;

    String temp =  "";
    int8_t Dotpos;
    double tempCountFlag;
   
  
    char asciiValues[3][10];
    char preAsciiValues[3][10];


    void _calculateCounts();
    String _readbuf( );

 
   char asciiTemp[10];
    uint8_t updateField = COUNT_NetWeight;
      double numericValues[3];
      
    void  _start();
    void countHandler();
    void   _updateOnScreen(uint8_t screenNum );

    void _rawCalculation();
};

void countingMachine:: _rawCalculation(  ) {
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

  // SP("\n1-->>> : ");  SP(asciiValues[COUNT_NetWeight]); SP(" : "); SP( asciiValues[COUNT]); SP(" : "); SPL(asciiValues[COUNT_UnitWeight]);
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
      //      numericValues[COUNT_UnitWeight] = 0.010;
      //      updateField = COUNT_UnitWeight;
      //      memset( asciiValues[updateField], '\0', 10);
      //      sprintf( asciiValues[updateField], "%lf", numericValues[updateField]);
      //      goto HERE;
      // _rawCalculation(  ); // callback
    }
  } else {
    numericValues[COUNT] = ( numericValues[COUNT_NetWeight] / numericValues[COUNT_UnitWeight] );
    if ( numericValues[COUNT] != tempCountFlag ) {
      tempCountFlag = numericValues[COUNT];
      SPL("flag ON");
      updateField = COUNT;
      memset( asciiValues[updateField], '\0', 10);
      sprintf( asciiValues[updateField], "%lf", numericValues[updateField]);
      if ( !strcmp(preAsciiValues[updateField] , asciiValues[updateField]) )
        goto HERE;
    } else {
      SPL("flag OFF");
    }
  }
  updateField = COUNT_NetWeight;
  //  SP("\n2-->>> : ");  SP(asciiValues[COUNT_NetWeight]); SP(" : "); SP(asciiValues[COUNT]); SP(" : "); SPL(asciiValues[COUNT_UnitWeight]);
}//end-_rawClaculation





void countingMachine:: countHandler() {

  //    Count_CheckOut(Mode)
  // numericValues[COUNT_UnitWeight] = FromMachine[COUNT_UnitWeight];
   SPL("\n 1- unit weight : " + String( asciiValues[COUNT_UnitWeight] ) );
   
  memset(asciiTemp, '\0', 10);
  strcpy( asciiTemp, _readbuf( ).c_str() );  asciiTemp[7] = '\0';
  if ( strlen(asciiTemp) > 5 ) {
    _rawCalculation();

    FromMachine[COUNT] =  numericValues[COUNT];
    FromMachine[COUNT_UnitWeight] = numericValues[COUNT_UnitWeight];
    FromMachine[COUNT_NetWeight] =  numericValues[COUNT_NetWeight];

    strcpy(FromMachineArray[COUNT_NetWeight], asciiValues[COUNT_NetWeight]);
    strcpy(FromMachineArray[COUNT],  asciiValues[COUNT]);
  
   
    _updateWindowCOUNT( COUNT_NetWeight );
    _updateWindowCOUNT(COUNT);
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

  //  initWeighingTFT( );
  initTFTHandler ( );
  printStringCOUNT( );
  showDigits.dotPosition = _getDecimal().c_str()[0] - 48;

  _updateWeightperCOUNT( "0.00" );
  _updateWindowCOUNT(COUNT_UnitWeight);

  pressed = 0;
  TouchInterrupt_ptr = myfastfunction;

#ifdef ATTACH_INTER
  ATTACH_TOUCH_INTERRUPT
#elif
  ISR_Called = 1;
#endif

  ISR_dtech = 0;

  Dotpos = _getDecimal().c_str()[0] - '0';
}


#endif
