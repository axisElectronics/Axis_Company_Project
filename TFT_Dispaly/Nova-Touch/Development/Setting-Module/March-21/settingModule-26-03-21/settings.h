
#ifndef SETTING_H
#define SETTING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <String.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <TJpg_Decoder.h>
using namespace std;

#include "customImages.h"
#include "customFontHeaders.h"
#include "UserInputKeyBoard.h"
#include "defaultSetting.h"

#define UP    1
#define Down  2
#define ENT   3
#define ESC   4

#define x1UP  405
#define x2UP  480
#define y1UP   34
#define y2UP   ( y1UP  + 64 )

#define xUP   ( ( xAxis >= x1UP  ) && ( xAxis <= x2UP  ) )
#define yUP   ( ( yAxis >= y1UP   ) && ( yAxis <= y2UP  ) )
/**********************************************************************************************/

#define x1DOWN  408
#define x2DOWN  480
#define y1DOWN  108
#define y2DOWN ( y1DOWN + 64 )

#define xDOWN  ( ( xAxis >= x1DOWN ) && ( xAxis <= x2DOWN ) )
#define yDOWN  ( ( yAxis >= y1DOWN  ) && ( yAxis <= y2DOWN ) )
/**********************************************************************************************/

#define x1ENT  408
#define x2ENT  480
#define y1ENT  188
#define y2ENT ( y1ENT + 64 )

#define xENT  ( ( xAxis >= x1ENT ) && ( xAxis <= x2ENT ) )
#define yENT  ( ( yAxis >= y1ENT  ) && ( yAxis <= y2ENT ) )
/**********************************************************************************************/

#define x1ESC  408
#define x2ESC  480
#define y1ESC  262
#define y2ESC ( y1ESC + 64 )

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
int8_t clockSetup(class settings *settings  );
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

class settings : public userKeyBoard, public defaultSettings
{

  public :
    struct _showDigit
    {
      char preValue[10];
      char currentValue[10];
      int xDigitPos[6];
      int yDigitPos[4];
      const void * digitFontSize = LED7SEG_STD18;
      int8_t dotpos;
    } showDigit;

    struct nova_setting
    {
      bool weighingMode;
      bool countingMode;
      bool checkWeighing;
      bool pricecomputing;

      bool WeigingUnits_kg;
      bool WeigingUnits_gm;
      bool WeigingUnits_t;
      bool WeigingUnits_oz;
      bool WeigingUnits_lb;

      char scaleInfo[3][50];

      unsigned int Baudrate;
      int8_t dataBits;
      int8_t parity;
      int8_t stopBits;
      int8_t stabilityFilter;

      char date[20];

      int8_t autoZero;
      char scaleCapacity[20];
      char eValue[5];
      char decimalValue[5];
      char noLoad_count[10];
      char load_count[10];
      char cali_weight[10];

    } userSetting;

    struct s_setting
    {
      bool      fullBlankFlag;
      bool      flag;
      char      buf[8];
      uint16_t  xText = 40;
      uint16_t  yText = 52;
      uint16_t  xCir  = 23;
      uint16_t  yCir  = 62;
      int8_t    dotIdx;
      int8_t    cnt;
      int8_t    pageTotalItem;
      uint32_t       stringIdx;
      uint32_t       baseStringIdx;
      uint32_t       baseEndStringIdx;
      //      class settings *classOBJ_set;
      //      class defaultSettings *classOBJ_dftsett;
      //      struct    su_settingMenu *sp_settingMenu;
      //      struct nova_setting *userSetting;
      //      struct _showDigit *showDigit;
    } basesetting;

    void importDefualtSettings();
    void info();
    void settingPageInit( );
    void settingPageImage();
    int8_t ButtonTouch( );
    void settingPageHandler( );
    void drawSettingString(  );
    int8_t findPageTotalItems(  );
    void blankRect(int8_t idx);
    void  handleDownButton( );
    void handleUPButton( );
    int8_t handleENTButton(  );
    int8_t handleESCButton(  );
    bool settingscalibrationImage();

    friend int8_t  nullFunc(  class settings *settings );

    friend int8_t weighingMode( class settings *settings );
    friend int8_t countingMode( class settings *settings  );
    friend int8_t checkWeighing( class settings *settings  );
    friend  int8_t priceComputing( class settings *settings  );

    friend int8_t scaleInfo( class settings& settings );
    friend int8_t weighingUnit_lb( class settings *settings  );
    friend int8_t weighingUnit_oz( class settings *settings  );
    friend int8_t weighingUnit_t( class settings *settings  );
    friend int8_t weighingUnit_gm( class settings *settings  );
    friend int8_t weighingUnit_kg( class settings *settings );

    friend  int8_t setBaudrate(class settings *settings );

    friend  int8_t dataBits( class settings *settings );
    friend  int8_t paritySetting( class settings *settings );
    friend  int8_t stopBits(class settings *settings  );
    friend int8_t stabilityFilter(class settings *settings );
    friend int8_t clockSetup( class settings *settings  );
    friend int8_t autoZero(class settings *settings  );
    friend int8_t capacitySetup(class settings *settings  );
    friend int8_t eValueSetup( class settings *settings);
    friend  int8_t decimalSetup( class settings *settings );

    friend  int8_t calibrateSetup( class settings *settings  );

    friend int8_t readNoLoadcount( class settings *settings );
    friend void readLoadcount(class settings *settings  );
    friend  void showCaliWeight(class settings *settings );

    friend int8_t saveAsDefault( class settings *settings );
    friend int8_t factoryDefault( class settings *settings );



};







#endif
