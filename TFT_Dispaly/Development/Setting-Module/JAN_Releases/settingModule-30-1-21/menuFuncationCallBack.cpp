
#include "settings.h"

int8_t autoZero( struct s_setting *basesetting )
{
  tft.fillScreen(TFT_BLACK);
  basesetting->classOBJ_set->settingPageImage();
  char option[5][10] = {"0 OFF" , "1 Div", "2 Div", "3 Div", "4 Div"};
  int8_t Size = 5;

  for (int8_t idx = 0; idx < ( Size ); ++idx )
  {
    tft.setCursor( basesetting->xText, ( 85 + 54 * idx ) );
    tft.print( option[idx] );
  }
  int8_t temp = handleDotsubMenu( basesetting, Size, basesetting->userSetting->autoZero );
  basesetting->userSetting->autoZero =  ( temp == -1 ) ? basesetting->userSetting->autoZero : temp;
  return 1;
}



int8_t clockSetup( struct s_setting *basesetting )
{
  basesetting->classOBJ_set->userInput.userInputArray_Size = 60;
  basesetting->classOBJ_set->userInput.userInputArray = new char[basesetting->classOBJ_set->userInput.userInputArray_Size];
  basesetting->classOBJ_set->init(&basesetting->classOBJ_set->userInput);

  basesetting->classOBJ_set->userInput.fixFormatstruct->fixFormatFlag = 1;
  basesetting->classOBJ_set->userInput.fixFormatstruct->fixFormatArraySize = 15;
  basesetting->classOBJ_set->userInput.fixFormatstruct->fieldType = Date;

  basesetting->classOBJ_set->takeUserInput( &basesetting->classOBJ_set->userInput );
  strcpy( basesetting->userSetting->date ,  basesetting->classOBJ_set->userInput.fixFormatstruct->fixFormatArray );
  Serial.println("World : " + String( basesetting->userSetting->date ));

  Enable_Text_Font()
  tft.fillScreen(TFT_BLACK);
  basesetting->classOBJ_set->settingPageImage();
  settingIndexBlankFullCircle();
  drawSettingString( basesetting, basesetting->stringIdx, 1 );
  settingIndexfillCircle( basesetting->dotIdx );

  return 1;
}


int8_t stabilityFilter( struct s_setting *basesetting )
{
  tft.fillScreen(TFT_BLACK);
  basesetting->classOBJ_set->settingPageImage();
  char option[5][10] = {"Low" , "Medium", "Normal", "High", "Strong"};
  int8_t Size = 5;

  for (int8_t idx = 0; idx < ( Size ); ++idx )
  {
    tft.setCursor( basesetting->xText, ( 85 + 54 * idx ) );
    tft.print( option[idx] );
  }
  int8_t temp = handleDotsubMenu( basesetting, Size, basesetting->userSetting->stabilityFilter );
  basesetting->userSetting->stabilityFilter =  ( temp == -1 ) ? basesetting->userSetting->stabilityFilter : temp;
  return 1;
}


int8_t stopBits( struct s_setting *basesetting )
{
  tft.fillScreen(TFT_BLACK);
  basesetting->classOBJ_set->settingPageImage();
  char option[2][10] = {"1 Bit" , "2 Bits"};
  int8_t Size = 2;

  for (int8_t idx = 0; idx < ( Size ); ++idx )
  {
    tft.setCursor( basesetting->xText, ( 85 + 54 * idx ) );
    tft.print( option[idx] );
  }
  int8_t temp = handleDotsubMenu( basesetting, Size, basesetting->userSetting->stopBits );
  basesetting->userSetting->stopBits = ( temp == -1 ) ? basesetting->userSetting->stopBits : temp;
  return 1;
}


int8_t paritySetting( struct s_setting *basesetting )
{
  tft.fillScreen(TFT_BLACK);
  basesetting->classOBJ_set->settingPageImage();
  char option[3][10] = {"None" , "Odd", "Even"};
  int8_t Size = 3;

  for (int8_t idx = 0; idx < ( Size ); ++idx )
  {
    tft.setCursor( basesetting->xText, ( 85 + 54 * idx ) );
    tft.print( option[idx] );
  }

  int8_t temp = handleDotsubMenu( basesetting, Size, basesetting->userSetting->parity );
  basesetting->userSetting->parity = ( temp == -1 ) ? basesetting->userSetting->parity : temp;
  return 1;
}


int8_t dataBits( struct s_setting *basesetting )
{
  tft.fillScreen(TFT_BLACK);
  basesetting->classOBJ_set->settingPageImage();
  char option[2][10] = {"8 bits" , "7 bits"};
  int8_t Size = 2;

  for (int8_t idx = 0; idx < ( Size ); ++idx )
  {
    tft.setCursor( basesetting->xText, ( 85 + 54 * idx ) );
    tft.print( option[idx] );
  }

  int8_t temp = handleDotsubMenu( basesetting, Size, basesetting->userSetting->dataBits );
  basesetting->userSetting->dataBits = ( temp == -1 ) ? basesetting->userSetting->dataBits : temp;
  return 1;
}

int8_t handleDotsubMenu( struct s_setting *basesetting, int8_t Size, int8_t dotIdx )
{

  settingIndexBlankFullCircle();
  settingIndexfillCircle( dotIdx );
  while ( 1 )
  {
    switch (  basesetting->classOBJ_set->ButtonTouch() )
    {
      case Down :
        ++dotIdx;
        if ( dotIdx >= Size )
        {
          dotIdx = Size - 1;
        }
        else
        {
          settingIndexBlankFullCircle()
          settingIndexfillCircle( dotIdx );
        }
        delay(80);
        break;
      case UP :
        --dotIdx;
        if ( dotIdx < 0 ) dotIdx = 0;
        else
        {
          settingIndexBlankFullCircle()
          settingIndexfillCircle( dotIdx );
        }
        delay(80);
        break;
      case ENT :
        basesetting->classOBJ_set->settingPageImage();
        settingIndexBlankFullCircle();
        drawSettingString( basesetting, basesetting->stringIdx, 1 );
        settingIndexfillCircle( basesetting->dotIdx );
        return dotIdx;
        break;
      case ESC :
        basesetting->classOBJ_set->settingPageImage();
        settingIndexBlankFullCircle();
        drawSettingString( basesetting, basesetting->stringIdx, 1 );
        settingIndexfillCircle( basesetting->dotIdx );
        return -1;
        break;
    }
    yield();
  }//end-while
}


int8_t setBaudrate( struct s_setting *basesetting )
{
  tft.fillScreen(TFT_BLACK);
  basesetting->classOBJ_set->settingPageImage();
  int baud[4] = {2400, 4800, 9600, 115200};
  int8_t Size = sizeof(baud) / sizeof(baud[0]);

  for (int8_t idx = 0; idx < ( Size ); ++idx )
  {
    tft.setCursor( basesetting->xText, ( 85 + 54 * idx ) );
    tft.print( baud[idx] );
  }
  int defaultValueIdx = 0;
  switch (basesetting->userSetting->Baudrate)
  {
    case 2400 : defaultValueIdx  = 0; break;
    case 4800:  defaultValueIdx  = 1; break;
    case 9600:  defaultValueIdx  = 2; break;
    case 115200:  defaultValueIdx  = 3; break;
    default : break;
  }

  switch ( handleDotsubMenu( basesetting, Size, defaultValueIdx ) )
  {
    case 0 : basesetting->userSetting->Baudrate = 2400; break;
    case 1 : basesetting->userSetting->Baudrate = 4800; break;
    case 2 : basesetting->userSetting->Baudrate = 9600; break;
    case 3 : basesetting->userSetting->Baudrate = 115200; break;
    default : break;
  }

  SPL("Buadrate : " + String(basesetting->userSetting->Baudrate) );
  return 1;
}



int8_t scaleInfo( struct s_setting *basesetting )
{
  tft.fillScreen(TFT_BLACK);
  tft.drawString(basesetting->userSetting->scaleInfo[0], 30, 80, 1);
  tft.drawString(basesetting->userSetting->scaleInfo[1], 30, 120, 1);
  delay(5000);
  basesetting->classOBJ_set->settingPageImage();
  settingIndexBlankFullCircle();
  drawSettingString( basesetting, basesetting->stringIdx, 1 );
  settingIndexfillCircle( basesetting->dotIdx );
  return 1;
}


int8_t weighingUnit_lb( struct s_setting *basesetting )
{
  enORDis( basesetting );
  basesetting->userSetting->WeigingUnits_lb =  handleDotsubMenu( basesetting, 2, basesetting->userSetting->WeigingUnits_lb );
  return 1;
}


int8_t weighingUnit_oz( struct s_setting *basesetting )
{
  enORDis( basesetting );
  basesetting->userSetting->WeigingUnits_oz =  handleDotsubMenu( basesetting, 2, basesetting->userSetting->WeigingUnits_oz );
  return 1;
}


int8_t weighingUnit_t( struct s_setting *basesetting )
{
  enORDis( basesetting );
  basesetting->userSetting->WeigingUnits_t =  handleDotsubMenu( basesetting, 2, basesetting->userSetting->WeigingUnits_t );
  return 1;
}


int8_t weighingUnit_gm( struct s_setting *basesetting )
{
  enORDis( basesetting );
  basesetting->userSetting->WeigingUnits_gm =  handleDotsubMenu( basesetting, 2, basesetting->userSetting->WeigingUnits_gm );
  return 1;
}


int8_t weighingUnit_kg( struct s_setting *basesetting )
{
  enORDis(basesetting );
  basesetting->userSetting->WeigingUnits_kg =  handleDotsubMenu( basesetting, 2, basesetting->userSetting->WeigingUnits_kg );
  return 1;
}


bool enableORdisable(char *buf, bool opt )
{
  uint16_t xAxis, yAxis;
  tft.fillScreen(TFT_BLACK);
  tft.drawString(buf, 20, 20, 1);

  tft.drawRoundRect(20, 70, 150, 50, 5, TFT_GREEN);
  tft.drawRoundRect(180, 70, 150, 50, 5, TFT_RED);
  tft.drawString("Enable", 30, 80, 1);
  tft.drawString("Disable", 190, 80, 1);
  tft.drawRoundRect(100, 140, 150, 50, 5, TFT_YELLOW);
  tft.drawString("Save", 120, 150, 1);



  if ( opt )
  {

    tft.fillRoundRect(20, 70, 150, 50, 5, TFT_GREEN);
    tft.drawString("Enable", 30, 80, 1);
  }
  else
  {

    tft.fillRoundRect(180, 70, 150, 50, 5, TFT_RED);
    tft.drawString("Disable", 190, 80, 1);
  }

  while (1)
  {
    if ( tft.getTouch(&xAxis, &yAxis) )
    {
      if (xEN && yEN )
      {
        if ( !opt )
        {
          tft.fillRoundRect(180, 70, 150, 50, 5, TFT_BLACK);
          tft.drawRoundRect(180, 70, 150, 50, 5, TFT_RED);
          tft.drawString("Disable", 190, 80, 1);

          tft.fillRoundRect(20, 70, 150, 50, 5, TFT_GREEN);
          tft.drawString("Enable", 30, 80, 1);
          opt = 1;
        }
      }
      else if (xDN && yDN )
      {
        if ( opt )
        {
          tft.fillRoundRect(20, 70, 150, 50, 5, TFT_BLACK);
          tft.drawRoundRect(20, 70, 150, 50, 5, TFT_GREEN);
          tft.drawString("Enable", 30, 80, 1);

          tft.fillRoundRect(180, 70, 150, 50, 5, TFT_RED);
          tft.drawString("Disable", 190, 80, 1);
          opt = 0;
        }
      }
      else if (xSAVE && ySAVE )
      {
        tft.fillRoundRect(100, 140, 150, 50, 5, TFT_YELLOW);
        tft.drawString("Save", 120, 150, 1);
        delay(500);
        break;
      }
      else
        Serial.println("ELSE Touch :\nxAxis --->>> " + String(xAxis) + "\nyAxis --->>> " + String(yAxis) );
    }
    yield();
  }//end-while
  return opt;
}


void enORDis( struct s_setting *basesetting )
{
  tft.fillScreen(TFT_BLACK);
  basesetting->classOBJ_set->settingPageImage();
  char option[2][10] = {"Disable", "Enable"};
  int8_t Size = 2;

  for (int8_t idx = 0; idx < ( Size ); ++idx )
  {
    tft.setCursor( basesetting->xText, ( 85 + 54 * idx ) );
    tft.print( option[idx] );
  }
}

int8_t countingMode( struct s_setting *basesetting )
{
  enORDis( basesetting );
  int8_t temp =  handleDotsubMenu( basesetting, 2, basesetting->userSetting->countingMode );
  basesetting->userSetting->countingMode = (temp == -1) ? basesetting->userSetting->countingMode : temp;
  SPL("setting menu countingMode : " + String(basesetting->userSetting->countingMode));
  return 1;
}

int8_t checkWeighing( struct s_setting *basesetting )
{
  enORDis( basesetting );
  int8_t temp =  handleDotsubMenu( basesetting, 2, basesetting->userSetting->checkWeighing );
  basesetting->userSetting->checkWeighing = (temp == -1) ? basesetting->userSetting->checkWeighing : temp;

  return 1;
}

int8_t priceComputing( struct s_setting *basesetting )
{
  enORDis( basesetting );
  int8_t temp =  handleDotsubMenu( basesetting, 2, basesetting->userSetting->pricecomputing );
  basesetting->userSetting->pricecomputing = (temp == -1) ? basesetting->userSetting->pricecomputing : temp;
  return 1;
}
