
#include "UserInputKeyBoard.h"



extern TFT_eSPI tft;
extern char keyArray[10][4];


//#define BlankRect(idx)  tft.fillRect(( 12 + idx*32 ), 18, 30, 50, TFT_BLACK);
#define BlankRect()  tft.fillRect( 208 , 18, 25, 40, TFT_BLACK);

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
#define findkey() oneTouch ? one : twoTouch ? two : threeTouch ? three : fourTouch ? four : fiveTouch ? five :\
  sixTouch ? six : sevenTouch ? seven : eightTouch ? eight : nineTouch ? nine : backTouch ? back :\
  delTouch ? del : entTouch ? ent : dotTouch ? dot : clrTouch ? clr : keepRunning;

enum touchKey _findkeypadTouch( uint16_t xAxis, uint16_t yAxis)
{
  return findkey();
}

int _getCharacter( int keyvalue )
{
  uint16_t xAxis = 0, yAxis = 0;
  int8_t entIdx = 0;
  uint8_t cnt = 0;
  int8_t tempKeyvalue = keepRunning;
  
  int tout = millis();
 
  BlankRect()
  tft.drawChar( keyArray[keyvalue][cnt], 211, 44, 1);

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

      BlankRect()
      tft.drawChar( keyArray[keyvalue][cnt], 211, 44, 1);
      delay(200);
    }
    yield();
    
  }//end-while
  
  BlankRect()
  return keyArray[keyvalue][cnt];
}

int handleKeyPad( )
{
  uint16_t xAxis = 0, yAxis = 0;
  int keyvalue;

  if ( tft.getTouch(&xAxis, &yAxis) )
  {
    keyvalue = _findkeypadTouch( xAxis, yAxis);

    switch ( keyvalue )
    {
      case keepRunning : goto END;
      case back :        goto END;
      case del :         goto END;
      case clr:          goto END;
      case dot:          goto END;
      case ent :         goto END;

      case zero:
      case one:
      case two:
      case three:
      case four:
      case five:
      case six:
      case seven:
      case eight:
      case nine:
               return _getCharacter( keyvalue );

    }//end-switch
  }

END:
  return keyvalue;
}


void takeUserInput( struct _userInput *userInput )
{
  uint8_t inc = 0;
  int temp = keepRunning;

  while (temp != back ) // -5
  {
    temp = handleKeyPad( );
    Serial.println("temp : " + String( temp ) );
    if (temp != keepRunning )
    {
      if ( userInput->entIdx <  userInput->userInputArray_Size )
      {
        userInput->userInputArray[ userInput->entIdx++ ] = temp;
        userInput->userInputArray[ userInput->entIdx ] = '_';
        if ( userInput->entIdx == userInput->userInputArray_Size-1)  userInput->userInputArray[ userInput->entIdx ] = '\0';
        Serial.println("--->>> " + String( userInput->userInputArray ) );
        tft.drawString( &userInput->userInputArray[++inc], userInput->text_xAxis, userInput->text_yAxis, 1);
      }
    }
  }//end-while
  Serial.println("OUTPUT-->> ");
}
