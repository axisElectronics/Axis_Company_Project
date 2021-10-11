

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
      char *_temp;
      uint16_t char_xAxis;
      uint16_t char_yAxis;
      int8_t charIdx;
      int8_t len; // this holds variable length accomedated into _tempArray.
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

    // set structure parameters here only at starting

    void moveUrDash( int8_t idx );
    uint8_t step_default(  );
    uint8_t step_clr    (  );
    uint8_t step_del    (  );
    void    step_back   (  );
    void    step_sw     (  );
    void    step_Left   (  );
    void    step_Right  (  );
    void noticBoard( uint16_t FGColor, uint16_t BGColor, bool switchFlag );
  public :
  
      headerInput() {
      
    //  tftInit();
      
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
    }//end-haeaderInput
  

    String takeStringInput(char *ptrbuff, int len);
    void step_edit();


};


#endif
