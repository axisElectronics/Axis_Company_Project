
#include "UserInputKeyBoard.h"



extern TFT_eSPI tft;
extern char keyArray[10][4];


//#define BlankRect(idx)  tft.fillRect(( 12 + idx*32 ), 18, 30, 50, TFT_BLACK);
 #define BlankRect()  tft.fillRect( 200 , 80, 30, 50, TFT_BLACK);                   

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


/******************************************************************
 *                                                                *
 *                                                                *
 *******************************************************************/
int8_t _findkeypadTouch( uint16_t xAxis, uint16_t yAxis)
{
  if ( oneTouch )
    //      if( xOne && yOne )
  {
    //        Serial.println("oneTouched : \nX : " + String(xAxis) + "\nY : " + String(yAxis) );;
    return 1;
  }
  else if ( twoTouch )
  {
    //        Serial.println("twoTouch");
    return 2;
  }
  else if ( threeTouch )
  {
    //        Serial.println("threeTouch");
    return 3;
  }
  else if ( fourTouch )
  {
    //        Serial.println("fourTouch");
    return 4;
  }
  else if ( fiveTouch )
  {
    //        Serial.println("fiveTouch");
    return 5;
  }
  else if ( sixTouch )
  {
    //        Serial.println("sixTouch");
    return 6;
  }
  else if ( sevenTouch )
  {
    //        Serial.println("sevenTouch");
    return 7;
  }
  else if ( eightTouch )
  {
    //        Serial.println("eightTouch");
    return 8;
  }
  else if ( nineTouch )
  {
    //        Serial.println("nineTouch");
    return 9;
  }
  else if ( backTouch )
  {
    //        Serial.println("backTouch");
    return -5;
  }
  else if ( delTouch )
  {
    //        Serial.println("delTouch");
    return -4;
  }
  else if ( dotTouch )
  {
    //        Serial.println("dotTouch");
    return -2;
  }
  else if ( zeroTouch )
  {
    //        Serial.println("zeroTouch");
    return -0;
  }
  else if ( entTouch )
  {
    //        Serial.println("entTouch");
    return -1;
  }
  else if ( clrTouch )
  {
    //        Serial.println("clrTouch");
    return -3;
  }

  return -20;//Not Pressed
}



char findChar(int8_t idx)
{
  int8_t entIdx = 0;
  uint16_t xAxis = 0, yAxis = 0;
  uint8_t cnt = 0;
  int8_t keyvalue;

  if ( tft.getTouch(&xAxis, &yAxis) )
  {
    keyvalue = _findkeypadTouch( xAxis, yAxis);

    if ( keyvalue != -20  )
    {
      int tout = millis();
      int8_t tempKeyvalue = -20;
      
//      BlankRect(idx)
       BlankRect()
      printCharArray( keyArray[keyvalue][cnt], idx );

      while ( ( millis() - tout ) < 1000 )
      {
        xAxis = 0;
        yAxis = 0;
        tft.getTouch(&xAxis, &yAxis);
        tempKeyvalue = _findkeypadTouch( xAxis, yAxis);

        if ( keyvalue == tempKeyvalue )
        {
          cnt++;
          tout = millis();
          while ( cnt >= 4) cnt = cnt - 4;
//          BlankRect(idx)
          BlankRect()
          printCharArray( keyArray[keyvalue][cnt], idx );
          delay(200);
        }
        yield();
      }//end-while
      BlankRect()
      return keyArray[keyvalue][cnt];
    }
  }   
  return -20;
}


void printCharArray( char printChar, int8_t idx )
{
//  tft.drawChar( printChar, ( 13 + idx*32), 50, 1);
tft.drawChar( printChar,203, 120, 1);
  Serial.println( String( printChar ) );
}
