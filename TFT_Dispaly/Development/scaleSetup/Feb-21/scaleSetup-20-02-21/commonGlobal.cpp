
/*********************************************************************
   @FileName : commonGlobal.cpp

 **********************************************************************/
#include "commonGlobal.h"

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
}

void WeighingHandle ::_updateNetWindow(  )
{
  char dest[10];
  bufferWithoutDot( dest,  weightFromMachineArrayNET );
  strcpy( showDigits->currentValue, dest);
  SPL("NET-->> : " + String(   weightFromMachineArrayNET ) );
  windowOne( );
}


void WeighingHandle ::_updateGrossWeight( char *Temp )
{
  weightFromMachine[GROSS] = strtod( Temp, NULL);
  SPL("GROSS### : " + String( weightFromMachine[GROSS]) );
  _updateNetWeight( );

  int8_t dotpos = 0;
  char temp[10];
  for (int i = 0; i < 10 ; temp[i++] = '0');

  // convert double value into char Array
  memset(weightFromMachineArrayGROSS, '\0' , 10);
  sprintf(weightFromMachineArrayGROSS, "%lf", weightFromMachine[GROSS]);
  // Adjust dot postion in Array
  findDotPosition(weightFromMachineArrayGROSS, dotpos);
  memmove( &temp[ 6 - showDigits->dotPosition - dotpos ], weightFromMachineArrayGROSS, 8);
  strcpy(weightFromMachineArrayGROSS, temp );
  weightFromMachineArrayGROSS[7] = '\0';
}

void  WeighingHandle ::_updateTareWeight( char *Temp )
{
  weightFromMachine[TARE] = strtod( Temp, NULL);

  int8_t dotpos = 0;
  char temp[10];
  for (int i = 0; i < 10 ; temp[i++] = '0');

  // convert double value into char Array
  memset( weightFromMachineArrayTARE, '\0' , 10);
  sprintf( weightFromMachineArrayTARE, "%lf", weightFromMachine[TARE]);

  // Adjust dot postion in Array
  findDotPosition( weightFromMachineArrayTARE, dotpos);
  memmove( &temp[ 6 - showDigits->dotPosition - dotpos ], weightFromMachineArrayTARE, 8);
  strcpy( weightFromMachineArrayTARE , temp );
  weightFromMachineArrayTARE[7] = '\0';

//  SPL("TARE### : " + String( weightFromMachineArrayTARE ) );
}

void WeighingHandle ::_updateNetWeight( )
{
  weightFromMachine[NET] = weightFromMachine[GROSS] -  weightFromMachine[TARE];

  int8_t dotpos = 0;
  char temp[10];
  for (int i = 0; i < 10 ; temp[i++] = '0');

  // convert double value into char Array
  memset( weightFromMachineArrayNET, '\0' , 10);
  sprintf( weightFromMachineArrayNET, "%lf", weightFromMachine[NET]);
  // Adjust dot postion in Array
  findDotPosition( weightFromMachineArrayNET, dotpos);
  memmove( &temp[ 6 - showDigits->dotPosition - dotpos ], weightFromMachineArrayNET, 8);
  strcpy(weightFromMachineArrayNET, temp );
  weightFromMachineArrayNET[7] = '\0';
//  SPL("NET### : " + String(weightFromMachineArrayNET ) );
}

void WeighingHandle ::handleTareCommand( String temp )
{
  String cmdBuf = "";
  int8_t stx = temp.indexOf(2);
  int8_t etx = temp.indexOf(3);

  for (int8_t leftIdx = stx + 3; leftIdx < etx; ++leftIdx )
  {
    cmdBuf += temp[leftIdx];
  }

  _updateTareWeight( (char *)cmdBuf.c_str() );

}

String  WeighingHandle :: handleFlags( String Weight )
{
  flags = Weight[7];
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


String  WeighingHandle :: _readbuf( )
{
HERE :
  String temp =  "";

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
      case '1':   break;
      case 'T': handleTareCommand( temp );  break;
      default : break;
    }
    goto HERE;
  }

  flags = temp[7];
  return temp;
}


void WeighingHandle :: windowThree( )
{
  static int8_t cnt_3 = 3;
  //  check with previous Value
  if ( !strcmp( showDigits->preValue[2] , showDigits->currentValue ) ) return;


  // 1. Set Font Size and style
  showDigits->digitFontSize  = 1;
  showDigits->digitFontStyle = LED7SEG_STD18;
  tft.setTextSize( showDigits->digitFontSize );
  tft.setFreeFont( (const GFXfont *)showDigits->digitFontStyle );

  // Add Leading Zero into current Value
  memmove( &showDigits->currentValue[1], &showDigits->currentValue[0], strlen(showDigits->currentValue) );
  showDigits->currentValue[0] = '0';
  showDigits->currentValue[7] = '\0';

  //  SPL("current : " + String( showDigits->currentValue ) );

  //4. draw blank rectangle only those digits which is different from previous Value.
  tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
  for (uint8_t idx = 0; idx < 7; ++idx )
  {
    if ( showDigits->preValue[2][idx] !=  showDigits->currentValue[idx] )
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
  int8_t dotPosition = 6 - showDigits->dotPosition;
  tft.fillCircle( 275 + ( dotPosition * 31), 216, 3, TFT_RED );

  //6. draw Digits only,, Those digits which is different from previous Value.
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  for (uint8_t idx = 0; idx < 7; ++idx )
  {
    if ( showDigits->preValue[2][idx] !=  showDigits->currentValue[idx] )
    {
      tft.drawChar(  showDigits->currentValue[idx], 249 + ( idx * 31) , 210,  1 );
    }
  }

  // end : update preVlaue
  strcpy( showDigits->preValue[2], showDigits->currentValue );
}


void WeighingHandle :: windowTwo( )
{
  //3. check with previous Value
  if ( !strcmp(  showDigits->preValue[1] , showDigits->currentValue ) ) return;

  // 1. Set Font Size
  showDigits->digitFontSize  = 1;
  showDigits->digitFontStyle = LED7SEG_STD18;
  tft.setTextSize( showDigits->digitFontSize );
  tft.setFreeFont( (const GFXfont *)showDigits->digitFontStyle );

  //2. Add Leading Zero into current Value
  memmove( &showDigits->currentValue[1], &showDigits->currentValue[0], strlen(showDigits->currentValue) );
  showDigits->currentValue[0] = '0';
  showDigits->currentValue[7] = '\0';

  //  SPL("current : " + String( showDigits->currentValue ) );

  //4. draw blank rectangle only those digits which is different from previous Value.
  tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
  for (uint8_t idx = 0; idx < 7; ++idx )
  {
    if ( showDigits->preValue[1][idx] !=  showDigits->currentValue[idx] )
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
  int8_t dotPosition = 6 - showDigits->dotPosition;
  tft.fillCircle( 41 + ( dotPosition * 31), 216, 3, TFT_RED );

  //6. draw Digits only,, Those digits which is different from previous Value.
  tft.setTextColor( TFT_GREEN, TFT_BLACK );
  for (uint8_t idx = 0; idx < 7; ++idx )
  {
    if ( showDigits->preValue[1][idx] !=  showDigits->currentValue[idx] )
    {
      tft.drawChar(  showDigits->currentValue[idx], 15 + ( idx * 31) , 210,  1);
    }
  }

  // end : update preVlaue
  strcpy( showDigits->preValue[1], showDigits->currentValue );
}

void WeighingHandle :: windowOne( )
{
  static int8_t cnt_1;
  //3. check with previous Value
  if ( !strcmp( showDigits->preValue[0] , showDigits->currentValue ) ) return;

  // 1. Set Font Size & Style
  showDigits->digitFontSize  = 1;
  showDigits->digitFontStyle = LED7SEG_STD40;
  tft.setTextSize( showDigits->digitFontSize );
  tft.setFreeFont( (const GFXfont *)showDigits->digitFontStyle );

  //2. Add Leading Zero into current Value
  memmove( &showDigits->currentValue[1], &showDigits->currentValue[0], strlen(showDigits->currentValue) );
  showDigits->currentValue[0] = '0';
  showDigits->currentValue[7] = '\0';

  //  SPL("current : " + String( showDigits->currentValue ) );

  //4. draw blank rectangle only those digits which is different from previous Value.
  if ( ( cnt_1++) > 20 )
  {
    for (uint8_t idx = 0; idx < 7; ++idx )
    {
      if ( showDigits->preValue[0][idx] !=  showDigits->currentValue[idx] )
      {
        tft.fillRect( 15  + ( idx * 65),  58,   62,  90,  TFT_BLACK);
        if ( cnt_1 > 22 ) cnt_1 = 0;
      }
    }
  }
  tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
  for (uint8_t idx = 0; idx < 7; ++idx )
  {
    if ( showDigits->preValue[0][idx] !=  showDigits->currentValue[idx] )
    {

      tft.drawChar( '8', 15 + ( idx * 65), 130,  1);
    }
  }

  //5. remove all previous dots and redraw at given position;
  for (uint8_t idx = 0; idx < 6; ++idx )
  {
    tft.fillCircle( 73 + ( idx * 65 ), 138, 8, TFT_BLACK);
  }
  int8_t dotPosition = 6 - showDigits->dotPosition;
  tft.fillCircle( 73 + ( dotPosition * 65), 138, 8, TFT_RED );

  //6. draw Digits only,, Those digits which is different from previous Value.
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  for (uint8_t idx = 0; idx < 7; ++idx )
  {
    if ( showDigits->preValue[0][idx] !=  showDigits->currentValue[idx] )
    {
      tft.drawChar( showDigits->currentValue[idx], 15 + ( idx * 65), 130,  1);
    }
  }

  // end : update preVlaue
  strcpy( showDigits->preValue[0], showDigits->currentValue );
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
   @ weighingMode()  -->>  This is base image which is used in all 4 modes with title modification.
   @ numKeypad()     -->>  This is keypad image which works in all 4 modes to get user data from screen when it touched by user.

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
                           all 4 modes with title modification.                                                                *
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
