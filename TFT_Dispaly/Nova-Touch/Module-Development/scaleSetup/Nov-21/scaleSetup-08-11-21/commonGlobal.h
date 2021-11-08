
#ifndef __COMMONGLOBAL_H
#define __COMMONGLOBAL_H

#include "headerFiles.h"


#include <FS.h>
#include <SPIFFS.h>

extern class WeighingHandle Wtft;

/********* MACROs Decalration ***********/
#define CS_PIN  26
// MOSI=11, MISO=12, SCK=13

// Please use proper RTC
#define DS1307_RTC 0
#define R8025_RTC 1

enum{
  BAUD_9600 = 9600,
  BAUD_115200 = 115200,
};


// Serial_1 will be used for getting data from Arduino NANO for Keypad data.
//#define RXD1 32
//#define TXD1 33


// Serial_2 will be used for getting Weighing Scale data from machine.
#define RXD2 16
#define TXD2 17

#define rgb565(r,g,b) { digitHandler->digitColor[0] = r;  digitHandler->digitColor[1] = g; digitHandler->digitColor[2] = b; }
#define findDotPosition( buf, dotpos) { while(buf[dotpos++] != '.'); dotpos--; }

/*************************** Defines ***************************/
#define basicImages(classX) {   \
    classX.weighingMode();      \
    classX.batterySymbl(LOWER);      \
    classX.stableSymbl(0);      \
    classX.zeroSymbl(0);        \
    classX.tareweightSymbl(0);  \
  }

#define  parshDateformat( dest, src ) { \
    if ( src[1] == '/' ) {    \
      dest[0] = '0';          \
      dest[1] = src[0];       \
      dest[2] = '/';          \
      \
      if ( src[3] == '/' ) {  \
        dest[3] = '0';          \
        dest[4] = src[2];       \
        dest[5] = src[3];       \
        dest[6] = src[6];       \
        dest[7] = src[7];       \
        dest[8] = '\0';         \
      }else  if ( src[4] == '/' ){\
        dest[3] = src[2];       \
        dest[4] = src[3];       \
        dest[5] = src[4];       \
        dest[6] = src[7];       \
        dest[7] = src[8];       \
        dest[8] = '\0';         \
      }                         \
    }else  if ( src[2] == '/' ) {\
      dest[0] =  src[0];      \
      dest[1] = src[1];       \
      dest[2] = '/';          \
      \
      if( src[4] == '/' ){    \
        dest[3] = '0';          \
        dest[4] = src[3];       \
        dest[5] = src[4];       \
        dest[6] = src[7];       \
        dest[7] = src[8];       \
        dest[8] = '\0';         \
      }else if( src[5] == '/' )\
      {                       \
        dest[3] = src[3];       \
        dest[4] = src[4];       \
        dest[5] = src[5];       \
        dest[6] = src[8];       \
        dest[7] = src[9];       \
        dest[8] = '\0';         \
      }                       \
    }                         \
  }

#define parshTimeFormat(dest, src) {  \
    if ( src[1] == ':' ) {                \
      dest[0] = '0';                    \
      dest[1] = src[0];                 \
      dest[2] = ':';                    \
    }else if( src[2] == ':' )           \
    {                                   \
      dest[0] = src[0];                 \
      dest[1] = src[1];                 \
      dest[2] = ':';                    \
    }                                   \
    \
    if ( src[3] == ':' ) {              \
      dest[3] = '0';                    \
      dest[4] = src[2];                 \
      dest[5] = '\0';                   \
    }else  if ( src[4] == ':' ){        \
      dest[3] = src[2];                 \
      dest[4] = src[3];                 \
      dest[5] = '\0';                   \
    }else if( src[5] == ':' ){          \
      dest[3] = src[3];                 \
      dest[4] = src[4];                 \
      dest[5] = '\0';                   \
    }                                   \
  }


#define Enable_Text_Font()  {                           \
    tft.setTextSize( 1 );                               \
    tft.setFreeFont( (const GFXfont *)EUROSTILE_B20 );  \
    tft.setTextColor(TFT_WHITE, TFT_BLACK);             \
  }


#define eliminateLeadingZeros(buf, dotpos, cnt) {         \
    for(uint8_t i=0; i < dotpos-1; i++)                   \
    {                                                     \
      if( buf[i] == '0')                                  \
        ++cnt;                                            \
      else                                                \
      {                                                   \
        break;                                            \
      }                                                   \
    }                                                     \
    \
  }

//dest and src must be differnt arrays
#define removeDotfromArray(dest, src )  for( uint8_t i=0; dest[i]; dest[i] =  src[i] != '.'? src[i++]: src[++i] );
#define removeSpecialCharfromArray(dest, src, specialChar) for( uint8_t i=0; dest[i]; dest[i] =  src[i] != specialChar ? src[i++]: src[++i] );


#define convertDoubleToCharArray(d_val, c_val) {                                \
    char temp[8];                                                               \
    dtostrf(d_val,7,3,temp);                                                    \
    memset(c_val, '0', 7);                                                      \
    uint8_t idx = 0;                                                            \
    findDotPosition(temp, idx);                                                 \
    idx = dotpos - idx;                                                         \
    strcpy( &c_val[idx], temp);                                                 \
    for(uint8_t i=0; i < 7; i++ ) { if( c_val[i] == 32 )  c_val[i] = 48;  }     \
    c_val[7] = '\0';                                                            \
  }


#define AdjustDot( buf ) {                                  \
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

/*  ==============================================================
                  Commands
    =============================================================

    1 @ CMD_STARTDATA :
    --------------------
        >> get continuous data from Machine.

    2 @ CMD_STOPDATA :
    -------------------
        >>  stop data coming from Weight machine
        >>   stop continuous data from machine SO that we didnot overflow buffer.

    3 @ CMD_GETTARE :
    -----------------
        >> get TARE data from Machine
*/

#define CMD_STARTDATA   {           \
    String StartCMD = "\2DT#1\3";   \
    Serial2.print( StartCMD );      \
  }

#define CMD_STOPDATA {            \
    String StopCMD = "\2DT#0\3";  \
    Serial2.print(StopCMD);       \
  }
#define CMD_ZERODATA {         \
    String StopCMD = "\2Z\3";  \
    Serial2.print(StopCMD);    \
  }

#define CMD_AUTOTARE {         \
    String StopCMD = "\2T\3";  \
    Serial2.print(StopCMD);    \
  }

#define CMD_GETTARE {              \
    String StartCMD = "\2GT\3";    \
    Serial2.print( StartCMD );     \
  }

#define STOP_SERIAL1   Serial1.end();
#define START_SERIAL1  Serial1.begin(BAUD_9600, SERIAL_8N1, RXD1, TXD1); delay(100); // keypad
#define STOP_SERIAL2   Serial2.end();
#define START_SERIAL2  Serial2.begin(BAUD_9600, SERIAL_8N1, RXD2, TXD2); delay(100);// Weight Mechine


#define EMPTY_SERIALBUFFER {                        \
    while ( Serial2.available() > 0 )  Serial2.read();  \
    while ( Serial1.available() > 0  ) Serial1.read();  \
    while ( Serial.available()  > 0 )  Serial.read();   \
  }


#define WQ                 0
#define TQ                 5
#define TareWeight         6
#define StopWeightData    12
#define StartWeightData   13
#define GetTareWeightData 14

#define Field_One_Touch    ( ( (xAxis > 28) && (xAxis < 470) ) && ( ( yAxis > 16 ) && ( yAxis < 142 ) ) )
#define Field_three_Touch  ( ( (xAxis > 300) && (xAxis < 480) ) && ( ( yAxis > 160 ) && ( yAxis < 240 ) ) )
#define Field_Two_Touch    ( ( (xAxis > 0  ) && (xAxis < 216) ) && ( ( yAxis > 160 ) && ( yAxis < 240 ) ) )
#define Taretouch_Auto     ( ( (xAxis > 405) && (xAxis < 480) ) && ( ( yAxis > 259 ) && ( yAxis < 313 ) ) )
#define Zerotouch          ( ( (xAxis > 300) && (xAxis < 385) ) && ( ( yAxis > 259 ) && ( yAxis < 313 ) ) )
#define ESC_Touch          ( ( (xAxis > 0  ) && (xAxis < 85 ) ) && ( ( yAxis > 249 ) && ( yAxis < 320 ) ) )
#define PRT_Touch          ( ( (xAxis > 205  ) && (xAxis < 280 ) ) && ( ( yAxis > 267 ) && ( yAxis < 320 ) ) )


#define  makeCustomColor( red,  green , blue ) (((red & 0xf8)<<8) + ((green & 0xfc)<<3)+(blue>>3))


/********************************************************************************************
   Below MACROs are used to identify The Touch of STARTUP window.
   User has few options Which are Listed below.
   @ Weight Mode          : After Touching This option, User is allowed to use functionality of Weighing Mechine.
   @ Pricing Mode         : After Touching This option, User is allowed to use functionality of Pricing Mechine.
   @ Counting Mode        : After Touching This option, User is allowed to use functionality of Counting Mechine.
   @ Check Weighing Mode  : After Touching This option, User is allowed to use functionality of Check Weighing Mechine.
   @ Setting Mode         : Setting Mode Allowed to User to change machine functionality as per Requirements.
                            After machine is configured by user. Next user can choose among 4 Modes.
   @ Go Back -->>> Which is not Implimented Yet.

 *********************************************************************************************/

#define x1Weighing  15
#define x2Weighing  108
#define y1Weighing  70
#define y2Weighing  230

#define xWeighingMode  ( ( xAxis >= x1Weighing ) && (xAxis <= x2Weighing ) )
#define yWeighingMode  ( ( yAxis >= y1Weighing  ) && ( yAxis <= y2Weighing ) )
#define WeighingModeTouchEnable() ( xWeighingMode && yWeighingMode )
//#define WeighingModeTouchEnable() ( ( (xAxis > 15) && (xAxis < 108) ) && ( ( yAxis > 70 ) && ( yAxis < 230 ) ) )

/**********************************************************************************************/

#define x1Counting  135
#define x2Counting  222
#define y1Counting  70
#define y2Counting  230

#define xCountingMode  ( ( xAxis >= x1Counting  ) && ( xAxis <= x2Counting ) )
#define yCountingMode  ( ( yAxis >= y1Counting  ) && ( yAxis <= y2Counting ) )
#define CountingModeTouchEnable() ( xCountingMode && yCountingMode )
/**********************************************************************************************/

#define x1Priceing 251
#define x2Priceing 345
#define y1Priceing 70
#define y2Priceing 230

#define xPriceingMode  ( ( xAxis >= x1Priceing  ) && ( xAxis <= x2Priceing ) )
#define yPriceingMode  ( ( yAxis >= y1Priceing  ) && ( yAxis <= y2Priceing ) )
#define PriceingModeTouchEnable() ( xPriceingMode && yPriceingMode )
/**********************************************************************************************/

#define x1Checking  377
#define x2Checking  470
#define y1Checking  70
#define y2Checking  230

#define xCheckingMode  ( ( xAxis >= x1Checking  ) && ( xAxis <= x2Checking ) )
#define yCheckingMode  ( ( yAxis >= y1Checking  ) && ( yAxis <= y2Checking ) )
#define CheckingModeTouchEnable() ( xCheckingMode && yCheckingMode )
/**********************************************************************************************/

#define x1Setting 13
#define x2Setting 270
#define y1Setting 252
#define y2Setting 303

#define xSetting  ( ( xAxis >= x1Setting  ) && ( xAxis <= x2Setting ) )
#define ySetting  ( ( yAxis >= y1Setting  ) && ( yAxis <= y2Setting ) )
#define SettingTouchEnable() ( xSetting && ySetting )
/**********************************************************************************************/

#define x1GoBack 258
#define x2GoBack 460
#define y1GoBack 252
#define y2GoBack 303

#define xGoBack ( ( xAxis >= x1GoBack  ) && ( xAxis <= x2GoBack ) )
#define yGoBack ( ( yAxis >= y1GoBack  ) && ( yAxis <= y2GoBack ) )
/**********************************************************************************************/


//enum _machine
//{
//  keep_running = 0,
//  WeighingMachine = 1,
//  CountingMachine,
//  PricingMachine,
//  CheckWeighing,
//  Settings,
//  Go_Back
//};

typedef enum e_popupButton{
  Xesc = 7,
  Xunit = 101,
  Xprint = 193,
  Xtare = 378,
  Xzero = 285,
  Ybutton = 228,
/**************************/  
  WeightMode  = 1,
  CountMode   = 2,
  PriceMode   = 3,
  CheckMode   = 4,
  SettingMode = 5,
};


struct task_list{
  char CmdName[16];
  char (*Task)(void);

};
char CmdWrite( struct task_list *CmdTaskHandler );


#define PRICE  1
#define perPCS 2

/*
enum {
  map_CmdESC   = 16,
  map_CmdUnits = 17,
  map_CmdPrint = 18,
  map_CmdZero  = 19,
  map_CmdTare  = 20  
}_touchButton;

*/


#define map_CmdTare   20
#define map_CmdZero   19
#define map_CmdPrint  18
#define map_CmdUnits  17
#define map_CmdESC    16

#define open_winOne   11
#define open_winTwo   12
#define open_winThree 13
#define open_tareWin       open_winThree
#define open_unitWeightWin open_winTwo
#define open_countWin      open_winOne
#define open_minWin        open_winThree
#define open_maxWin        open_winTwo
#define open_unitPrice     open_winTwo

/*******************************************************************************
 * 
 * 
 *******************************************************************************/
typedef enum e_batterySymbol {
  LOWER,
  UPPER
};


class WeighingHandle : public settings {

  private :
    


  public :
    uint8_t flags;
    double FromMachine[3];
    char FromMachineArray[3][10];
    char maxvalue[10];
    char minvalue[10];
    static char preTime[10];
   
    
    struct _imageAttri{
      bool (*ModeHeader)(void);
      bool (*stableUnstableFlag)(bool showHide);
      bool (*tareFlag)(bool showHide);
      bool (*zeroFlag)(bool showHide);
    };

    struct _showDigits{
      int8_t selectWindow;
      int8_t digitFontSize;
      const void *digitFontStyle;
      char preValue[3][10];
      char currentValue[10];
      uint8_t dotPosition;
      uint16_t fontDigitColor;
      uint16_t backDigitColor;
      int8_t cntBlankRect;
      bool spclFlag = 0;
      uint16_t showDigitColorWindowOne = TFT_GREEN;
      uint16_t showDigitColorWindowTwo = TFT_GREEN;
      uint16_t showDigitColorWindowThree = TFT_GREEN;
    } showDigits;


    struct _digithandler{
//      enum _machine machineMode;
      struct _imageAttri imageCallback;
      struct s_setting *basesetting;
    } handler;

    struct rtcvar {
      uint8_t hour;
      uint8_t minute;
      uint8_t second;

      uint8_t mday;
      uint8_t mon;
      uint16_t year;
      uint8_t wday;

    } RTCHandler;


    void drawJpeg(const char *filename, int xpos, int ypos);
    void  jpegRender(int xpos, int ypos);
    void jpegInfo();


    /******************************************
          >>>> common function <<<<
     ******************************************/
    void    initTFTHandler(  );
    void    initWeighingTFT( );

    String  handleFlags( char *Weight  );
    void  windowOne(  );
    void _windowOne(char *preValue, char *currentValue);
    void  windowOne(int8_t tempDotPos );
    
    void  windowTwo( );
    void _windowTwo(char *preValue, char *currentValue);    
    void  windowTwo(int8_t tempDotPos );
    
    void  windowThree( );
    void _windowThree(char *preValue, char *currentValue);  
    void  windowThree(int8_t tempDotPos );
    
    /* Basic images required for all 4 Modes */
    bool weighingMode();
    bool BootingImage();
    bool startUPImage();
    bool numKeypad();

    bool tareweightSymbl(uint8_t flag);
    bool batterySymbl(e_batterySymbol Mode);
    bool zeroSymbl(uint8_t flag);
    bool stableSymbl(uint8_t flag);

    bool cross(uint16_t x, uint16_t y);
    void readyAxisScales();

    /******************************************
              >>>> Weight function <<<<
    ******************************************/
    int8_t startWeighing();
    String _readbufWeight( );
    void _updateTareWeight(  char *Temp );
    void _updateGrossWeight(  );
    void _updateNetWeight( char *Temp );
    void _updateWindow( uint8_t win );
    void handleTareCommand( char *Weight  );
   
    int8_t  handleTouchFuncationality_Weight();
   
    /******************************************
                >>>> Price computing function <<<<
      ******************************************/

    int8_t startPriceComputing(); 
   
    /******************************************
      >>>> COUNT computing function <<<<
     ******************************************/

    int8_t   startCountComputing();
    void  updateTotalPcsWindow();
  

    /******************************************
      >>>> Check Weighing function <<<<
     ******************************************/
    int8_t   startCheckWeighing();
   

    /*******************************************
                >>>>  RTC Module  <<<<<
     *******************************************/
     void  DateNTimeHandler();

     bool checkTouch(uint8_t touchMode );



     void popupButton(e_popupButton x, e_popupButton y,  uint16_t lineColor, uint8_t thickness=4);
     void popupButton( int8_t Mode, uint16_t lineColor=TFT_WHITE, uint8_t thickness=4);
     void batteryAnimation(int blevel);
};

/*******************************************************************************
 * 
 *******************************************************************************/

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
