
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
#include "SEPL328_keypad.h"

using namespace std;

#define ALPHANUM   0
#define NUM        1

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


class userKeyBoard: public  SEPL328_customKeypad
{
  private :
  bool fixformat;
  public :

    struct _fixFormat
    {
      bool fixFormatFlag;         // it must be True if you want to use fix format.
      char *fixFormatArray;       //
      int8_t fixFormatArraySize;  //
      int8_t len;
      /*
         This charcter must be present in fix format.
         i.e
         fixFormat : DD/MM/YY
         rolloverChar :  '/'
      */
      char *rollOverChar;
      /*
        Fix Format Type Fields :
        --------------------------
        1 :  Date
        2 :  Time
        3 :  Password
        4 :  stringType

      */
      int fieldType;
      /*
          Switch flag can be used to take input in two formats.
          1. Numaric Only
          2. Alpha-Numaric Only
      */
      bool switchFlag;

      /*
         Don't touch these variables untill you don't know proper underStanding of this library.
      */
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
    } userFixFormat;


    struct _userInput
    {
      char *userInputArray;       // User input array pointer. User responsibility.
      int8_t userInputArray_Size; // Size of Array in which user Want to take input
      const GFXfont* customFont;  // custom font pointer
      int invalidKey[11]; // If you user want to disable any keyboard key
      bool switchFlag; // switch flag which emphasis user to take inout in specified format.
      bool numericSwitchFlag;
      bool AlphaNumSwitchFlag;

      struct _fixFormat *fixFormatstruct; // it need to be deprecate in next few version...!!!!

      char fieldLable[20]; // What Text you want to display on window.
      /*
         These settings are main attributes to take data. Don't touch untill you don't know full working...!!!
      */
      uint8_t  entIdx = 15;
      uint16_t text_xAxis = 12;
      uint16_t text_yAxis = 15;
      uint16_t text_FGColor;
      uint16_t text_BGColor;
      uint16_t FieldLable_FGColor;
      uint16_t FieldLable_BGColor;
      uint16_t noticBoard_FGColor;
      uint16_t noticBoard_BGColor;

      void ( userKeyBoard :: *userInputParser)( struct _userInput * );
      void ( userKeyBoard :: *userInputStructureParamInit)(struct _userInput *);
      void ( userKeyBoard :: *userCustomKeypad)( struct _userInput * );

    } userInput;

    userKeyBoard( )
    {

    }

    /********************************************************************************/
    /* userInputKeyBoard.h */
    void init(  );
    int8_t _findkeypadTouch( uint16_t xAxis, uint16_t yAxis);
    void printCharArray( char printChar, int8_t idx );
    int  handleKeyPad(  );
    void takeUserInput(  char *argc );

    void keypadParamInit( );

    void keypadDataparser( );

    bool keypadImage();
    int _getCharacter( int keyvalue, bool switchFlag );

    void userInputFixFormat(  );
    void DrawfixFormatChar(char fixChar, int8_t idx );
    void moveUrDash( int8_t idx );
    void noticBoard( uint16_t FGColor, uint16_t BGColor, bool switchFlag );
    void printkeypadLable(  );

    /********************************************************************************/
    /* userGetFunctions.h */
    void userInputDate( char temp );
    char *getTime(  char *formatbuffer, int8_t len, char *rollOverChar  );
    char *getDate(  char *formatbuffer, int8_t len, char *rollOverChar );
    char *getStringType(  int8_t minLen, int8_t maxLen );
    char *getPassword(  int8_t minLen, int8_t maxLen );
    void configFormats( );



    /******************************/
    // Keypad button funcations
    uint8_t step_default(  char temp, uint8_t inc    );
    uint8_t step_clr    (  char temp, uint8_t inc    );
    uint8_t step_del    (  char temp, uint8_t inc    );
    void    step_back   (  char temp, uint8_t inc    );
    void    step_sw     (  char temp, uint8_t inc    );


    ~userKeyBoard( )
    {

    }

};


#endif
