
#ifndef __COMMONGLOBAL_H
#define __COMMONGLOBAL_H

#include "headerFiles.h"

/* DEBUG Window */

/********* MACROs Decalration ***********/
#define RXD1 32
#define TXD1 33
#define RXD2 16
#define TXD2 17

#define rgb565(r,g,b) { digitHandler->digitColor[0] = r;  digitHandler->digitColor[1] = g; digitHandler->digitColor[2] = b; }
#define findDotPosition( buf, dotpos) { while(buf[dotpos++] != '.'); dotpos--; }

/***** Defines *****/
#define basicImages(classX) {                               \
    classX.weighingMode();      \
    classX.batterySymbl();      \
    classX.stableSymbl(0);      \
    classX.zeroSymbl(0);        \
    classX.tareweightSymbl(0);  \
  }



#define Enable_Text_Font()  {                                                     \
    tft.setTextSize( 1 );                               \
    tft.setFreeFont( (const GFXfont *)EUROSTILE_B20 );  \
    tft.setTextColor(TFT_WHITE, TFT_BLACK);             \
  }


#define eliminateLeadingZeros(buf, dotpos, cnt) {         \
    for(uint8_t i=0; i < dotpos-1; i++)           \
    {                                             \
      if( buf[i] == '0')                        \
        ++cnt;                                    \
      else                                      \
      {                                         \
        break;                                  \
      }                                         \
    }                                             \
  }


#define bufferWithoutDot(dest, src)  {   \
    for(uint8_t i=0, j=0; i < 7; i++ )   \
    {                                    \
      if( src[i] != '.' )                \
        dest[j++] = src[i];              \
    }                                    \
  }



#define convertDoubleToCharArray(d_val, c_val) {                                                        \
    char temp[8];                                                              \
    dtostrf(d_val,7,3,temp);                                                    \
    memset(c_val, '0', 7);                                                      \
    uint8_t idx = 0;                                                            \
    findDotPosition(temp, idx);                                                 \
    idx = dotpos - idx;                                                         \
    strcpy( &c_val[idx], temp);                                                 \
    for(uint8_t i=0; i < 7; i++ ) { if( c_val[i] == 32 )  c_val[i] = 48;  }     \
    c_val[7] = '\0';                                                            \
  }


#define AdjustDot( buf ) {                                            \
    if( !(strstr(buf, ".") ) )                              \
    {                                                       \
      char temp[7] = {0};                                   \
      uint8_t dotcnt = 0;                                   \
      memset(temp, '0', 7);                                 \
      findDotPosition( buf, dotcnt );                       \
      strcpy( &temp[digitHandler->dotpos - dotcnt], buf );  \
      strcpy( buf, (const char *)temp);                     \
    }                                                       \
  }//End-AdjustDot




#define WQ                 0
#define TQ                 5
#define TareWeight         6
#define StopWeightData    12
#define StartWeightData   13
#define GetTareWeightData 14

#define Field_three_Touch  ( ( (xAxis > 300) && (xAxis < 480) ) && ( ( yAxis > 160 ) && ( yAxis < 240 ) ) )
#define Field_Two_Touch    ( ( (xAxis > 0) && (xAxis < 216) ) && ( ( yAxis > 160 ) && ( yAxis < 240 ) ) )
#define Taretouch_Auto     ( ( (xAxis > 405) && (xAxis < 480) ) && ( ( yAxis > 259 ) && ( yAxis < 313 ) ) )
#define Zerotouch          ( ( (xAxis > 300) && (xAxis < 385) ) && ( ( yAxis > 259 ) && ( yAxis < 313 ) ) )
#define ESC_Touch          ( ( (xAxis > 0) && (xAxis < 82) ) && ( ( yAxis > 252 ) && ( yAxis < 301 ) ) )


#define  makeCustomColor( red,  green , blue ) (((red & 0xf8)<<8) + ((green & 0xfc)<<3)+(blue>>3));

struct _touchHandler
{
  double *field_one_var;
  double *field_two_var;
  double *field_three_var;
  uint8_t refresh;
};

struct _modeAttri
{
  int Mode;
  uint8_t windowIndex;
  char *windowName;

};

struct _windowAttri
{

  const void *digitFontSize;
  const char *weightValue;
  uint16_t *xPosDigit;
  uint16_t yPosDigit;
  char preValue[8];
  uint8_t digitColor[3];// [0]=R, [1]=G, [2]=B
  uint16_t *xcirpos;
  uint16_t ycirpos;
  uint8_t rCir;
  uint8_t dotpos;
  uint8_t drawBlankRactangeFlag_1[6];
  uint8_t drawBlankRactangeFlag_2[6];
  uint8_t drawBlankRactangeFlag_3[6];

};

struct _imageAttri
{
  bool (*baseWindowImageOneImageOne)(void);
  bool (*modeHeader)(void);
  bool (*keypad)(void);
  bool (*stableUnstableFlag)(bool showHide);
  bool (*tareFlag)(bool showHide);
  bool (*zeroFlag)(bool showHide);
};

enum _machine
{
  keep_running = 0,
  WeighingMachine = 1,
  CountingMachine,
  PricingMachine,
  CheckWeighing,
  Settings,
  Go_Back
};

struct _setting
{
  char maxVal[9];
  char minVal[9];
  char model[9];
  char calibration[9];
  char decimal[9];
  char date[9];
  char timeVal[9];
  char capacity[9];
};




struct task_list
{
  char CmdName[16];
  char (*Task)(void);

};
char CmdWrite( struct task_list *CmdTaskHandler );

class WeighingHandle
{

    uint16_t xField_One_DigitPos[6] = {15, 88, 166, 244, 322, 400};
    uint16_t xField_One_CirPos[5] = {72, 151, 228, 308, 387 };
    uint16_t yField_One_DigitPos = 130;
    uint16_t yField_One_CirPos = 140;

    uint16_t xField_two_digitPos[6] = {12, 48, 85, 120, 158, 196};
    uint16_t xField_two_CirPos[5] = {40, 75, 116, 152, 190};
    uint16_t yField_two_DigitPos = 212;
    uint16_t yField_two_CirPos = 214;

    uint16_t xField_three_digitPos[6] = {255, 291, 327, 365, 396, 435};
    uint16_t xField_three_CirPos[5] = {284, 321, 359, 392, 429};
    uint16_t yField_three_DigitPos = 212;
    uint16_t yField_three_CirPos = 214;


  public :
    uint8_t flags;
    
  struct _showDigits
  {
    int8_t digitFontSize;
    const void *digitFontStyle;
    char preValue[10];
    char currentValue[10];
    int8_t dotPosition;
    uint16_t fontDigitColor;
    uint16_t backDigitColor;
    int8_t cntBlankRect;
  }showDigits;
  
    struct _digithandler
    {
      int Mode;
      enum _machine machineMode;
      uint8_t windowIndex;
      char *windowName;

      const void *digitFontSize;
      const char *weightValue;
      uint16_t *xPosDigit;
      uint16_t yPosDigit;
      char preValue[8];
      uint8_t digitColor[3];// [0]=R, [1]=G, [2]=B
      uint16_t *xcirpos;
      uint16_t ycirpos;
      uint8_t rCir;
      uint8_t dotpos;
      bool (*blankRactange)(uint16_t idx);

      uint8_t drawBlankRactangeFlag_1[6];
      uint8_t drawBlankRactangeFlag_2[6];
      uint8_t drawBlankRactangeFlag_3[6];

      struct _setting machineSetting;
      struct _modeAttri modeAttributes;
      struct _windowAttri windowAttributes;
      struct _touchHandler touchHandler;
      struct _imageAttri imageCallback;
      struct s_setting *basesetting;

      uint8_t *drawBlankRactangeFlag;

      struct _showDigits showDigits;
    } handler;


    /* Funcation decalration */
    String _readbuf( );
    void    initTFTHandler(  );
    void initWeighingTFT( );
  
    /* Basic images required for all 4 modes */
    bool weighingMode();
    bool BootingImage();
    bool startUPImage();
    bool numKeypad();
    bool tareweightSymbl(uint8_t flag);
    bool batterySymbl();
    bool zeroSymbl(uint8_t flag);
    bool stableSymbl(uint8_t flag);
    bool cross(uint16_t x, uint16_t y);

    // Test Funcations
    void Testfun( );

};


char nullfunc();


#define x1Zero  343
#define x2Zero  415
#define y1Zero  240
#define y2Zero  304

#define xZero  ( ( xAxis >= x1Zero ) && (xAxis <= x2Zero ) )
#define yZero ( ( yAxis >= y1Zero  ) && ( yAxis <= y2Zero ) )
/**********************************************************************************************/

#define x1One  255
#define x2One  320
#define y1One  150
#define y2One  210

#define xOne  ( ( xAxis >= x1One ) && ( xAxis <= x2One ) )
#define yOne  ( ( yAxis >= y1One  ) && ( yAxis <= y2One ) )
/**********************************************************************************************/

#define x1Two  345
#define x2Two  415
#define y1Two  150
#define y2Two  210

#define xTwo  ( ( xAxis >= x1Two ) && (xAxis <= x2Two ) )
#define yTwo  ( ( yAxis >= y1Two  ) && ( yAxis <= y2Two ) )
/**********************************************************************************************/

#define x1Three  429
#define x2Three  480
#define y1Three  150
#define y2Three  210

#define xThree  ( ( xAxis >= x1Three ) && (xAxis <= x2Three ) )
#define yThree ( ( yAxis >= y1Three  ) && ( yAxis <= y2Three ) )
/**********************************************************************************************/

#define x1Four  255
#define x2Four  320
#define y1Four  70
#define y2Four  120

#define xFour  ( ( xAxis >= x1Four ) && (xAxis <= x2Four ) )
#define yFour ( ( yAxis >= y1Four  ) && ( yAxis <= y2Four ) )
/**********************************************************************************************/

#define x1Five  345
#define x2Five  415
#define y1Five  70
#define y2Five  120

#define xFive  ( ( xAxis >= x1Five ) && (xAxis <= x2Five ) )
#define yFive ( ( yAxis >= y1Five  ) && ( yAxis <= y2Five ) )
/**********************************************************************************************/

#define x1Six  429
#define x2Six  480
#define y1Six  70
#define y2Six  120

#define xSix  ( ( xAxis >= x1Six ) && (xAxis <= x2Six ) )
#define ySix ( ( yAxis >= y1Six  ) && ( yAxis <= y2Six ) )
/**********************************************************************************************/


#define x1Seven  255
#define x2Seven  320
#define y1Seven  0
#define y2Seven  50

#define xSeven  ( ( xAxis >= x1Seven ) && (xAxis <= x2Seven ) )
#define ySeven ( ( yAxis >= y1Seven  ) && ( yAxis <= y2Seven ) )
/**********************************************************************************************/


#define x1Eight  345
#define x2Eight  415
#define y1Eight  0
#define y2Eight  50

#define xEight  ( ( xAxis >= x1Eight ) && (xAxis <= x2Eight ) )
#define yEight ( ( yAxis >= y1Eight  ) && ( yAxis <= y2Eight ) )
/**********************************************************************************************/


#define x1Nine  429
#define x2Nine  480
#define y1Nine  0
#define y2Nine  50

#define xNine ( ( xAxis >= x1Nine ) && (xAxis <= x2Nine ) )
#define yNine ( ( yAxis >= y1Nine  ) && ( yAxis <= y2Nine ) )
/**********************************************************************************************/

#define x1Dot  255
#define x2Dot  320
#define y1Dot  240
#define y2Dot  303

#define xDot ( ( xAxis >= x1Dot  ) && ( xAxis <= x2Dot ) )
#define yDot ( ( yAxis >= y1Dot  ) && ( yAxis <= y2Dot ) )
/******************************************************************************/

#define x1Del  165
#define x2Del  240
#define y1Del  240
#define y2Del  304

#define xDel ( ( xAxis >= x1Del  ) && ( xAxis <= x2Del ) )
#define yDel ( ( yAxis >= y1Del  ) && ( yAxis <= y2Del ) )
/******************************************************************************/

#define x1Clear  165
#define x2Clear  240
#define y1Clear  150
#define y2Clear  210

#define xClear ( ( xAxis >= x1Clear  ) && ( xAxis <= x2Clear ) )
#define yClear ( ( yAxis >= y1Clear  ) && ( yAxis <= y2Clear ) )
/******************************************************************************/

#define x1Ent  429
#define x2Ent  480
#define y1Ent  240
#define y2Ent  304

#define xEnt ( ( xAxis >= x1Ent  ) && ( xAxis <= x2Ent ) )
#define yEnt ( ( yAxis >= y1Ent  ) && ( yAxis <= y2Ent ) )
/******************************************************************************/

#define x1Del  255
#define x2Del  320
#define y1Del  240
#define y2Del  303

#define xDel ( ( xAxis >= x1Del  ) && ( xAxis <= x2Del ) )
#define yDel ( ( yAxis >= y1Del  ) && ( yAxis <= y2Del ) )
/******************************************************************************/



#endif
