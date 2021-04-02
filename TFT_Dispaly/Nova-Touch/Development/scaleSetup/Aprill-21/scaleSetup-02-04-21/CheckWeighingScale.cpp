

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
  printStringCHECK( );
  showDigits.dotPosition = _getDecimal().c_str()[0] - 48;

  _updateWeightWindowCHECK( "1.00", MAX );
  _updateWeightWindowCHECK( "0.00", MIN );
  _updateWindowCHECK(MIN);
  _updateWindowCHECK(MAX);

  while (1)
  {
    if ( handleTouchFuncationality_CHECK() == -1 )
    {
      //      CMD_STOPDATA
      STOP_SERIAL2
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
      printStringCHECK( );
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
      printStringCHECK( );
      _updateWindowCHECK(MIN);
      showDigits.dotPosition = tempDot;
      delete[]  kbd.userInput.userInputArray;
    }//end-if(field-Three)
    else if ( Zerotouch )
    {
      CMD_ZERODATA
    }
    else if ( Taretouch_Auto )
    {
      CMD_AUTOTARE
    }
    else if ( ESC_Touch )
    {
      SPL("ESCTouch...\n");
      CMD_STOPDATA
      EMPTY_SERIALBUFFER
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
  //  if ( win == MAX )  SPL("MAX### : " + String( FromMachineArray[win] ) );
  //  else  SPL("MIN### : " + String( FromMachineArray[win] ) );

}




void WeighingHandle ::  _updateWindowCHECK( uint8_t win )
{

  bufferWithoutDot( showDigits.currentValue,  FromMachineArray[win] );
  showDigits.currentValue[6] = '\0';

  switch (win)
  {
    case CHECK_NetWeight   :  windowOneCHECK( );    break;
    case MAX               :  windowTwo( );    break;
    case MIN               :  windowThree( );  break;
  }



}



void WeighingHandle :: windowOneCHECK( )
{
  static int8_t cnt_1 = 0;
  int8_t leadingZero = 0;

  //3. check with previous Value
  if ( !strcmp( showDigits.preValue[0] , showDigits.currentValue ) )  return;

  //check valid Dot position.
  /*
     Bug Fix : showDigits.dotPosition was corrupting again again.
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

  //2. Add Leading Zero into current Value
  memmove( &showDigits.currentValue[1], &showDigits.currentValue[0], strlen(showDigits.currentValue) );
  showDigits.currentValue[0] = '0';
  showDigits.currentValue[7] = '\0';


  eliminateLeadingZeros( showDigits.currentValue, ( 7 - showDigits.dotPosition ), leadingZero )


  //4. draw blank rectangle only those digits which is different from previous Value.
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

  tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
  for (uint8_t idx = 0; idx < 7; ++idx )
  {
    if ( showDigits.preValue[0][idx] !=  showDigits.currentValue[idx] )
    {

      tft.drawChar( '8', 15 + ( idx * 65), 130,  1);
    }
  }

  //5. remove all previous dots and redraw at given position;
  for (uint8_t idx = 0; idx < 6; ++idx )
  {
    tft.fillCircle( 73 + ( idx * 65 ), 138, 8, TFT_BLACK);
  }
  int8_t dotPosition = 6 - showDigits.dotPosition;
  tft.fillCircle( 73 + ( dotPosition * 65), 138, 8, TFT_RED );

  //6. draw Digits only,, Those digits which is different from previous Value.
  if ( ( FromMachine[CHECK_NetWeight] < FromMachine[MIN] ) )   tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  else if ( ( FromMachine[CHECK_NetWeight] > FromMachine[MAX] ) )  tft.setTextColor(TFT_RED, TFT_BLACK);
  else tft.setTextColor(TFT_GREEN, TFT_BLACK);

  // find '-'ve sign if draw all digit.
  uint8_t i = 0;
  while ( FromMachineArray[CHECK_NetWeight][i] )
  {
    if ( FromMachineArray[CHECK_NetWeight][i++] == '-' )
    {
      // leadingZero = 0;
      strcpy( showDigits.preValue[0], "ABCDEFGH" );
      break;
    }
  }



  for (uint8_t idx = leadingZero; idx < 7; ++idx )
  {
    if ( showDigits.preValue[0][idx] !=  showDigits.currentValue[idx] )
    {
      tft.drawChar( showDigits.currentValue[idx], 15 + ( idx * 65), 130,  1);
    }
  }

  // end : update preVlaue
  strcpy( showDigits.preValue[0], showDigits.currentValue );
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
        FromMachine[CHECK_NetWeight] = 0.00;
        FromMachine[MIN] = 0.00;
        FromMachine[MAX] = 1.00;
        _updateWeightperCOUNT( "1.00" );
        strcpy(showDigits.preValue[CHECK_NetWeight], "ABCDEFGH");
        strcpy(showDigits.preValue[MIN], "ABCDEFGH");
        strcpy(showDigits.preValue[MAX], "ABCDEFGH");

        _updateWindowCHECK(CHECK_NetWeight);
        _updateWindowCHECK(MIN);
        _updateWindowCHECK(MAX);
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

bool  WeighingHandle :: printStringCHECK( )
{
  countStripImage();
  String weightUnit = _getWeighingUnit();
  tft.setTextSize( 1 );
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

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
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setCursor(18, 310);  tft.print(weightUnit);

}
