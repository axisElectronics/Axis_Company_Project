
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



bool switchFlag = 0;

/******************************************************************
 *                                                                *
 *                                                                *
 *******************************************************************/
void userKeyBoard::customKeypad( struct _userInput *userInput )
{

}

void userKeyBoard::init( struct _userInput *userInput )
{
  keypadParamInit( userInput );

}

void userKeyBoard :: keypadParamInit( struct _userInput *userInput )
{
  userInput->entIdx = 15;
  userInput->text_xAxis   = 12;
  userInput->text_yAxis   = 25;
  userInput->text_FGColor   = TFT_GREEN;
  userInput->text_BGColor =  TFT_BLACK;
  userInput->FieldLable_FGColor = TFT_RED;
  userInput->FieldLable_BGColor = TFT_YELLOW;
  userInput->noticBoard_FGColor = TFT_GREEN;
  userInput->noticBoard_BGColor = TFT_BLACK;
  userInput->customFont = mplus_R13;
  //  userInput->userInputParser = &userKeyBoard :: keypadDataparser;
  //  userInput->userInputStructureParamInit = &userKeyBoard :: keypadParamInit;
  //  userInput->userCustomKeypad = NULL;
  userInput->fixFormatstruct = &userFixFormat;

  strcpy( userInput->fieldLable, "Keypad");

  for ( uint8_t i = 0; i < 11; i++ ) userInput->invalidKey[i] = -20;

  memset( userInput->userInputArray, '\0', userInput->userInputArray_Size);
  memset( userInput->userInputArray, ' ',   userInput->entIdx );
  userInput->userInputArray[ userInput->entIdx] = '_';

  tft.setFreeFont( userInput->customFont );
  tft.setTextSize(1);
  tft.setTextColor(userInput->text_FGColor,  userInput->text_BGColor );
  tft.drawString(userInput->userInputArray, userInput->text_xAxis, userInput->text_yAxis, 1);
}



int8_t  userKeyBoard :: _findkeypadTouch( uint16_t xAxis, uint16_t yAxis)
{
  return findkey();
}



int userKeyBoard ::  _getCharacter( int keyvalue, bool switchFlag, struct _userInput *userInput )
{
  uint16_t xAxis = 0, yAxis = 0;
  int8_t entIdx = 0;
  uint8_t cnt = 0;
  int8_t tempKeyvalue = keepRunning;

  int tout = millis();
  if ( !( userInput->fixFormatstruct && userInput->fixFormatstruct->fixFormatFlag ) )
  {
    BlankRect()
    tft.drawChar( keyArray[keyvalue][cnt], 211, 44, 1);
  }

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
        if (  userInput->fixFormatstruct && userInput->fixFormatstruct->fixFormatFlag )
        {
          switch ( userInput->fixFormatstruct->fieldType )
          {
            case stringType :
            case Password :
              DrawfixFormatChar( keyArray[keyvalue][cnt],  userInput->fixFormatstruct->idx,  userInput );
              moveUrDash( userInput->fixFormatstruct->idx, userInput );
              break;
          }//end-switch
        }//end-if
        else if ( !( userInput->fixFormatstruct && userInput->fixFormatstruct->fixFormatFlag ) )
        {
          BlankRect()
          tft.drawChar( keyArray[keyvalue][cnt], 211, 44, 1);
        }
        delay(200);
      }//end-if
      yield();
    }//end-while
  }

  if ( !( userInput->fixFormatstruct && userInput->fixFormatstruct->fixFormatFlag ) )
    BlankRect()

    return keyArray[keyvalue][cnt];
}


int userKeyBoard ::  handleKeyPad( struct _userInput *userInput )
{
  uint16_t xAxis = 0, yAxis = 0;
  int keyvalue = -20;

  if ( tft.getTouch(&xAxis, &yAxis) )
  {
    keyvalue = _findkeypadTouch( xAxis, yAxis);

    for (uint8_t i = 0; i < 11; i++)
    {
      if ( keyvalue == userInput->invalidKey[i] )
      {
        keyvalue = keepRunning;
        break;
      }
    }

    switch ( keyvalue )
    {
      case keepRunning : goto END;
      case back :        goto END;
      case del :         goto END;
      case clr:          goto END;
      case dot:      delay(350); return '.';
      case ent :         goto END;
      case sw :
        if ( userInput->fixFormatstruct && userInput->fixFormatstruct->fixFormatFlag )
        {
          switch ( userInput->fixFormatstruct->fieldType )
          {
            case Date :
            case Time : break;
            case stringType :
            case Password :
              userInput->switchFlag = !userInput->switchFlag;
              delay(350);
              break;
            default : break;
          }
        }
        else if ( !( userInput->fixFormatstruct && userInput->fixFormatstruct->fixFormatFlag ) )
        {
          if ( userInput->numericSwitchFlag );
          else
            userInput->switchFlag = !userInput->switchFlag;
          delay(350);
        }
        break;

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

        return _getCharacter( keyvalue, userInput->switchFlag, userInput );
    }//end-switch
  }

END:
  return keyvalue;
}

void userKeyBoard ::  keypadDataparser(  struct _userInput *userInput )
{
  int i;
  for ( i = 0; i < userInput->entIdx - 15; i++ )
  {
    userInput->userInputArray[ i ] =  userInput->userInputArray[ 15 + i ];
  }
  userInput->userInputArray[ i ] = '\0';
}



bool userKeyBoard ::  keypadImage()
{
  tft.setSwapBytes(true);
  tft.pushImage(0, 0, PLAX_IT_KEYSImage_Width, PLAX_IT_KEYSImage_Hight, PLAX_IT_KEYSArray );
}


void userKeyBoard ::  noticBoard( uint16_t FGColor, uint16_t BGColor, bool switchFlag)
{
  tft.setFreeFont( mplus_R8 );
  tft.setTextSize(1);
  tft.setTextColor(FGColor, BGColor );

  if ( switchFlag )
  {
    tft.fillRect(0, 70, 240, 75, BGColor );
    tft.drawString( "* Alpha Numeric Mode ", 5, 80, 1);
  }
  else
  {
    tft.fillRect(0, 70, 240, 75, BGColor );
    tft.drawString( "* Numaric Mode ", 5, 80, 1);
  }

  tft.setFreeFont( mplus_R13 );
}



void userKeyBoard ::  printkeypadLable( struct _userInput *userInput )
{
  tft.setFreeFont( mplus_R8 );
  tft.setTextColor( userInput->FieldLable_FGColor,  userInput->FieldLable_BGColor );
  tft.drawString("                        ", 15, 0, 1);
  tft.drawString( userInput->fieldLable, 15, 0, 1);
  tft.setFreeFont( mplus_R13 );
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
}

/*
   @ All Activities will be driven by this Function "userInputFixFormat".
      - This Function is supported by two basic function names are "DrawfixFormatChar" and "moveUrDash"
        -- These Function provides Felexiblity to Developer to print char at spacified Location.
        -- Screen is devided into 17 spaces. Developer can access by providing indexes.

*/
void userInputFixFormat( struct _userInput *userInput )
{



}


void userKeyBoard ::  moveUrDash( int8_t idx, struct _userInput *userInput )
{
  /* Draw dash charcter */
  /* Remove all previous Dashes. It is must operation */
  for (  int8_t idx = 0; idx < userInput->fixFormatstruct->maxLengthScreen; idx++ )
    tft.fillRect( userInput->fixFormatstruct->charXDash + (userInput->fixFormatstruct->charWDash * idx), 47, userInput->fixFormatstruct->charWDash, userInput->fixFormatstruct->charHDash, TFT_BLACK);

  /* Print your Dash as per Index */
  tft.drawChar( '-',  userInput->fixFormatstruct->charXDash + (idx * userInput->fixFormatstruct->charWDash),  userInput->fixFormatstruct->charYDash, 1);
}


void userKeyBoard ::  DrawfixFormatChar(char fixChar, int8_t idx,  struct _userInput *userInput)
{
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  //   Serial.println("idx-->> " + String(idx) );

  tft.fillRect( userInput->fixFormatstruct->rectX + (userInput->fixFormatstruct->rectW * idx), userInput->fixFormatstruct->rectY, userInput->fixFormatstruct->rectW, userInput->fixFormatstruct->rectH, TFT_BLACK);
  tft.drawChar( fixChar, userInput->fixFormatstruct->charX + (idx * userInput->fixFormatstruct->rectW), userInput->fixFormatstruct->charY, 1);

}


void  userKeyBoard :: takeUserInput( struct _userInput *userInput, char *argc )
{
  uint8_t inc = 0;
  int8_t temp = keepRunning;

  tft.fillScreen(TFT_BLACK);
  keypadImage();

  if ( userInput->fixFormatstruct && userInput->fixFormatstruct->fixFormatFlag )
  {
    configFormats( userInput );
  }//end-if
  else
  {
    tft.drawString( &userInput->userInputArray[inc], userInput->text_xAxis, userInput->text_yAxis, 1);
    noticBoard( userInput->noticBoard_FGColor, userInput->noticBoard_BGColor, userInput->switchFlag );
    strcpy( userInput->fieldLable, "Keypad");
    printkeypadLable( userInput );
  }


if( argc != NULL )
{
  while( inc < strlen(argc) )
   {
    temp = argc[inc];
    if( ( userInput->fixFormatstruct->fieldType == Date ) || ( userInput->fixFormatstruct->fieldType == Time ) )
    if ( userInput->fixFormatstruct->rollOverChar[0] == temp ){++inc; continue; }
    
    inc = step_default(userInput, temp, inc);
   }

   if( ( userInput->fixFormatstruct->fieldType == Date ) || ( userInput->fixFormatstruct->fieldType == Time ) ) inc = 0;
}

temp = keepRunning;
  while ( 1 )
  {
    temp = handleKeyPad( userInput );

    switch ( temp )
    {
      case keepRunning : break;
      case back :
        step_back(userInput, temp, inc);
        return;

      case clr :
        inc = step_clr(userInput, temp, inc);
        break;

      case ent:
        keypadDataparser(userInput);
        /* Now  userInput->userInputArray; buffer can we used as user final data */
        if ( userInput->fixFormatstruct && userInput->fixFormatstruct->fixFormatFlag )
        {
          userInput->fixFormatstruct->idx = 0;
        }

        return;

      case del :
        inc = step_del(userInput, temp, inc);
        break;

      case dot : break;

      case sw :
        step_sw(userInput, temp, inc);
        break;

      default :
        inc = step_default(userInput, temp, inc);
        break;
    }//end-switch

  }//end-while
  Serial.println("OUTPUT-->> ");
}



void userKeyBoard ::  step_back( struct _userInput *userInput, char temp, uint8_t inc )
{
  if ( userInput->fixFormatstruct && userInput->fixFormatstruct->fixFormatFlag )
  {
    memset( userInput->fixFormatstruct->fixFormatArray, '\0', userInput->fixFormatstruct->fixFormatArraySize );
    userInput->fixFormatstruct->fixFormatFlag = 0;
    userInput->fixFormatstruct->idx = 0;
    delete[]userInput->fixFormatstruct->fixFormatArray;
  }
  memset( userInput->userInputArray, '\0', userInput->userInputArray_Size );
}

uint8_t userKeyBoard ::  step_clr( struct _userInput *userInput, char temp, uint8_t inc )
{
  if ( userInput->fixFormatstruct && userInput->fixFormatstruct->fixFormatFlag )
  {
    switch ( userInput->fixFormatstruct->fieldType )
    {
      case Date :
        memset( userInput->fixFormatstruct->fixFormatArray, '\0', userInput->fixFormatstruct->fixFormatArraySize );
        userInput->fixFormatstruct->fixFormatFlag = 1;
        userInput->fixFormatstruct->idx = 0;
        strcpy(userInput->fixFormatstruct->fixFormatArray, "DD/MM/YY");
        {
          fixFormatFullBlankScreen
          int8_t lenght =  ( userInput->fixFormatstruct->len < userInput->fixFormatstruct->maxLengthScreen ? userInput->fixFormatstruct->len : userInput->fixFormatstruct->maxLengthScreen );
          for (int8_t i = 0; i < lenght ; i++ )
          {
            DrawfixFormatChar( userInput->fixFormatstruct->fixFormatArray[i], i,  userInput );
          }
          moveUrDash( userInput->fixFormatstruct->idx, userInput );
        }
        break;
      case Time :
        memset( userInput->fixFormatstruct->fixFormatArray, '\0', userInput->fixFormatstruct->fixFormatArraySize );
        userInput->fixFormatstruct->fixFormatFlag = 1;
        userInput->fixFormatstruct->idx = 0;
        strcpy(userInput->fixFormatstruct->fixFormatArray, "HH:MM:SS");

        {
          fixFormatFullBlankScreen
          int8_t lenght =  ( userInput->fixFormatstruct->len < userInput->fixFormatstruct->maxLengthScreen ? userInput->fixFormatstruct->len : userInput->fixFormatstruct->maxLengthScreen );

          for (int8_t i = 0; i < lenght ; i++ )
          {
            DrawfixFormatChar( userInput->fixFormatstruct->fixFormatArray[i], i,  userInput );
          }
          moveUrDash( userInput->fixFormatstruct->idx, userInput );
        }
        break;
      case Password :
      case stringType :
        memset( userInput->fixFormatstruct->fixFormatArray, '\0', userInput->fixFormatstruct->fixFormatArraySize);
        for (  userInput->fixFormatstruct->idx;  userInput->fixFormatstruct->idx >= 0 ;  --userInput->fixFormatstruct->idx )
          DrawfixFormatChar( userInput->fixFormatstruct->fixFormatArray[ userInput->fixFormatstruct->idx],  userInput->fixFormatstruct->idx, userInput );
        userInput->fixFormatstruct->idx = 0;
        moveUrDash( userInput->fixFormatstruct->idx, userInput );
        break;

      default :
        break;
    }
  }
  else
  {
    //    userInput->userInputStructureParamInit( userInput );
    inc = 0;
    userInput->entIdx = 15;
    memset( userInput->userInputArray, '\0', userInput->userInputArray_Size);
    memset( userInput->userInputArray, ' ',   userInput->entIdx );
    userInput->userInputArray[ userInput->entIdx] = '_';
    tft.drawString( &userInput->userInputArray[inc], userInput->text_xAxis, userInput->text_yAxis, 1);
  }
  return inc;
}

uint8_t userKeyBoard ::  step_del( struct _userInput *userInput, char temp, uint8_t inc )
{
  if (  userInput->fixFormatstruct && userInput->fixFormatstruct->fixFormatFlag && (  userInput->fixFormatstruct->idx  < userInput->fixFormatstruct->fixFormatArraySize ) )
  {
    if ( userInput->fixFormatstruct->idx = ( ( userInput->fixFormatstruct->idx <= 0 ) ? 0 : userInput->fixFormatstruct->idx ) )
    {
      switch ( userInput->fixFormatstruct->fieldType)
      {
        case Date :
        case Time :
          userInput->fixFormatstruct->fixFormatArray[ userInput->fixFormatstruct->idx] = '\0';
          DrawfixFormatChar( userInput->fixFormatstruct->fixFormatArray[ userInput->fixFormatstruct->idx],  userInput->fixFormatstruct->idx, userInput );
          moveUrDash( --userInput->fixFormatstruct->idx, userInput );
          if ( userInput->fixFormatstruct->rollOverChar[0] == userInput->fixFormatstruct->fixFormatArray[ userInput->fixFormatstruct->idx ] ) moveUrDash( --userInput->fixFormatstruct->idx, userInput );
          break;

        case Password :
          if ( ( userInput->fixFormatstruct->idx < userInput->fixFormatstruct->maxLen ) && ( userInput->fixFormatstruct->idx < userInput->fixFormatstruct->maxLengthScreen - 1 ))
          {
            userInput->fixFormatstruct->fixFormatArray[ userInput->fixFormatstruct->idx] = '\0';
            DrawfixFormatChar( userInput->fixFormatstruct->fixFormatArray[ userInput->fixFormatstruct->idx],  userInput->fixFormatstruct->idx, userInput );
            moveUrDash( --userInput->fixFormatstruct->idx, userInput );
          }
          else if (  userInput->fixFormatstruct->idx >= userInput->fixFormatstruct->maxLengthScreen - 1 )
          {
            userInput->fixFormatstruct->fixFormatArray[ --userInput->fixFormatstruct->idx ] = '\0';
          }
          break;

        case stringType :
          if ( ( userInput->fixFormatstruct->idx < userInput->fixFormatstruct->maxLen ) && ( userInput->fixFormatstruct->idx < userInput->fixFormatstruct->maxLengthScreen - 1 ))
          {
            userInput->fixFormatstruct->fixFormatArray[ userInput->fixFormatstruct->idx] = '\0';
            for (int8_t idx = 0; idx <  strlen( userInput->fixFormatstruct->fixFormatArray ) ; idx++ )
              DrawfixFormatChar( userInput->fixFormatstruct->fixFormatArray[ userInput->fixFormatstruct->idx],  userInput->fixFormatstruct->idx, userInput );

            moveUrDash( --userInput->fixFormatstruct->idx, userInput );
          }
          else if (  userInput->fixFormatstruct->idx >= userInput->fixFormatstruct->maxLengthScreen - 1 )
          {
            --userInput->fixFormatstruct->idx;
            moveUrDash(  userInput->fixFormatstruct->maxLengthScreen - 1, userInput );

            for (int8_t idx = 0, j = userInput->fixFormatstruct->idx - 15; idx <  userInput->fixFormatstruct->maxLengthScreen - 1 ; idx++ , j++)
              DrawfixFormatChar( userInput->fixFormatstruct->fixFormatArray[ j ], idx, userInput );

            userInput->fixFormatstruct->fixFormatArray[ userInput->fixFormatstruct->idx ] = '\0';

          }//end-else-if
          break;
      }//end-switch
    }//end-if

    delay(300);
  }//end-if
  else if ( !( userInput->fixFormatstruct && userInput->fixFormatstruct->fixFormatFlag ) )
  {
    if ( userInput->entIdx <= 15 )
    {
      //      userInput->userInputStructureParamInit( userInput );
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

  }//end-if
  return inc;
}


uint8_t userKeyBoard ::  step_default( struct _userInput *userInput, char temp, uint8_t inc )
{
  if (  userInput->fixFormatstruct && userInput->fixFormatstruct->fixFormatFlag && (  userInput->fixFormatstruct->idx  < userInput->fixFormatstruct->len ) )
  {
    userInputDate( userInput, temp );
    inc++;
  }
  else if (  !( userInput->fixFormatstruct && userInput->fixFormatstruct->fixFormatFlag ) )
  {
    if ( userInput->entIdx <  userInput->userInputArray_Size - 1 )
    {
      userInput->userInputArray[ userInput->entIdx++ ] = temp;
      userInput->userInputArray[ userInput->entIdx ] = '_';
      if ( userInput->entIdx >=  userInput->userInputArray_Size - 1 )
      {
        userInput->userInputArray[ userInput->userInputArray_Size - 1 ] = '\0';
      }
      Serial.println("idx-->> " + String(userInput->entIdx) );
      Serial.println("--->>>## " + String( userInput->userInputArray ) );
      tft.drawString( &userInput->userInputArray[++inc], userInput->text_xAxis, userInput->text_yAxis, 1);
    }
   
    }//end-if
  delay(350);
  return inc;
}

void userKeyBoard ::  step_sw( struct _userInput *userInput, char temp, uint8_t inc )
{
  if (  userInput->fixFormatstruct && userInput->fixFormatstruct->fixFormatFlag )
  {
    switch ( userInput->fixFormatstruct->fieldType )
    {
      case Date :
        noticBoard( userInput->noticBoard_FGColor, userInput->noticBoard_BGColor, userInput->fixFormatstruct->switchFlag );
        tft.setTextColor(userInput->text_FGColor,  userInput->text_BGColor );
        break;

      case Time :
        noticBoard( userInput->noticBoard_FGColor, userInput->noticBoard_BGColor, userInput->fixFormatstruct->switchFlag );
        tft.setTextColor(userInput->text_FGColor,  userInput->text_BGColor );
        break;
      case Password :
      case stringType :
      default :
        noticBoard( userInput->noticBoard_FGColor, userInput->noticBoard_BGColor, userInput->switchFlag );
        tft.setTextColor(userInput->text_FGColor,  userInput->text_BGColor );
        break;
    }//end-switch
  }//end-if
  else if ( !( userInput->fixFormatstruct && userInput->fixFormatstruct->fixFormatFlag ) )
  {
    noticBoard( userInput->noticBoard_FGColor, userInput->noticBoard_BGColor, userInput->switchFlag );
    tft.setTextColor(userInput->text_FGColor,  userInput->text_BGColor );
  }
}
