

#ifndef _inputHeader_H
#define _inputHeader_H

#include "UserInputKeyBoard.h"
extern TFT_eSPI tft;

class headerInput: public userKeyBoard {
  private :
    int8_t _savekbdStatus;
    bool   _traverseFlag;
    int8_t _saveCurrentIdx;
    char _temp[100];

    typedef struct _charControl {
      const GFXfont* customFont;
      uint16_t char_xAxis;
      uint16_t char_yAxis;
      int8_t charIdx;
    };

    typedef  struct _dashControl {
      /* Draw dash charcter */
      uint16_t charXDash;
      uint16_t charYDash;
      uint16_t charWDash;
      uint16_t charHDash;

      uint16_t rectX;
      uint16_t rectY;
      uint16_t rectW;
      uint16_t rectH;

      uint16_t charX;
      uint16_t charY;

      uint16_t dashColor;

      int8_t dashIdx;
    };

    typedef struct _screenControl {
      int maxLengthScreen = 17;
      char userInputArray[70];
      bool switchFlag = 0;
      _charControl charControl;
      _dashControl dashControl;
    };

    _screenControl screenControl;

  public :
    // set structure parameters here only at starting
    headerInput() {
      // set input screen max length
      screenControl.maxLengthScreen = 17;

      // set dash charcter parameters
      screenControl.dashControl.charXDash = 15;
      screenControl.dashControl.charYDash = 60;
      screenControl.dashControl.charWDash = 13;
      screenControl.dashControl.charHDash = 10;

      screenControl.dashControl.rectX = 15;
      screenControl.dashControl.rectY = 25;
      screenControl.dashControl.rectW = 13;
      screenControl.dashControl.rectH = 22;

      screenControl.dashControl.charX =  screenControl.dashControl.rectX;
      screenControl.dashControl.charY = 44;
      screenControl.dashControl.dashColor = TFT_GREEN;

      // set character properties
      screenControl.charControl.char_xAxis = 12;
      screenControl.charControl.char_yAxis = 18;
      screenControl.charControl.customFont = mplus_R13;

      tft.setFreeFont( screenControl.charControl.customFont );
      tft.setTextColor( screenControl.dashControl.dashColor ,  TFT_WHITE );
      tft.setTextSize(1);

      _traverseFlag = true;

      if ( _traverseFlag ) {
        _traverseFlag = false;
        if ( !_saveCurrentIdx && screenControl.dashControl.dashIdx >= 12 ) {
          _saveCurrentIdx = 12;
        }
        else if ( !_saveCurrentIdx &&  screenControl.dashControl.dashIdx < 12 ) {
          _saveCurrentIdx =  screenControl.dashControl.dashIdx;
        }
        else if ( _saveCurrentIdx > 0 && _saveCurrentIdx <= 12 ) {
          /* leave as it is */
        }
      }
      _traverseFlag = true;
    }

    void moveUrDash( int8_t idx );
    String takeStringInput(char *ptrbuff, int len);
    uint8_t step_default(    );
    uint8_t step_clr    (   );
    uint8_t step_del    (    );
    void    step_back   (     );
    void    step_sw     (    );
    void    step_Left();
    void    step_Right ();


};

static int8_t switchFlag = 0;

void headerInput:: moveUrDash( int8_t idx ) {
  /* Draw dash charcter */
  /* Remove all previous Dashes. It is must operation */
  for (  int8_t idx = 0; idx < screenControl.maxLengthScreen; ++idx )
    tft.fillRect( screenControl.dashControl.charXDash + (screenControl.dashControl.charWDash * idx), 47, screenControl.dashControl.charWDash, screenControl.dashControl.charHDash, TFT_BLACK);

  /* Print your Dash as per Index */
  tft.drawChar( '-',  screenControl.dashControl.charXDash + (idx * screenControl.dashControl.charWDash),  screenControl.dashControl.charYDash, 1);
}//end-moveUrDash


String headerInput:: takeStringInput(char *ptrbuff, int len) {
  if ( len == 0 ) return "";

  memset(_temp, '\0', 100);
  switchFlag = switchFlag ? 0 : 1;
  userInput.switchFlag = switchFlag;

  keypadImage();
  noticBoard( TFT_GREEN, TFT_BLACK, userInput.switchFlag );
  moveUrDash(screenControl.dashControl.dashIdx % screenControl.maxLengthScreen );
  _savekbdStatus = keepRunning;
  // There is not any invalid key.
  for (int i = 0; i < 12; ++i ) {
    userInput.invalidKey[i] = keepRunning;
  }



  for (;  ;) {

    if ( (_savekbdStatus = handleKeyPad(  ) ) != keepRunning ) {

      switch (_savekbdStatus) {
        // This piece of code is responsiable for Left Arrow operations to move cursor.
        case Left:
          step_Left();
          break;

        // This piece of code is responsiable for Right Arrow operations to move cursor.
        case Right:
          step_Right();
          break;
        case ent:
          Serial.printf("array==> %s\n", _temp);
          break;

        case sw :
          step_sw();
          break;

        case del :
          step_del();
          break;

        default :
          step_default();
          break;
      }//end-switch
    }//end-if
    yield();
  }//end-for()


  return _temp;
}//end-takeStringInput


void headerInput:: step_Left() {
    if( _traverseFlag ){
    if ( _saveCurrentIdx >= 0 && _saveCurrentIdx < 12  && _saveCurrentIdx < screenControl.dashControl.dashIdx ){
      if(  screenControl.dashControl.dashIdx > 13 ){
        _saveCurrentIdx = 13;
        Serial.printf("set: %d\n", _saveCurrentIdx);
      }
    }
  }
  
  if ( _saveCurrentIdx > 12 ) {
    _saveCurrentIdx = 12;
    moveUrDash( _saveCurrentIdx-- );
  }
  else if ( _saveCurrentIdx <= 12 &&  _saveCurrentIdx > 0 ) {
    moveUrDash( _saveCurrentIdx-- );
  }
  else if ( _saveCurrentIdx <= 0 ) {
    _saveCurrentIdx = 0;
    moveUrDash( _saveCurrentIdx );
    if ( screenControl.charControl.charIdx-- > 0 ) {
      char arr[15];
      strncpy(arr, &_temp[screenControl.charControl.charIdx], 13 );
      arr[14] = '\0';
      tft.drawString( arr, screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
    } else {
      screenControl.charControl.charIdx = 0;
    }

  }
  _traverseFlag = false;
  delay(50);
}


void headerInput:: step_Right () {
     if( _traverseFlag ){
    if ( _saveCurrentIdx >= 0 && _saveCurrentIdx < 12  && _saveCurrentIdx < screenControl.dashControl.dashIdx ){
      if(  screenControl.dashControl.dashIdx > 13 ){
        _saveCurrentIdx = 13;
        Serial.printf("set: %d\n", _saveCurrentIdx);
      }
      else if(  screenControl.dashControl.dashIdx <= 13 ){
         _saveCurrentIdx = screenControl.dashControl.dashIdx;
      }
    }
  }
  
  if ( _saveCurrentIdx >= 0 && _saveCurrentIdx < 12  && _saveCurrentIdx < screenControl.dashControl.dashIdx ) {
    moveUrDash( ++_saveCurrentIdx );
  }
  else if ( _saveCurrentIdx >= screenControl.dashControl.dashIdx && _saveCurrentIdx < 12 ) {
    _saveCurrentIdx = screenControl.dashControl.dashIdx;
    moveUrDash( _saveCurrentIdx );

  }
  else if ( _saveCurrentIdx >= 12 && _saveCurrentIdx <= screenControl.dashControl.dashIdx ) {
    _saveCurrentIdx = 13;
    moveUrDash( _saveCurrentIdx );
    if ( screenControl.charControl.charIdx++ < strlen(_temp) - 13 ) {
      char arr[15];
      strncpy(arr, &_temp[screenControl.charControl.charIdx], 13 );
      arr[14] = '\0';
      tft.drawString( arr, screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
    } else {
      screenControl.charControl.charIdx = strlen(_temp) - 13;
    }

  }
  
  _traverseFlag = false;
  delay(50);
}


uint8_t headerInput:: step_default(    ) {
  //Add received character into temp array

  _temp[screenControl.dashControl.dashIdx] = (char)_savekbdStatus;
  _temp[strlen(_temp)] = '\0';

  // check temp array size  and keypad Screen so that I can decide it should scroll OR NOT
  if ( screenControl.dashControl.dashIdx < 12 ) {
    screenControl.charControl.charIdx = 0;
  }
  else if ( screenControl.dashControl.dashIdx > 12 ) {
    screenControl.charControl.charIdx++;
  }

  // Show array content on screen
  tft.drawString( &_temp[screenControl.charControl.charIdx], screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
  screenControl.dashControl.dashIdx++;

  // check if we have reached at last character of keypad Screen
  // Now no need to mode your dash
  if ( screenControl.dashControl.dashIdx < 12 ) {
    moveUrDash( screenControl.dashControl.dashIdx );
  }
  else {
    moveUrDash( 13 );
  }

  delay(100);
}


uint8_t headerInput:: step_clr    (    ) {
  memset(_temp, '\0', sizeof(_temp) );
  moveUrDash( screenControl.dashControl.dashIdx = 0 );
  screenControl.charControl.charIdx = 0;
  _savekbdStatus = 0;
  tft.drawString( &_temp[screenControl.charControl.charIdx], screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
}


uint8_t headerInput:: step_del    (    ) {

  if ( _traverseFlag  ) {
    HERE:
    if ( screenControl.dashControl.dashIdx > 13 ) {

      _temp[ --screenControl.dashControl.dashIdx ] = '\0';

      if ( screenControl.charControl.charIdx-- > 0 ) {
        char arr[15];
        strncpy(arr, &_temp[screenControl.charControl.charIdx], 13 );
        arr[14] = '\0';
        tft.drawString( arr, screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
      }//end-if
    }

    else if (screenControl.dashControl.dashIdx <= 13 ) {
      if ( screenControl.charControl.charIdx <= 0 ) {

        char arr[17] = {0};
        memset(arr, ' ', 16);
        tft.drawString( arr, screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);

        screenControl.charControl.charIdx = 0;
        if ( screenControl.dashControl.dashIdx > 0 )
          _temp[ --screenControl.dashControl.dashIdx ] = '\0';
        _saveCurrentIdx = screenControl.dashControl.dashIdx;
        moveUrDash( _saveCurrentIdx );
        Serial.printf("***dash= %d ***\n", screenControl.dashControl.dashIdx);
        tft.drawString( &_temp[screenControl.charControl.charIdx], screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
        // _saveCurrentIdx = screenControl.dashControl.dashIdx;
      }

    }//end-else
  }//end-if( _traverseFlag )

  else  {
Serial.printf("*************\n \
_saveCurrentIdx ==>> %d\n \
dashIdx ==>> %d\n\
charIdx ==>> %d\n", _saveCurrentIdx,  screenControl.dashControl.dashIdx, screenControl.charControl.charIdx );

if ( _saveCurrentIdx == 11 && screenControl.dashControl.dashIdx >= 11) {
  goto HERE; 
}//end-if
else if( _saveCurrentIdx < 13 && screenControl.dashControl.dashIdx >= 13 ){
   goto HERE; 
}//end-else-if()
else if( _saveCurrentIdx < 13 && screenControl.dashControl.dashIdx <= 13 ){
   goto HERE; 
}//end-else-if()
}//end-else
  delay(50);
}//end-step_del


void    headerInput:: step_back   (    ) {

}


void    headerInput:: step_sw     (    ) {
  switchFlag = switchFlag ? 0 : 1;
  userInput.switchFlag = switchFlag;
  noticBoard( TFT_GREEN, TFT_BLACK, userInput.switchFlag );
}

#endif
