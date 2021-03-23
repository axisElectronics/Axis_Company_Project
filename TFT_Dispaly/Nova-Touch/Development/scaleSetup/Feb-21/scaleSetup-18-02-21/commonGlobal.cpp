
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

  //    refresh_preValues();
}

void WeighingHandle :: initTFTHandler ( )
{
  weighingMode();
  batterySymbl();
  stableSymbl(0);
  zeroSymbl(0);
  tareweightSymbl(0);
}

String  WeighingHandle :: _readbuf( )
{
  HERE :
  String temp = String(NULL);
  //  String raw_temp  = String(NULL);

  // 1. Get data from weighing machine
  if ( Serial2.available() > 12  )
  {
    temp = Serial2.readStringUntil('=');
  }

  //2. check, is String temp contains <STX> <ETX> ??
  int8_t stx = temp.indexOf(2);
  if ( stx >= 0 )
  {
    int8_t etx = temp.indexOf(3);
    SPL("### : " + String(etx) );
    goto HERE;
  }
  return temp;
}



/******************************************************************
 * @makeCustomColor :                                                              
 * (#) we can make custom color accordingly changed the value of RGB color.   
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
