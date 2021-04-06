
#ifndef _UserInputKeyBoard_H
#define _UserInputKeyBoard_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <SPI.h>
#include <TFT_eSPI.h>
#include <String.h>
#include <vector>

#include "customFontHeaders.h"

using namespace std; 



#define Date       1
#define Time       2
#define Password   3
#define stringType 4

#define fixFormatFullBlankScreen    for(  int8_t idx = 0; idx < 17; idx++ )\
                                           tft.fillRect(15 + (13*idx), 25, 13, 22, TFT_BLACK); 



  
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


class userKeyBoard
{
  public :

struct _fixFormat
{
    char *fixFormatArray;
    int8_t fixFormatArraySize;
    int8_t len;
    bool fixFormatFlag;
    char *rollOverChar;
    int fieldType;
    bool switchFlag;
    int8_t minLen;
    int8_t maxLen;
    int maxLengthScreen = 17;
  
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

  char* (*getCallBackFunc)( struct _userInput *userInput, struct _fixFormat *userFixFormat, char *formatbuffer, int8_t len, char *rollOverChar  );//call back function.
}userFixFormat;


struct _userInput
  {
    char *userInputArray;
    int8_t userInputArray_Size;
    const GFXfont* customFont;
    int invalidKey[11];
    bool switchFlag;
    bool numericSwitchFlag;
    bool AlphaNumSwitchFlag;
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
    void ( userKeyBoard :: *userInputParser)( struct _userInput * );
    void ( userKeyBoard :: *userInputStructureParamInit)(struct _userInput *);
    void ( userKeyBoard :: *userCustomKeypad)( struct _userInput * );
    
  }userInput;

  userKeyBoard( )
  {
    
  }

  /********************************************************************************/
/* userInputKeyBoard.h */
void init( struct _userInput *userInput );
int8_t _findkeypadTouch( uint16_t xAxis, uint16_t yAxis);
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
void noticBoard( uint16_t FGColor, uint16_t BGColor, bool switchFlag );
void printkeypadLable( struct _userInput *userInput );

/********************************************************************************/
/* userGetFunctions.h */
void userInputDate(  struct _userInput *userInput , char temp );
char *getTime( struct _userInput *userInput, struct _fixFormat *userFixFormat, char *formatbuffer, int8_t len, char *rollOverChar  );
char *getDate( struct _userInput *userInput, struct _fixFormat *userFixFormat, char *formatbuffer, int8_t len, char *rollOverChar );
char *getStringType( struct _userInput *userInput, struct _fixFormat *userFixFormat, int8_t minLen, int8_t maxLen );
char *getPassword( struct _userInput *userInput, struct _fixFormat *userFixFormat, int8_t minLen, int8_t maxLen );
void configFormats(struct _userInput *userInput );



/******************************/
uint8_t step_default( struct _userInput *userInput, char temp, uint8_t inc );
uint8_t step_clr( struct _userInput *userInput, char temp, uint8_t inc );
uint8_t step_del( struct _userInput *userInput, char temp, uint8_t inc );
void step_back( struct _userInput *userInput, char temp, uint8_t inc );
void step_sw( struct _userInput *userInput, char temp, uint8_t inc );


  ~userKeyBoard( )
  {
    
  }

};


#endif
