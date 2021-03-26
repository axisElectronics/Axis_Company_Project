
#ifndef __COMMONGLOBAL_H
#define __COMMONGLOBAL_H

#include "headerFiles.h"

#include <FS.h>
#include <SPIFFS.h>

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
    }                                           \
    \
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

/*  ==============================================================
 *                Commands 
 *  =============================================================
 *  
 *  1 @ CMD_STARTDATA :
 *  --------------------
 *      >> get continuous data from Machine.
 *      
 *  2 @ CMD_STOPDATA :    
 *  -------------------
 *      >>  stop data coming from Weight machine
 *      >>   stop continuous data from machine SO that we didnot overflow buffer.
 *      
 *  3 @ CMD_GETTARE :
 *  -----------------
 *      >> get TARE data from Machine
 */

#define CMD_STARTDATA   {                                 \
                          String StartCMD = "\2DT#1\3";   \
                          Serial2.print( StartCMD );      \
                        }

#define CMD_STOPDATA {                              \
                      String StopCMD = "\2DT#0\3";  \
                      Serial2.print(StopCMD);       \
                    }


#define CMD_GETTARE {                               \
                      String StartCMD = "\2GT\3";    \
                      Serial2.print( StartCMD );     \
                    }


#define EMPTY_SERIALBUFFER {                                                \
                            while ( Serial2.available() )  Serial2.read();  \ 
                            while ( Serial1.available() )  Serial1.read();  \
                            while ( Serial.available() )   Serial.read();   \
                           }


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
#define ESC_Touch          ( ( (xAxis > 0) && (xAxis < 85) ) && ( ( yAxis > 259 ) && ( yAxis < 313 ) ) )


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


struct task_list
{
  char CmdName[16];
  char (*Task)(void);

};
char CmdWrite( struct task_list *CmdTaskHandler );

#define GROSS 0
#define NET   1
#define TARE  2


#define PRICE  1
#define perPCS 2

#define COUNT 1

class WeighingHandle : public settings

{

  public :
    uint8_t flags;
    double FromMachine[3];
    char FromMachineArray[3][10];
   
    
    struct _imageAttri
    {
      bool (*ModeHeader)(void);
      bool (*stableUnstableFlag)(bool showHide);
      bool (*tareFlag)(bool showHide);
      bool (*zeroFlag)(bool showHide);
    };

    struct _showDigits
    {
      int8_t selectWindow;
      int8_t digitFontSize;
      const void *digitFontStyle;
      char preValue[3][10];
      char currentValue[10];
      uint8_t dotPosition;
      uint16_t fontDigitColor;
      uint16_t backDigitColor;
      int8_t cntBlankRect;
      bool spclFlag=0;
    } showDigits;


    struct _digithandler
    {
      enum _machine machineMode;
      struct _imageAttri imageCallback;
      struct s_setting *basesetting;
    } handler;

void drawJpeg(const char *filename, int xpos, int ypos);
void  jpegRender(int xpos, int ypos);
void jpegInfo();
    /******************************************
          >>>> common function <<<<
     ******************************************/
    void    initTFTHandler(  );
    void    initWeighingTFT( );

    String  handleFlags( char *Weight  );
    void    windowOne( );
    void    windowTwo( );
    void    windowThree( );

    /* Basic images required for all 4 Modes */
    bool weighingMode();
    bool BootingImage();
    bool startUPImage();
    bool numKeypad();

    bool tareweightSymbl(uint8_t flag);
    bool batterySymbl();
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
    bool printStringWeight( );
    int8_t  handleTouchFuncationality_Weight();
    bool weightStripImage();
    
    /******************************************
                >>>> Price computing function <<<<
      ******************************************/

    int8_t startPriceComputing();
    void _updateWindowPricing( uint8_t win );
    void _updateTotalWeight( char *Temp );
    void _updateWeightPrice();
    void _updateWeightperPrice( char *Temp );
    int8_t handleTouchFuncationality_PRICE();
    String _readbufPrice( );
    bool printStringPrice( );
    bool priceStripImage();
    /******************************************
      >>>> COUNT computing function <<<<
     ******************************************/

    int8_t   startCountComputing();
    void   _updateWindowCOUNT( uint8_t win );
    void   _updateTotalWeightCOUNT( char *Temp );
    void   _updateWeightCOUNT();
    void   _updateWeightperCOUNT( char *Temp );
    int8_t   handleTouchFuncationality_COUNT();
    String _readbufCOUNT( );
    bool printStringCOUNT( );
    bool countStripImage();
    /******************************************
      >>>> Check Weighing function <<<<
     ******************************************/
     int8_t   startCheckWeighing();
     void   _updateWindowCHECK( uint8_t win );
     int8_t   handleTouchFuncationality_CHECK();
     String _readbufCHECK( );
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
