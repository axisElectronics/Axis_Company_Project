
#ifndef _UserInputKeyBoard_H
#define _UserInputKeyBoard_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <SPI.h>
#include <TFT_eSPI.h>
#include <String.h>
#include <vector>

#include "mplus_1m_regular13pt7b.h" 
#include "mplus_1m_regular8pt7b.h"


using namespace std; 

#define mplus_R13   &mplus_1m_regular13pt7b 
#define mplus_R8   &mplus_1m_regular8pt7b

struct _userInput
  {
    char *userInputArray;
    int8_t userInputArray_Size;
    const GFXfont* customFont;
    uint16_t text_xAxis;
    uint16_t text_yAxis;
    uint16_t text_FGcolor;
    uint16_t text_BGColor;
    uint8_t  entIdx;
    char (*userInputParser)(char *);
    void (*userInputStructureParamInit)(struct _userInput *);
    
  };

enum touchKey
{
  keepRunning = -20,
  sw = -6,
  back,
  del,
  clr,
  dot,
  ent,
  zero = 0,
  one,
  two,
  three,
  four,
  five,
  six,
  seven,
  eight,
  nine,
};

enum touchKey _findkeypadTouch( uint16_t xAxis, uint16_t yAxis);
void printCharArray( char printChar, int8_t idx );
int handleKeyPad();
void takeUserInput( struct _userInput *userInput );
void keypadParamInit( struct _userInput *userInput );
void userInputParsher(  struct _userInput *userInput );
bool keypadImage();
int _getCharacter( int keyvalue, bool switchFlag );


class userKeyBoard
{
  public :
  userKeyBoard( );
  ~userKeyBoard( );

};


#endif
