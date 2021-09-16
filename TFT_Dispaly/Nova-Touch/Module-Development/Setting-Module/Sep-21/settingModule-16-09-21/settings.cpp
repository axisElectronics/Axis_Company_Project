
#include "settings.h"

/*************************************************************************
          >>>> MACROs <<<<

 **************************************************************************/
#define fullBlankRect() for(int8_t idx=0; idx < 5; idx++ ) blankRect( idx );





int8_t nullFunc( class settings *settings  )
{
  SPL("I am Null function");
}



extern class novaSecurity novaSecure;

#define RXD2 16
#define TXD2 17

int8_t SecurityTest(  class settings *settings  ) {
  // =============================================
  // Security Check
  // =============================================
  // novaSecure.enterPassword();

  // =============================================
  // Show data coming on Screen from Serials
  // =============================================

  tft.fillScreen(TFT_BLACK);
  tft.setTextSize( 1 );
  tft.setFreeFont( (const GFXfont *)EUROSTILE_B10 );
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  // start serial port-2
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Weight Mechine

  int16_t x, y, s = 1;
  x = 20;
  y = 150;
  class userKeyBoard kbd;
  while ( kbd.getMappedKey(  ) != -2 ) {


    if ( Serial2.available() > 0 ) {
      char buff = (char)Serial2.read();
      tft.drawChar( buff, x, y, s);

      x = (x + 15);

      if ( ( x > 450 ) && ( y < 310 ) ) {
        x = 20;
        y = (y + 20);
      }
      else if ( y > 300  ) {
        tft.fillScreen(TFT_BLACK);
        x = 20;
        y = 150;
      }

    }//end-if

    /* Hold data upto next action */
    if (kbd.getMappedKey() == -1) {
      while ( kbd.getMappedKey(  ) != -2 ) {
        yield();
      }
    }


    yield();
  }
  /*
    tft.drawString("Data : ", 10, 10, 1);
    tft.drawString("Flag : ", 10, 50, 1);
    tft.drawString("Bat : ", 10, 100, 1);
    String temp;
    char buff[15];
    memset(buff, '\0', 15);
    char flag[3];
    class userKeyBoard kbd;

    while( kbd.getMappedKey(  ) != -2){
    temp =  "";
    // 1. Get data from weighing machine
    if ( Serial2.available() > 0 )
    temp = Serial2.readStringUntil('=');

    if( temp.length() > 5 ){

    //      Serial.print("Battery : ");
    //      Serial.println(temp[7], HEX);

      //show battery level in Hex
      sprintf(&buff[7], "%X", temp.c_str()[7]);
      buff[9] = '\0';
      tft.drawString(&buff[7], 100, 100, 1);

      // show actual data received at Serial-2 of esp32
      strcpy(buff, temp.c_str());
      buff[7] = '\0';
    //      Serial.println("==============>>> ");
    //      Serial.print("buff : ");
    //      Serial.println(temp);
      tft.drawString(buff, 100, 10, 1);

    //      Serial.print("flag : ");
    //      Serial.println(temp[8], HEX);

      // show flag value in HEX
      sprintf(&buff[8], "%X", temp.c_str()[8]);
      buff[10] = '\0';
      tft.drawString(&buff[8], 100, 50, 1);

    }
      yield();
    }

    Serial2.end();
  */



  // =============================================
  // Load setting Screen with previous settings
  // =============================================
  Enable_Text_Font()
  tft.fillScreen(TFT_BLACK);
  settings->settingPageImage();
  FsettingIndexBlankFullCircle();
  settings->basesetting.fullBlankFlag = 1;
  settings->drawSettingString( );
  FsettingIndexfillCircle( settings->basesetting.dotIdx );
  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );
  return 1;
}


struct su_settingMenu settingMenuArray[50] = {
  /* 0 */ { 10, "Application Setup"          , SUB_MENU  , nullFunc/* This function define cordingly */ },
  /* 1 */ { 11, "Scale Setup"                , SUB_MENU  , nullFunc/* This function define cordingly */ },
  /* 2 */ { 12, "Weighing Units"             , SUB_MENU  , nullFunc /* This function define cordingly */},
  /* 3 */ { 13, "Scale Information"          , 0         , scaleInfo/* This function define cordingly */ },
  /* 4 */ { 14, "Printer Configration"       , SUB_MENU  , nullFunc/* This function define cordingly */ },
  /* 5 */ { 15, "Calibration"                , SUB_MENU  , nullFunc/* This function define cordingly */ },
  /* 6 */ { 16, "Save as Default"            , 0         , saveAsDefault/* This function define cordingly */ },
  /* 7 */ { 17, "Restore Default"            , 0         , nullFunc/* This function define cordingly */ },
  /* 8 */ { 18, "Factory Default"            , 0         , factoryDefault/* This function define cordingly */ },
  /* 9 */ { 19, "Service"                     , 0        , SecurityTest/* This function define cordingly */ },

  /* 1.1 */ { 100, "Weighing Mode"           , 0         , weighingMode/* This function define cordingly */ },
  /* 1.2 */ { 101, "Counting Mode"           , 0         , countingMode/* This function define cordingly */ },
  /* 1.3 */ { 102, "Check Weighing"          , 0         , checkWeighing/* This function define cordingly */ },
  /* 1.4 */ { 103, "Price Computing"         , 0         , priceComputing /* This function define cordingly */ },

  /* 2.1 */ { 110, "Date Setup"              , 0         , DateSetup/* This function define cordingly */ },
  /* 2.2 */ { 111, "Auto Zero"               , 0         , autoZero/* This function define cordingly */ },
  /* 2.3 */ { 112, "Stability Filter Setup"  , 0         , stabilityFilter/* This function define cordingly */ },
  /* 2.4 */ { 113, "COM1 Parameters"         , SUB_MENU  , nullFunc/* This function define cordingly */ },
  /* 2.5 */ { 114, "Time Setup"              , 0         , TimeSetup/* This function define cordingly */ },

  /* 3.1 */ { 120, "Kilogram(kg)"            , 0         , weighingUnit_kg/* This function define cordingly */ },
  /* 3.1 */ { 121, "Grams(gm)"               , 0         , weighingUnit_gm/* This function define cordingly */ },
  /* 3.1 */ { 122, "Tons(T)"                 , 0         , weighingUnit_t/* This function define cordingly */ },
  /* 3.1 */ { 123, "Ounces(Oz)"              , 0         , weighingUnit_oz/* This function define cordingly */ },
  /* 3.1 */ { 124, "Pounds(lb)"              , 0         , weighingUnit_lb/* This function define cordingly */ },

  /* 4.1 */ { 140, "Header"                  , SUB_MENU  , nullFunc/* This function define cordingly */ },
  /* 4.2 */ { 141, "Footer"                  , SUB_MENU  , nullFunc/* This function define cordingly */ },
  /* 4.3 */ { 142, "P-Type"                  , 0         , nullFunc/* This function define cordingly */ },

  /* 5.1 */ { 150, "Capacity"                , 0         , capacitySetup/* This function define cordingly */ },
  /* 5.1 */ { 151, "E value"                 , 0         , eValueSetup/* This function define cordingly */ },
  /* 5.1 */ { 152, "Decimal"                 , 0         , decimalSetup/* This function define cordingly */ },
  /* 5.1 */ { 153, "Calibrate"               , 0         , calibrateSetup/* This function define cordingly */ },


  /* 2.4.1 */ { 1130, "Baudrate"             , 0         , setBaudrate/* This function define cordingly */ },
  /* 2.4.2 */ { 1131, "Data Bits"            , 0         , dataBits/* This function define cordingly */ },
  /* 2.4.3 */ { 1132, "Parity"               , 0         , paritySetting/* This function define cordingly */ },
  /* 2.4.4 */ { 1133, "Stop Bits"            , 0         , stopBits/* This function define cordingly */ },
  /* 2.4.5 */ { 1134, "Flow Control"         , 0         , nullFunc/* This function define cordingly */ },

  /* 4.1.1 */ { 1400, "Header-1"             , 0         , addHeaderOne/* This function define cordingly */ },
  /* 4.1.1 */ { 1401, "Header-2"             , 0         , addHeaderTwo/* This function define cordingly */ },
  /* 4.1.1 */ { 1402, "Header-3"             , 0         , addHeaderThree/* This function define cordingly */ },
  /* 4.1.1 */ { 1403, "Header-4"             , 0         , addHeaderFour/* This function define cordingly */ },
  /* 4.1.1 */ { 1404, "Header-5"             , 0         , addHeaderFive/* This function define cordingly */ },

  /* 4.2.1 */ { 1410, "Footer-1"             , 0         , addFooterOne/* This function define cordingly */ },
  /* 4.2.1 */ { 1411, "Footer-2"             , 0         , addFooterTwo/* This function define cordingly */ },
  /* 4.2.1 */ { 1412, "Footer-3"             , 0         , addFooterThree/* This function define cordingly */ },

  /* Dummy */ { 100000, "COM1 Parameters"    , 0         , nullFunc/* This function define cordingly */ },
};



/*
     This array is totally underControl of an Developer.
     Make sure that user can add setting path dynamically --->>> Not needed
     based on < su_settingMenu.idx >
*/


void  settings :: info()
{
  Serial.print("\n\n**************************************************************************************\n\n");
  Serial.print("This call of API tells user to do necessary tasks.\n\n");
  Serial.print(" 1. Create two structure name :\n > struct s_setting basesetting; \n > struct su_settingMenu settingMenuArray[User_SIZE];  \n\n" );
  Serial.print(" 2. Member variables of structure #basesetting are  \n\n");
  Serial.print("struct s_setting\n{\n  bool      flag;\n  char      buf[8];\n  uint16_t  xText = 40;\n  uint16_t  yText = 52; \n  uint16_t  xCir  = 23;\n  uint16_t  yCir  = 62;  \n  int8_t    dotIdx;\n  int       stringIdx;\n  int       baseStringIdx;\n  int       baseEndStringIdx;\n  class settings *classOBJ_sett;\n  int8_t    (settings :: *buttonTouch)();\n  struct    su_settingMenu *sp_settingMenu;\n};\n\n");
  Serial.print(" 3.  Member variables of structure #settingMenu are  \n\n");
  Serial.print("struct su_settingMenu\n{\n  int idx;\n  char buf[20];\n  int8_t sub_func;\n  int8_t (*callbackfunc)( struct _digithandler *digitHandler );\n};\n\n");
  Serial.print(" 4. we are using class name setting which has usefull member functions.\n\n");
  Serial.print("class settings\n{\n\n  public :\n  void info();\n  void settingPageImage();\n  void settingPageInit();\n  int8_t ButtonTouch( );\n\n};\n");
  Serial.print(" ");
  Serial.print("\n\n**************************************************************************************\n\n");
}


/*
   @ settingpageInit()
   (#) This function initilize basic setting used in settingPage function.
      - This function set base index for string
      - This function initize callback functions also
*/

#define RXD2 16
#define TXD2 17

void settings ::  settingPageInit( )
{

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Weight Mechine

  basesetting.baseStringIdx = basesetting.stringIdx = 10;
  basesetting.baseEndStringIdx = 20;

  initDefaultSettings( );
  //  settingPageHandler( );
  strcpy(  userScaleSetup.scaleInformation[ developer ], "Vivek Yadav" );
  strcpy(  userScaleSetup.scaleInformation[ baseVersion ], "novaTouch0.0.0" );
  strcpy(  userScaleSetup.scaleInformation[ modifiedVersion ], "Under Dev" );

  strcpy( userSetting.scaleInfo[developer], userScaleSetup.scaleInformation[ developer ]       );
  strcpy( userSetting.scaleInfo[baseVersion], userScaleSetup.scaleInformation[ baseVersion ]     );
  strcpy( userSetting.scaleInfo[modifiedVersion], userScaleSetup.scaleInformation[ modifiedVersion ] );

  //=====================================
  //  Initilise RTC module
  //=====================================
  initRTC();
  //==============================================
  //  Initilise mapped  Physical KeyBoard module
  //==============================================
  intiSEPL328_Keypad();

  //  const char HWT[10] = "\2HWT#0\3";
  //  Serial2.print(HWT);
  //  Serial.println("HIgh Precision...!!!!");
}

/********************************************************************
   @ baseImage of setting page wihtout any String.
 *********************************************************************/

void settings :: settingPageImage()
{
  tft.setSwapBytes(true);
  tft.pushImage(0, 0, settingPageImageWidth, settingPageImageHeight, settingPageImageArr );

}

/********************************************************************
   @ ButtonTouch ::
    (#) This function takes care of all button touch functionality
********************************************************************/
#define isKeyPressed_NO 5

int8_t settings :: ButtonTouch( )
{

  uint16_t xAxis, yAxis;
  int8_t Phykey;
  if ( tft.getTouch(&xAxis, &yAxis ) )
  {
    if (xUP && yUP ) {
      return UP;  //UP
    }
    else if (xDOWN && yDOWN ) {
      return Down;  //DOWN
    }
    else if (xENT && yENT ) {
      return ENT;  //ENT
    }
    else if (xESC && yESC ) {
      return ESC;  //ESC
    }
  }//end-if
  else {
    //     Serial.println("phykey : " + String( getMappedKey(  ) ) );
    //    Serial.println("phykey : " + String( navigationKeyMapping() ) );
    if ( ( Phykey = navigationKeyMapping() ) != isKeyPressed_NO ) return Phykey;
  }

  return isKeyPressed_NO;
}//end-ButtonTouch( )

/***********************************************************
   @ blankRect :
    - This funcation helps us to draw a rectange over strings.
      So that we can erase printed strings.
   # NOTE : FullBlankRect is an MACRO which internally calls above
      funcation.
 ***********************************************************/

void settings :: blankRect(int8_t idx)
{
  uint16_t rectYPos[5] = {50, 104, 155, 208, 260 };
  tft.fillRect( basesetting.xText, rectYPos[idx], 350, 48,  RGB(200, 100, 50 ));
}



void settings :: drawSettingString(  )
{

  if ( basesetting.fullBlankFlag )
    fullBlankRect()

    int8_t idx;
  for ( idx = 0; settingMenuArray[idx].Idx != basesetting.stringIdx ; idx++ );

  for ( int8_t ptridx = idx, strpos = 0; ptridx < ( 5 + idx ); ptridx++, strpos++ )
  {
    if (  !(  settingMenuArray[ptridx].Idx / basesetting.baseEndStringIdx ) )
    {
      tft.setCursor( basesetting.xText, ( 85 + 54 * strpos ) );
      tft.print( settingMenuArray[ptridx].buf);
    }
    else break;
  }//end-for
}//end-drawSettingString

/*
   @  settingPage ::
    (#) This function is the entry point of machine settings.
        - It also takes care UP, Down, ESC and ENT button functionality.
        - According to various steps taken by user, appropriate functions will be called.
        -
*/

int8_t  settings :: findPageTotalItems(  )
{

  // 1. find at what index string is present.
  int8_t idx;
  int cnt = 0;
  for ( idx = 0;  settingMenuArray[idx].Idx != basesetting.stringIdx ; idx++ );
  for ( idx, basesetting.pageTotalItem = 0; !(  settingMenuArray[ idx ].Idx / basesetting.baseEndStringIdx ); ++basesetting.pageTotalItem, ++idx );
  //  SPL("PageItem--->>>" + String( basesetting.pageTotalItem ) );
  return basesetting.pageTotalItem;
}

void  settings :: settingPageHandler( )
{
  // 1. validate User -only admin at this time
  tft.fillScreen(TFT_BLACK);
  Enable_Text_Font()
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString( "You need admin privilages..!!!", 30, 80, 1);
  tft.drawString( "Please Enter Password..!!!", 30, 130, 1);
  delay(3000);
  if ( novaSecure.enterPassword() == admin ) {
    // This is the base image of setting
    settingPageImage();
    settingIndexBlankFullCircle();
    settingIndexfillCircle( basesetting.dotIdx);
    Enable_Text_Font()

    findPageTotalItems( );
    basesetting.fullBlankFlag = 1;
    drawSettingString(  );


    // keep running untill ESC button is not pressed.
    while ( 1 ) {
      switch ( ButtonTouch() )
      {
        case Down:
          handleDownButton( );
          break;

        case UP:
          handleUPButton(  );
          break;
        case ENT:
          handleENTButton( );
          break;

        case ESC:
          if ( handleESCButton( ) == -1 )
            return;
          break;
      }
      yield();
    }
  }//end-if(admin)
  else {

  }
}//end-settingPage




int8_t  settings ::  handleESCButton(  )
{
  if ( ( basesetting.baseStringIdx / 10 ) >= 10)
  {
    basesetting.stringIdx =  ( basesetting.baseStringIdx ) / 10;
    basesetting.dotIdx =  ( basesetting.stringIdx )  % 10;
    basesetting.baseStringIdx = basesetting.stringIdx - basesetting.dotIdx;
    basesetting.baseEndStringIdx = basesetting.baseStringIdx + 10;

    if ( basesetting.dotIdx > 4 )
    {
      basesetting.stringIdx = basesetting.baseStringIdx + basesetting.dotIdx % 4;
      basesetting.dotIdx = 4;
    }
    else
      basesetting.stringIdx = basesetting.baseStringIdx;

    findPageTotalItems( );
    settingIndexBlankFullCircle();
    basesetting.cnt = 0;
    basesetting.fullBlankFlag = 1;
    drawSettingString( );
    settingIndexfillCircle( basesetting.dotIdx );
    delay(250);

    return 1;
  }
  else if ( ( basesetting.baseStringIdx / 10 ) < 10 )
  {
    basesetting.cnt = 0;
    basesetting.dotIdx = 0;
    basesetting.stringIdx = 10;
    basesetting.baseStringIdx = 10;
    basesetting.baseEndStringIdx = 20;

    return -1;
  }

  return 0;
}





int8_t settings :: handleENTButton(  )
{
  int8_t idx;
  for ( idx = 0; settingMenuArray[idx].Idx != ( basesetting.stringIdx + basesetting.dotIdx ); idx++ );
  if ( settingMenuArray[idx].sub_func == SUB_MENU )
  {
    basesetting.baseStringIdx     = ( basesetting.stringIdx + basesetting.dotIdx ) * 10;
    basesetting.baseEndStringIdx  =   basesetting.baseStringIdx + 10;
    basesetting.stringIdx         =   basesetting.baseStringIdx;

    findPageTotalItems( );
    settingIndexBlankFullCircle();
    basesetting.dotIdx = 0;
    basesetting.cnt = 0;
    basesetting.fullBlankFlag = 1;
    drawSettingString(  );
    settingIndexfillCircle( basesetting.dotIdx );
  }
  else
  {

    if ( settingMenuArray[idx].callbackfunc( this ) )
    {

    }
  }
  delay(80);
  return 1;
}


void settings :: handleUPButton( )
{
  --basesetting.dotIdx;

  if ( basesetting.dotIdx < 0 )
  {
    basesetting.dotIdx  = 0;
    if (  basesetting.stringIdx >  basesetting.baseStringIdx )
    {
      basesetting.cnt--;
      --basesetting.stringIdx;
      basesetting.fullBlankFlag = 1;
      drawSettingString(  );
    }
  }
  settingIndexBlankFullCircle();
  settingIndexfillCircle( basesetting.dotIdx );

  delay(80);
}

void  settings :: handleDownButton(  )
{
  basesetting.dotIdx++;
  if ( basesetting.dotIdx >= 5 )
  {
    basesetting.dotIdx  = 4;
    if ( ( basesetting.cnt + basesetting.dotIdx ) <  basesetting.pageTotalItem )
    {
      ++basesetting.stringIdx;
      basesetting.fullBlankFlag = 1;
      drawSettingString(   );
      basesetting.cnt++;
      if ( ( basesetting.cnt + basesetting.dotIdx ) >=  basesetting.pageTotalItem )
      {
        settingIndexBlankFullCircle()
        settingIndexfillCircle( --basesetting.dotIdx );
      }

    }
    else
    {
      --basesetting.dotIdx;
    }
  }
  else
  {
    if ( ( basesetting.cnt + basesetting.dotIdx )  < basesetting.pageTotalItem )
    {
      settingIndexBlankFullCircle()
      settingIndexfillCircle( basesetting.dotIdx );
    }
  }

  delay(80);
}


bool  settings :: settingscalibrationImage()
{
  tft.setSwapBytes(true);
  tft.pushImage(0, 0, SettingcalibrationWidth, SettingcalibrationHeight, SettingcalibrationArr );
}


String settings :: getHeader() {

  String temp = "";
  for(uint8_t idx=0; idx < 5; ++idx ){
    String str = "";
    str += _getHeader(idx);
    if( str.length() != 0 ){
      temp += str + '\n';
    }
  } 
  return temp;
}

String settings :: getFooter() {

  String temp = "";
  for(uint8_t idx=0; idx < 3; ++idx ){
    String str = "";
    str += _getFooter(idx);
    if( str.length() != 0 ){
      temp += str + '\n';
    }
  }
  
  return temp;
}
