
#include "settings.h"

struct nova_setting nova_setting;

struct su_settingMenu settingMenuArray[50] = { 
/* 0 */ { 10, "Application Setup" , SUB_MENU, nullFunc/* This function define cordingly */ },
/* 1 */ { 11, "Scale Setup"        , SUB_MENU, nullFunc/* This function define cordingly */ },
/* 2 */ { 12, "Weighing Units"     , SUB_MENU, nullFunc /* This function define cordingly */},
/* 3 */ { 13, "Scale Information"       , 0, scaleInfo/* This function define cordingly */ },
/* 4 */ { 14, "Printer Configration"       , 0, nullFunc/* This function define cordingly */ },
/* 5 */ { 15, "Save as Default"       , 0, nullFunc/* This function define cordingly */ },

/* 1.1 */ { 100, "Counting Mode"  , 0, countingMode/* This function define cordingly */ },
/* 1.2 */ { 101, "Check Weighing"  , 0, checkWeighing/* This function define cordingly */ },
/* 1.3 */ { 102, "Price Computing"  , 0, priceComputing /* This function define cordingly */ },

/* 2.1 */ { 110, "Clock Setup"  , 0, clockSetup/* This function define cordingly */ },
/* 2.2 */ { 111, "Auto Zero"  , 0, autoZero/* This function define cordingly */ },
/* 2.3 */ { 112, "Stability Filter Setup"  , 0, stabilityFilter/* This function define cordingly */ },
/* 2.4 */ { 113, "COM1 Parameters"  , SUB_MENU, nullFunc/* This function define cordingly */ },

/* 3.1 */ { 120, "Kilogram(kg)"  , 0, weighingUnit_kg/* This function define cordingly */ },
/* 3.1 */ { 121, "Grams(gm)"  , 0, weighingUnit_gm/* This function define cordingly */ },
/* 3.1 */ { 122, "Tons(T)"  , 0, weighingUnit_t/* This function define cordingly */ },
/* 3.1 */ { 123, "Ounces(Oz)"  , 0, weighingUnit_oz/* This function define cordingly */ },
/* 3.1 */ { 124, "Pounds(lb)"  , 0, weighingUnit_lb/* This function define cordingly */ },

/* 2.4.1 */ { 1130, "Baudrate"  , 0, setBaudrate/* This function define cordingly */ },
/* 2.4.2 */ { 1131, "Data Bits"  , 0, dataBits/* This function define cordingly */ },
/* 2.4.3 */ { 1132, "Parity"  , 0, paritySetting/* This function define cordingly */ },
/* 2.4.4 */ { 1133, "Stop Bits"  , 0, stopBits/* This function define cordingly */ },
/* 2.4.5 */ { 1134, "Flow Control"  , 0, nullFunc/* This function define cordingly */ },

/* Dummy */ { 100000, "COM1 Parameters"  , 0, nullFunc/* This function define cordingly */ },
  };



int8_t nullFunc( struct s_setting *basesetting )
{
  SPL("I am Null function");
}

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
void settingPageInit( struct s_setting *basesetting, struct su_settingMenu *settingMenuArray, class settings *classOBJ_set)
{
  basesetting->baseStringIdx = basesetting->stringIdx = 10;
  basesetting->baseEndStringIdx = 20;

  // allocate memory upto the size of settingmenu
  basesetting->sp_settingMenu = new su_settingMenu;

  // setting structure array which holds user defined strings and functions.
  basesetting->sp_settingMenu = settingMenuArray;

  // ButtonTouch is a function which will be called by callBack function.
  //  basesetting->buttonTouch = &settings ::ButtonTouch;

  // Start While loop keep running.
  basesetting->flag = LOOP_EN ;

  //set class object
  basesetting->classOBJ_set = classOBJ_set;// class object

  // settings
   basesetting->userSetting = &nova_setting;
   strcpy(basesetting->userSetting->scaleInfo[0], "Version : 2.11.11");
   strcpy(basesetting->userSetting->scaleInfo[1], "Serial# : 20452/3032");
}



void settings :: settingPageImage()
{
  tft.setSwapBytes(true);
  tft.pushImage(0, 0, settingPageImageWidth, settingPageImageHeight, settingPageImageArr );

}

/*
   @ ButtonTouch ::
    (#) This function takes care of all button touch functionality
*/

int8_t settings :: ButtonTouch( )
{
  uint16_t xAxis, yAxis;
  if ( tft.getTouch(&xAxis, &yAxis) )
  {
    if (xUP && yUP )           return UP; //UP
    else if (xDOWN && yDOWN )  return Down; //DOWN
    else if (xENT && yENT )    return ENT; //ENT
    else if (xESC && yESC )    return ESC; //ESC
    else
      Serial.println("ELSE Touch :\nxAxis --->>> " + String(xAxis) + "\nyAxis --->>> " + String(yAxis) );
  }
  return 5;
}


#define fullBlankRect() for(int8_t idx=0; idx < 5; idx++ ) blankRect( idx );

void blankRect(int8_t idx)
{
  uint16_t rectYPos[5] = {50, 104, 155, 208, 260 };
  tft.fillRect( basesetting.xText, rectYPos[idx], 350, 48,  RGB(200, 100, 50 ));
}


void menuString( struct s_setting *basesetting )
{
  //  if( )


}


void drawSettingString( struct s_setting *basesetting, int searchIdx, bool fullBlankFlag )
{

  SPL("Search IDX : " + String(searchIdx) );
  if ( fullBlankFlag )
    fullBlankRect()

    int8_t idx;
  for ( idx = 0;  basesetting->sp_settingMenu[idx].Idx != searchIdx ; idx++ );

  for ( int8_t ptridx = idx, strpos = 0; ptridx < ( 5 + idx ); ptridx++, strpos++ )
  {
    if (  !(  basesetting->sp_settingMenu[ptridx].Idx / basesetting->baseEndStringIdx ) )
    {
      tft.setCursor( basesetting->xText, ( 85 + 54 * strpos ) );
      tft.print( basesetting->sp_settingMenu[ptridx].buf);
    }
    else break;
  }//end-for
}


/*
   @  settingPage ::
    (#) This function is the entry point of machine settings.
        - It also takes care UP, Down, ESC and ENT button functionality.
        - According to various steps taken by user, appropriate functions will be called.
        -
*/

int8_t findPageTotalItems( struct s_setting *basesetting, int startBase, int endBase)
{
  // Input Block
//  SPL("PageItem--->>>" + String( basesetting->pageTotalItem ) );
//  SPL("startBase--->>>" + String( startBase ) );
//  SPL("endBase--->>>" + String( endBase ) );


  // 1. find at what index string is present.
  int8_t idx;
  int cnt = 0;
  for ( idx = 0;  basesetting->sp_settingMenu[idx].Idx != startBase ; idx++ );
  for ( idx, basesetting->pageTotalItem = 0; !( basesetting->sp_settingMenu[ idx ].Idx / basesetting->baseEndStringIdx ); ++basesetting->pageTotalItem, ++idx );
//  SPL("PageItem--->>>" + String( basesetting->pageTotalItem ) );
  return basesetting->pageTotalItem;
}


void settingPageHandler( struct s_setting *basesetting )
{
  // This is the base image of setting
  basesetting->classOBJ_set->settingPageImage();
  settingIndexBlankFullCircle();
  settingIndexfillCircle( basesetting->dotIdx);
  Enable_Text_Font()
  findPageTotalItems( basesetting, basesetting->stringIdx, basesetting->baseEndStringIdx);

  drawSettingString( basesetting, basesetting->stringIdx, 0 );
  // keep running untill ESC button is not pressed.
  while ( 1 ) {
    switch (  basesetting->classOBJ_set->ButtonTouch() ) // it is callback function
    {
      case Down:
        handleDownButton( basesetting );
        break;

      case UP:
        handleUPButton( basesetting );
        break;
      case ENT:
        handleENTButton( basesetting );
        break;

      case ESC:
        if( handleESCButton( basesetting ) == -1 )
            return;
        break;
    }
    yield();
  }

  Serial.println("While Loop Ends and back  to menu touch ");
}//end-settingPage

int8_t handleESCButton( struct s_setting *basesetting )
{
  if ( ( basesetting->baseStringIdx / 10 ) >= 10)
  {
    basesetting->stringIdx =  ( basesetting->baseStringIdx ) / 10;
    basesetting->dotIdx =  ( basesetting->stringIdx )  % 10;
    basesetting->baseStringIdx = basesetting->stringIdx - basesetting->dotIdx;
    basesetting->baseEndStringIdx = basesetting->baseStringIdx + 10;
    basesetting->stringIdx = basesetting->baseStringIdx;

    findPageTotalItems( basesetting, basesetting->baseStringIdx, basesetting->baseEndStringIdx);
    settingIndexBlankFullCircle();
    basesetting->cnt = 0;
    drawSettingString( basesetting,   basesetting->stringIdx, 1 );
    settingIndexfillCircle( basesetting->dotIdx );
    return 1;
  }
  else if( ( basesetting->baseStringIdx / 10 ) < 10 )
  {
    return -1;
  }
  return 0;
}

int8_t handleENTButton( struct s_setting *basesetting )
{
  int8_t idx;
  for ( idx = 0;  basesetting->sp_settingMenu[idx].Idx != ( basesetting->stringIdx + basesetting->dotIdx ); idx++ );
  if ( basesetting->sp_settingMenu[idx].sub_func == SUB_MENU )
  {
    basesetting->baseStringIdx = ( basesetting->stringIdx + basesetting->dotIdx ) * 10;
    basesetting->baseEndStringIdx = basesetting->baseStringIdx + 10;
    basesetting->stringIdx = basesetting->baseStringIdx;

    findPageTotalItems( basesetting, basesetting->baseStringIdx, basesetting->baseEndStringIdx);
    settingIndexBlankFullCircle();
    basesetting->dotIdx = 0;
    basesetting->cnt = 0;
    drawSettingString( basesetting, basesetting->stringIdx, 1 );
    settingIndexfillCircle( basesetting->dotIdx );
  }
  else
  {
    if ( basesetting->sp_settingMenu[idx].callbackfunc(basesetting) )
    {
      
    }
  }
  delay(80);
   return 1;
}


void handleUPButton( struct s_setting *basesetting )
{
  --basesetting->dotIdx;

  if ( basesetting->dotIdx < 0 )
  {
    basesetting->dotIdx  = 0;
    if (  basesetting->stringIdx >  basesetting->baseStringIdx )
    {
      basesetting->cnt--;
      drawSettingString( basesetting, --basesetting->stringIdx, 1 );
    }
  }

  settingIndexBlankFullCircle();
  settingIndexfillCircle( basesetting->dotIdx );

  delay(80);
}

void handleDownButton( struct s_setting *basesetting )
{
  basesetting->dotIdx++;
  if ( basesetting->dotIdx >= 5 )
  {
    basesetting->dotIdx  = 4;
    if ( ( basesetting->cnt + basesetting->dotIdx ) <  basesetting->pageTotalItem )
    {
      drawSettingString( basesetting, ++basesetting->stringIdx, 1 );
      basesetting->cnt++;
      if ( ( basesetting->cnt + basesetting->dotIdx ) >=  basesetting->pageTotalItem )
      {
        settingIndexBlankFullCircle()
        settingIndexfillCircle( --basesetting->dotIdx );
      }

    }
    else
    {
      --basesetting->dotIdx;
    }
  }
  else
  {
    if ( ( basesetting->cnt + basesetting->dotIdx )  < basesetting->pageTotalItem )
    {
      settingIndexBlankFullCircle()
      settingIndexfillCircle( basesetting->dotIdx );
    }
  }

  delay(80);
}
