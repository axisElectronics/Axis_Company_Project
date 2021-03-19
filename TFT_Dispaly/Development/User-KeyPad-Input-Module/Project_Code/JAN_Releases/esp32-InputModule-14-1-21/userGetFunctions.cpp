#include "UserInputKeyBoard.h"

extern TFT_eSPI tft;


void configFormats(struct _userInput *userInput )
{
    switch ( userInput->fixFormatstruct->fieldType )
    {
      case Date:
        getDate( userInput,  userInput->fixFormatstruct, "DD/MM/YY", strlen("DD/MM/YY"), "/" );
        noticBoard( userInput->noticBoard_FGColor, userInput->noticBoard_BGColor, userInput->fixFormatstruct->switchFlag );
        printkeypadLable( userInput );
        moveUrDash(  userInput->fixFormatstruct->idx, userInput );
        break;

      case Time:
        getTime( userInput,  userInput->fixFormatstruct, "HH:MM:SS", strlen("HH:MM:SS"), ":" );
        noticBoard( userInput->noticBoard_FGColor, userInput->noticBoard_BGColor, userInput->fixFormatstruct->switchFlag );
        printkeypadLable( userInput );
        moveUrDash(  userInput->fixFormatstruct->idx, userInput );
        break;

     case Password :
        getPassword( userInput,  userInput->fixFormatstruct, 5, 10);
        noticBoard( userInput->noticBoard_FGColor, userInput->noticBoard_BGColor, userInput->switchFlag );
        printkeypadLable( userInput );
        moveUrDash(  userInput->fixFormatstruct->idx, userInput );
        break;  

     case stringType :
        getStringType( userInput,  userInput->fixFormatstruct, 0, 20);
        noticBoard( userInput->noticBoard_FGColor, userInput->noticBoard_BGColor, userInput->switchFlag );
        printkeypadLable( userInput );
        moveUrDash(  userInput->fixFormatstruct->idx, userInput );
        break;  

      default :
        break;

    }//ens-switch
}


void userInputDate(  struct _userInput *userInput, char temp )
{
  switch (userInput->fixFormatstruct->fieldType)
  {
    case Date :
    case Time :            
      moveUrDash( userInput->fixFormatstruct->idx, userInput );
      userInput->fixFormatstruct->fixFormatArray[ userInput->fixFormatstruct->idx] = temp;     
      DrawfixFormatChar( userInput->fixFormatstruct->fixFormatArray[ userInput->fixFormatstruct->idx ],  userInput->fixFormatstruct->idx, userInput );
      moveUrDash( ++userInput->fixFormatstruct->idx, userInput );
      if ( userInput->fixFormatstruct->rollOverChar[0] == userInput->fixFormatstruct->fixFormatArray[ userInput->fixFormatstruct->idx] ) moveUrDash( ++userInput->fixFormatstruct->idx, userInput );
      delay(350);
      break;

    case Password :
    Serial.println("PasswordIdx : " + String(userInput->fixFormatstruct->idx) );
      if ( userInput->fixFormatstruct->idx < userInput->fixFormatstruct->maxLen )
      {
        moveUrDash( userInput->fixFormatstruct->idx, userInput );
        userInput->fixFormatstruct->fixFormatArray[ userInput->fixFormatstruct->idx] = temp;
        DrawfixFormatChar( '*',  userInput->fixFormatstruct->idx, userInput );
        moveUrDash( ++userInput->fixFormatstruct->idx, userInput );
        if ( userInput->fixFormatstruct->idx >= userInput->fixFormatstruct->maxLen )
           moveUrDash( --userInput->fixFormatstruct->idx, userInput );
        delay(350);
      }
      break;

     case stringType :
  
      if ( userInput->fixFormatstruct->idx < userInput->fixFormatstruct->maxLen )
      { 
        if( userInput->fixFormatstruct->idx < 16)
        {
          Serial.println("stringIDX : " + String(userInput->fixFormatstruct->idx) + " StringMAX : " + String(userInput->fixFormatstruct->maxLen) );
          moveUrDash( userInput->fixFormatstruct->idx, userInput );
          userInput->fixFormatstruct->fixFormatArray[ userInput->fixFormatstruct->idx] = temp;
          DrawfixFormatChar(  userInput->fixFormatstruct->fixFormatArray[ userInput->fixFormatstruct->idx], userInput->fixFormatstruct->idx, userInput );
          moveUrDash( ++userInput->fixFormatstruct->idx, userInput );       
        }
      else if(  userInput->fixFormatstruct->idx >= 16 )
        {
          Serial.println("stringIDX_1 : " + String(userInput->fixFormatstruct->idx) + " StringMAX : " + String(userInput->fixFormatstruct->maxLen) );
          moveUrDash( 16, userInput );
          userInput->fixFormatstruct->fixFormatArray[ userInput->fixFormatstruct->idx ] = temp;
          for(int8_t idx = 0, j = userInput->fixFormatstruct->idx - 15; idx < 16 ; idx++ , j++)
            DrawfixFormatChar( userInput->fixFormatstruct->fixFormatArray[j], idx , userInput );
          
          userInput->fixFormatstruct->idx++;
        }//end-else-if
      }//end-if
      delay(350);
     break;
  }//end-switch
  Serial.println("--->>>>@ " + String( userInput->fixFormatstruct->fixFormatArray ) );
}



char *getStringType( struct _userInput *userInput, struct _fixFormat *userFixFormat, int8_t minLen, int8_t maxLen )
{
  getPassword(userInput, userFixFormat, minLen, maxLen );
  userInput->fixFormatstruct->switchFlag = 1;
  strcpy( userInput->fieldLable, "String");
}



char *getPassword( struct _userInput *userInput, struct _fixFormat *userFixFormat, int8_t minLen, int8_t maxLen )
{
  /*
     1@ : User need to send _fixFormat structure instance
     2@ : User need to send _userInput Structure instance
     3@ : User need to send fixFormat Array size
  */
  userInput->fixFormatstruct = userFixFormat;

  userInput->fixFormatstruct->fixFormatArray = new char[userInput->fixFormatstruct->fixFormatArraySize];
  memset(userInput->fixFormatstruct->fixFormatArray, '\0', userInput->fixFormatstruct->fixFormatArraySize);

  userInput->fixFormatstruct->minLen = minLen;
  userInput->fixFormatstruct->maxLen = maxLen;
  userInput->fixFormatstruct->len = maxLen;
  userInput->fixFormatstruct->switchFlag = 1;
  strcpy( userInput->fieldLable, "Password");


}
char *getTime( struct _userInput *userInput, struct _fixFormat *userFixFormat, char *formatbuffer, int8_t len, char *rollOverChar  )
{
  getDate( userInput,  userInput->fixFormatstruct, formatbuffer , len, rollOverChar );
  userInput->fixFormatstruct->fieldType = Time;
  userInput->fixFormatstruct->switchFlag = 0;
  strcpy( userInput->fieldLable, "Time");

}

char *getDate( struct _userInput *userInput, struct _fixFormat *userFixFormat, char *formatbuffer, int8_t len, char *rollOverChar  )
{
  /*
     1@ : User need to send _fixFormat structure instance
     2@ : User need to send fixFormat Array size
     3@ : User need to send _userInput Structure instance
     4@ : User need to send user Date string i.e "DD/MM/YY"
     5@ : User need to send rolloverCharacter.
  */

  userInput->fixFormatstruct = userFixFormat;

  userInput->fixFormatstruct->fixFormatArray = new char[userInput->fixFormatstruct->fixFormatArraySize];
  memset(userInput->fixFormatstruct->fixFormatArray, '\0', userInput->fixFormatstruct->fixFormatArraySize);

  strcpy( userInput->fixFormatstruct->fixFormatArray, formatbuffer );
  userInput->fixFormatstruct->len = len;
  userInput->fixFormatstruct->rollOverChar = rollOverChar; // It can be multiple chacter developer want to rollOver in Fix Format.

  /* Developer has only 17 character size to print fix format. */
  fixFormatFullBlankScreen
  for (int8_t i = 0; i < userInput->fixFormatstruct->maxLengthScreen; i++ )
  {
    DrawfixFormatChar( userInput->fixFormatstruct->fixFormatArray[i], i,  userInput );
  }
  moveUrDash( 0, userInput );

  userInput->fixFormatstruct->fixFormatFlag = 1;
  userInput->fixFormatstruct->switchFlag = 0;
  userInput->fixFormatstruct->fieldType = Date;
  strcpy( userInput->fieldLable, "Date");
  //  userInput->fixFormatstruct->getCallBackFunc = getDate;
}
