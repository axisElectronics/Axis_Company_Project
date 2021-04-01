

#include "commonGlobal.h"

extern class userKeyBoard kbd;


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
 

  //  initWeighingTFT( );
  initTFTHandler ( );
  printStringPrice();
  showDigits.dotPosition = _getDecimal().c_str()[0] - 48;;


  _updateWeightperPrice( "1.00" );
  _updateWindowPricing(perPCS);

  while (1)
  {    
    if ( handleTouchFuncationality_PRICE() == -1 ) {
      CMD_STOPDATA
      EMPTY_SERIALBUFFER
      return -1;
    }

    memset(src, '\0', 10);
    strcpy(src, _readbufPrice( ).c_str() );   src[7] = '\0';
   
    if ( strlen(src) > 5 )
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
      SPL("ESCTouch...\n"); return -1;

    }
    else if ( Field_Two_Touch  )
    {
      int8_t tempDot = showDigits.dotPosition;

      kbd.userInput.userInputArray_Size = 25;
      kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];
      kbd.init(  );
      kbd.userInput.numericSwitchFlag = 1;

      kbd.takeUserInput(  NULL );
      SPL("keyboard : " + String( kbd.userInput.userInputArray ) );

      _updateWeightperPrice( kbd.userInput.userInputArray );

      strcpy(src, _readbufPrice( ).c_str() );

      initTFTHandler();
      printStringPrice( );

      _updateWindowPricing(perPCS);

      showDigits.dotPosition = tempDot;
      delete[] kbd.userInput.userInputArray; 
    }
  }
}


void  WeighingHandle ::_updateWindowPricing( uint8_t win )
{
  bufferWithoutDot(  showDigits.currentValue,  FromMachineArray[win] );  
  showDigits.currentValue[6] = '\0';

  switch (win)
  {
    case GROSS   :   windowThree( ); break;
    case PRICE   :   windowOne( ); break;
    case perPCS  :   windowTwo( ); break;
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
             //   handleTareCommand( (char *)temp.c_str() );
        break;

      default : break;
    }
    temp = "";
    goto HERE;
  }

  handleFlags( (char *)temp.c_str() );
  return temp;
}


bool  WeighingHandle :: priceStripImage()
{
  tft.setSwapBytes(true);
  tft.pushImage(10, 0, PriceComputingModeStripWidth, PriceComputingModeStripHeight, PriceComputingModeStrip );
}


bool  WeighingHandle :: printStringPrice( )
{
  String weightUnit = "kg";
  tft.setTextSize( 1 );
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.setFreeFont( (const GFXfont *)EUROSTILE_B13 );
  tft.setCursor(18, 50);  tft.print("TOTAL Price");
//  tft.setCursor(400, 50);  tft.print(weightUnit);

  tft.setFreeFont( (const GFXfont *)EUROSTILE_B10 );
  tft.setCursor(18, 173);  tft.print("Unit Price");
    tft.setCursor(180, 173);   tft.print(weightUnit);

  tft.setCursor(260, 173);  tft.print("Total WT");
  tft.setCursor(420, 173);  tft.print(weightUnit);

  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setCursor(18, 310);  tft.print(weightUnit);
}
