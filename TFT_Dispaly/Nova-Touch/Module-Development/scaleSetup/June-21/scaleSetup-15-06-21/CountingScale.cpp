
#include "commonGlobal.h"


extern class userKeyBoard kbd;

char netWeight[10];

#define COUNT_UnitWeight  0
#define COUNT             1
#define COUNT_NetWeight   2

/****************************************************************************
                    >>>>> COUNT   <<<<<<

   @ startCountComputing() :
    - This funcation is main starting point for user.
       call this funcation. Leave reset to handle price
       computing. It will take care all activity. User need  not call anyother function.

    ==================================================================================


 *******************************************************************************/


int8_t  WeighingHandle :: startCountComputing()
{
  char src[10];
  char timeArray[10];
  char preTime[10];

  //  initWeighingTFT( );
  initTFTHandler ( );
  printStringCOUNT( );
  showDigits.dotPosition = _getDecimal().c_str()[0] - 48;

  _updateWeightperCOUNT( "1.00" );
  _updateWindowCOUNT(COUNT_UnitWeight);


  while (1)
  {
    if ( handleTouchFuncationality_COUNT() == -1 )
    {
      //      CMD_STOPDATA
      STOP_SERIAL2
      EMPTY_SERIALBUFFER
      return -1;
    }

    memset(src, '\0', 10);
    strcpy(src, _readbufCOUNT( ).c_str() );
    src[7] = '\0';
    if ( strlen(src) > 5 )
    {
      _updateTotalWeightCOUNT( src );

      _updateWindowCOUNT( COUNT_NetWeight );
      _updateWindowCOUNT(COUNT);

    }

    //======================================================
    // write only when pre and current values of Time are different
    memset(timeArray, '\0', 10);
    parshTimeFormat(timeArray, getRTCTime().c_str() )
    if ( strcmp( preTime, timeArray ) ) {
      tft.setFreeFont( (const GFXfont *)EUROSTILE_B7 );
      tft.setTextColor(TFT_BLACK, TFT_BLACK);
      tft.fillRect(400, 295, 50, 20, TFT_WHITE);
      tft.setCursor(400, 310);
      tft.print("  " + String(timeArray) + String("pm") );
      SPL("  " + String(timeArray) + String("pm") );
      strcpy( preTime, timeArray );
    }
    //======================================================
    yield();
  }

}

void  WeighingHandle ::_updateWindowCOUNT( uint8_t win )
{
  if (win == COUNT_NetWeight)
  {
    /*
       Bug Fix : variable curruption .i.e FromMachineArray[win]
       SPL("NetWeight : " + String(netWeight) );
       SPL("NetWeight current : " + String(showDigits.currentValue) );
    */
    bufferWithoutDot( showDigits.currentValue,  netWeight );
    showDigits.currentValue[6] = '\0';

  }
  else
  {
    bufferWithoutDot( showDigits.currentValue,  FromMachineArray[win] );
    showDigits.currentValue[6] = '\0';
  }

  switch (win)
  {
    case COUNT_NetWeight   :  windowThree( );  break;
    case COUNT             :  updateTotalPcsWindow(); break;
    case COUNT_UnitWeight  :  windowTwo( );   break;
  }

}

/* @ ---->>> void  WeighingHandle :: _updateTotalWeightCOUNT( char *Temp )  <<<----
    --------------------------------------------------------------------------------
   1. This function takes data char data as Input.
   2. Convert into double type.
   3. call < _updateWeightCOUNT(); > This calculate number of pcs based on calculated weight and unit pcs.
*/

void  WeighingHandle :: _updateTotalWeightCOUNT( char *Temp )
{
  FromMachine[COUNT_NetWeight] = strtod( Temp, NULL);
  uint8_t dotpos = 0;
  char temp[8];
  for (int i = 0; i < 8 ; temp[i++] = '0');  temp[7] = '\0';

  // convert double value into char Array
  memset( FromMachineArray[COUNT_NetWeight], '\0' , 10);
  sprintf( FromMachineArray[COUNT_NetWeight], "%lf", FromMachine[COUNT_NetWeight]);
  // Adjust dot postion in Array
  findDotPosition( FromMachineArray[COUNT_NetWeight], dotpos);

  //check valid Dot position.
  /*
     Bug Fix : showDigits.dotPosition was corrupting again & again.
  */
  showDigits.dotPosition = _getDecimal().c_str()[0] - '0';

  for (int8_t i = 0, j = ( 6 - showDigits.dotPosition - dotpos); ( i < 7 ) && ( j < 7 ) ; i++, j++ )
  {
    temp[ j ] = FromMachineArray[COUNT_NetWeight][i];
  }

  temp[7] = '\0';
  strcpy( FromMachineArray[COUNT_NetWeight], temp );

  FromMachineArray[COUNT_NetWeight][7] = '\0';
  /*
     Bug Fix : netWeight need to store explicitly.
  */
  strcpy(netWeight,  FromMachineArray[COUNT_NetWeight] );
  //  SPL("Count-GROSS### : " + String( FromMachineArray[COUNT_NetWeight] ) );
  //SPL("Count-Netweight### : " + String( netWeight ) );

  _updateWeightCOUNT();

}

void WeighingHandle :: _updateWeightCOUNT(  )
{
  FromMachine[COUNT] = round( FromMachine[COUNT_NetWeight] /  FromMachine[COUNT_UnitWeight] ) ;
  uint8_t dotpos = 0;
  char temp[8];
  for (int i = 0; i < 8 ; temp[i++] = '0');  temp[7] = '\0';

  // convert double value into char Array
  memset( FromMachineArray[COUNT], '\0' , 10);
  sprintf( FromMachineArray[COUNT], "%lf", FromMachine[COUNT]);
  // Adjust dot postion in Array
  findDotPosition( FromMachineArray[COUNT], dotpos);

  for (int8_t i = 0, j = ( 6 - showDigits.dotPosition - dotpos); ( i < 7 ) && ( j < 7 ) ; i++, j++ )
  {
    temp[ j ] = FromMachineArray[COUNT][i];
  }

  temp[7] = '\0';
  strcpy( FromMachineArray[COUNT], temp );

  FromMachineArray[COUNT][7] = '\0';
  //  SPL("PRICE### : " + String(FromMachineArray[PRICE] ) );
}

void WeighingHandle :: _updateWeightperCOUNT( char *Temp )
{
  FromMachine[COUNT_UnitWeight] = strtod( Temp, NULL);

  int8_t dotpos = 0;
  char temp[8];
  for (int i = 0; i < 8 ; temp[i++] = '0');  temp[7] = '\0';

  // convert double value into char Array
  memset( FromMachineArray[COUNT_UnitWeight], '\0' , 10);
  sprintf( FromMachineArray[COUNT_UnitWeight], "%lf", FromMachine[COUNT_UnitWeight]);
  // Adjust dot postion in Array
  findDotPosition( FromMachineArray[COUNT_UnitWeight], dotpos);

  for (int8_t i = 0, j = ( 6 - showDigits.dotPosition - dotpos); ( i < 7 ) && ( j < 7 ) ; i++, j++ )
  {
    temp[ j ] = FromMachineArray[COUNT_UnitWeight][i];
  }

  temp[7] = '\0';
  strcpy( FromMachineArray[COUNT_UnitWeight], temp );

  FromMachineArray[COUNT_UnitWeight][7] = '\0';
  //  SPL("perPCS### : " + String(FromMachineArray[perPCS] ) );
}


int8_t  WeighingHandle :: handleTouchFuncationality_COUNT()
{
  uint16_t xAxis = 0, yAxis = 0, threshold = 1000;

  if ( tft.getTouch(&xAxis, &yAxis, threshold ) )
  {
    if ( Taretouch_Auto )
    {
      CMD_AUTOTARE
    }
    else if ( Zerotouch )
    {
      CMD_ZERODATA
    }
    else if ( ESC_Touch )
    {
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
      SPL("keyboard : " + String( kbd.userInput.userInputArray ) );

      // update unit weight value.
      _updateWeightperCOUNT( kbd.userInput.userInputArray );

      // Load screen images again.
      START_SERIAL2
      STOP_SERIAL1

      initTFTHandler();
      printStringCOUNT( );

      // Show updated unitWeight value on Screen.
      _updateWindowCOUNT( COUNT_UnitWeight );
      _updateWindowCOUNT( COUNT_NetWeight );
      _updateWindowCOUNT( COUNT );

      delete[]  kbd.userInput.userInputArray;
    }
  }//end-if
   else
  {  
    switch(  getactiveMachineKeyMapping() )
    {
      case map_CmdESC   : return -1;
      case map_CmdUnits : break; // as per requirement
      case map_CmdPrint : break; // as per requirement
      case map_CmdZero  : CMD_ZERODATA break;
      case map_CmdTare  : CMD_AUTOTARE break;
    }//end-switch
  }//end-else
  
}




String  WeighingHandle ::_readbufCOUNT( )
{
  String temp =  "";;
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

  handleFlags( (char *)temp.c_str() );
  return temp;

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
