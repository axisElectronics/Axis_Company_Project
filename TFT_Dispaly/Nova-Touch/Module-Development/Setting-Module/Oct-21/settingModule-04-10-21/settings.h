
#ifndef SETTING_H
#define SETTING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <String.h>
#include <vector>

using namespace std;

#include "customImages.h"
#include "customFontHeaders.h"
#include "UserInputKeyBoard.h"
#include "defaultSetting.h"
#include "RTCHeader.h"
#include "SEPL328_keypad.h"
#include "novaSecurity.h"
#include "prtLabel.h"


#define UP    1
#define Down  2
#define ENT   3
#define ESC   4

#define x1UP  415
#define x2UP  480
#define y1UP  40
#define y2UP  107

#define xUP   ( ( xAxis >= x1UP  ) && ( xAxis <= x2UP  ) )
#define yUP   ( ( yAxis >= y1UP   ) && ( yAxis <= y2UP  ) )
/**********************************************************************************************/

#define x1DOWN  415
#define x2DOWN  480
#define y1DOWN  122
#define y2DOWN  185

#define xDOWN  ( ( xAxis >= x1DOWN ) && ( xAxis <= x2DOWN ) )
#define yDOWN  ( ( yAxis >= y1DOWN  ) && ( yAxis <= y2DOWN ) )
/**********************************************************************************************/

#define x1ENT  415
#define x2ENT  480
#define y1ENT  198
#define y2ENT  262

#define xENT  ( ( xAxis >= x1ENT ) && ( xAxis <= x2ENT ) )
#define yENT  ( ( yAxis >= y1ENT  ) && ( yAxis <= y2ENT ) )
/**********************************************************************************************/

#define x1ESC  415
#define x2ESC  480
#define y1ESC  285
#define y2ESC  320

#define xESC  ( ( xAxis >= x1ESC ) && ( xAxis <= x2ESC ) )
#define yESC  ( ( yAxis >= y1ESC  ) && ( yAxis <= y2ESC ) )
/**********************************************************************************************/


#define x1EN  10
#define x2EN  154
#define y1EN  60
#define y2EN  120

#define xEN  ( ( xAxis >= x1EN ) && ( xAxis <= x2EN ) )
#define yEN  ( ( yAxis >= y1EN  ) && ( yAxis <= y2EN ) )
/**********************************************************************************************/

#define x1DN  178
#define x2DN  333
#define y1DN  60
#define y2DN 120

#define xDN  ( ( xAxis >= x1DN ) && ( xAxis <= x2DN ) )
#define yDN  ( ( yAxis >= y1DN  ) && ( yAxis <= y2DN ) )
/**********************************************************************************************/

#define x1SAVE  80
#define x2SAVE  242
#define y1SAVE  140
#define y2SAVE  188

#define xSAVE  ( ( xAxis >= x1SAVE ) && ( xAxis <= x2SAVE ) )
#define ySAVE  ( ( yAxis >= y1SAVE  ) && ( yAxis <= y2SAVE ) )
/**********************************************************************************************/

extern TFT_eSPI tft;
typedef int lastNodeId;

#define  SPL Serial.println
#define  SP  Serial.print
#define RGB(red, green, blue ) (((red & 0xf8)<<8) + ((green & 0xfc)<<3)+(blue>>3))



#define LOOP_EN    1
#define LOOP_DIS   0
#define SUB_MENU   10

#define settingIndexBlankFullCircle() { for(int8_t i=0; i < 6; i++ )\
      tft.fillCircle( basesetting.xCir, ( 73 + 54*i ), 12, TFT_BLACK); }
#define settingIndexBlankCircle(dotIdx)  tft.fillCircle(basesetting.xCir, ( 73 + 54*dotIdx ), 12, TFT_BLACK);
#define settingIndexfillCircle(dotIdx)   tft.fillCircle( basesetting.xCir, ( 73 + 54*dotIdx ), 12, TFT_GREEN );


#define FsettingIndexBlankFullCircle() { for(int8_t i=0; i < 6; i++ )\
      tft.fillCircle( settings->basesetting.xCir, ( 73 + 54*i ), 12, TFT_BLACK); }
#define FsettingIndexBlankCircle(dotIdx)  tft.fillCircle( settings->basesetting.xCir, ( 73 + 54*dotIdx ), 12, TFT_BLACK);
#define FsettingIndexfillCircle(dotIdx)   tft.fillCircle( settings->basesetting.xCir, ( 73 + 54*dotIdx ), 12, TFT_GREEN );


#define Enable_Text_Font()  {                                                   \
    tft.setTextSize( 1 );                               \
    tft.setFreeFont( (const GFXfont *)EUROSTILE_B20 );  \
    tft.setTextColor(TFT_WHITE, TFT_BLACK);             \
  }





int8_t  nullFunc(  class settings *settings );


int8_t addProductName( class settings *settings );
int8_t addOperatorName( class settings *settings );
int8_t addShift( class settings *settings );

int8_t addHeaderOne( class settings *settings );
int8_t addHeaderTwo( class settings *settings );
int8_t addHeaderThree( class settings *settings );
int8_t addHeaderFour( class settings *settings );
int8_t addHeaderFive( class settings *settings );

int8_t addFooterOne( class settings *settings );
int8_t addFooterTwo( class settings *settings );
int8_t addFooterThree( class settings *settings );

bool enableORdisable( class settings *settings );
void enORDis( class settings *settings );
int8_t handleDotsubMenu( class settings *settings, int8_t Size, int8_t dotIdx );


int8_t weighingMode( class settings *settings );
int8_t countingMode( class settings *settings );
int8_t checkWeighing(class settings *settings  );
int8_t priceComputing(class settings *settings  );

int8_t scaleInfo( class settings *settings );
int8_t weighingUnit_lb( class settings *settings );
int8_t weighingUnit_oz( class settings *settings );
int8_t weighingUnit_t( class settings *settings );
int8_t weighingUnit_gm( class settings *settings );
int8_t weighingUnit_kg( class settings *settings );


int8_t setBaudrate(class settings *settings );
int8_t dataBits( class settings *settings );
int8_t paritySetting( class settings *settings );
int8_t stopBits( class settings *settings );
int8_t stabilityFilter(class settings *settings );
int8_t DateSetup(class settings *settings  );
int8_t TimeSetup(  class settings *settings );
int8_t autoZero( class settings *settings );
int8_t capacitySetup( class settings *settings );
int8_t eValueSetup( class settings *settings);
int8_t decimalSetup(  class settings *settings);

int8_t calibrateSetup( class settings *settings );
bool settingscalibrationImage( class settings *settings );
int8_t readNoLoadcount( class settings *settings );
void readLoadcount(class settings *settings  );
void showCaliWeight(class settings *settings );

int8_t saveAsDefault( class settings *settings  );
int8_t factoryDefault( class settings *settings );


int8_t callBackFuncations( class settings *settings );

struct su_settingMenu
{
  int Idx;
  char buf[50];
  int8_t sub_func;
  int8_t ( *callbackfunc ) ( class settings *settings );
} ;

#define NO 0
#define YES 1
#define MAX_NODE 17 

 int8_t en_saveResponse( class settings *settings );
 int8_t dis_saveResponse( class settings *settings );
 
class settings : public userKeyBoard, public defaultSettings, public customRTC, public prtLabel
{
  private :
    class novaSecurity novaSecure;
    int _checkChildId;
    int _callingFunctionId;
    
    typedef  struct userSettings {
      int id;
      int nextNode;
      int preNode;
      int ParentNode;
      int childNode;
      String operationName;
      bool subPage; // 0= call callBack function, 1= Show Next Page menu     
      int8_t (*userCallBack)( class settings *settings );
    }ListNode;
  
//    vector<userSettings> 
    userSettings userMenu[MAX_NODE], tempuserMenu;
      
      typedef struct s_handlePage {
      int8_t dot; // dot display on TFT screen it max value can reach up [0...4] < 5
      int8_t pageIdx; // This is for traversal into arrIdx array indexes
      int8_t maxNodeIdx ; // upto which Point dot can move.
      int8_t arrPageIdx[5]; // every index will show the depth of page and change index only after Enter  
      int8_t insertId; // This is the ID which say which structure must be inster now.
      userSettings temp[5]; // It can accomadate only intity.
    };   
    s_handlePage handlePage;


  public :
 
   bool author=0;
   friend  int8_t en_saveResponse( class settings *settings );
   friend int8_t dis_saveResponse( class settings *settings );
 
    
    void showOnTFT();
    void  loadDisplayContent();
    // This structure takes care all digit operations.
    struct _showDigit {
      char preValue[10];      // this field holds previouls data. which was written on screen.
      char currentValue[10];  // This fields holds current data. Which user want to see on window
      int xDigitPos[6];       // This holds every digits 'X' position.
      int yDigitPos[4];       // This holds every digits 'Y' position.
      const void * digitFontSize = LED7SEG_STD18; // This holds custom digit fonts.
      int8_t dotpos;          // dot position.
    } showDigit;
    /*
       @ :- nova_Setting
          1 -> This Structure belongs to setting module.
          2 -> It holds All setting parametrs to be changed.
    */
    struct nova_setting
    {
      bool weighingMode; // [1] if user want to enable this utility , [0] disable this utility
      bool countingMode; // [1] if user want to enable this utility , [0] disable this utility
      bool checkWeighing;// [1] if user want to enable this utility , [0] disable this utility
      bool pricecomputing;// [1] if user want to enable this utility , [0] disable this utility

      bool WeigingUnits_kg;// [1] if user want to use utility in this Calibareted weight, [0] disable this utility
      bool WeigingUnits_gm;// [1] if user want to use utility in this Calibareted weight, [0] disable this utility
      bool WeigingUnits_t;// [1] if user want to use utility in this Calibareted weight, [0] disable this utility
      bool WeigingUnits_oz;// [1] if user want to use utility in this Calibareted weight, [0] disable this utility
      bool WeigingUnits_lb;// [1] if user want to use utility in this Calibareted weight, [0] disable this utility

      char scaleInfo[3][50];// This field uses the scale information.
      // [1] : Developer Name/ manufacture's name
      // [2] : Version number
      // [3] : Date

      unsigned int Baudrate;// This Serial BaudRate at which scale is working. Application is not yet decided.
      int8_t dataBits; // This is for serial Communication. Application is not yet decided.
      int8_t parity;// This is for serial Communication. Application is not yet decided.
      int8_t stopBits;// This is for serial Communication. Application is not yet decided.

      int8_t stabilityFilter;

      char date[20]; // this field hold date for Furture communication.
      char TimeArray[20];// this field hold date for Furture communication.

      int8_t autoZero;

      char scaleCapacity[20];// This field belongs to scale calibration.
      char eValue[5]; // This field belongs to scale calibration.
      char decimalValue[5]; // This field belongs to scale calibration.
      char noLoad_count[10];// This field belongs to scale calibration.
      char load_count[10];// This field belongs to scale calibration.
      char cali_weight[10];// This field belongs to scale calibration.
    } userSetting;


    /*
       @s_setting :
        1 > This structure take care all about setting module screen.
        2 >
    */
    struct s_setting
    {
      bool      fullBlankFlag; // [1] draw a rectange on string fields to erase previous text.
      bool      flag;
      char      buf[8];

      // draw strings on screen parameters
      uint16_t  xText = 40;
      uint16_t  yText = 52;
      uint16_t  xCir  = 23;
      uint16_t  yCir  = 62;

      int8_t    dotIdx; // it holds dot position
      int8_t    cnt;
      int8_t    pageTotalItem; // calculate items to be present on screen.
      uint32_t  stringIdx; // it holds string index to be search to write strings on screen.
      uint32_t  baseStringIdx;
      uint32_t  baseEndStringIdx; // it holds the last string index to be present on a page.
    } basesetting;


    // These funcations used to operate on setting module.
    void importDefualtSettings();
    void info();
    void settingPageInit( );
    void settingPageImage();
    int8_t ButtonTouch( );
    void settingPageHandler( );
    void drawSettingString(  );
    int8_t findPageTotalItems(  );
    void blankRect(int8_t idx);

    // these function take care all the functionality of Buttons.
    // these take care all calculation and as per requirement whenever
    // they hit my user under setting module.
    void  handleDownButton( );
    void handleUPButton( );
    int8_t handleENTButton(  );
    int8_t handleESCButton(  );

    bool settingscalibrationImage();

    String getHeader();
    String getFooter();


    void settingUserPageHandler();

    // These funcations are external becase it can be changed any time as per need.
    //
    friend int8_t  nullFunc(  class settings *settings );

    friend int8_t weighingMode( class settings *settings );
    friend int8_t countingMode( class settings *settings  );
    friend int8_t checkWeighing( class settings *settings  );
    friend  int8_t priceComputing( class settings *settings  );

    friend  int8_t scaleInfo( class settings& settings );
    friend  int8_t weighingUnit_lb( class settings *settings  );
    friend  int8_t weighingUnit_oz( class settings *settings  );
    friend  int8_t weighingUnit_t( class settings *settings  );
    friend  int8_t weighingUnit_gm( class settings *settings  );
    friend  int8_t weighingUnit_kg( class settings *settings );

    friend  int8_t setBaudrate(class settings *settings );

    friend  int8_t dataBits( class settings *settings );
    friend  int8_t paritySetting( class settings *settings );
    friend  int8_t stopBits(class settings *settings  );
    friend  int8_t stabilityFilter(class settings *settings );
    friend  int8_t DateSetup( class settings *settings  );
    friend  int8_t TimeSetup(  class settings *settings );
    friend  int8_t autoZero(class settings *settings  );

    friend  int8_t capacitySetup(class settings *settings  );
    friend  int8_t eValueSetup( class settings *settings);
    friend  int8_t decimalSetup( class settings *settings );

    friend  int8_t calibrateSetup( class settings *settings  );

    friend  int8_t readNoLoadcount( class settings *settings );
    friend void readLoadcount(class settings *settings  );
    friend  void showCaliWeight(class settings *settings );

    friend int8_t saveAsDefault( class settings *settings );
    friend int8_t factoryDefault( class settings *settings );

    friend int8_t addHeaderOne( class settings *settings );
    friend int8_t addHeaderTwo( class settings *settings );
    friend int8_t addHeaderThree( class settings *settings );
    friend int8_t addHeaderFour( class settings *settings );
    friend int8_t addHeaderFive( class settings *settings );

    friend int8_t addFooterOne( class settings *settings );
    friend int8_t addFooterTwo( class settings *settings );
    friend int8_t addFooterThree( class settings *settings );

    friend int8_t addProductName( class settings *settings );
    friend int8_t addOperatorName( class settings *settings );
    friend int8_t addShift( class settings *settings );


};


#endif
