
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

#define NOLOAD       1
#define LOAD         2
#define CALI_WEIGHT  3
#define SUBMIT       4


/******************************************************************
   @makeCustomColor :
   (#) we can make custom color accordingly changed the value of RGB color.
 *                                                                *
 *******************************************************************/

#define makeCustomColor( red, green , blue ) (((red & 0xf8)<<8) + ((green & 0xfc)<<3) + (blue>>3))

int8_t saveAsDefault( struct s_setting *basesetting )
{

  // 1. Application Setup
  basesetting->classOBJ_dftsett->userScaleSetup.application[WeighingMode_] =  basesetting->userSetting->weighingMode;
  basesetting->classOBJ_dftsett->userScaleSetup.application[CountingMode_] =  basesetting->userSetting->countingMode;
  basesetting->classOBJ_dftsett->userScaleSetup.application[CheckWeighing_] = basesetting->userSetting->checkWeighing;
  basesetting->classOBJ_dftsett->userScaleSetup.application[PriceWeighing_] = basesetting->userSetting->pricecomputing;

  // 2. scale setup
  strcpy(  basesetting->classOBJ_dftsett->userScaleSetup.dateString, basesetting->userSetting->date );
  basesetting->classOBJ_dftsett->userScaleSetup.autoZero = basesetting->userSetting->autoZero;
  basesetting->classOBJ_dftsett->userScaleSetup.stabilityFactor = basesetting->userSetting->stabilityFilter;
  // COM parameters
  basesetting->classOBJ_dftsett->userScaleSetup.comPara[BAUDRATE] = basesetting->userSetting->Baudrate;
  basesetting->classOBJ_dftsett->userScaleSetup.comPara[DataBits] = basesetting->userSetting->dataBits;
  basesetting->classOBJ_dftsett->userScaleSetup.comPara[Parity]   = basesetting->userSetting->parity;
  basesetting->classOBJ_dftsett->userScaleSetup.comPara[StopBits] = basesetting->userSetting->stopBits;

  //3. Weighing Units
  basesetting->classOBJ_dftsett->userScaleSetup.WeighingUnits[kg] = basesetting->userSetting->WeigingUnits_kg;
  basesetting->classOBJ_dftsett->userScaleSetup.WeighingUnits[gm] = basesetting->userSetting->WeigingUnits_gm;
  basesetting->classOBJ_dftsett->userScaleSetup.WeighingUnits[tons] = basesetting->userSetting->WeigingUnits_t;
  basesetting->classOBJ_dftsett->userScaleSetup.WeighingUnits[Ounces] = basesetting->userSetting->WeigingUnits_oz;
  basesetting->classOBJ_dftsett->userScaleSetup.WeighingUnits[Pounds] = basesetting->userSetting->WeigingUnits_lb;

  //4. scale Information
  strcpy(  basesetting->classOBJ_dftsett->userScaleSetup.scaleInformation[ developer ], "Vivek Yadav" );
  strcpy(  basesetting->classOBJ_dftsett->userScaleSetup.scaleInformation[ baseVersion ], "novaTouch0.0.0" );
  strcpy(  basesetting->classOBJ_dftsett->userScaleSetup.scaleInformation[ modifiedVersion ], "Under Dev" );

  //5. calibration
  strcpy(  basesetting->classOBJ_dftsett->userScaleSetup.capacity, basesetting->userSetting->scaleCapacity );
  strcpy(  basesetting->classOBJ_dftsett->userScaleSetup.eVlaue, basesetting->userSetting->eValue )  ;
  strcpy(  basesetting->classOBJ_dftsett->userScaleSetup.decimal, basesetting->userSetting->decimalValue )  ;
  strcpy(  basesetting->classOBJ_dftsett->userScaleSetup.calibartionWeight, basesetting->userSetting->cali_weight )  ;

  SPL("save as default-dataBits  : " + String( basesetting->userSetting->dataBits ) );

  return basesetting->classOBJ_dftsett->saveAsDefault();

}


int8_t factoryDefault( struct s_setting *basesetting )
{
  return basesetting->classOBJ_dftsett->factoryDefault();
}



void realTimeWeight(struct _showDigit *showDigit, int yPos )
{

  SPL("realTime : " + String( showDigit->currentValue ) );

  // 1. check coming must not same as previous
  if ( !strcmp( showDigit->preValue,  showDigit->currentValue ) ) return;

  // 2. Set color and didgit size
  /* Set Font Size */
  tft.setTextSize( 1 );
  tft.setFreeFont( (const GFXfont *)showDigit->digitFontSize );

  //3. remove all dot other than valid dot
  for ( int8_t idx = 0; idx < 7; ++idx )
    tft.fillCircle( ( 42 + 30 * idx ), 298, 2, makeCustomColor(9, 9, 9) );

  // 4. print background digits.
  /* Print Background Digit */
  tft.setTextColor( makeCustomColor(9, 9, 9), TFT_BLACK); // Fadded Green BackGround
  for ( int8_t idx = 0; idx < 7; ++idx )
  {
    if ( showDigit->currentValue[idx] != showDigit->preValue[idx] )
      tft.drawChar( '8', ( 16 + (idx) * 30 ) , yPos );
  }

  //5. print dot at given position.
  tft.fillCircle( ( 42 + 30 * showDigit->dotpos ), 298, 2, TFT_GREEN );

  // 6. checking leading Zeroes
  int8_t idx = 0;
  while ( showDigit->currentValue[idx] == '0') ++idx;

  // 7. print those digit only which is not same as previous.
  for ( --idx; idx < 7; ++idx )
  {
    if ( ( showDigit->currentValue[idx] != showDigit->preValue[idx] )  )
    {
      tft.setTextColor( makeCustomColor(9, 9, 9), TFT_BLACK);
      tft.drawChar( '8', ( 16 + (idx * 30) ) , yPos );
      tft.setTextColor( TFT_GREEN, TFT_BLACK);
      tft.drawChar( showDigit->currentValue[idx], ( 16 +  (idx * 30) ) , yPos );
    }
  }

  // 8. update prevalue with new current value.
  strcpy( showDigit->preValue,  showDigit->currentValue );
  return ;
}//end-


void showDigitfun(struct _showDigit *showDigit, int yPos )
{
  int8_t len = strlen(showDigit->currentValue);
  int8_t digitIndex = 7;

  // 1. check coming must not same as previous
  if ( !strcmp( showDigit->preValue,  showDigit->currentValue ) ) return;

  // 2. Set color and didgit size
  /* Set Font Size */
  tft.setTextSize( 1 );
  tft.setFreeFont( (const GFXfont *)showDigit->digitFontSize );

  // 3. print background digits.
  /* Print Background Digit */
  tft.setTextColor( makeCustomColor(9, 9, 9), TFT_BLACK); // Fadded Green BackGround
  for ( int8_t idx = 0; idx < 8; ++idx )
  {
    if (  showDigit->currentValue[idx] == '.' );
    else if ( showDigit->currentValue[idx] != showDigit->preValue[idx] )
      tft.drawChar( '8', ( 16 + (idx) * 26 ) , yPos );
  }

  //4. checking leading Zeroes
  int8_t idx = 0;
  while (showDigit->currentValue[idx] == '0') ++idx;

  // 5. print those digit only which is not same as previous.
  for ( ; idx < 8; ++idx )
  {
    if (  showDigit->currentValue[idx] == '.'  ); // Do Nothing
    else if ( ( showDigit->currentValue[idx] != showDigit->preValue[idx] )  )
    {
      tft.setTextColor( makeCustomColor(9, 9, 9), TFT_BLACK);
      tft.drawChar( '8', ( 16 + (idx * 26) ) , yPos );
      tft.setTextColor( TFT_GREEN, TFT_BLACK);
      tft.drawChar( showDigit->currentValue[idx], ( 16 +  (idx * 26) ) , yPos );
    }
  }

  //6. update prevalue with new current value.
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
      if ( Serial2.readStringUntil('K') )
        break;
      yield();
    }//end-while

  }//end-if

  //3. Now start reading data from machine untill No-Load value button is not pressed
  // print count on serial
  memset( basesetting->showDigit->currentValue, '\0', 10);
  while ( caliButtonTouch() != NOLOAD )
  {
    if ( Serial2.available() > 5 )
    {
      noLoad_count =  Serial2.readStringUntil('=');
      basesetting->showDigit->currentValue[0] = '0';
      strcpy( & basesetting->showDigit->currentValue[1], noLoad_count.c_str() );
      basesetting->showDigit->currentValue[strlen(  basesetting->showDigit->currentValue) - 2] = '\0';
      showDigitfun(  basesetting->showDigit, 65 );

    }

    yield();
  }


  memset( basesetting->showDigit->currentValue, '\0', 10);
  memset( basesetting->showDigit->preValue, '\0', 10);

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
  memset( basesetting->showDigit->currentValue, '\0', 10);
  basesetting->showDigit->currentValue[0] = '0';

  while ( caliButtonTouch() != LOAD )
  {
    if ( Serial2.available() > 5 )
    {
      load_count =  Serial2.readStringUntil('=');

      strcpy( & basesetting->showDigit->currentValue[1], load_count.c_str() );
      basesetting->showDigit->currentValue[strlen(  basesetting->showDigit->currentValue) - 2] = '\0';
      showDigitfun(  basesetting->showDigit, 140 );
    }

    yield();
  }
  memset( basesetting->showDigit->currentValue, '\0', 10);
  memset( basesetting->showDigit->preValue, '\0', 10);

  Serial.println("Load : " + String( load_count ) );
  strcpy(  basesetting->userSetting->load_count, load_count.c_str() );
  basesetting->userSetting->load_count[ strlen( basesetting->userSetting->load_count ) - 2 ] = '\0';
  Serial.println("Load : " + String(  basesetting->userSetting->load_count ) );

  // 5. calibrated weight take data from User

  basesetting->classOBJ_set->userInput.userInputArray_Size = 25; // This Size must be (16 + user requirements)
  basesetting->classOBJ_set->userInput.userInputArray = new char[basesetting->classOBJ_set->userInput.userInputArray_Size];
  basesetting->classOBJ_set->init(&basesetting->classOBJ_set->userInput );

  basesetting->classOBJ_set->userInput.numericSwitchFlag = 1; // only Numeric values are allowed
  {
    // read previous capacity from file system
    String temp = basesetting->classOBJ_dftsett->_getCalibrationWeight();
    strcpy( basesetting->userSetting->cali_weight, temp.c_str() );
  }

  basesetting->classOBJ_set->takeUserInput( &basesetting->classOBJ_set->userInput, basesetting->userSetting->cali_weight );
  strcpy( basesetting->userSetting->cali_weight ,   &basesetting->classOBJ_set->userInput.userInputArray[15] );

  if ( basesetting->userSetting->cali_weight[ strlen( basesetting->userSetting->cali_weight ) - 1 ] == '_' )
    basesetting->userSetting->cali_weight[ strlen( basesetting->userSetting->cali_weight ) - 1 ] = '\0';

  tft.fillScreen(TFT_BLACK);
  settingscalibrationImage();

  // 6.start showing data coming from machine untill submit button is not pressed.
  memset( basesetting->showDigit->preValue, '\0', 10 );
  basesetting->showDigit->currentValue[0] = '0';
  strcpy( & basesetting->showDigit->currentValue[1], basesetting->userSetting->noLoad_count );
  showDigitfun(  basesetting->showDigit, 65 );

  memset( basesetting->showDigit->preValue, '\0', 10 );
  basesetting->showDigit->currentValue[0] = '0';
  strcpy( & basesetting->showDigit->currentValue[1], basesetting->userSetting->load_count );
  showDigitfun(  basesetting->showDigit, 140 );

  memset( basesetting->showDigit->preValue, '\0', 10 );
  memset( basesetting->showDigit->currentValue, '\0', 10 );
  memset(  basesetting->showDigit->currentValue, '0', 8 - strlen( basesetting->userSetting->cali_weight ) );
  strcpy( & basesetting->showDigit->currentValue[ 8 - strlen( basesetting->userSetting->cali_weight) ], basesetting->userSetting->cali_weight );

  showDigitfun(  basesetting->showDigit, 215 );
  memset( basesetting->showDigit->preValue, '\0', 10 );
  memset( basesetting->showDigit->currentValue, '\0', 10 );


  Enable_Text_Font()
  delete[] basesetting->classOBJ_set->userInput.userInputArray;

}

void showCaliWeight( struct s_setting *basesetting )
{
  String CalibratedWeight = String(NULL);

  while ( 1 )
  {
    if ( caliButtonTouch() == CALI_WEIGHT )
    {
      if ( strlen( basesetting->userSetting->noLoad_count) && strlen(basesetting->userSetting->load_count) && strlen(basesetting->userSetting->cali_weight) )
      {
        // 1. Send calibartion command
        char buf[50] = {0};
        strcat(buf, "\2CAL#1,");
        strcat(buf, basesetting->userSetting->noLoad_count);
        buf[ strlen(buf) ] = ',';
        strcat(buf, basesetting->userSetting->load_count);
        buf[ strlen(buf) ] = ',';
        strcat(buf, basesetting->userSetting->cali_weight);
        buf[ strlen(buf) ] = '\3';

        //        Serial.println("Command--->>>> : " + String(buf));
        Serial2.write(buf);

        //2. wait till you received "OK" from machine
        while (1)
        {
          if ( Serial2.readStringUntil('K') )
            break;
          yield();
        }//end-while

      }//end-if

      // 3. back to main screen again. So that we can show calibrated data on screen which iscoming from machine
      tft.fillScreen(TFT_BLACK);
      settingscalibrationImage();

      // 4.start showing data coming from machine untill submit button is not pressed.
      memset( basesetting->showDigit->preValue, '\0', 10 );
      basesetting->showDigit->currentValue[0] = '0';
      strcpy( & basesetting->showDigit->currentValue[1], basesetting->userSetting->noLoad_count );
      showDigitfun(  basesetting->showDigit, 65 );

      memset( basesetting->showDigit->preValue, '\0', 10 );
      basesetting->showDigit->currentValue[0] = '0';
      strcpy( & basesetting->showDigit->currentValue[1], basesetting->userSetting->load_count );
      showDigitfun(  basesetting->showDigit, 140 );

      memset( basesetting->showDigit->preValue, '\0', 10 );
      memset( basesetting->showDigit->currentValue, '\0', 10 );
      memset(  basesetting->showDigit->currentValue, '0', 8 - strlen( basesetting->userSetting->cali_weight ) );
      strcpy( & basesetting->showDigit->currentValue[ 8 - strlen( basesetting->userSetting->cali_weight) ], basesetting->userSetting->cali_weight );

      showDigitfun(  basesetting->showDigit, 215 );
      memset( basesetting->showDigit->preValue, '\0', 10 );
      memset( basesetting->showDigit->currentValue, '\0', 10 );

      // 5. Show data untill submit button is not pressed
      char temp[10];
      int8_t dotpos = 0;
      while ( caliButtonTouch() != SUBMIT )
      {
HERE:
        if ( Serial2.available() > 5 )
        {

          CalibratedWeight =  Serial2.readStringUntil('=');

          strcpy(  basesetting->showDigit->currentValue, CalibratedWeight.c_str() );
          basesetting->showDigit->currentValue[strlen(  basesetting->showDigit->currentValue) - 2] = '\0';

          //6. remove dot from Array

          dotpos = 0;
          memset(temp, '\0', 10);
          while ( basesetting->showDigit->currentValue[dotpos] != '.') {
            if ( ++dotpos >= 10 ) goto HERE;
          }
          basesetting->showDigit->dotpos = dotpos;
          strncpy(temp,  basesetting->showDigit->currentValue, dotpos );
          strcat(temp, & basesetting->showDigit->currentValue[++dotpos] );

          memset( basesetting->showDigit->currentValue, '\0', 10);
          basesetting->showDigit->currentValue[0] = '0';
          strcpy( & basesetting->showDigit->currentValue[1], temp );

          realTimeWeight(  basesetting->showDigit, 290 );

        }//end-if
        yield();
      }//end-while

      // 6. back to menu screen
      Enable_Text_Font()
      tft.fillScreen(TFT_BLACK);
      basesetting->classOBJ_set->settingPageImage();
      settingIndexBlankFullCircle();
      drawSettingString( basesetting, basesetting->stringIdx, 1 );
      settingIndexfillCircle( basesetting->dotIdx );
      return;
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
    }//end-else if
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

  String tempStr = basesetting->classOBJ_dftsett->_getAutoZero();
  SPL("\nAutoZero : " + String(tempStr) );

  if ( tempStr == option[0]) basesetting->userSetting->autoZero = 0;
  else if ( tempStr == option[1]) basesetting->userSetting->autoZero = 1;
  else if ( tempStr == option[2]) basesetting->userSetting->autoZero = 2;
  else if ( tempStr == option[3]) basesetting->userSetting->autoZero = 3;
  else if ( tempStr == option[4]) basesetting->userSetting->autoZero = 4;


  int8_t temp = handleDotsubMenu( basesetting, Size, basesetting->userSetting->autoZero );
  basesetting->userSetting->autoZero =  ( temp == -1 ) ? basesetting->userSetting->autoZero : temp;
  return 1;
}

int8_t calibrateSetup( struct s_setting *basesetting )
{
  settingscalibrationImage();

  readNoLoadcount( basesetting);
  readLoadcount( basesetting );
  showCaliWeight( basesetting );

  return 1;
}

int8_t decimalSetup( struct s_setting *basesetting )
{
  basesetting->classOBJ_set->userInput.userInputArray_Size = 17; // This Size must be (16 + user requirements)
  basesetting->classOBJ_set->userInput.userInputArray = new char[basesetting->classOBJ_set->userInput.userInputArray_Size];
  basesetting->classOBJ_set->init(&basesetting->classOBJ_set->userInput );

  basesetting->classOBJ_set->userInput.numericSwitchFlag = 1; // only Numeric values are allowed
  {
    // read previous capacity from file system
    String temp = basesetting->classOBJ_dftsett->_getDecimal();
    strcpy( basesetting->userSetting->decimalValue, temp.c_str() );
  }
  basesetting->classOBJ_set->takeUserInput( &basesetting->classOBJ_set->userInput, basesetting->userSetting->decimalValue );
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
  basesetting->classOBJ_set->userInput.userInputArray_Size = 17; // This Size must be (16 + user requirements)
  basesetting->classOBJ_set->userInput.userInputArray = new char[basesetting->classOBJ_set->userInput.userInputArray_Size];
  basesetting->classOBJ_set->init(&basesetting->classOBJ_set->userInput );

  basesetting->classOBJ_set->userInput.numericSwitchFlag = 1; // only Numeric values are allowed
  {
    // read previous capacity from file system
    String temp = basesetting->classOBJ_dftsett->_getEvalue();
    strcpy( basesetting->userSetting->eValue, temp.c_str() );
  }
  basesetting->classOBJ_set->takeUserInput( &basesetting->classOBJ_set->userInput, basesetting->userSetting->eValue );
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
  {
    // read previous capacity from file system
    String temp = basesetting->classOBJ_dftsett->_getCapacity();
    strcpy( basesetting->userSetting->scaleCapacity, temp.c_str() );
      SPL("capacity :-->> " + String( basesetting->userSetting->scaleCapacity ) );
  }

  basesetting->classOBJ_set->takeUserInput( &basesetting->classOBJ_set->userInput, basesetting->userSetting->scaleCapacity );
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
  char temp[10];
  basesetting->classOBJ_set->userInput.userInputArray_Size = 10;
  basesetting->classOBJ_set->userInput.userInputArray = temp;
  basesetting->classOBJ_set->init(&basesetting->classOBJ_set->userInput);

  basesetting->classOBJ_set->userInput.fixFormatstruct->fixFormatFlag = 1;
  basesetting->classOBJ_set->userInput.fixFormatstruct->fixFormatArraySize = 15;
  basesetting->classOBJ_set->userInput.fixFormatstruct->fieldType = Date;

  strcpy( basesetting->userSetting->date, basesetting->classOBJ_dftsett->_getDate().c_str() );
  Serial.println("Date : " + String(  basesetting->classOBJ_dftsett->_getDate() ));
  basesetting->classOBJ_set->takeUserInput( &basesetting->classOBJ_set->userInput, basesetting->userSetting->date );

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
  {
    String temp = basesetting->classOBJ_dftsett->_getStability();
    basesetting->userSetting->stabilityFilter = (temp == "Low") ? 0 : (temp == "Medium") ? 1 : (temp == "Normal") ? 2 : (temp == "High") ? 3 : (temp == "Strong") ? 4 : basesetting->userSetting->stabilityFilter;
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

  {
    String temp = basesetting->classOBJ_dftsett->_getCOMPara( StopBits );
    basesetting->userSetting->stopBits = (temp == "1 bit") ? 0 : (temp == "2 bits") ? 1 :  basesetting->userSetting->stopBits ;
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

  {
    String temp = basesetting->classOBJ_dftsett->_getCOMPara( Parity );
    basesetting->userSetting->parity = (temp == "None") ? 0 : (temp == "odd") ? 1 : (temp == "Even") ? 2 : basesetting->userSetting->parity;
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

  {
    String temp = basesetting->classOBJ_dftsett->_getCOMPara( DataBits );
    Serial.println("databits : " + String(temp));
    basesetting->userSetting->dataBits = (temp == "8 bits") ? 0 : (temp == "7 bits") ? 1 :  basesetting->userSetting->dataBits;
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
  String temp =  basesetting->classOBJ_dftsett->_getCOMPara( BAUDRATE );
  SPL( "Setting-Baudrate-temp : " + String( temp ) );
  basesetting->userSetting->Baudrate = ( temp == "2400" ) ? 2400 : ( temp == "4800" ) ? 4800 : ( temp == "9600" ) ? 9600 : ( temp == "115200" ) ? 115200 : basesetting->userSetting->Baudrate;
  SPL( "Setting-Baudrate : " + String( basesetting->userSetting->Baudrate ) );

  switch ( basesetting->userSetting->Baudrate )
  {
    case 2400   :    basesetting->userSetting->Baudrate  = 0; break;
    case 4800   :    basesetting->userSetting->Baudrate  = 1; break;
    case 9600   :    basesetting->userSetting->Baudrate  = 2; break;
    case 115200 :    basesetting->userSetting->Baudrate  = 3; break;
    default     :    break;
  }

  switch ( handleDotsubMenu( basesetting, Size, basesetting->userSetting->Baudrate ) )
  {
    case 0   :    basesetting->userSetting->Baudrate  = 0; break;
    case 1   :    basesetting->userSetting->Baudrate  = 1; break;
    case 2   :    basesetting->userSetting->Baudrate  = 2; break;
    case 3   :    basesetting->userSetting->Baudrate  = 3; break;
    default  :    break;
  }

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
  basesetting->userSetting->WeigingUnits_lb = ( basesetting->classOBJ_dftsett->_getWeighingUnit() == "Pounds(lb)" ? 1 : 0 );
  if (  basesetting->userSetting->WeigingUnits_lb )
  {
    basesetting->userSetting->WeigingUnits_kg = 0;
    basesetting->userSetting->WeigingUnits_gm = 0;
    basesetting->userSetting->WeigingUnits_t = 0;
    basesetting->userSetting->WeigingUnits_oz = 0;
  }

  int8_t temp = handleDotsubMenu( basesetting, 2, basesetting->userSetting->WeigingUnits_lb );
  basesetting->userSetting->WeigingUnits_lb  = ( temp == -1 ) ?  basesetting->userSetting->WeigingUnits_lb : temp;

  return 1;
}


int8_t weighingUnit_oz( struct s_setting *basesetting )
{
  enORDis( basesetting );
  basesetting->userSetting->WeigingUnits_oz = ( basesetting->classOBJ_dftsett->_getWeighingUnit() == "Ounces(oz)" ? 1 : 0 );
  if (  basesetting->userSetting->WeigingUnits_oz )
  {
    basesetting->userSetting->WeigingUnits_kg = 0;
    basesetting->userSetting->WeigingUnits_gm = 0;
    basesetting->userSetting->WeigingUnits_t = 0;
    //    basesetting->userSetting->WeigingUnits_oz = 0;
    basesetting->userSetting->WeigingUnits_lb = 0;
  }
  int8_t temp = handleDotsubMenu( basesetting, 2, basesetting->userSetting->WeigingUnits_oz );
  basesetting->userSetting->WeigingUnits_oz  = ( temp == -1 ) ?  basesetting->userSetting->WeigingUnits_oz : temp;

  return 1;
}


int8_t weighingUnit_t( struct s_setting *basesetting )
{
  enORDis( basesetting );
  basesetting->userSetting->WeigingUnits_t = ( basesetting->classOBJ_dftsett->_getWeighingUnit() == "Tons" ? 1 : 0 );
  if (  basesetting->userSetting->WeigingUnits_t )
  {
    basesetting->userSetting->WeigingUnits_kg = 0;
    basesetting->userSetting->WeigingUnits_gm = 0;
    //    basesetting->userSetting->WeigingUnits_t = 0;
    basesetting->userSetting->WeigingUnits_oz = 0;
    basesetting->userSetting->WeigingUnits_lb = 0;
  }
  int8_t temp = handleDotsubMenu( basesetting, 2, basesetting->userSetting->WeigingUnits_t );
  basesetting->userSetting->WeigingUnits_t  = ( temp == -1 ) ?  basesetting->userSetting->WeigingUnits_t : temp;

  return 1;
}


int8_t weighingUnit_gm( struct s_setting *basesetting )
{
  enORDis( basesetting );
  basesetting->userSetting->WeigingUnits_gm = ( basesetting->classOBJ_dftsett->_getWeighingUnit() == "gm" ? 1 : 0 );
  if (  basesetting->userSetting->WeigingUnits_gm )
  {
    basesetting->userSetting->WeigingUnits_kg = 0;
    //    basesetting->userSetting->WeigingUnits_gm = 0;
    basesetting->userSetting->WeigingUnits_t = 0;
    basesetting->userSetting->WeigingUnits_oz = 0;
    basesetting->userSetting->WeigingUnits_lb = 0;
  }
  int8_t temp = handleDotsubMenu( basesetting, 2, basesetting->userSetting->WeigingUnits_gm );
  basesetting->userSetting->WeigingUnits_gm  = ( temp == -1 ) ?  basesetting->userSetting->WeigingUnits_gm : temp;

  return 1;
}


int8_t weighingUnit_kg( struct s_setting *basesetting )
{
  enORDis(basesetting );

  basesetting->userSetting->WeigingUnits_kg = ( basesetting->classOBJ_dftsett->_getWeighingUnit() == "kg" ? 1 : 0 );
  if (  basesetting->userSetting->WeigingUnits_kg )
  {
    basesetting->userSetting->WeigingUnits_gm = 0;
    basesetting->userSetting->WeigingUnits_t  = 0;
    basesetting->userSetting->WeigingUnits_oz = 0;
    basesetting->userSetting->WeigingUnits_lb = 0;
  }
  int8_t temp = handleDotsubMenu( basesetting, 2, basesetting->userSetting->WeigingUnits_kg );
  basesetting->userSetting->WeigingUnits_kg  = ( temp == -1 ) ?  basesetting->userSetting->WeigingUnits_kg : temp;

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

  for ( int8_t idx = 0; idx < ( Size ); ++idx )
  {
    tft.setCursor( basesetting->xText, ( 85 + 54 * idx ) );
    tft.print( option[idx] );
  }
}


int8_t weighingMode( struct s_setting *basesetting )
{
  enORDis( basesetting );
  basesetting->userSetting->weighingMode = ( basesetting->classOBJ_dftsett->_getApplication() == "Weighing Mode" ) ? 1 : 0;

  int8_t temp =  handleDotsubMenu( basesetting, 2, basesetting->userSetting->weighingMode );
  basesetting->userSetting->weighingMode = (temp == -1) ? basesetting->userSetting->weighingMode : temp;

  return 1;
}


int8_t countingMode( struct s_setting *basesetting )
{
  enORDis( basesetting );
  basesetting->userSetting->countingMode = ( basesetting->classOBJ_dftsett->_getApplication() == "Counting Mode" ) ? 1 : 0;

  int8_t temp =  handleDotsubMenu( basesetting, 2, basesetting->userSetting->countingMode );
  basesetting->userSetting->countingMode = (temp == -1) ? basesetting->userSetting->countingMode : temp;

  return 1;
}

int8_t checkWeighing( struct s_setting *basesetting )
{
  enORDis( basesetting );

  basesetting->userSetting->checkWeighing = ( basesetting->classOBJ_dftsett->_getApplication() == "Check Weighing" ) ? 1 : 0;

  int8_t temp =  handleDotsubMenu( basesetting, 2, basesetting->userSetting->checkWeighing );
  basesetting->userSetting->checkWeighing = (temp == -1) ? basesetting->userSetting->checkWeighing : temp;
  return 1;
}

int8_t priceComputing( struct s_setting *basesetting )
{
  enORDis( basesetting );

  basesetting->userSetting->pricecomputing = ( basesetting->classOBJ_dftsett->_getApplication() == "Price Weighing" ) ? 1 : 0;

  int8_t temp =  handleDotsubMenu( basesetting, 2, basesetting->userSetting->pricecomputing );
  basesetting->userSetting->pricecomputing = (temp == -1) ? basesetting->userSetting->pricecomputing : temp;
  return 1;
}
