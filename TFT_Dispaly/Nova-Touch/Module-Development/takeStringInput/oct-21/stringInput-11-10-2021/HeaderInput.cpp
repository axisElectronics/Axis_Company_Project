#include "HeaderInput.h"

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
  screenControl.dashControl.dashIdx = 0;
  screenControl.charControl.charIdx = 0;

  keypadImage();
  noticBoard( TFT_GREEN, TFT_BLACK, userInput.switchFlag );
  moveUrDash(screenControl.dashControl.dashIdx );
  _savekbdStatus = keepRunning;

  // There is not any invalid key.
  for (int i = 0; i < 12; ++i ) {
    userInput.invalidKey[i] = keepRunning;
  }


  // check if there is anydata available into this buffer then show data on screen
  if (strlen(ptrbuff) != 0) {
    strcpy(_temp, ptrbuff);

    // Show array content on screen
    char arr[15];
    memset(arr, '\0', sizeof(arr));
    strncpy(arr, &_temp[screenControl.charControl.charIdx], 13 );
    tft.drawString( arr, screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
  }//end-if(ptrbuff)

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
          return _temp;

        case sw :
          step_sw();
          break;

        case del :
          step_del();
          break;

        case back :
          step_back();
          return _temp;

        case clr:
          step_clr( );
          break;

        case edit:
          step_edit();
          break;

        default :
          step_default();
          break;
      }//end-switch
    }//end-if


    yield();
  }//end-for()

  ptrbuff = _temp;
  return _temp;
}//end-takeStringInput




void headerInput:: step_Left() {
  --screenControl.dashControl.dashIdx;

  if ( screenControl.dashControl.dashIdx <= 0 ) {
    screenControl.dashControl.dashIdx = 0;

    screenControl.charControl.charIdx = (screenControl.charControl.charIdx <= 0 ) ? 0 : --screenControl.charControl.charIdx;
    // Show array content on screen
    char arr[15];
    memset(arr, '\0', sizeof(arr));
    memset(arr, ' ', sizeof(arr) - 1);
    tft.drawString( arr, screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
    strncpy(arr, &_temp[screenControl.charControl.charIdx], 13 );
    tft.drawString( arr, screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
  }//end-if

  moveUrDash ( screenControl.dashControl.dashIdx );
  delay(50);
}


void headerInput:: step_Right () {

  if ( screenControl.dashControl.dashIdx >= 13 ) {
    screenControl.dashControl.dashIdx = 13;
    if ( ( screenControl.dashControl.dashIdx + screenControl.charControl.charIdx ) < strlen(_temp) ) {
      screenControl.charControl.charIdx++;
      // Show array content on screen
      char arr[15];
      memset(arr, '\0', sizeof(arr));
      memset(arr, ' ', sizeof(arr) - 1);
      tft.drawString( arr, screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
      strncpy(arr, &_temp[screenControl.charControl.charIdx], 13 );
      tft.drawString( arr, screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
    }
  }
  else if ( screenControl.dashControl.dashIdx >= 0 && screenControl.dashControl.dashIdx < 13 ) {
    ++screenControl.dashControl.dashIdx;
  }

  moveUrDash ( screenControl.dashControl.dashIdx );
  delay(50);
}




uint8_t headerInput:: step_default(    ) {
  //Add received character into temp array
  if ( screenControl.dashControl.dashIdx <= 12 &&  screenControl.dashControl.dashIdx < strlen(_temp) ) {
    int idx = screenControl.charControl.charIdx + screenControl.dashControl.dashIdx;
    char *arr = new char[strlen(_temp) - idx + 1];
    memset(arr, '\0', strlen(_temp) - idx);
    // copy all content upto last
    strcpy(arr, &_temp[idx]);
    // add new character at dash position
    _temp[idx] = (char)_savekbdStatus;
    _temp[idx + 1] = '\0';
    // concatinate with remaining array data.
    strcat(_temp, arr);
    delete[] arr;
    //draw empty blocks
    char emptyBlock[15];
    memset(emptyBlock, '\0', sizeof(emptyBlock));
    memset(emptyBlock, ' ', sizeof(emptyBlock) - 1);
    tft.drawString( emptyBlock, screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);

    // check temp array size  and keypad Screen so that I can decide it should scroll OR NOT
      screenControl.dashControl.dashIdx++;
      if(  screenControl.dashControl.dashIdx > 12 ){
       screenControl.dashControl.dashIdx=12;
       screenControl.charControl.charIdx++; 
      }
    // Show array content on screen
    char newarr[15];
    memset(newarr, '\0', sizeof(newarr));
    strncpy(newarr,  &_temp[screenControl.charControl.charIdx], 13);
    tft.drawString( newarr, screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
  }//end-if
  else {
    _temp[strlen(_temp)] = (char)_savekbdStatus;
    _temp[strlen(_temp)] = '\0';
    // check temp array size  and keypad Screen so that I can decide it should scroll OR NOT
    if ( strlen(_temp) <= 13 ) {
      screenControl.charControl.charIdx = 0;
      screenControl.dashControl.dashIdx = strlen(_temp);
    }
    else if ( strlen(_temp) > 13 ) {
      screenControl.charControl.charIdx++;
    }
    // Show array content on screen
    tft.drawString( &_temp[screenControl.charControl.charIdx], screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
  }//end-else


  // check if we have reached at last character of keypad Screen
  // Now we need not to move our dash
  moveUrDash( screenControl.dashControl.dashIdx );
  delay(100);
}


uint8_t headerInput:: step_clr    (    ) {

  screenControl.dashControl.dashIdx = 0;
  screenControl.charControl.charIdx = 0;
  _savekbdStatus = 0;
  moveUrDash(  screenControl.dashControl.dashIdx );

  memset(_temp, '\0', sizeof(_temp) );
  memset(_temp, ' ', sizeof(_temp) - 1 );
  tft.drawString( _temp, screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
}


uint8_t headerInput:: step_del    (    ) {

  int len =  screenControl.dashControl.dashIdx + screenControl.charControl.charIdx + 1;

  if ( strlen(_temp) > 13 ) {
    if ( screenControl.dashControl.dashIdx >= 12 ) {

      if ( len >= strlen(_temp) ) {
        _temp[ strlen(_temp) - 1 ] = '\0';
        // Show array content on screen
        char arr[15];
        memset(arr , '\0', sizeof(arr));
        screenControl.charControl.charIdx = screenControl.charControl.charIdx <= 0 ? 0 : --screenControl.charControl.charIdx;
        strncpy(arr, &_temp[screenControl.charControl.charIdx], 13 );
        if ( strlen(arr) < 13 ) {
          char temp[15];
          memset(temp, '\0', sizeof(temp));
          memset(temp, ' ', sizeof(temp) - 1);
          tft.drawString(temp, screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
        }
        tft.drawString( arr, screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
      }
    }
    else if ( screenControl.dashControl.dashIdx < 12 ) {
HERE:

      len =  screenControl.dashControl.dashIdx + screenControl.charControl.charIdx + 1;
      if ( len < strlen(_temp) ) {

        char *arr = new char[ (strlen(_temp) - len) + 1];
        memset(arr, '\0', ( strlen(_temp) - len ) + 1 );
        strcpy(arr,  &_temp[len] );
        //Serial.printf("==>>%s, %d\n", arr, strlen( arr) );
        _temp[ screenControl.dashControl.dashIdx + screenControl.charControl.charIdx ] = '\0';
        strcat( _temp, arr );
        // Show array content on screen
        char arrTemp[15];
        memset(arrTemp, '\0', sizeof(arrTemp));
        screenControl.charControl.charIdx = screenControl.charControl.charIdx <= 0 ? 0 : --screenControl.charControl.charIdx;
        strncpy(arrTemp,  &_temp[screenControl.charControl.charIdx], 13);
        if ( strlen(arrTemp) < 13 ) {
          char temp[15];
          memset(temp, '\0', sizeof(temp));
          memset(temp, ' ', sizeof(temp) - 1);
          tft.drawString(temp, screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
        }
        tft.drawString( arrTemp, screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
        delete[] arr;
      }
    }

  }
  else if (  strlen(_temp) <= 13 ) {
    if ( screenControl.dashControl.dashIdx >= strlen(_temp) - 1 ) {

      _temp[strlen(_temp) - 1] = '\0';
      screenControl.dashControl.dashIdx = screenControl.dashControl.dashIdx <= 0 ? 0 : --screenControl.dashControl.dashIdx;
      moveUrDash( screenControl.dashControl.dashIdx );
      if ( strlen(_temp) < 13 ) {
        char temp[15];
        memset(temp, '\0', sizeof(temp));
        memset(temp, ' ', sizeof(temp) - 1);
        tft.drawString(temp, screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);
      }
      tft.drawString( _temp, screenControl.charControl.char_xAxis,  screenControl.charControl.char_yAxis, 1);

    }
    if ( screenControl.dashControl.dashIdx < strlen(_temp) ) {
      goto HERE;
    }
  }


  delay(50);
}//end-step_del


void  headerInput:: step_back   (    ) {
  step_clr();
  return;
}


void    headerInput:: step_sw     (    ) {
  switchFlag = switchFlag ? 0 : 1;
  userInput.switchFlag = switchFlag;
  noticBoard( TFT_GREEN, TFT_BLACK, userInput.switchFlag );
}



void headerInput:: step_edit() {
  //1. get touch cordinates to so that we can come here.
  Serial.printf("i am edit option.,...\n");
  // content must be available in array before edit something.
  if (sizeof(_temp) == 0) return;

  if ( sizeof(_temp) >= screenControl.dashControl.dashIdx ) {

    //     while(1){
    //
    //      yield();
    //     }
  }

  return;
}



void headerInput:: noticBoard( uint16_t FGColor, uint16_t BGColor, bool switchFlag ) {
  tft.setFreeFont( mplus_R8 );
  tft.setTextSize(1);
  tft.setTextColor(FGColor, BGColor );

  if ( switchFlag ) {
    tft.fillRect(0, 297, 240, 75, BGColor );
    tft.drawString( "* Alpha Numeric Mode ", 5, 297, 1);
  }
  else {
    tft.fillRect(0, 297, 240, 75, BGColor );
    tft.drawString( "* Numaric Mode ", 5, 297, 1);
  }

  tft.setFreeFont( mplus_R13 );
}
