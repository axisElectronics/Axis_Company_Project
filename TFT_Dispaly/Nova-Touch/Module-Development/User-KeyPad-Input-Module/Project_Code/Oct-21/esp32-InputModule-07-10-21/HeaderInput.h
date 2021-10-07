

#ifndef _inputHeader_H
#define _inputHeader_H

#include "UserInputKeyBoard.h"
extern TFT_eSPI tft;

class headerInput: public userKeyBoard {
  private :
    int8_t _savekbdStatus;
    bool   _traverseFlag;
    int8_t _saveCurrentIdx;
    
    typedef struct _charControl{
      const GFXfont* customFont;
      uint16_t char_xAxis;
      uint16_t char_yAxis;
      uint8_t charIdx;
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
      bool switchFlag=0;
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
      char temp[100];
      memset(temp, '\0', 100);

      keypadImage();
      noticBoard( TFT_GREEN, TFT_BLACK, userInput.switchFlag );
      moveUrDash(screenControl.dashControl.dashIdx % screenControl.maxLengthScreen );            
      _savekbdStatus = keepRunning;
      for (;  ;) {
     
       if( (_savekbdStatus = handleKeyPad(  ) ) != keepRunning ){
       
        switch(_savekbdStatus){
            // This piece of code is responsiable for Left Arrow operations to move cursor. 
            case Left:             
                
                if( _traverseFlag ){
                  _traverseFlag = false;
                  if( !_saveCurrentIdx && screenControl.dashControl.dashIdx >= 12 ){
                    _saveCurrentIdx = 12;
                  }
                  else if( !_saveCurrentIdx &&  screenControl.dashControl.dashIdx < 12 ){
                     _saveCurrentIdx =  screenControl.dashControl.dashIdx; 
                  }
                  else if( _saveCurrentIdx > 0 && _saveCurrentIdx <= 12 ){
                    /* leave as it is */
                  }
                }
                 Serial.printf("==>> %d\n", _saveCurrentIdx);
                if( _saveCurrentIdx > 12 ){
                  _saveCurrentIdx = 12;
                  moveUrDash( _saveCurrentIdx-- );
                }
                else if( _saveCurrentIdx <= 12 &&  _saveCurrentIdx > 0 ){
                     moveUrDash( _saveCurrentIdx-- );
                }
                else if( _saveCurrentIdx <= 0 ){
                   _saveCurrentIdx = 0;
                  moveUrDash( _saveCurrentIdx );
                }
                delay(50);
                break;
                
            // This piece of code is responsiable for Right Arrow operations to move cursor.    
            case Right:
             Serial.printf("==>> %d\n", _saveCurrentIdx);
             if( _saveCurrentIdx >= 0 && _saveCurrentIdx < 12  && _saveCurrentIdx < screenControl.dashControl.dashIdx ){                   
                 moveUrDash( ++_saveCurrentIdx );
            }
            else if( _saveCurrentIdx >= screenControl.dashControl.dashIdx && _saveCurrentIdx < 12 ){
               _saveCurrentIdx = screenControl.dashControl.dashIdx;
               moveUrDash( _saveCurrentIdx );
               
            }
            else if( _saveCurrentIdx >= 12 && _saveCurrentIdx <= screenControl.dashControl.dashIdx ){
                _saveCurrentIdx = 13;
                moveUrDash( _saveCurrentIdx );
                
            }
            _traverseFlag = true;
             delay(50); 
            break;
            case ent:
            Serial.printf("array==> %s\n", temp);
            break;
          
             case sw :
                userInput.switchFlag != userInput.switchFlag;  
                noticBoard( TFT_GREEN, TFT_BLACK, userInput.switchFlag );            
                break;
                
             case del :          
              moveUrDash(screenControl.dashControl.dashIdx % screenControl.maxLengthScreen );
              tft.drawString( &temp[0], screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
              
             break;
             
             default :     
                 //Add received character into temp array 
                  
                 temp[screenControl.dashControl.dashIdx]= (char)_savekbdStatus;
                 temp[strlen(temp)] = '\0';

                 // check temp array size  and keypad Screen so that I can decide it should scroll OR NOT
                 if( screenControl.dashControl.dashIdx < 12 ){
                  screenControl.charControl.charIdx = 0;
                 }
                 else if( screenControl.dashControl.dashIdx > 12 ){
                  screenControl.charControl.charIdx++;
                 }
                 
                 // Show array content on screen
                 tft.drawString( &temp[screenControl.charControl.charIdx], screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1); 
                 screenControl.dashControl.dashIdx++;

                 // check if we have reached at last character of keypad Screen
                 // Now no need to mode your dash
                 if( screenControl.dashControl.dashIdx < 12 ){
                  moveUrDash( screenControl.dashControl.dashIdx );    
                 }
                 else{
                  moveUrDash( 13 );
                 }
                                      
               break;
          }//end-switch
          }//end-if
        yield();
      }//end-for()
      
      
      return temp;
    }//end-takeStringInput


};

#endif
