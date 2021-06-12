#include "UserInputKeyBoard.h"

extern TFT_eSPI tft;


void  userKeyBoard :: configFormats( )
{
    switch (userInput.fixFormatstruct->fieldType )
    {
      case Date:
        
        getDate( "DD/MM/YY", strlen("DD/MM/YY"), "/" );
        noticBoard(userInput.noticBoard_FGColor,userInput.noticBoard_BGColor,userInput.fixFormatstruct->switchFlag );
        printkeypadLable(  );
        moveUrDash( userInput.fixFormatstruct->idx );
        break;

      case Time:
        getTime( "HH:MM:SS", strlen("HH:MM:SS"), ":" );
        noticBoard(userInput.noticBoard_FGColor,userInput.noticBoard_BGColor,userInput.fixFormatstruct->switchFlag );
        printkeypadLable(  );
        moveUrDash( userInput.fixFormatstruct->idx );
        break;

     case Password :
        getPassword( 5, 20);
        noticBoard(userInput.noticBoard_FGColor,userInput.noticBoard_BGColor,userInput.switchFlag );
        printkeypadLable(  );
        moveUrDash( userInput.fixFormatstruct->idx );
        break;  

     case stringType :
        getStringType( 0,userInput.fixFormatstruct->fixFormatArraySize );
        noticBoard(userInput.noticBoard_FGColor,userInput.noticBoard_BGColor,userInput.switchFlag );
        printkeypadLable(  );
        moveUrDash( userInput.fixFormatstruct->idx );
        break;  

      default :
        break;

    }//ens-switch
}


void userKeyBoard ::  userInputDate(   char temp )
{
  switch (userInput.fixFormatstruct->fieldType)
  {
    case Date :
    case Time :            
      moveUrDash( userInput.fixFormatstruct->idx );
     userInput.fixFormatstruct->fixFormatArray[userInput.fixFormatstruct->idx] = temp;     
      DrawfixFormatChar(userInput.fixFormatstruct->fixFormatArray[userInput.fixFormatstruct->idx ], userInput.fixFormatstruct->idx );
      moveUrDash( ++userInput.fixFormatstruct->idx );
      if (userInput.fixFormatstruct->rollOverChar[0] == userInput.fixFormatstruct->fixFormatArray[userInput.fixFormatstruct->idx] ) moveUrDash( ++userInput.fixFormatstruct->idx );
      delay(350);
      break;

    case Password :
     if (userInput.fixFormatstruct->idx <userInput.fixFormatstruct->maxLen )
      { 
        if(userInput.fixFormatstruct->idx <userInput.fixFormatstruct->maxLengthScreen-1 )
        {           
              moveUrDash(userInput.fixFormatstruct->idx );
             userInput.fixFormatstruct->fixFormatArray[userInput.fixFormatstruct->idx] = temp;
              DrawfixFormatChar( '*', userInput.fixFormatstruct->idx );
              moveUrDash( ++userInput.fixFormatstruct->idx );
              if (userInput.fixFormatstruct->idx >=userInput.fixFormatstruct->maxLen )
                 moveUrDash( --userInput.fixFormatstruct->idx );                        
        }
        else if( userInput.fixFormatstruct->idx >= userInput.fixFormatstruct->maxLengthScreen-1 )
        {          
          moveUrDash( userInput.fixFormatstruct->maxLengthScreen-1 );
         userInput.fixFormatstruct->fixFormatArray[userInput.fixFormatstruct->idx ] = temp;
          for(int8_t idx = 0, j =userInput.fixFormatstruct->idx - 15; idx < userInput.fixFormatstruct->maxLengthScreen-1 ; idx++ , j++)
            DrawfixFormatChar( '*', idx  );
          
         userInput.fixFormatstruct->idx++;
        }
      }//end-if
      delay(350); 
      break;
      
     case stringType :
  
      if (userInput.fixFormatstruct->idx <userInput.fixFormatstruct->maxLen )
      { 
        if(userInput.fixFormatstruct->idx < userInput.fixFormatstruct->maxLengthScreen-1 )
        {         
          moveUrDash(userInput.fixFormatstruct->idx );
         userInput.fixFormatstruct->fixFormatArray[userInput.fixFormatstruct->idx] = temp;
          DrawfixFormatChar( userInput.fixFormatstruct->fixFormatArray[userInput.fixFormatstruct->idx],userInput.fixFormatstruct->idx );
          moveUrDash( ++userInput.fixFormatstruct->idx );       
        }
      else if( (userInput.fixFormatstruct->idx >= userInput.fixFormatstruct->maxLengthScreen-1 ) && (userInput.fixFormatstruct->idx <userInput.fixFormatstruct->fixFormatArraySize-1 ) )
        {          
          moveUrDash( userInput.fixFormatstruct->maxLengthScreen-1 );
         userInput.fixFormatstruct->fixFormatArray[userInput.fixFormatstruct->idx ] = temp;
          for(int8_t idx = 0, j =userInput.fixFormatstruct->idx - 15; idx < userInput.fixFormatstruct->maxLengthScreen-1 ; idx++ , j++)
            DrawfixFormatChar(userInput.fixFormatstruct->fixFormatArray[j], idx  );
          
         userInput.fixFormatstruct->idx++;
        }//end-else-if
        else 
        {
          Serial.println("Size Excedded");
        }
      }//end-if
      delay(350);
     break;
  }//end-switch
  Serial.println("--->>>>@ " + String(userInput.fixFormatstruct->fixFormatArray ) );
}



char*  userKeyBoard :: getStringType( int8_t minLen, int8_t maxLen )
{
  getPassword( minLen, maxLen );
 userInput.fixFormatstruct->switchFlag = 1;
  strcpy(userInput.fieldLable, "String");
}



char * userKeyBoard :: getPassword( int8_t minLen, int8_t maxLen )
{
  /*
     1@ : User need to send _fixFormat structure instance
     2@ : User need to send _userInput Structure instance
     3@ : User need to send fixFormat Array size
  */
 userInput.fixFormatstruct = &userFixFormat;

 userInput.fixFormatstruct->fixFormatArray = new char[ userInput.fixFormatstruct->fixFormatArraySize];
  memset( userInput.fixFormatstruct->fixFormatArray, '\0', userInput.fixFormatstruct->fixFormatArraySize);

 userInput.fixFormatstruct->minLen = minLen;
 userInput.fixFormatstruct->maxLen = maxLen;
 userInput.fixFormatstruct->len = maxLen;
 userInput.fixFormatstruct->switchFlag = 1;
  strcpy(userInput.fieldLable, "Password");


}
char * userKeyBoard :: getTime(  char *formatbuffer, int8_t len, char *rollOverChar  )
{
  getDate( formatbuffer , len, rollOverChar );
 userInput.fixFormatstruct->fieldType = Time;
 userInput.fixFormatstruct->switchFlag = 0;
  strcpy(userInput.fieldLable, "Time");

}

char * userKeyBoard :: getDate( char *formatbuffer, int8_t len, char *rollOverChar  )
{
  /*
     1@ : User need to send _fixFormat structure instance
     2@ : User need to send fixFormat Array size
     3@ : User need to send _userInput Structure instance
     4@ : User need to send user Date string i.e "DD/MM/YY"
     5@ : User need to send rolloverCharacter.
  */

 userInput.fixFormatstruct = &userFixFormat;

 userInput.fixFormatstruct->fixFormatArray = new char[userInput.fixFormatstruct->fixFormatArraySize];
  memset( userInput.fixFormatstruct->fixFormatArray, '\0', userInput.fixFormatstruct->fixFormatArraySize );

  strcpy(userInput.fixFormatstruct->fixFormatArray, formatbuffer );
 
 userInput.fixFormatstruct->len = len;
 userInput.fixFormatstruct->rollOverChar = rollOverChar; // It can be multiple chacter developer want to rollOver in Fix Format.
 userInput.invalidKey[0] = dot;
  
  /* Developer has only 17 character size to print fix format. */
  fixFormatFullBlankScreen
  int8_t lenght =  (userInput.fixFormatstruct->len < userInput.fixFormatstruct->maxLengthScreen ? userInput.fixFormatstruct->len : userInput.fixFormatstruct->maxLengthScreen ); 
  for (int8_t i = 0; i < lenght ; i++ )
  {
    DrawfixFormatChar(userInput.fixFormatstruct->fixFormatArray[i], i );
  }
  moveUrDash( 0 );

 userInput.fixFormatstruct->fixFormatFlag = 1;
 userInput.fixFormatstruct->switchFlag = 0;
 userInput.fixFormatstruct->fieldType = Date;
  strcpy(userInput.fieldLable, "Date");  
}
