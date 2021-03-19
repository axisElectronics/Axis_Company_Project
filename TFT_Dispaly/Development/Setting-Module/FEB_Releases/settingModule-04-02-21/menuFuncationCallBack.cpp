
#include "settings.h"

#define x1NOLOAD  250
#define x2NOLOAD  471
#define y1NOLOAD  4
#define y2NOLOAD  64

#define xNOLOAD  ( ( xAxis >= x1NOLOAD ) && ( xAxis <= x2NOLOAD ) )
#define yNOLOAD  ( ( yAxis >= y1NOLOAD  ) && ( yAxis <= y2NOLOAD ) )
/**********************************************************************************************/


#define x1LOAD  250
#define x2LOAD  471
#define y1LOAD  89
#define y2LOAD  152

#define xLOAD  ( ( xAxis >= x1LOAD ) && ( xAxis <= x2LOAD ) )
#define yLOAD  ( ( yAxis >= y1LOAD  ) && ( yAxis <= y2LOAD ) )
/**********************************************************************************************/

#define x1CALI_WEIGHT  250
#define x2CALI_WEIGHT  471
#define y1CALI_WEIGHT   172
#define y2CALI_WEIGHT  243

#define xCALI_WEIGHT  ( ( xAxis >= x1CALI_WEIGHT ) && ( xAxis <= x2CALI_WEIGHT ) )
#define yCALI_WEIGHT  ( ( yAxis >= y1CALI_WEIGHT  ) && ( yAxis <= y2CALI_WEIGHT ) )
/**********************************************************************************************/


#define x1SUBMIT  250
#define x2SUBMIT  471
#define y1SUBMIT  262
#define y2SUBMIT  320

#define xSUBMIT  ( ( xAxis >= x1SUBMIT ) && ( xAxis <= x2SUBMIT ) )
#define ySUBMIT  ( ( yAxis >= y1SUBMIT  ) && ( yAxis <= y2SUBMIT ) )
/**********************************************************************************************/



#define NOLOAD 1
#define LOAD   2
#define CALI_WEIGHT 3
#define SUBMIT  4


struct _showDigit
{
  char preValue[10];
  char currentValue[10];
  char xDigitPos[6];
  char yDigitPos[4];
  const void * digitFontSize = LED7SEG_STD18;

} showDigit;


void showDigitfun(struct _showDigit *showDigit, int8_t yPos )
{
  // 1. check coming must not same as previous
//  if ( !strcmp( showDigit->preValue,  showDigit->currentValue ) ) return;

  // 2. Set color and didgit size
  /* Set Font Size */
  tft.setTextSize( 1 );
  tft.setFreeFont( (const GFXfont *)showDigit->digitFontSize );

  // 3. print background digits.
  /* Print Background Digit */
//  tft.setTextColor( makeCustomColor(9, 9, 9), TFT_BLACK); // Fadded Green BackGround
 tft.setTextColor( TFT_GREEN, TFT_BLACK);
 
//  for ( int idx = 5; i >= 0; i++ )
  tft.drawChar( '8', 15 , 65 );
   tft.drawChar( '8', 40 , 65 );
//    tft.drawChar( '8', showDigit->xDigitPos[idx], showDigit->yDigitPos[yPos] );


while(1)
{
  
  yield();
}

  strcpy( showDigit->preValue,  showDigit->currentValue );
  return;
}





bool settingscalibrationImage()
{
  tft.setSwapBytes(true);
  tft.pushImage(0, 0, SettingcalibrationWidth, SettingcalibrationHeight, SettingcalibrationArr );
}

int8_t caliButtonTouch()
{
  uint16_t xAxis, yAxis;
  if ( tft.getTouch(&xAxis, &yAxis) )
  {
    if (xNOLOAD && yNOLOAD ) return NOLOAD; //NOLOAD
    else if (xLOAD && yLOAD ) return LOAD;
    else if (xCALI_WEIGHT && yCALI_WEIGHT ) return CALI_WEIGHT;
    else if ( xSUBMIT && ySUBMIT ) return SUBMIT;
    else
      Serial.println("ELSE Touch :\nxAxis --->>> " + String(xAxis) + "\nyAxis --->>> " + String(yAxis) );
  }
  return 5;
}



void readNoLoadcount( struct s_setting *basesetting )
{
  String noLoad_count  = String(NULL);

  // 1. send command to machine so that machine can send NO-LOAD counts.
  // three value required for this command.
  // i.e <STX>CAL#0,capacity,decimal,eValue<ETX>

  if ( strlen( basesetting->userSetting->scaleCapacity) && strlen(basesetting->userSetting->decimalValue) && strlen(basesetting->userSetting->eValue) )
  {
    char buf[50] = {0};
    strcat(buf, "\2CAL#0,");
    strcat(buf, basesetting->userSetting->scaleCapacity);
    buf[ strlen(buf) ] = ',';
    strcat(buf, basesetting->userSetting->decimalValue);
    buf[ strlen(buf) ] = ',';
    strcat(buf, basesetting->userSetting->eValue);
    buf[ strlen(buf) ] = '\3';

    Serial.println("Command--->>>> : " + String(buf));
    Serial2.write(buf);

    //2. wait till you received "OK" from machine
    while (1)
    {
      //    if ( Serial2.available() > 1 )
      if ( Serial2.readStringUntil('K') )
        break;
      yield();
    }//end-while

  }//end-if

  //3. Now start reading data from machine untill No-Load value button is not pressed
  // print count on serial

  while ( caliButtonTouch() != NOLOAD )
  {
    if ( Serial2.available() > 5 )
      noLoad_count =  Serial2.readStringUntil('=');

    yield();
  }

  Serial.println("NOload : " + String( noLoad_count ) );
  strcpy( basesetting->userSetting->noLoad_count, noLoad_count.c_str() );
  basesetting->userSetting->noLoad_count[ strlen( basesetting->userSetting->noLoad_count ) - 2 ] = '\0';
  Serial.println("NOload : " + String(  basesetting->userSetting->noLoad_count ) );

}

void readLoadcount( struct s_setting *basesetting )
{

  String load_count  =  String(NULL);
  String cali_weight  = String(NULL);

  // 4. put load and again take count values
  while ( caliButtonTouch() != LOAD )
  {
    if ( Serial2.available() > 5 )
      load_count =  Serial2.readStringUntil('=');

    yield();
  }

  Serial.println("Load : " + String( load_count ) );
  strcpy(  basesetting->userSetting->load_count, load_count.c_str() );
  basesetting->userSetting->load_count[ strlen( basesetting->userSetting->load_count ) - 2 ] = '\0';
  Serial.println("Load : " + String(  basesetting->userSetting->load_count ) );

  // 5. calibrated weight

  basesetting->classOBJ_set->userInput.userInputArray_Size = 25; // This Size must be (16 + user requirements)
  basesetting->classOBJ_set->userInput.userInputArray = new char[basesetting->classOBJ_set->userInput.userInputArray_Size];
  basesetting->classOBJ_set->init(&basesetting->classOBJ_set->userInput );

  basesetting->classOBJ_set->userInput.numericSwitchFlag = 1; // only Numeric values are allowed
  basesetting->classOBJ_set->takeUserInput( &basesetting->classOBJ_set->userInput );
  strcpy( basesetting->userSetting->cali_weight ,   &basesetting->classOBJ_set->userInput.userInputArray[15] );

  if ( basesetting->userSetting->cali_weight[ strlen( basesetting->userSetting->cali_weight ) - 1 ] == '_' )
    basesetting->userSetting->cali_weight[ strlen( basesetting->userSetting->cali_weight ) - 1 ] = '\0';


  Enable_Text_Font()
  tft.fillScreen(TFT_BLACK);

  settingscalibrationImage();

  delete[] basesetting->classOBJ_set->userInput.userInputArray;


}

void showCaliWeight( struct s_setting *basesetting )
{
  SPL( "cali_weight : " + String( basesetting->userSetting->cali_weight) );

  while ( 1 )
  {
    if ( caliButtonTouch() == CALI_WEIGHT )
    {
      if ( strlen( basesetting->userSetting->noLoad_count) && strlen(basesetting->userSetting->load_count) && strlen(basesetting->userSetting->cali_weight) )
      {
        char buf[50] = {0};
        strcat(buf, "\2CAL#1,");
        strcat(buf, basesetting->userSetting->noLoad_count);
        buf[ strlen(buf) ] = ',';
        strcat(buf, basesetting->userSetting->load_count);
        buf[ strlen(buf) ] = ',';
        strcat(buf, basesetting->userSetting->cali_weight);
        buf[ strlen(buf) ] = '\3';

        Serial.println("Command--->>>> : " + String(buf));
        Serial2.write(buf);

        //2. wait till you received "OK" from machine
        while (1)
        {
          if ( Serial2.readStringUntil('K') )
            break;
          yield();
        }//end-while

      }//end-if
      tft.fillScreen(TFT_BLACK);
      settingscalibrationImage();
    }
    else if ( caliButtonTouch() == SUBMIT  )
    {
      Enable_Text_Font()
      tft.fillScreen(TFT_BLACK);
      basesetting->classOBJ_set->settingPageImage();
      settingIndexBlankFullCircle();
      drawSettingString( basesetting, basesetting->stringIdx, 1 );
      settingIndexfillCircle( basesetting->dotIdx );
      return;
    }

    yield();
  }//end-while
}//end-showCaliWeight


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

int8_t calibrateSetup( struct s_setting *basesetting )
{

  SPL("Hello calibration : ");
  SPL("capacity : " + String( basesetting->userSetting->scaleCapacity ) );
  SPL("eValue : " + String( basesetting->userSetting->eValue ) );
  SPL("Decimal : " + String( basesetting->userSetting->decimalValue ) );

  settingscalibrationImage();

  showDigitfun( &showDigit, 1 );

  readNoLoadcount( basesetting);
  readLoadcount( basesetting );
  showCaliWeight( basesetting );
  delay(500);
  int8_t cnt = 0;
  while ( cnt < 5 )
  {
    SPL( "final weight : " + String( Serial2.readStringUntil('=') ) );
    cnt++;
  }

  return 1;
}

int8_t decimalSetup( struct s_setting *basesetting )
{
  basesetting->classOBJ_set->userInput.userInputArray_Size = 20; // This Size must be (16 + user requirements)
  basesetting->classOBJ_set->userInput.userInputArray = new char[basesetting->classOBJ_set->userInput.userInputArray_Size];
  basesetting->classOBJ_set->init(&basesetting->classOBJ_set->userInput );

  basesetting->classOBJ_set->userInput.numericSwitchFlag = 1; // only Numeric values are allowed
  basesetting->classOBJ_set->takeUserInput( &basesetting->classOBJ_set->userInput );
  strcpy( basesetting->userSetting->decimalValue ,   &basesetting->classOBJ_set->userInput.userInputArray[15] );

  if ( basesetting->userSetting->decimalValue[ strlen( basesetting->userSetting->decimalValue ) - 1 ] == '_' )
    basesetting->userSetting->decimalValue[ strlen( basesetting->userSetting->decimalValue ) - 1 ] = '\0';

  //  SPL("decimalValue : " + String( basesetting->userSetting->decimalValue));
  Enable_Text_Font()
  tft.fillScreen(TFT_BLACK);
  basesetting->classOBJ_set->settingPageImage();
  settingIndexBlankFullCircle();
  drawSettingString( basesetting, basesetting->stringIdx, 1 );
  settingIndexfillCircle( basesetting->dotIdx );

  delete[] basesetting->classOBJ_set->userInput.userInputArray;
  return 1;
}



int8_t eValueSetup( struct s_setting *basesetting )
{
  basesetting->classOBJ_set->userInput.userInputArray_Size = 20; // This Size must be (16 + user requirements)
  basesetting->classOBJ_set->userInput.userInputArray = new char[basesetting->classOBJ_set->userInput.userInputArray_Size];
  basesetting->classOBJ_set->init(&basesetting->classOBJ_set->userInput );

  basesetting->classOBJ_set->userInput.numericSwitchFlag = 1; // only Numeric values are allowed
  basesetting->classOBJ_set->takeUserInput( &basesetting->classOBJ_set->userInput );
  strcpy( basesetting->userSetting->eValue , &basesetting->classOBJ_set->userInput.userInputArray[15] );

  if ( basesetting->userSetting->eValue[ strlen( basesetting->userSetting->eValue ) - 1 ] == '_' )
    basesetting->userSetting->eValue[ strlen( basesetting->userSetting->eValue ) - 1 ] = '\0';

  //  SPL("evalue : " + String( basesetting->userSetting->eValue));
  Enable_Text_Font()
  tft.fillScreen(TFT_BLACK);
  basesetting->classOBJ_set->settingPageImage();
  settingIndexBlankFullCircle();
  drawSettingString( basesetting, basesetting->stringIdx, 1 );
  settingIndexfillCircle( basesetting->dotIdx );

  delete[] basesetting->classOBJ_set->userInput.userInputArray;
  return 1;
}


int8_t capacitySetup( struct s_setting *basesetting )
{
  basesetting->classOBJ_set->userInput.userInputArray_Size = 25; // This Size must be (15 + user requirements)
  basesetting->classOBJ_set->userInput.userInputArray = new char[basesetting->classOBJ_set->userInput.userInputArray_Size];
  basesetting->classOBJ_set->init(&basesetting->classOBJ_set->userInput );

  basesetting->classOBJ_set->userInput.numericSwitchFlag = 1; // only Numeric values are allowed
  basesetting->classOBJ_set->takeUserInput( &basesetting->classOBJ_set->userInput );
  strcpy( basesetting->userSetting->scaleCapacity ,   &basesetting->classOBJ_set->userInput.userInputArray[15] );

  if ( basesetting->userSetting->scaleCapacity[ strlen( basesetting->userSetting->scaleCapacity ) - 1 ] == '_' )
    basesetting->userSetting->scaleCapacity[ strlen( basesetting->userSetting->scaleCapacity ) - 1 ] = '\0';

  Enable_Text_Font()
  tft.fillScreen(TFT_BLACK);
  basesetting->classOBJ_set->settingPageImage();
  settingIndexBlankFullCircle();
  drawSettingString( basesetting, basesetting->stringIdx, 1 );
  settingIndexfillCircle( basesetting->dotIdx );

  delete[] basesetting->classOBJ_set->userInput.userInputArray;
  return 1;
}


int8_t clockSetup( struct s_setting *basesetting )
{
  basesetting->classOBJ_set->userInput.userInputArray_Size = 10;
  basesetting->classOBJ_set->userInput.userInputArray = new char[basesetting->classOBJ_set->userInput.userInputArray_Size];
  basesetting->classOBJ_set->init(&basesetting->classOBJ_set->userInput);

  basesetting->classOBJ_set->userInput.fixFormatstruct->fixFormatFlag = 1;
  basesetting->classOBJ_set->userInput.fixFormatstruct->fixFormatArraySize = 15;
  basesetting->classOBJ_set->userInput.fixFormatstruct->fieldType = Date;

  basesetting->classOBJ_set->takeUserInput( &basesetting->classOBJ_set->userInput );
  strcpy( basesetting->userSetting->date ,  basesetting->classOBJ_set->userInput.fixFormatstruct->fixFormatArray );
  //  Serial.println("World : " + String( basesetting->userSetting->date ));

  Enable_Text_Font()
  tft.fillScreen(TFT_BLACK);
  basesetting->classOBJ_set->settingPageImage();
  settingIndexBlankFullCircle();
  drawSettingString( basesetting, basesetting->stringIdx, 1 );
  settingIndexfillCircle( basesetting->dotIdx );

  delete[] basesetting->classOBJ_set->userInput.userInputArray;
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

  //  SPL("Buadrate : " + String(basesetting->userSetting->Baudrate) );
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
  //  SPL("setting menu countingMode : " + String(basesetting->userSetting->countingMode));
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
