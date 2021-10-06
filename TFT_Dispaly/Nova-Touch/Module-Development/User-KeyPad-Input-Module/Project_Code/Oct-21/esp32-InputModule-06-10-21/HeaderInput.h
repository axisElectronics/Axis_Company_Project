

#ifndef _inputHeader_H
#define _inputHeader_H

#include "UserInputKeyBoard.h"
extern TFT_eSPI tft;

class headerInput: public userKeyBoard {
  private :
    int8_t _savekbdStatus;
    
    typedef struct _charControl{
      const GFXfont* customFont;
      uint16_t char_xAxis = 12;
      uint16_t char_yAxis = 15;
    };
    
    typedef  struct _dashControl {
      /* Draw dash charcter */
      uint16_t charXDash = 15;
      uint16_t charYDash = 60;
      uint16_t charWDash = 13;
      uint16_t charHDash = 10;

      uint16_t rectX = 15;
      uint16_t rectY = 25;
      uint16_t rectW = 13;
      uint16_t rectH = 22;

      uint16_t charX = rectX;
      uint16_t charY = 44;

      uint16_t dashColor;

      int8_t dashIdx;
    };

    typedef struct _screenControl {
      int maxLengthScreen = 17;
      char userInputArray[70];
      bool switchFlag=0;
      _charControl charControl;
      _dashControl dashControl;
    };

    _screenControl screenControl;

  public :
  
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
      screenControl.charControl.char_yAxis = 15;
      screenControl.charControl.customFont = mplus_R13;
      
      tft.setFreeFont( screenControl.charControl.customFont );
      tft.setTextColor( screenControl.dashControl.dashColor ,  TFT_WHITE );
      tft.setTextSize(1);
    }

    void moveUrDash( int8_t idx ) {
      /* Draw dash charcter */
      /* Remove all previous Dashes. It is must operation */
      for (  int8_t idx = 0; idx < screenControl.maxLengthScreen; ++idx )
        tft.fillRect( screenControl.dashControl.charXDash + (screenControl.dashControl.charWDash * idx), 47, screenControl.dashControl.charWDash, screenControl.dashControl.charHDash, TFT_BLACK);

      /* Print your Dash as per Index */
      tft.drawChar( '-',  screenControl.dashControl.charXDash + (idx * screenControl.dashControl.charWDash),  screenControl.dashControl.charYDash, 1);
    }//end-moveUrDash

    
    String takeStringInput(char *ptrbuff, int len) {
      if ( len == 0 ) return "";
      String temp = "ABCD";

      keypadImage();
      _savekbdStatus = keepRunning;
      for (;  ;) {
//        moveUrDash(screenControl.dashControl.dashIdx % screenControl.maxLengthScreen );
       if( (_savekbdStatus = handleKeyPad(  ) ) != keepRunning ){
        switch(_savekbdStatus){
             case sw :
                switchFlag != switchFlag;
                step_sw( _savekbdStatus, switchFlag );
                break;
          }
          }//end-if
        yield();
      }//end-for()
      return temp;
    }//end-takeStringInput


};

#endif
