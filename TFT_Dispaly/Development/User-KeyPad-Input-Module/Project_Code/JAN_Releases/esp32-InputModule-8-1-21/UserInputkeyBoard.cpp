
#include "UserInputKeyBoard.h"
#include "keyBoardImage.h"


extern TFT_eSPI tft;

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

#define x1SW 65
#define x2SW 144
#define y1SW 153
#define y2SW 205

#define xSW ( ( xAxis >= x1SW ) && ( xAxis <= x2SW ) ) 
#define ySW ( ( yAxis >= y1SW ) && ( yAxis <= y2SW ) ) 
#define swTouch (xSW && ySW) 

#define findkey() zeroTouch ? zero : oneTouch ? one : twoTouch ? two : threeTouch ? three : fourTouch ? four : fiveTouch ? five :\
  sixTouch ? six : sevenTouch ? seven : eightTouch ? eight : nineTouch ? nine : backTouch ? back :\
  delTouch ? del : entTouch ? ent : dotTouch ? dot : clrTouch ? clr : swTouch ? sw : keepRunning;


char keyArray[10][4] = {
  {'0', ' ', '/', '@' },
  {'1', 'A', 'B', 'C' },
  {'2', 'D', 'E', 'F' },
  {'3', 'G', 'H', 'I' },
  {'4', 'J', 'K', 'L' },
  {'5', 'M', 'N', 'O' },
  {'6', 'P', 'Q', 'R' },
  {'7', 'S', 'T', 'U' },
  {'8', 'V', 'W', 'X' },
  {'9', 'Y', 'Z', '-' },
};



/******************************************************************
 *                                                                *
 *                                                                *
 *******************************************************************/
void keypadParamInit( struct _userInput *userInput )
{
  userInput->entIdx = 15;
  userInput->text_xAxis   = 12;
  userInput->text_yAxis   = 25;
  userInput->text_FGcolor   = TFT_GREEN;
  userInput->text_BGColor =  TFT_BLACK;
  userInput->customFont = mplus_R13;

  memset( userInput->userInputArray, '\0', userInput->userInputArray_Size);
  memset( userInput->userInputArray, ' ',  15 );
  userInput->userInputArray[15] = '_';

  tft.setFreeFont( userInput->customFont );
  tft.setTextSize(1);
  tft.setTextColor(userInput->text_FGcolor,  userInput->text_BGColor );
  tft.drawString(userInput->userInputArray, userInput->text_xAxis, userInput->text_yAxis, 1);
}



enum touchKey _findkeypadTouch( uint16_t xAxis, uint16_t yAxis)
{
  return findkey();
}

int _getCharacter( int keyvalue, bool switchFlag )
{
  uint16_t xAxis = 0, yAxis = 0;
  int8_t entIdx = 0;
  uint8_t cnt = 0;
  int8_t tempKeyvalue = keepRunning;

  int tout = millis();

  BlankRect()
  tft.drawChar( keyArray[keyvalue][cnt], 211, 44, 1);
  
  if ( switchFlag )
  {
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
  }
  else delay(500);

  BlankRect()
  return keyArray[keyvalue][cnt];
}

bool switchFlag = 0;

int handleKeyPad( )
{
  uint16_t xAxis = 0, yAxis = 0;
  int keyvalue = -20;
  
  if ( tft.getTouch(&xAxis, &yAxis) )
  {
    keyvalue = _findkeypadTouch( xAxis, yAxis);
    switch ( keyvalue )
    {
      case keepRunning : goto END;
      case back :        goto END;
      case del :         goto END;
      case clr:          goto END;
      case dot:      delay(350); return '.';
      case ent :         goto END;
      case sw :   switchFlag = !switchFlag; delay(350); break;
      
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
        return _getCharacter( keyvalue, switchFlag );
    }//end-switch
  }

END:
  return keyvalue;
}

void userInputParsher(  struct _userInput *userInput )
{
  int i;
  for ( i = 0; i < userInput->entIdx - 15; i++ )
  {
    userInput->userInputArray[ i ] =  userInput->userInputArray[ 15 + i ];
  }
  userInput->userInputArray[ i ] = '\0';
}



bool keypadImage()
{
  tft.setSwapBytes(true);
  tft.pushImage(0, 0, PLAX_IT_KEYSImage_Width, PLAX_IT_KEYSImage_Hight, PLAX_IT_KEYSArray );
}


void noticBoard(  )
{
  tft.setFreeFont( mplus_R8 );
  tft.setTextSize(1);
  if( switchFlag )
   {
    tft.fillRect(0, 70, 240, 75, TFT_BLACK );
    tft.drawString( "* Alpha Numeric Mode ", 5, 80, 1);  
   }
  else
   {
    tft.fillRect(0, 70, 240, 75, TFT_BLACK );
    tft.drawString( "* Numaric Mode ", 5, 80, 1);
   }

   tft.setFreeFont( mplus_R13 );
}


void takeUserInput( struct _userInput *userInput )
{
  uint8_t inc = 0;
  int8_t temp = keepRunning;

  tft.fillScreen(TFT_BLACK);
  keypadImage();
  noticBoard(  );
  userInput->userInputStructureParamInit =  keypadParamInit;
  userInput->userInputStructureParamInit( userInput );

  while ( 1 ) 
  {
    temp = handleKeyPad( );
    switch ( temp )
    {
      case keepRunning : break;
      case back : userInput->userInputStructureParamInit( userInput );  return;

      case clr :
        userInput->userInputStructureParamInit( userInput );
        inc = 0;
        tft.drawString( &userInput->userInputArray[inc], userInput->text_xAxis, userInput->text_yAxis, 1);
        break;

      case ent:
        userInputParsher( userInput );
        /*Now  userInput->userInputArray; buffer can we used as user final data */
        return;

      case del :
        if ( userInput->entIdx <= 15 )
        {
          userInput->userInputStructureParamInit( userInput );
          inc = 0;
        }
        else if ( userInput->entIdx > 15 )
        {
          userInput->userInputArray[ userInput->entIdx-- ] = '\0';
          userInput->userInputArray[ userInput->entIdx ] = '_';
          --inc;
        }
        tft.drawString( &userInput->userInputArray[inc], userInput->text_xAxis, userInput->text_yAxis, 1);
        delay(300);
        break;
      case dot : break;
      case sw : noticBoard(); break;

       default :
       if ( userInput->entIdx <  userInput->userInputArray_Size )
        {
          userInput->userInputArray[ userInput->entIdx++ ] = temp;
          userInput->userInputArray[ userInput->entIdx ] = '_';
          if ( userInput->entIdx == userInput->userInputArray_Size - 1)  userInput->userInputArray[ userInput->entIdx ] = '\0';
          Serial.println("--->>> " + String( userInput->userInputArray ) );
          tft.drawString( &userInput->userInputArray[++inc], userInput->text_xAxis, userInput->text_yAxis, 1);
        }
        break;
    }//end-switch

  }//end-while
  Serial.println("OUTPUT-->> ");
}
