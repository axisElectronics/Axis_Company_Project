
#include "commonGlobal.h"

extern class userKeyBoard kbd;

/****************************************************************************
                    >>>>> COUNT   <<<<<<

   @ startCountComputing() :
    - This funcation is main starting point for user.
       call this funcation. Leave reset to handle price
       computing. It will take care all activity. User need  not call anyother function.

    ==================================================================================


 *******************************************************************************/
//extern class settings set;

int8_t  WeighingHandle :: startCountComputing()
{
  char src[12];
  char dest[10];

  //  initWeighingTFT( );
  initTFTHandler ( );
  printStringCOUNT( );
  showDigits.dotPosition = _getDecimal().c_str()[0] - 48;

  _updateWeightperCOUNT( "1.00" );
  _updateWindowCOUNT(COUNT_perPCS);


  while (1)
  {
    memset(dest, '\0', 10);
    memset(src, '\0', 10);
    strcpy(src, _readbufCOUNT( ).c_str() );

    if ( handleTouchFuncationality_COUNT() == -1 )
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

      _updateWindowCOUNT(COUNT_GROSS);
      _updateWindowCOUNT(COUNT);

    }
    yield();
  }

}

void  WeighingHandle ::_updateWindowCOUNT( uint8_t win )
{
  char dest[10];

  if ( win != COUNT_perPCS )
  {
    bufferWithoutDot( dest,  FromMachineArray[win] );
    strcpy( showDigits.currentValue, dest);
    showDigits.currentValue[6] = '\0';
  }

  switch (win)
  {
    case COUNT_GROSS   :   windowThree( );  break;
    case COUNT   :         windowOne( ); break;
    
    case COUNT_perPCS  :

      bufferWithoutDot( dest,  FromMachineArray[COUNT_perPCS] );
      dest[7] = '\0';
      for (uint8_t idx = 0;  showDigits.currentValue[idx] = dest[idx]; ++idx );
      showDigits.currentValue[6] = '\0';    
      windowTwo( );
      break;
  }

}

/* @ ---->>> void  WeighingHandle :: _updateTotalWeightCOUNT( char *Temp )  <<<----
 *  --------------------------------------------------------------------------------
 * 1. This function takes data char data as Input.
 * 2. Convert into double type.
 * 3. call < _updateWeightCOUNT(); > This calculate number of pcs based on calculated weight and unit pcs.
 */

void  WeighingHandle :: _updateTotalWeightCOUNT( char *Temp )
{
  FromMachine[COUNT_GROSS] = strtod( Temp, NULL);
  uint8_t dotpos = 0;
  char temp[8];
  for (int i = 0; i < 8 ; temp[i++] = '0');  temp[7] = '\0';

  // convert double value into char Array
  memset( FromMachineArray[COUNT_GROSS], '\0' , 10);
  sprintf( FromMachineArray[COUNT_GROSS], "%lf", FromMachine[COUNT_GROSS]);
  // Adjust dot postion in Array
  findDotPosition( FromMachineArray[COUNT_GROSS], dotpos);

  for (int8_t i = 0, j = ( 6 - showDigits.dotPosition - dotpos); ( i < 7 ) && ( j < 7 ) ; i++, j++ )
  {
    temp[ j ] = FromMachineArray[COUNT_GROSS][i];
  }

  temp[7] = '\0';
  strcpy( FromMachineArray[COUNT_GROSS], temp );

  FromMachineArray[COUNT_GROSS][7] = '\0';
  //  SPL("GROSS### : " + String( FromMachineArray[GROSS] ) );
  _updateWeightCOUNT();

}

void WeighingHandle :: _updateWeightCOUNT(  )
{
  FromMachine[COUNT] = ( FromMachine[COUNT_GROSS] /  FromMachine[COUNT_perPCS] ) ;
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
  FromMachine[COUNT_perPCS] = strtod( Temp, NULL);

  int8_t dotpos = 0;
  char temp[8];
  for (int i = 0; i < 8 ; temp[i++] = '0');  temp[7] = '\0';

  // convert double value into char Array
  memset( FromMachineArray[COUNT_perPCS], '\0' , 10);
  sprintf( FromMachineArray[COUNT_perPCS], "%lf", FromMachine[COUNT_perPCS]);
  // Adjust dot postion in Array
  findDotPosition( FromMachineArray[COUNT_perPCS], dotpos);

  for (int8_t i = 0, j = ( 6 - showDigits.dotPosition - dotpos); ( i < 7 ) && ( j < 7 ) ; i++, j++ )
  {
    temp[ j ] = FromMachineArray[COUNT_perPCS][i];
  }

  temp[7] = '\0';
  strcpy( FromMachineArray[COUNT_perPCS], temp );

  FromMachineArray[COUNT_perPCS][7] = '\0';
  //  SPL("perPCS### : " + String(FromMachineArray[perPCS] ) );
}


int8_t  WeighingHandle :: handleTouchFuncationality_COUNT()
{
  uint16_t xAxis = 0, yAxis = 0, threshold = 1000;
  char src[12] = {0};
  if ( tft.getTouch(&xAxis, &yAxis, threshold ) )
  {
    if ( Taretouch_Auto )
    {
            char cmd[20] = "\2T\3";
            Serial2.print(cmd);
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
    else if ( Field_Two_Touch  )
    {
      int8_t tempDot = showDigits.dotPosition;

      kbd.userInput.userInputArray_Size = 25;
      kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];
      kbd.init(  );
      kbd.userInput.numericSwitchFlag = 1;

      kbd.takeUserInput( NULL );
      SPL("keyboard : " + String( kbd.userInput.userInputArray ) );

      _updateWeightperCOUNT( kbd.userInput.userInputArray );

      strcpy(src, _readbufPrice( ).c_str() );

      initTFTHandler();
      printStringCOUNT( );

      _updateWindowCOUNT(COUNT_perPCS);
      showDigits.dotPosition = tempDot;
    }
  }
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
        FromMachine[COUNT_GROSS] = 0.00;
        FromMachine[COUNT] = 0.00;
        FromMachine[COUNT_perPCS] = 1.00;
        _updateWeightperCOUNT( "1.00" );
        strcpy(showDigits.preValue[COUNT_perPCS], "ABCDEFGH");
        _updateWindowPricing(COUNT_perPCS);
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
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setCursor(18, 310);  tft.print(weightUnit);


}

void  WeighingHandle :: updateTotalPcsWindow()
{
  

  
}
