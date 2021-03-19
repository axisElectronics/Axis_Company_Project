
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

struct _fixFormat
{
    char fixFormatArray[10];
    int8_t fixFormatArraySize;
    bool fixFormatFlag;
    char rollOverChar;

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

  int8_t idx;// choose any among 17 places

  
};


struct _userInput
  {
    char *userInputArray;
    int8_t userInputArray_Size;
    const GFXfont* customFont;
    int invalidKey[11];
    
   struct _fixFormat *fixFormatstruct;
    
    char fieldLable[20];
    uint16_t text_xAxis;
    uint16_t text_yAxis;
    uint16_t text_FGColor;
    uint16_t text_BGColor;
    uint16_t FieldLable_FGColor;
    uint16_t FieldLable_BGColor;
    uint16_t noticBoard_FGColor;
    uint16_t noticBoard_BGColor;
    uint8_t  entIdx;
    void (*userInputParser)( struct _userInput * );
    void (*userInputStructureParamInit)(struct _userInput *);
    void (*userCustomKeypad)( struct _userInput * );
    
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
int  handleKeyPad( struct _userInput *userInput );
void takeUserInput( struct _userInput *userInput );
void keypadParamInit( struct _userInput *userInput );
void keypadDataparser(  struct _userInput *userInput );
bool keypadImage();
int _getCharacter( int keyvalue, bool switchFlag, struct _userInput *userInput );
void customKeypad( struct _userInput *userInput );
void userInputFixFormat( struct _userInput *userInput );
void DrawfixFormatChar(char fixChar, int8_t idx,  struct _userInput *userInput);
void moveUrDash( int8_t idx,  struct _userInput *userInput );



class userKeyBoard
{
  public :
  userKeyBoard( );
  ~userKeyBoard( );

};


#endif
