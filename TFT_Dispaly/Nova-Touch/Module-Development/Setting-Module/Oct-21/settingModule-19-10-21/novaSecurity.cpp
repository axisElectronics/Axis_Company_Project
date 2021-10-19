
#include "settings.h"

class novaSecurity novaSecure;

int8_t  novaSecurity:: enterPassword() {

  // if( ( millis() - reAttemped_TimeOut) < 10000 ) return TimeOutERROR;

  //1. take user password
  kbd.userInput.userInputArray_Size = 10;
  kbd.userInput.userInputArray = _tempPassword;
  kbd.init( );
  kbd.userInput.numericSwitchFlag = ALPHANUM;
  kbd.userInput.fixFormatstruct->fixFormatFlag = 1;
  kbd.userInput.fixFormatstruct->fixFormatArraySize = 15;
  //  kbd.userInput.fixFormatstruct->fieldType = stringType;
  kbd.userInput.fixFormatstruct->fieldType = Password;

  kbd.takeUserInput( NULL );
  //  Serial.println("Password : " + String( kbd.userInput.fixFormatstruct->fixFormatArray ) );
  strcpy(_tempPassword, kbd.userInput.fixFormatstruct->fixFormatArray );
  Serial.println("Password temp : " + String( _tempPassword ) );
  // if length has zero it means. User Want to go back.
  if( strlen(_tempPassword) == 0 ) return ERROR;



  
  //2. now validate user
  if ( strlen( _tempPassword ) >= minPasswordLength ) {
    switch ( validatePassword( ) ) {
      case root    : SPL("You are root..!!");  faildAttemped = 0; return root;
      case admin   : if ( !strcmp(_adminPassword , _tempPassword) ) {
          SPL("You are admin..!!");
          faildAttemped = 0;
          return admin;
        }else goto HERE;
      case service : if( !strcmp(_servicePassword , _tempPassword ) ) {
        SPL("You are Samart service Man..!!"); 
        faildAttemped = 0;
        return service;
      }else goto HERE;
      default : 
      HERE :
      if ( ++faildAttemped < minPasswordRetryAttemped ) {
          tft.fillScreen(TFT_BLACK);
          tft.drawString( "Password is not Correct.", 30, 80, 1);
          tft.drawString( "Try Again ...!!1", 30, 120, 1);
          delay(3000);
          enterPassword();
        } else {
          tft.fillScreen(TFT_BLACK);
          tft.drawString( "you have exhusted your 3 attempts", 30, 80, 1);
          tft.drawString( "Try Again after SomeTime...!!", 30, 120, 1);
          delay(3000);
          faildAttemped = 0;
          reAttemped_TimeOut = millis();
          return TimeOutERROR;
        }
        break;
    }//end-switch
  }//end-if( length of password )
  // return response
  return 1;
}

void novaSecurity:: setAdminPassword(char *setAdmin) {
  memset(spiffs.userScaleSetup.password[admin], '\0', sizeof(spiffs.userScaleSetup.password[admin] ) );
  strcpy(spiffs.userScaleSetup.password[admin], setAdmin);
  spiffs._setPassword();
}



void  novaSecurity:: _fetchPassword() {
  spiffs._getPassword();
}



int8_t novaSecurity:: validatePassword() {
  if ( !strcmp(_tempPassword, _rootPassword ) ) {
    return root;
  }

  if ( !strcmp(_tempPassword, _adminPassword ) ) {
    return admin;
  }

  if ( !strcmp(_tempPassword, _servicePassword ) ) {
    return service;
  }
  return ERROR;
}
