


#include "commonGlobal.h"

extern class userKeyBoard kbd;

extern boolean pressed;
extern uint32_t TimeOut;
extern uint16_t xAxis, yAxis;
extern int8_t ISR_dtech;

extern volatile int *int_status;
extern volatile int *int_clear;
extern volatile int *int_SET;

/*****************************************************************************
   @ startWeighing :
    - This funcation is main starting point for user.
      call this funcation. Leave rest to handle weight
      computing. It will take care all activity. User need
      not call anyother function.
   ==================================================================================
   @ handleTareCommand( char *Weight ) :
    - This fncation is called when machine respond with tare weight.
      This funcation helps program to update its tare window which is
      small screen[windowThree].
   ==================================================================================

   @ _updateWindow( uint8_t win ) :
    - This funcantion helps to upadte NET, GROSS and TARE window as per requirments
      and given parameters.
      i.e
      # NET   : If you choose this it means NET weight coming from machine
                will render on big screen[windowOne].

      # GROSS : If you choose this it means total weight coming from machine
                will render on small screen[windowTwo].

      # TARE  : If you choose this it means TARE weight coming from machine
               will render on small screen[windowThree].

    - user can update Tare weight value by touching small screen[windowThree].
   ==================================================================================

   @ _updateNetWeight( char *Temp ) :
    - This funcation used to convert ASCII character vlaue into float value for computation.
      and updata Two variable name "FromMachine[NET]" and "FromMachineArray[NET]".
      ans it call _updateGROSSWeight(  ) function.
   ==================================================================================
   @ _updateGROSSWeight(  ) :
    - This funcation used to convert ASCII character vlaue into float value for computation.
      and updata Two variable name "FromMachine[GROSS]" and "FromMachineArray[GROSS]".
   ==================================================================================
   @ _updateTareWeight( char *Temp ) :
   - This funcation used to convert ASCII character vlaue into float value for computation.
      and updata Two variable name "FromMachine[TARE]" and "FromMachineArray[TARE]".
   ==================================================================================

   @  printStringWeight( ) :
   -  This funcation print Texts on Screens and Weight.
   ==================================================================================

 ******************************************************************************/

class  WeighingHandle weightClass;

void IRAM_ATTR weightTouch(void) {

  pressed = tft.getTouch( &xAxis, &yAxis);
  pressed = weightClass.handleTouchFuncationality_Weight();
  //   SPL("\nweightTouch......");
}



static uint32_t readSkipCount = 1;
int8_t  WeighingHandle :: startWeighing()
{
  char src[12];
  char timeArray[10];
  char preTime[10];
  //  initWeighingTFT( ); //do'nt use more then one time in same program flow. it cause problems.
  initTFTHandler ( );
  printStringWeight( );

  uint8_t tempdot = _getDecimal().c_str()[0];
  showDigits.dotPosition = tempdot - 48;

  // get TARE data from Machine
  CMD_GETTARE
  _readbufWeight( );

  ATTACH_TOUCH_INTERRUPT_Weight

  while (1)
  {
    if ( pressed == -1 )
    {
      //      CMD_STOPDATA
      detachInterrupt(digitalPinToInterrupt(GPIOPin_TIRQ));
      STOP_SERIAL2
      EMPTY_SERIALBUFFER
      pressed = 0;
      return -1;
    }

    memset(src, '\0', 10);
    strcpy(src, _readbufWeight( ).c_str() );  src[7] = '\0';

    if ( strlen(src) > 5 )
    {
      if ( readSkipCount++ >= 2 ) {
        _updateNetWeight( src );
        readSkipCount = 1;
      }
      _updateWindow( NET );

      if ( handleTouchFuncationality_Weight() == -1 )
      {
        //      CMD_STOPDATA
        STOP_SERIAL2
        EMPTY_SERIALBUFFER
        return -1;
      }

      _updateWindow( GROSS );
    }//end-if

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
      //  SPL("  " + String(timeArray) + String("pm") );
      strcpy( preTime, timeArray );
    }
    //======================================================
    yield();
  }//end- while
}//end-startweighing


/*******************************************************


 ********************************************************/
void WeighingHandle ::handleTareCommand( char *Weight  )
{
  String cmdBuf = "";
  String temp = Weight;

  int8_t stx = temp.indexOf(2);
  int8_t etx = temp.indexOf(3);

  for (int8_t leftIdx = stx + 3; leftIdx < etx; ++leftIdx )
  {
    cmdBuf += temp[leftIdx];
  }
  _updateTareWeight( (char *) cmdBuf.c_str() );
  _updateWindow(TARE);
}

/*******************************************************


 ********************************************************/

void WeighingHandle ::_updateWindow( uint8_t win )
{

  bufferWithoutDot( showDigits.currentValue,  FromMachineArray[win] );
  showDigits.currentValue[6] = '\0';

  switch (win)
  {
    case NET   :   windowOne( );   break;
    case GROSS :   windowTwo( );   break;
    case TARE  :   windowThree( ); break;
  }

}


/*******************************************************


 ********************************************************/

void WeighingHandle ::_updateNetWeight( char *Temp )
{

  FromMachine[NET] = strtod( Temp, NULL);

  int8_t dotpos = 0;
  char temp[8];
  for (int i = 0; i < 8 ; temp[i++] = '0');  temp[7] = '\0';

  // convert double value into char Array
  memset( FromMachineArray[NET], '\0' , 10);
  sprintf( FromMachineArray[NET], "%lf", FromMachine[NET]);
  // Adjust dot postion in Array
  findDotPosition( FromMachineArray[NET], dotpos);

  for (int8_t i = 0, j = ( 6 - showDigits.dotPosition - dotpos); ( i < 7 ) && ( j < 7 ) ; i++, j++ )
  {
    temp[ j ] = FromMachineArray[NET][i];
  }

  temp[7] = '\0';
  strcpy( FromMachineArray[NET], temp );

  FromMachineArray[NET][7] = '\0';
  //  SPL("NET### : " + String(FromMachineArray[NET] ) );
  _updateGrossWeight( );

}


/******************************************************************


 *******************************************************************/

void WeighingHandle ::_updateGrossWeight(  )
{
  FromMachine[GROSS]  = FromMachine[NET] +  FromMachine[TARE];

  int8_t dotpos = 0;
  char temp[8];
  for (int i = 0; i < 8 ; temp[i++] = '0');

  // convert double value into char Array
  memset(FromMachineArray[GROSS], '\0' , 10);
  sprintf(FromMachineArray[GROSS], "%lf", FromMachine[GROSS]);

  // Adjust dot postion in Array
  findDotPosition(FromMachineArray[GROSS], dotpos);

  for (int8_t i = 0, j = ( 6 - showDigits.dotPosition - dotpos); ( i < 7 ) && ( j < 7 ) ; i++, j++ )
  {
    temp[ j ] = FromMachineArray[GROSS][i];
  }

  for (int8_t i = 0; i < 7; i++ )
  {
    FromMachineArray[GROSS][i] = temp[i];
  }
  FromMachineArray[GROSS][7] = '\0';
  //  SPL("GROSS### : " + String( FromMachineArray[GROSS]) );
}

void  WeighingHandle ::_updateTareWeight( char *Temp )
{
  FromMachine[TARE] = strtod( Temp, NULL);

  int8_t dotpos = 0;
  char temp[8];
  for (int i = 0; i < 8 ; temp[i++] = '0'); temp[7] = '\0';

  // convert double value into char Array
  memset( FromMachineArray[TARE], '\0' , 10);
  sprintf( FromMachineArray[TARE], "%lf", FromMachine[TARE]);

  // Adjust dot postion in Array
  findDotPosition( FromMachineArray[TARE], dotpos);

  for (int8_t i = 0, j = ( 6 - showDigits.dotPosition - dotpos); ( i < 7 ) && ( j < 7 ) ; i++, j++ )
  {
    temp[ j ] = FromMachineArray[TARE][i];
  }

  for (int8_t i = 0; i < 7; i++ )
  {
    FromMachineArray[TARE][i] = temp[i];
  }

  FromMachineArray[TARE][7] = '\0';

  //  SPL("updateTare showDigits.dotPosition -->> " + String( showDigits.dotPosition ) );
  //  SPL("TARE### : " + String( FromMachineArray[TARE] ) );

}

bool   WeighingHandle :: weightStripImage()
{
  tft.setSwapBytes(true);
  tft.pushImage(10, 0, WeightModeStripWidth, WeightModeStripHeight, WeightModeStrip );
}

bool  WeighingHandle :: printStringWeight( )
{
  String weightUnit = _getWeighingUnit();
  weightStripImage();
  //  SPL("weightUnit : " + String(weightUnit) );
  tft.setTextSize( 1 );

  char dateArray[10];
  SPL("getDate+  : " + _getDate() );
  parshDateformat(dateArray,  _getDate().c_str() );
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


String  WeighingHandle :: _readbufWeight( )
{

  String temp =  "";
HERE :
  // 1. Get data from weighing machine
  if ( Serial2.available() > 12  )
  {
    temp = Serial2.readStringUntil('=');
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
        FromMachine[NET] = 0.00;
        FromMachine[TARE] = 0.00;
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
  if (  temp.length() > 7 )
  {
    handleFlags( (char *)temp.c_str() );
  }
  return temp;
}






int8_t  WeighingHandle :: handleTouchFuncationality_Weight()
{
  char src[12] = {0};

  if ( pressed )
  {
    detachInterrupt(digitalPinToInterrupt(GPIOPin_TIRQ));
    SPL("pressed : " + String(pressed));
    
    if ( Taretouch_Auto )
    {
      CMD_AUTOTARE
      xAxis = 0;
      yAxis = 0;
      pressed = 0;
      ATTACH_TOUCH_INTERRUPT_Weight
    }
    else if ( Zerotouch )
    {
      CMD_ZERODATA
      xAxis = 0;
      yAxis = 0;
      pressed = 0;
      ATTACH_TOUCH_INTERRUPT_Weight
    }
    else if ( ESC_Touch )
    {
      SPL("ESCTouch...>>X:  " + String(xAxis) + "Y : " + String(yAxis) );
      xAxis = 0;
      yAxis = 0;
      pressed = 0;
      pressed =  -1;
    }
    else if ( Field_three_Touch  )
    {
      STOP_SERIAL2
      START_SERIAL1

      kbd.userInput.userInputArray_Size = 25;
      kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];
      kbd.init( );
      kbd.userInput.numericSwitchFlag = 1;

      kbd.takeUserInput(  NULL );
      //     SPL("keyboard : " + String( kbd.userInput.userInputArray ) );

      START_SERIAL2

      char cmd[20] = "\2T#";
      strcat(cmd, kbd.userInput.userInputArray );
      cmd[strlen(cmd)] = 3;

      Serial2.print(cmd);
      //  SPL("Manual Tare : " + String(cmd) );

      initTFTHandler();
      printStringWeight( );

      CMD_GETTARE
      _readbufWeight( );

      delete[]  kbd.userInput.userInputArray;
    } else {
      xAxis = 0;
      yAxis = 0;
      pressed = 0;
      ATTACH_TOUCH_INTERRUPT_Weight
    }
  }//end-if
  else
  {
    switch (  getactiveMachineKeyMapping() )
    {
      case map_CmdESC   : pressed = 0; return -1;
      case map_CmdUnits : break; // as per requirement
      case map_CmdPrint : break; // as per requirement
      case map_CmdZero  : CMD_ZERODATA break;
      case map_CmdTare  : CMD_AUTOTARE break;
    }//end-switch
  }//end-else

  return 0;
}//end-handleTouch
