
/*********************************************************************
   @FileName : commonGlobal.cpp

 **********************************************************************/
#include "commonGlobal.h"


//struct _showDigits showDigits;
class userKeyBoard kbd;

/******* MACROs declaration ********/
#define spacing 90
#define digitMargin 90
#define xdigitMargin digitMargin
#define ydigitMargin digitMargin

#define xClr 160
#define yClr 145
#define xOne   ( xClr + spacing)
#define yOne    yClr
#define xTwo    ( xOne + spacing )
#define yTwo    yOne
#define xThree  ( xTwo + spacing )
#define yThree  yTwo

#define xFour   xOne
#define yFour   60
#define xFive   ( xOne + spacing )
#define yFive   yFour
#define xSix  ( xTwo + spacing )
#define ySix    yFive

#define xSeven  xOne
#define ySeven  0
#define xEight ( xOne + spacing )
#define yEight  ySeven
#define xNine   ( xTwo + spacing )
#define yNine   yEight


#define xBack 2
#define yBack  239

#define xDel (160)
#define yDel  yBack

#define xDot  ( xDel + spacing )
#define yDot  yBack

#define xZero ( xDot + spacing )
#define yZero yBack

#define xEnt ( xZero + spacing )
#define yEnt yBack

#define zeroTouch      ( ( (xAxis > xZero  )   && (xAxis < ( xZero  + digitMargin ) ) )   && ( ( yAxis > yZero  ) && ( yAxis < ( yZero + ydigitMargin  ) ) ) )
#define oneTouch       ( ( (xAxis > xOne   )   && (xAxis < ( xOne   + digitMargin ) ) )   && ( ( yAxis > yOne   ) && ( yAxis < ( yOne + ydigitMargin   ) ) ) )
#define twoTouch       ( ( (xAxis > xTwo   )   && (xAxis < ( xTwo   + digitMargin ) ) )   && ( ( yAxis > yTwo   ) && ( yAxis < ( yTwo + ydigitMargin   ) ) ) )
#define threeTouch     ( ( (xAxis > xThree )   && (xAxis < ( xThree + digitMargin ) ) )   && ( ( yAxis > yThree ) && ( yAxis < ( yThree + ydigitMargin ) ) ) )
#define fourTouch      ( ( (xAxis > xFour  )   && (xAxis < ( xFour  + digitMargin ) ) )   && ( ( yAxis > yFour  ) && ( yAxis < ( yFour + ydigitMargin  ) ) ) )
#define fiveTouch      ( ( (xAxis > xFive  )   && (xAxis < ( xFive  + digitMargin ) ) )   && ( ( yAxis > yFive  ) && ( yAxis < ( yFive + ydigitMargin  ) ) ) )
#define sixTouch       ( ( (xAxis > xSix   )   && (xAxis < ( xSix   + digitMargin ) ) )   && ( ( yAxis > ySix   ) && ( yAxis < ( ySix + ydigitMargin   ) ) ) )
#define sevenTouch     ( ( (xAxis > xSeven )   && (xAxis < ( xSeven + digitMargin ) ) )   && ( ( yAxis > ySeven ) && ( yAxis < ( ySeven + ydigitMargin ) ) ) )
#define eightTouch     ( ( (xAxis > xEight )   && (xAxis < ( xEight + digitMargin ) ) )   && ( ( yAxis > yEight ) && ( yAxis < ( yEight + ydigitMargin ) ) ) )
#define nineTouch      ( ( (xAxis > xNine  )   && (xAxis < ( xNine  + digitMargin ) ) )   && ( ( yAxis > yNine  ) && ( yAxis < ( yNine + ydigitMargin  ) ) ) )
#define dotTouch       ( ( (xAxis > xDot   )   && (xAxis < ( xDot   + digitMargin ) ) )   && ( ( yAxis > yDot  ) && ( yAxis < ( yDot + ydigitMargin  ) ) ) )
#define delTouch       ( ( (xAxis > xDel   )   && (xAxis < ( xDel   + digitMargin ) ) )   && ( ( yAxis > yDel  ) && ( yAxis < ( yDel + ydigitMargin  ) ) ) )
#define clrTouch       ( ( (xAxis > xClr   )   && (xAxis < ( xClr   + digitMargin ) ) )   && ( ( yAxis > yClr  ) && ( yAxis < ( yClr + ydigitMargin  ) ) ) )
#define backTouch      ( ( (xAxis > xBack  )   && (xAxis < ( xBack  + 135         ) ) )   && ( ( yAxis > yBack ) && ( yAxis < ( yBack + ydigitMargin ) ) ) )
#define entTouch       ( ( (xAxis > xEnt   )   && (xAxis < ( xEnt   + digitMargin ) ) )   && ( ( yAxis > yEnt  ) && ( yAxis < ( yEnt + ydigitMargin  ) ) ) )

/******* Class declaration ********/
extern TFT_eSPI tft;


/******* external function and variable **********/

/******* class functions and variable **********/

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
  char src[12];
  char dest[10];

//  initWeighingTFT( );
  initTFTHandler ( );
  printStringCOUNT( );
  showDigits.dotPosition = 2;

  _updateWeightperCOUNT( "1.00" );
  _updateWindowCOUNT(perPCS);


  while (1)
  {
    memset(dest, '\0', 10);
    memset(src, '\0', 10);
    strcpy(src, _readbufCOUNT( ).c_str() );

    if ( handleTouchFuncationality_COUNT() == -1 ) return -1;

    bufferWithoutDot(dest, src);
    src[7] = '\0';

    if ( strlen(dest) > 5 )
    {
      _updateTotalWeightCOUNT( src );

      _updateWindowCOUNT(GROSS);
      _updateWindowCOUNT(COUNT);

    }
    yield();
  }

}

void  WeighingHandle ::_updateWindowCOUNT( uint8_t win )
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
    case GROSS   :   windowOne( ); break;
    case COUNT   :   windowTwo( ); break;
    case perPCS  :

      bufferWithoutDot( dest,  FromMachineArray[perPCS] );
      dest[7] = '\0';
      for (uint8_t idx = 0;  showDigits.currentValue[idx] = dest[idx]; ++idx );
      showDigits.currentValue[6] = '\0';
      windowThree( );
      break;
  }

}



void  WeighingHandle :: _updateTotalWeightCOUNT( char *Temp )
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
  //  SPL("GROSS### : " + String( FromMachineArray[GROSS] ) );
  _updateWeightCOUNT();

}

void WeighingHandle :: _updateWeightCOUNT(  )
{
  FromMachine[COUNT] = ( FromMachine[GROSS] /  FromMachine[perPCS] ) ;
  uint8_t dotpos = 0;
  char temp[8];
  for (int i = 0; i < 8 ; temp[i++] = '0');  temp[7] = '\0';

  // convert double value into char Array
  memset( FromMachineArray[PRICE], '\0' , 10);
  sprintf( FromMachineArray[PRICE], "%lf", FromMachine[PRICE]);
  // Adjust dot postion in Array
  findDotPosition( FromMachineArray[PRICE], dotpos);

  for (int8_t i = 0, j = ( 6 - showDigits.dotPosition - dotpos); ( i < 7 ) && ( j < 7 ) ; i++, j++ )
  {
    temp[ j ] = FromMachineArray[PRICE][i];
  }

  temp[7] = '\0';
  strcpy( FromMachineArray[PRICE], temp );

  FromMachineArray[PRICE][7] = '\0';
  //  SPL("PRICE### : " + String(FromMachineArray[PRICE] ) );
}

void WeighingHandle :: _updateWeightperCOUNT( char *Temp )
{
  FromMachine[perPCS] = strtod( Temp, NULL);

  int8_t dotpos = 0;
  char temp[8];
  for (int i = 0; i < 8 ; temp[i++] = '0');  temp[7] = '\0';

  // convert double value into char Array
  memset( FromMachineArray[perPCS], '\0' , 10);
  sprintf( FromMachineArray[perPCS], "%lf", FromMachine[perPCS]);
  // Adjust dot postion in Array
  findDotPosition( FromMachineArray[perPCS], dotpos);

  for (int8_t i = 0, j = ( 6 - showDigits.dotPosition - dotpos); ( i < 7 ) && ( j < 7 ) ; i++, j++ )
  {
    temp[ j ] = FromMachineArray[perPCS][i];
  }

  temp[7] = '\0';
  strcpy( FromMachineArray[perPCS], temp );

  FromMachineArray[perPCS][7] = '\0';
  //  SPL("perPCS### : " + String(FromMachineArray[perPCS] ) );
}


int8_t  WeighingHandle :: handleTouchFuncationality_COUNT()
{
  uint16_t xAxis = 0, yAxis = 0;
  char src[12] = {0};
  if ( tft.getTouch(&xAxis, &yAxis) )
  {
    if ( Taretouch_Auto )
    {
      //      char cmd[20] = "\2T\3";
      //      Serial2.print(cmd);
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
    else if ( Field_three_Touch  )
    {
      int8_t tempDot = showDigits.dotPosition;

      kbd.userInput.userInputArray_Size = 25;
      kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];
      kbd.init(&kbd.userInput);
      kbd.userInput.numericSwitchFlag = 1;

      kbd.takeUserInput( &kbd.userInput, NULL );
      SPL("keyboard : " + String( kbd.userInput.userInputArray ) );

      _updateWeightperPrice( kbd.userInput.userInputArray );

      strcpy(src, _readbufPrice( ).c_str() );

      initTFTHandler();
      printStringCOUNT( );

      _updateWindowPricing(perPCS);

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

bool  WeighingHandle :: printStringCOUNT( )
{
  String weightUnit = "kg";
  tft.setTextSize( 1 );
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.setFreeFont( (const GFXfont *)EUROSTILE_B13 );
  tft.setCursor(18, 50);  tft.print("TOTAL WT");
  tft.setCursor(400, 50);  tft.print(weightUnit);

  tft.setFreeFont( (const GFXfont *)EUROSTILE_B10 );
  tft.setCursor(18, 173);  tft.print("Total PCS");
  //  tft.setCursor(180, 173);   tft.print(weightUnit);

  tft.setCursor(260, 173);  tft.print("unit WT");
  tft.setCursor(420, 173);  tft.print(weightUnit);

  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setCursor(18, 310);  tft.print(weightUnit);


}




/*******************************************************************************
                         ----> Price Computing <-----
    @ startPriceComputing() :
      - This funcation is main starting point for user.
        call this funcation. Leave reset to handle price
        computing. It will take care all activity. User need
        not call anyother function.
   ==================================================================================
    @ handleTouchFuncationality_PRICE() :
      - This funcation need to take care all TOUCH activity
        On your price computing application i.e AuotZero.

      - It doesnot respond on taretouch.
   ==================================================================================
    @ _updateWindowPricing( uint8_t win ) :
      - This funcation update TFT screen as per given parameters[MACROS]
      i.e
        # GROSS : If you choose this it means total weight coming from machine
                  will render on big screen[windowOne].

        # PRICE : If you choose this it means we are showing calculated price on
                  small screen[windowTwo].
                  price = totalWeight * perPCSRate

        # perPCS : If you choose this it means we are showing per piece price on
                  small screen[windowThree].

     - User can update perPCS price dynamically by touching screen[windowThree].
   ==================================================================================
   @ _updateTotalWeight( char *Temp ) :
    - This funcation used to convert ASCII character vlaue into float value for computation.
      and updata Two variable name "FromMachine[GROSS]" and "FromMachineArray[GROSS]".
      ans it call _updateWeightPrice(  ) function.
   ==================================================================================
   @ _updateWeightPrice(  ) :
    - This funcation used to convert ASCII character vlaue into float value for computation.
      and updata Two variable name "FromMachine[PRICE]" and "FromMachineArray[PRICE]".
   ==================================================================================
   @  _updateWeightperPrice( char *Temp ) :
    - This funcation used to convert ASCII character vlaue into float value for computation.
      and updata Two variable name "FromMachine[perPCS]" and "FromMachineArray[perPCS]".
   ==================================================================================

   @ _readbufPrice( ) :
    - This funcation is used to read data from machine and update flags
   ==================================================================================

 *******************************************************************************/

int8_t  WeighingHandle :: startPriceComputing()
{
  char src[12];
  char dest[10];

//  initWeighingTFT( );
  initTFTHandler ( );
  printStringPrice();
  showDigits.dotPosition = 2;


  _updateWeightperPrice( "1.00" );
  _updateWindowPricing(perPCS);

  while (1)
  {
    memset(dest, '\0', 10);
    memset(src, '\0', 10);
    strcpy(src, _readbufPrice( ).c_str() );

    if ( handleTouchFuncationality_PRICE() == -1 ) return -1;

    bufferWithoutDot(dest, src);
    src[7] = '\0';

    if ( strlen(dest) > 5 )
    {
      _updateTotalWeight( src );

      _updateWindowPricing(GROSS);
      _updateWindowPricing(PRICE);

    }
    yield();
  }

}

int8_t  WeighingHandle :: handleTouchFuncationality_PRICE()
{
  uint16_t xAxis = 0, yAxis = 0;
  char src[12] = {0};
  if ( tft.getTouch(&xAxis, &yAxis) )
  {
    if ( Taretouch_Auto )
    {
      //      char cmd[20] = "\2T\3";
      //      Serial2.print(cmd);
    }
    else if ( Zerotouch )
    {

      char cmd[20] = "\2Z\3";
      Serial2.print(cmd);
    }
    else if ( ESC_Touch )
    {
      SPL("ESCTouch...\n"); return -1;
    }
    else if ( Field_three_Touch  )
    {
      int8_t tempDot = showDigits.dotPosition;

      kbd.userInput.userInputArray_Size = 25;
      kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];
      kbd.init(&kbd.userInput);
      kbd.userInput.numericSwitchFlag = 1;

      kbd.takeUserInput( &kbd.userInput, NULL );
      SPL("keyboard : " + String( kbd.userInput.userInputArray ) );

      _updateWeightperPrice( kbd.userInput.userInputArray );

      strcpy(src, _readbufPrice( ).c_str() );

      initTFTHandler();
      printStringPrice( );

      _updateWindowPricing(perPCS);

      showDigits.dotPosition = tempDot;
    }
  }
}


void  WeighingHandle ::_updateWindowPricing( uint8_t win )
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
    case GROSS   :   windowOne( ); break;
    case PRICE   :   windowTwo( ); break;
    case perPCS  :

      bufferWithoutDot( dest,  FromMachineArray[perPCS] );
      dest[7] = '\0';
      for (uint8_t idx = 0;  showDigits.currentValue[idx] = dest[idx]; ++idx );
      showDigits.currentValue[6] = '\0';
      windowThree( );
      break;
  }

}



void WeighingHandle :: _updateTotalWeight( char *Temp )
{

  FromMachine[GROSS] = strtod( Temp, NULL);

  int8_t dotpos = 0;
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
  //  SPL("GROSS### : " + String( FromMachineArray[GROSS] ) );
  _updateWeightPrice();

}

void WeighingHandle :: _updateWeightPrice(  )
{
  FromMachine[PRICE] = ( FromMachine[GROSS] *  FromMachine[perPCS] ) ;

  int8_t dotpos = 0;
  char temp[8];
  for (int i = 0; i < 8 ; temp[i++] = '0');  temp[7] = '\0';

  // convert double value into char Array
  memset( FromMachineArray[PRICE], '\0' , 10);
  sprintf( FromMachineArray[PRICE], "%lf", FromMachine[PRICE]);

  // Adjust dot postion in Array
  findDotPosition( FromMachineArray[PRICE], dotpos);

  if ( ( 6 - showDigits.dotPosition - dotpos) < 0 ) {
    dotpos = 0;
    showDigits.spclFlag = 1;
  }
  else {
    dotpos = ( 6 - showDigits.dotPosition - dotpos);
    showDigits.spclFlag = 0;
  }

  for (int8_t i = 0, j = dotpos; ( i < 7 ) && ( j < 7 ) ; i++, j++ )
  {
    temp[ j ] = FromMachineArray[PRICE][i];
  }
  temp[7] = '\0';

  strcpy( FromMachineArray[PRICE], temp );
  FromMachineArray[PRICE][7] = '\0';
  //  SPL("PRICEEXIT### : " + String(FromMachineArray[PRICE] ) );
}


void WeighingHandle :: _updateWeightperPrice( char *Temp )
{
  FromMachine[perPCS] = strtod( Temp, NULL);

  int8_t dotpos = 0;
  char temp[8];
  for (int i = 0; i < 8 ; temp[i++] = '0');  temp[7] = '\0';

  // convert double value into char Array
  memset( FromMachineArray[perPCS], '\0' , 10);
  sprintf( FromMachineArray[perPCS], "%lf", FromMachine[perPCS]);
  // Adjust dot postion in Array
  findDotPosition( FromMachineArray[perPCS], dotpos);

  for (int8_t i = 0, j = ( 6 - showDigits.dotPosition - dotpos); ( i < 7 ) && ( j < 7 ) ; i++, j++ )
  {
    temp[ j ] = FromMachineArray[perPCS][i];
  }

  temp[7] = '\0';
  strcpy( FromMachineArray[perPCS], temp );

  FromMachineArray[perPCS][7] = '\0';
  //  SPL("perPCS### : " + String(FromMachineArray[perPCS] ) );
}

String  WeighingHandle :: _readbufPrice( )
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
        FromMachine[GROSS] = 0.00;
        FromMachine[PRICE] = 0.00;
        FromMachine[perPCS] = 1.00;
        _updateWeightperPrice( "1.00" );
        strcpy(showDigits.preValue[perPCS], "ABCDEFGH"); //
        _updateWindowPricing( perPCS );
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


bool  WeighingHandle :: printStringPrice( )
{
  String weightUnit = "kg";
  tft.setTextSize( 1 );
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.setFreeFont( (const GFXfont *)EUROSTILE_B13 );
  tft.setCursor(18, 50);  tft.print("TOTAL WT");
  tft.setCursor(400, 50);  tft.print(weightUnit);

  tft.setFreeFont( (const GFXfont *)EUROSTILE_B10 );
  tft.setCursor(18, 173);  tft.print("Total Price");
  //  tft.setCursor(180, 173);   tft.print(weightUnit);

  tft.setCursor(260, 173);  tft.print("unit Price");
  tft.setCursor(420, 173);  tft.print(weightUnit);

  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setCursor(18, 310);  tft.print(weightUnit);
}



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

int8_t  WeighingHandle :: startWeighing()
{
  char src[12];
  char dest[10];

//  initWeighingTFT( ); //do'nt use more then one time in same program flow. it cause problems.
  initTFTHandler ( );
  printStringWeight( );
  showDigits.dotPosition = 2;

  _updateTareWeight( "0.00" );
  _updateWindow( TARE );

  while (1)
  {
    memset(dest, '\0', 10);
    memset(src, '\0', 10);

    if ( handleTouchFuncationality_Weight() == -1 )
    {
      SPL("I am Rachna...!!!!");
      Serial.println(uxTaskGetStackHighWaterMark(NULL));
      SP("Free HeapSize : ");
      Serial.println(ESP.getFreeHeap());
      delay(500);
      return -1;
    }

    strcpy(src, _readbufWeight( ).c_str() );

    bufferWithoutDot(dest, src);
    src[7] = '\0';

    if ( strlen(dest) > 5 )
    {
      _updateNetWeight( src );

      _updateWindow( NET );
      _updateWindow( GROSS );
    }
    yield();
  }

return 0;
}



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



void WeighingHandle ::_updateWindow( uint8_t win )
{
  char dest[10];
  if ( win != TARE )
  {
    bufferWithoutDot( dest,  FromMachineArray[win] );
    strcpy( showDigits.currentValue, dest);
    showDigits.currentValue[6] = '\0';
  }

  switch (win)
  {
    case NET   :   windowOne( ); break;
    case GROSS :   windowTwo( ); break;
    case TARE  :
      bufferWithoutDot( dest,  FromMachineArray[TARE] );

      strcpy( showDigits.currentValue, dest);
      showDigits.currentValue[6] = '\0';
      windowThree( );
      break;
  }

}


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

bool  WeighingHandle :: printStringWeight( )
{
  //  String weightUnit = handler->basesetting->classOBJ_dftsett->_getWeighingUnit();
  String weightUnit = "kg";
  tft.setTextSize( 1 );
  tft.setFreeFont( (const GFXfont *)EUROSTILE_B13 );
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(18, 50);  tft.print("NET WT");
  tft.setCursor(400, 50);  tft.print(weightUnit);
  tft.setFreeFont( (const GFXfont *)EUROSTILE_B10 );
  tft.setCursor(18, 173);  tft.print("GROSS WT");
  tft.setCursor(180, 173);   tft.print(weightUnit);
  tft.setCursor(260, 173);  tft.print("TARE WT");
  tft.setCursor(420, 173);  tft.print(weightUnit);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setCursor(18, 310);  tft.print(weightUnit);
}


String  WeighingHandle :: _readbufWeight( )
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

  handleFlags( (char *)temp.c_str() );
  return temp;
}


int8_t  WeighingHandle :: handleTouchFuncationality_Weight()
{
  uint16_t xAxis = 0, yAxis = 0;
  char src[12] = {0};

   SP("--->>>># ");
      Serial.println(uxTaskGetStackHighWaterMark(NULL));
  if ( tft.getTouch(&xAxis, &yAxis) )
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
      delay(250);
      return -1;
    }
    else if ( Field_three_Touch  )
    {
      kbd.userInput.userInputArray_Size = 25;
      kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];
      kbd.init(&kbd.userInput);
      kbd.userInput.numericSwitchFlag = 1;

      kbd.takeUserInput( &kbd.userInput, NULL );
      SPL("keyboard : " + String( kbd.userInput.userInputArray ) );

      char cmd[20] = "\2T#";
      strcat(cmd, kbd.userInput.userInputArray );
      cmd[strlen(cmd)] = 3;

      Serial2.print(cmd);
      SPL("Manual Tare : " + String(cmd) );
      strcpy(src, _readbufWeight( ).c_str() );

      initTFTHandler();

    }
  }
}


/****************************************************************************
                    ---->> Common Funcations <<----
   @ handleFlags :
    - This funcation handles all common flags. when and which flag must be ON
      it take cares.
   ===============================================================================

   @ initWeighingTFT( ) :
    - This funcation setup TFT touch screen parameters and UART and their baudRate.
   ====================================================================================

   @ initTFTHandler ( ) :
    - This funcation is use to call Images which is necessary for basic funcationality.
      and update preVlaue with default "ABCDEFGH".
   =====================================================================================

 *****************************************************************************/


String  WeighingHandle :: handleFlags( char *Weight )
{
  flags = Weight[8];
  /* check Zero Bit */
  ( flags & 0x01 ) ? zeroSymbl(1) : zeroSymbl(0);
  /* check Tare Bit */
  (  flags & 0x02 ) ? tareweightSymbl(1) : tareweightSymbl(0);
  /* check stable Bit */
  ( flags & 0x04 ) ? stableSymbl(1) : stableSymbl(0);
  /* Check Negative Falg */
  if ( flags & 0x40 ) Weight[0] = '-';

  return Weight;
}

void WeighingHandle :: initWeighingTFT( )
{
  Serial.begin(9600); // DEBUG WINDOW
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1); // Keypad interface
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Weight Mechine
  SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));

  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  uint16_t calData[5] = { 329, 3444, 448, 3193, 2 };
  tft.setTouch(calData);
  tft.fillScreen(TFT_BLACK);

}

void WeighingHandle :: initTFTHandler ( )
{
  weighingMode();
  batterySymbl();
  stableSymbl(0);
  zeroSymbl(0);
  tareweightSymbl(0);


  strcpy( showDigits.preValue[0], "ABCDEFGH" );
  strcpy( showDigits.preValue[1], "ABCDEFGH" );
  strcpy( showDigits.preValue[2], "ABCDEFGH" );

}




void WeighingHandle :: windowThree( )
{

  static int8_t cnt_3 = 3;
  int8_t leadingZero = 0;
  //  check with previous Value
  if ( !strcmp( showDigits.preValue[2] , showDigits.currentValue ) ) return;


  // 1. Set Font Size and style
  showDigits.digitFontSize  = 1;
  showDigits.digitFontStyle = LED7SEG_STD18;
  tft.setTextSize( showDigits.digitFontSize );
  tft.setFreeFont( (const GFXfont *)showDigits.digitFontStyle );

  // Add Leading Zero into current Value
  memmove( &showDigits.currentValue[1], &showDigits.currentValue[0], strlen(showDigits.currentValue) );
  showDigits.currentValue[0] = '0';
  showDigits.currentValue[7] = '\0';

  eliminateLeadingZeros( showDigits.currentValue, ( 7 - showDigits.dotPosition ), leadingZero )

  //4. draw blank rectangle only those digits which is different from previous Value.
  tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
  for (uint8_t idx = 0; idx < 7; ++idx )
  {
    if ( showDigits.preValue[2][idx] !=  showDigits.currentValue[idx] )
    {
      tft.fillRect( 249  + ( idx * 31),  180,   30,  40,  TFT_BLACK );
      tft.drawChar( '8', 249 + ( idx * 31) , 210,  1 );
    }

  }
  //5. remove all previous dots and redraw at given position;
  for (uint8_t idx = 0; idx < 6; ++idx )
  {
    tft.fillCircle( 275 + ( idx * 31), 216, 3, TFT_BLACK);
  }
  int8_t dotPosition = 6 - showDigits.dotPosition;
  tft.fillCircle( 275 + ( dotPosition * 31), 216, 3, TFT_RED );

  //6. draw Digits only,, Those digits which is different from previous Value.
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  for (uint8_t idx = leadingZero; idx < 7; ++idx )
  {
    if ( showDigits.preValue[2][idx] !=  showDigits.currentValue[idx] )
    {
      tft.drawChar(  showDigits.currentValue[idx], 249 + ( idx * 31) , 210,  1 );
    }
  }

  // end : update preVlaue
  strcpy( showDigits.preValue[2], showDigits.currentValue );

}


void WeighingHandle :: windowTwo( )
{
  int8_t leadingZero = 0;

  //3. check with previous Value
  if ( !strcmp(  showDigits.preValue[GROSS] , showDigits.currentValue ) ) return;

  // 1. Set Font Size
  showDigits.digitFontSize  = 1;
  showDigits.digitFontStyle = LED7SEG_STD18;
  tft.setTextSize( showDigits.digitFontSize );
  tft.setFreeFont( (const GFXfont *)showDigits.digitFontStyle );

  //2. Add Leading Zero into current Value

  if (  showDigits.spclFlag )
  {
    showDigits.currentValue[6] = '0';
  }
  else
  {
    memmove( &showDigits.currentValue[1], &showDigits.currentValue[0], strlen(showDigits.currentValue) );
    showDigits.currentValue[0] = '0';
  }
  showDigits.currentValue[7] = '\0';

  eliminateLeadingZeros( showDigits.currentValue, ( 7 - showDigits.dotPosition ), leadingZero )

  //4. draw blank rectangle only those digits which is different from previous Value.
  tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
  for (uint8_t idx = 0; idx < 7; ++idx )
  {
    if ( showDigits.preValue[1][idx] !=  showDigits.currentValue[idx] )
    {
      tft.fillRect( 15  + ( idx * 31),  180,   30,  40,  TFT_BLACK );
      tft.drawChar(  '8', 15 + ( idx * 31) , 210,  1);
    }
  }
  //5. remove all previous dots and redraw at given position;
  for (uint8_t idx = 0; idx < 6; ++idx )
  {
    tft.fillCircle( 41 + ( idx * 31), 216, 3, TFT_BLACK);
  }
  int8_t dotPosition = 6 - showDigits.dotPosition;
  tft.fillCircle( 41 + ( dotPosition * 31), 216, 3, TFT_RED );

  //6. draw Digits only,, Those digits which is different from previous Value.
  tft.setTextColor( TFT_GREEN, TFT_BLACK );
  for (uint8_t idx = leadingZero; idx < 7; ++idx )
  {
    if ( showDigits.preValue[1][idx] !=  showDigits.currentValue[idx] )
    {
      tft.drawChar(  showDigits.currentValue[idx], 15 + ( idx * 31) , 210,  1);
    }
  }

  // end : update preVlaue
  strcpy( showDigits.preValue[1], showDigits.currentValue );
}

void WeighingHandle :: windowOne( )
{
  static int8_t cnt_1 = 0;
  int8_t leadingZero = 0;

  //3. check with previous Value
  if ( !strcmp( showDigits.preValue[0] , showDigits.currentValue ) ) return;

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
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
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


/******************************************************************
   @makeCustomColor :
   (#) we can make custom color accordingly changed the value of RGB color.
 *                                                                *
 *******************************************************************/


/********************************************************************************************************************************

   @ batterySymbl()  -- >> This Function is responsible for drawing battery Symbol
   @ stableSymbl()   -->>  This Function is reposible for drawing weight stable symbol
   @ tareweightSymbl -->>  This Function is reponsible for drawing tare weight symbol
   @ zeroSymbl       -->>  This Function is responsible for drawing Zero symbol.
   @ BootingImage()  -->>  This is booting image. It is doing Nothing but it looks good. It can be used later but not for now.
   @ startUPImage()  -->>  StartUP Image shows 4 Menu option to choose and accordingly program works.
   @ weighingMode()  -->>  This is base image which is used in all 4 Modes with title modification.
   @ numKeypad()     -->>  This is keypad image which works in all 4 Modes to get user data from screen when it touched by user.

 ********************************************************************************************************************************/

bool WeighingHandle :: batterySymbl()
{
  tft.setSwapBytes(true);
  tft.pushImage(250, 2, bettry_symbWidth, bettry_symbHeight, bettry_symb );
}

/********************************************************************************************************************************/
bool  WeighingHandle :: stableSymbl(uint8_t flag)
{
  tft.setSwapBytes(true);
  if ( flag )
    tft.pushImage(310, 4, stable_symbWidth, stable_symbHeight, Stable_symbl );
  else
    tft.pushImage(310, 4, stable_symbWidth, stable_symbHeight, hideStable_symbl );

}


bool  WeighingHandle :: tareweightSymbl(uint8_t flag)
{
  tft.setSwapBytes(true);
  if ( flag )
    tft.pushImage(350, 4, taresymblWidth, taresymblHeight, Tare_symb );
  else
    tft.pushImage(350, 4, taresymblWidth, taresymblHeight, hideTare_symb );
}


bool WeighingHandle :: zeroSymbl(uint8_t flag)
{
  tft.setSwapBytes(true);

  if (flag)
    tft.pushImage(420, 4, zeroSymblWidth, zeroSymblHeight, ZeroSymbl );
  else
    tft.pushImage(420, 4, zeroSymblWidth, zeroSymblHeight, hideSymbolZero );
}

/******************************************************************
 *                                                                *
 *                                                                *
 *******************************************************************/

bool  WeighingHandle :: BootingImage() {
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  tft.pushImage(40, 50, bootWidth, bootHeight, Boot);

  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
}

/******************************************************************
 *                                                                *
 *                                                                *
 *******************************************************************/

bool  WeighingHandle :: startUPImage() {
  tft.setSwapBytes(true);
  tft.pushImage(1, 1, startUPWidth, startUPHeight, StartUP );
}

/******************************************************************
   @ weighingMode()  -->>  This is base image which is used in
                           all 4 Modes with title modification.                                                                *
 *******************************************************************/
bool  WeighingHandle :: weighingMode()
{
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  tft.pushImage(5, 22, WeighingModeWidth, WeighingModeHeight, WeighingMode );
}

bool WeighingHandle :: numKeypad()
{
  tft.setSwapBytes(true);
  tft.pushImage(0, 3, NumkeypadWidth, NumkeypadHeight, Numkeypad );
}

bool WeighingHandle :: cross(uint16_t x, uint16_t y)
{
  tft.setSwapBytes(true);
  tft.pushImage(x, y, crossImageWidth, crossImageHeight, crossImage );
}
