

#include "commonGlobal.h"

extern class userKeyBoard kbd;

#define CHECK_NetWeight 1
#define MAX   0
#define MIN   2

/****************************************************************************
                      ---->> Check Weighing <<----


****************************************************************************/


int8_t WeighingHandle :: startCheckWeighing()
{
  char src[12];

  //  initWeighingTFT( );
  initTFTHandler ( );
  // printStringCOUNT( );
  showDigits.dotPosition = _getDecimal().c_str()[0] - 48;

   _updateWeightWindowCHECK(  "10.00", MAX );
   _updateWeightWindowCHECK(  "0.00", MIN );
   _updateWindowCHECK(MIN);
   _updateWindowCHECK(MAX);

  while (1)
  {
    if ( handleTouchFuncationality_CHECK() == -1 )
    {
      CMD_STOPDATA
      EMPTY_SERIALBUFFER
      return -1;
    }

    memset(src, '\0', 10);
    strcpy(src, _readbufCHECK( ).c_str() );  src[7] = '\0';

    if ( strlen( src ) > 5 )
    {
      _updateTotalWeightCHECK( src );

      _updateWindowCHECK(CHECK_NetWeight);
      _updateWindowCHECK(MIN);
      _updateWindowCHECK(MAX);
    }//end-if
    yield();
  }//end-while()

}




int8_t WeighingHandle :: handleTouchFuncationality_CHECK()
{
  uint16_t xAxis = 0, yAxis = 0, threshold = 1000;
  char src[12] = {0};
  int8_t tempDot = showDigits.dotPosition;

  if ( tft.getTouch(&xAxis, &yAxis, threshold ) )
  {
    if ( Field_Two_Touch  )
    {
      SPL("Field Two Touch \nxAxis : " + String(xAxis ) + "\nyAxis : " + String(yAxis) );

      kbd.userInput.userInputArray_Size = 25;
      kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];
      kbd.init(  );
      kbd.userInput.numericSwitchFlag = 1;

      kbd.takeUserInput( NULL );
      SPL("keyboard : " + String( kbd.userInput.userInputArray ) );
      if ( strlen( kbd.userInput.userInputArray ) > 0 )
      {
        strcpy( FromMachineArray[MAX],  kbd.userInput.userInputArray );
        _updateWeightWindowCHECK(  FromMachineArray[MAX], MAX );
        
      }

      initTFTHandler();
      printStringCOUNT( );
      _updateWindowCHECK(MAX);
      showDigits.dotPosition = tempDot;

      delete[] kbd.userInput.userInputArray;
    }//end-if( field-Two )

    else if ( Field_three_Touch  )
    {
      int8_t tempDot = showDigits.dotPosition;

      kbd.userInput.userInputArray_Size = 25;
      kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];
      kbd.init(  );
      kbd.userInput.numericSwitchFlag = 1;

      kbd.takeUserInput( NULL );
      SPL("keyboard : " + String( kbd.userInput.userInputArray ) );
      if ( strlen( kbd.userInput.userInputArray ) > 0 )
      {
        strcpy(  FromMachineArray[MIN],  kbd.userInput.userInputArray );
        _updateWeightWindowCHECK(  FromMachineArray[MIN], MIN );
      }

      initTFTHandler();
      //      printStringCOUNT( );
      _updateWindowCHECK(MIN);
      showDigits.dotPosition = tempDot;
      delete[]  kbd.userInput.userInputArray;
    }//end-if(field-Three)
    else if ( Zerotouch )
    {
      char cmd[20] = "\2Z\3";
      Serial2.print(cmd);
    }
    else if ( ESC_Touch )
    {
      SPL("ESCTouch...\n");
      return -1;
    }
    else
    {
      SPL("xAxis : " + String(xAxis ) + "\nyAxis : " + String(yAxis) );
    }

  }//end-if( TFT_Touch )
}

void  WeighingHandle :: _updateTotalWeightCHECK( char *Temp )
{
  FromMachine[CHECK_NetWeight] = strtod( Temp, NULL);
  uint8_t dotpos = 0;
  char temp[8];
  for (int i = 0; i < 8 ; temp[i++] = '0');  temp[7] = '\0';

  // convert double value into char Array
  memset( FromMachineArray[CHECK_NetWeight], '\0' , 10);
  sprintf( FromMachineArray[CHECK_NetWeight], "%lf", FromMachine[CHECK_NetWeight]);
  // Adjust dot postion in Array
  findDotPosition( FromMachineArray[CHECK_NetWeight], dotpos);

  //check valid Dot position.
  /*
     Bug Fix : showDigits.dotPosition was corrupting again & again.
  */
  showDigits.dotPosition = _getDecimal().c_str()[0] - '0';

  for (int8_t i = 0, j = ( 6 - showDigits.dotPosition - dotpos); ( i < 7 ) && ( j < 7 ) ; i++, j++ )
  {
    temp[ j ] = FromMachineArray[CHECK_NetWeight][i];
  }

  temp[7] = '\0';
  strcpy( FromMachineArray[CHECK_NetWeight], temp );

  FromMachineArray[CHECK_NetWeight][7] = '\0';

  // SPL("CHECK_NetWeight### : " + String( FromMachineArray[CHECK_NetWeight] ) );
}




void  WeighingHandle :: _updateWeightWindowCHECK( char *Temp, uint8_t win)
{
  FromMachine[win] = strtod( Temp, NULL);
  uint8_t dotpos = 0;
  char temp[8];
  for (int i = 0; i < 8 ; temp[i++] = '0');  temp[7] = '\0';

  // convert double value into char Array
  memset( FromMachineArray[win], '\0' , 10);
  sprintf( FromMachineArray[win], "%lf", FromMachine[win]);
  // Adjust dot postion in Array
  findDotPosition( FromMachineArray[win], dotpos);

  for (int8_t i = 0, j = ( 6 - showDigits.dotPosition - dotpos); ( i < 7 ) && ( j < 7 ) ; i++, j++ )
  {
    temp[ j ] = FromMachineArray[win][i];
  }
  temp[7] = '\0';
  strcpy( FromMachineArray[win], temp );

  FromMachineArray[win][7] = '\0';
  if ( win == MAX )  SPL("MAX### : " + String( FromMachineArray[win] ) );
  else  SPL("MIN### : " + String( FromMachineArray[win] ) );

}




void WeighingHandle ::  _updateWindowCHECK( uint8_t win )
{

  bufferWithoutDot( showDigits.currentValue,  FromMachineArray[win] );
  showDigits.currentValue[6] = '\0';

  switch (win)
  {
    case CHECK_NetWeight   :  windowOne( );    break;
    case MAX               :  windowTwo( );    break;
    case MIN               :  windowThree( );  break;
  }



}

String WeighingHandle ::  _readbufCHECK( )
{
  String temp =  "";
HERE :
  // 1. Get data from weighing machine
  if ( Serial2.available() > 12  )
  {
    temp = Serial2.readStringUntil('=');
  }
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
      case '1':
        FromMachine[GROSS] = 0.00;
        FromMachine[PRICE] = 0.00;
        FromMachine[perPCS] = 1.00;
        _updateWeightperCOUNT( "1.00" );
        strcpy(showDigits.preValue[perPCS], "ABCDEFGH");
        _updateWindowPricing(perPCS);
        break;
      case 'T':
        //        handleTareCommand( (char *)temp.c_str() );
        break;

      default : break;
    }
    temp = "";
    goto HERE;
  }

  handleFlags( (char *)temp.c_str() );
  return temp;
}
