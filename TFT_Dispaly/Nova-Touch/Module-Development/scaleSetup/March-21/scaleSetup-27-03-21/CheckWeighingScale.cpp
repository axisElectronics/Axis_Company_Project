

#include "commonGlobal.h"

extern class userKeyBoard kbd;



/****************************************************************************
                      ---->> Check Weighing <<----


****************************************************************************/


int8_t WeighingHandle :: startCheckWeighing()
{
  char src[12];
  char dest[10];

  //  initWeighingTFT( );
  initTFTHandler ( );
  // printStringCOUNT( );
  showDigits.dotPosition = _getDecimal().c_str()[0] - 48;

  _updateWeightperCOUNT( "1.00" );
  _updateWindowCOUNT(perPCS);


  while (1)
  {
    memset(dest, '\0', 10);
    memset(src, '\0', 10);
    strcpy(src, _readbufCHECK( ).c_str() );

    if ( handleTouchFuncationality_CHECK() == -1 )
    {
      CMD_STOPDATA
      EMPTY_SERIALBUFFER
      return -1;
    }

    bufferWithoutDot(dest, src);
    src[7] = '\0';

    if ( strlen(dest) > 5 )
    {
      _updateTotalWeightCOUNT( src );

      _updateWindowCHECK(GROSS);
      _updateWindowCHECK(COUNT);

    }
    yield();
  }

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
        strcpy( maxvalue,  kbd.userInput.userInputArray );
        _updateWeightMaxWindow( kbd.userInput.userInputArray );
      }

      strcpy(src, _readbufPrice( ).c_str() );

      initTFTHandler();
      printStringCOUNT( );
      _updateWindowCHECK(MAX);
      showDigits.dotPosition = tempDot;
    }
  }
  else if ( Field_three_Touch  )
  {
    int8_t tempDot = showDigits.dotPosition;

    kbd.userInput.userInputArray_Size = 25;
    kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];
    kbd.init(  );
    kbd.userInput.numericSwitchFlag = 1;

    kbd.takeUserInput( NULL );
    SPL("keyboard : " + String( kbd.userInput.userInputArray ) );
    strcpy( minvalue,  kbd.userInput.userInputArray );

    _updateWeightperPrice( kbd.userInput.userInputArray );

    strcpy(src, _readbufPrice( ).c_str() );

    initTFTHandler();
    printStringCOUNT( );

    _updateWindowPricing(perPCS);

    showDigits.dotPosition = tempDot;
  }
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
    // SPL("xAxis : " + String(xAxis ) + "\nyAxis : " + String(yAxis) );
  }

}

void  WeighingHandle :: _updateWeightMaxWindow( char *Temp )
{
  FromMachine[GROSS] = strtod( Temp, NULL);
  uint8_t dotpos = 0;
  char temp[8];
  for (int i = 0; i < 8 ; temp[i++] = '0');  temp[7] = '\0';

  // convert double value into char Array
  memset( FromMachineArray[GROSS], '\0' , 10);
  sprintf( FromMachineArray[GROSS], "%lf", FromMachine[GROSS]);
  // Adjust dot postion in Array
  findDotPosition( FromMachineArray[GROSS], dotpos);

  for (int8_t i = 0, j = ( 6 - showDigits.dotPosition - dotpos); ( i < 7 ) && ( j < 7 ) ; i++, j++ )
  {
    temp[ j ] = FromMachineArray[GROSS][i];
  }

  temp[7] = '\0';
  strcpy( FromMachineArray[GROSS], temp );

  FromMachineArray[GROSS][7] = '\0';
  SPL("MAX### : " + String( FromMachineArray[GROSS] ) );
}




void WeighingHandle ::  _updateWindowCHECK( uint8_t win )
{
  char dest[10];

  if ( win != perPCS )
  {
    bufferWithoutDot( dest,  FromMachineArray[win] );
    strcpy( showDigits.currentValue, dest);
    showDigits.currentValue[6] = '\0';
  }

  switch (win)
  {
    case GROSS   :  windowOne( ); break;
    case COUNT   :  windowTwo( ); break;
    case perPCS  :

      bufferWithoutDot( dest,  FromMachineArray[perPCS] );
      dest[7] = '\0';
      for (uint8_t idx = 0;  showDigits.currentValue[idx] = dest[idx]; ++idx );
      showDigits.currentValue[6] = '\0';
      windowThree( );
      break;

    case MAX  : windowTwo( );    break;
    case MIN  : windowThree( );  break;
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
