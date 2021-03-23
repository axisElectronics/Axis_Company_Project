
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

int8_t saveAsDefault(  class settings *settings  )
{


  // 1. Application Setup
  for (int8_t i = 0; i < 4; i++ )  settings->userScaleSetup.application[i] = 0;

  settings->userScaleSetup.application[WeighingMode_ ] =  settings->userSetting.weighingMode;
  settings->userScaleSetup.application[CountingMode_ ] =  settings->userSetting.countingMode;
  settings->userScaleSetup.application[CheckWeighing_] =  settings->userSetting.checkWeighing;
  settings->userScaleSetup.application[PriceWeighing_] =  settings->userSetting.pricecomputing;

  // 2. scale setup
  strcpy( settings->userScaleSetup.dateString, settings->userSetting.date );
  settings->userScaleSetup.autoZero = settings->userSetting.autoZero;
  settings->userScaleSetup.stabilityFactor = settings->userSetting.stabilityFilter;
  // COM parameters
  settings->userScaleSetup.comPara[BAUDRATE] = settings->userSetting.Baudrate;
  settings->userScaleSetup.comPara[DataBits] = settings->userSetting.dataBits;
  settings->userScaleSetup.comPara[Parity]   = settings->userSetting.parity;
  settings->userScaleSetup.comPara[StopBits] = settings->userSetting.stopBits;

  //3. Weighing Units
  settings->userScaleSetup.WeighingUnits[kg]     = settings->userSetting.WeigingUnits_kg;
  settings->userScaleSetup.WeighingUnits[gm]     = settings->userSetting.WeigingUnits_gm;
  settings->userScaleSetup.WeighingUnits[tons]   = settings->userSetting.WeigingUnits_t;
  settings->userScaleSetup.WeighingUnits[Ounces] = settings->userSetting.WeigingUnits_oz;
  settings->userScaleSetup.WeighingUnits[Pounds] = settings->userSetting.WeigingUnits_lb;

  //4. scale Information
  strcpy(  settings->userScaleSetup.scaleInformation[ developer ], "Vivek Yadav" );
  strcpy(  settings->userScaleSetup.scaleInformation[ baseVersion ], "novaTouch0.0.0" );
  strcpy(  settings->userScaleSetup.scaleInformation[ modifiedVersion ], "Under Dev" );

  //5. calibration
  strcpy( settings->userScaleSetup.capacity          , settings->userSetting.scaleCapacity   );
  strcpy( settings->userScaleSetup.eVlaue            , settings->userSetting.eValue          );
  strcpy( settings->userScaleSetup.decimal           , settings->userSetting.decimalValue    );
  strcpy( settings->userScaleSetup.calibartionWeight , settings->userSetting.cali_weight     );
 
  settings->saveAsDefault();

  return 1;
}


void  settings :: importDefualtSettings()
{




}









int8_t factoryDefault(  class settings *settings )
{
  return settings->factoryDefault();
}



void realTimeWeight( class settings *settings, int yPos )
{

  SPL("realTime : " + String( settings->showDigit.currentValue ) );

  // 1. check coming must not same as previous
  if ( !strcmp( settings->showDigit.preValue,  settings->showDigit.currentValue ) ) return;

  // 2. Set color and didgit size
  /* Set Font Size */
  tft.setTextSize( 1 );
  tft.setFreeFont( (const GFXfont *)settings->showDigit.digitFontSize );

  //3. remove all dot other than valid dot
  for ( int8_t idx = 0; idx < 7; ++idx )
    tft.fillCircle( ( 42 + 30 * idx ), 298, 2, makeCustomColor(9, 9, 9) );

  // 4. print background digits.
  /* Print Background Digit */
  tft.setTextColor( makeCustomColor(9, 9, 9), TFT_BLACK); // Fadded Green BackGround
  for ( int8_t idx = 0; idx < 7; ++idx )
  {
    if ( settings->showDigit.currentValue[idx] != settings->showDigit.preValue[idx] )
      tft.drawChar( '8', ( 16 + (idx) * 30 ) , yPos );
  }

  //5. print dot at given position.
  tft.fillCircle( ( 42 + 30 * settings->showDigit.dotpos ), 298, 2, TFT_GREEN );

  // 6. checking leading Zeroes
  int8_t idx = 0;
  while ( settings->showDigit.currentValue[idx] == '0') ++idx;

  // 7. print those digit only which is not same as previous.
  for ( --idx; idx < 7; ++idx )
  {
    if ( ( settings->showDigit.currentValue[idx] != settings->showDigit.preValue[idx] )  )
    {
      tft.setTextColor( makeCustomColor(9, 9, 9), TFT_BLACK);
      tft.drawChar( '8', ( 16 + (idx * 30) ) , yPos );
      tft.setTextColor( TFT_GREEN, TFT_BLACK);
      tft.drawChar( settings->showDigit.currentValue[idx], ( 16 +  (idx * 30) ) , yPos );
    }
  }

  // 8. update prevalue with new current value.
  strcpy( settings->showDigit.preValue,  settings->showDigit.currentValue );
  return ;
}//end-


void showDigitfun( class settings *settings, int yPos )
{
  int8_t len = strlen( settings->showDigit.currentValue);
  int8_t digitIndex = 7;

  // 1. check coming must not same as previous
  if ( !strcmp( settings->showDigit.preValue,  settings->showDigit.currentValue ) ) return;

  // 2. Set color and didgit size
  /* Set Font Size */
  tft.setTextSize( 1 );
  tft.setFreeFont( (const GFXfont *)settings->showDigit.digitFontSize );

  // 3. print background digits.
  /* Print Background Digit */
  tft.setTextColor( makeCustomColor(9, 9, 9), TFT_BLACK); // Fadded Green BackGround
  for ( int8_t idx = 0; idx < 8; ++idx )
  {
    if (  settings->showDigit.currentValue[idx] == '.' );
    else if ( settings->showDigit.currentValue[idx] != settings->showDigit.preValue[idx] )
      tft.drawChar( '8', ( 16 + (idx) * 26 ) , yPos );
  }

  //4. checking leading Zeroes
  int8_t idx = 0;
  while (settings->showDigit.currentValue[idx] == '0') ++idx;

  // 5. print those digit only which is not same as previous.
  for ( ; idx < 8; ++idx )
  {
    if (  settings->showDigit.currentValue[idx] == '.'  ); // Do Nothing
    else if ( ( settings->showDigit.currentValue[idx] != settings->showDigit.preValue[idx] )  )
    {
      tft.setTextColor( makeCustomColor(9, 9, 9), TFT_BLACK);
      tft.drawChar( '8', ( 16 + (idx * 26) ) , yPos );
      tft.setTextColor( TFT_GREEN, TFT_BLACK);
      tft.drawChar( settings->showDigit.currentValue[idx], ( 16 +  (idx * 26) ) , yPos );
    }
  }

  //6. update prevalue with new current value.
  strcpy( settings->showDigit.preValue,  settings->showDigit.currentValue );
  return;
}






//
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

int8_t readNoLoadcount( class settings *settings )
{
  String noLoad_count  = String(NULL);

  // 1. send command to machine so that machine can send NO-LOAD counts.
  // three value required for this command.
  // i.e <STX>CAL#0,capacity,decimal,eValue<ETX>

  SPL("scaleCapacity : " +  String( settings->userSetting.scaleCapacity ) );
  SPL("decimalValue : " +  String( settings->userSetting.decimalValue ) );
  SPL("eValue : " +  String( settings->userSetting.eValue ) );
  saveAsDefault(settings); 

  if ( strlen( settings->userSetting.scaleCapacity) && strlen(settings->userSetting.decimalValue) && strlen(settings->userSetting.eValue) )
  {
    char buf[50] = {0};
    strcat(buf, "\2CAL#0,");
    strcat(buf, settings->userSetting.scaleCapacity);
    buf[ strlen(buf) ] = ',';
    strcat(buf, settings->userSetting.decimalValue);
    buf[ strlen(buf) ] = ',';
    strcat(buf, settings->userSetting.eValue);
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

    //3. Now start reading data from machine untill No-Load value button is not pressed
    // print count on serial
    memset( settings->showDigit.currentValue, '\0', 10);
    while ( caliButtonTouch() != NOLOAD )
    {
      if ( Serial2.available() > 5 )
      {
        noLoad_count =  Serial2.readStringUntil('=');
        settings->showDigit.currentValue[0] = '0';
        strcpy( &settings->showDigit.currentValue[1], noLoad_count.c_str() );
        settings->showDigit.currentValue[strlen(  settings->showDigit.currentValue) - 2] = '\0';
        showDigitfun(  settings, 65 );
      }

      yield();
    }//end-if


    memset(  settings->showDigit.currentValue, '\0', 10);
    memset( settings->showDigit.preValue, '\0', 10);

    Serial.println("NOload : " + String( noLoad_count ) );
    strcpy( settings->userSetting.noLoad_count, noLoad_count.c_str() );
    settings->userSetting.noLoad_count[ strlen( settings->userSetting.noLoad_count ) - 2 ] = '\0';
    Serial.println("NOload : " + String(  settings->userSetting.noLoad_count ) );
    return 1;
  }//end-if

  return 0;
}

void readLoadcount( class settings *settings )
{

  String load_count  =  String(NULL);
  String cali_weight  = String(NULL);

  // 4. put load and again take count values
  memset( settings->showDigit.currentValue, '\0', 10);
  settings->showDigit.currentValue[0] = '0';

  while ( caliButtonTouch() != LOAD )
  {
    if ( Serial2.available() > 5 )
    {
      load_count =  Serial2.readStringUntil('=');

      strcpy( & settings->showDigit.currentValue[1], load_count.c_str() );
      settings->showDigit.currentValue[strlen(  settings->showDigit.currentValue) - 2] = '\0';
      showDigitfun(  settings, 140 );
    }

    yield();
  }
  memset( settings->showDigit.currentValue, '\0', 10);
  memset( settings->showDigit.preValue, '\0', 10);

  Serial.println("Load : " + String( load_count ) );
  strcpy(  settings->userSetting.load_count, load_count.c_str() );
  settings->userSetting.load_count[ strlen( settings->userSetting.load_count ) - 2 ] = '\0';
  Serial.println("Load : " + String(  settings->userSetting.load_count ) );

  // 5. calibrated weight take data from User

  settings->userInput.userInputArray_Size = 25; // This Size must be (16 + user requirements)
  settings->userInput.userInputArray = new char[settings->userInput.userInputArray_Size];
  settings->init( );

  settings->userInput.numericSwitchFlag = 1; // only Numeric values are allowed
  {
    // read previous capacity from file system
    String temp = settings->_getCalibrationWeight();
    strcpy( settings->userSetting.cali_weight, temp.c_str() );
  }

  settings->takeUserInput(  settings->userSetting.cali_weight );
  strcpy( settings->userSetting.cali_weight ,   &settings->userInput.userInputArray[15] );

  if ( settings->userSetting.cali_weight[ strlen( settings->userSetting.cali_weight ) - 1 ] == '_' )
    settings->userSetting.cali_weight[ strlen( settings->userSetting.cali_weight ) - 1 ] = '\0';

  tft.fillScreen(TFT_BLACK);
  settings->settingscalibrationImage();

  // 6.start showing data coming from machine untill submit button is not pressed.
  memset( settings->showDigit.preValue, '\0', 10 );
  settings->showDigit.currentValue[0] = '0';
  strcpy( & settings->showDigit.currentValue[1], settings->userSetting.noLoad_count );
  showDigitfun(  settings, 65 );

  memset( settings->showDigit.preValue, '\0', 10 );
  settings->showDigit.currentValue[0] = '0';
  strcpy( & settings->showDigit.currentValue[1], settings->userSetting.load_count );
  showDigitfun(  settings, 140 );

  memset( settings->showDigit.preValue, '\0', 10 );
  memset( settings->showDigit.currentValue, '\0', 10 );
  memset(  settings->showDigit.currentValue, '0', 8 - strlen( settings->userSetting.cali_weight ) );
  strcpy( & settings->showDigit.currentValue[ 8 - strlen( settings->userSetting.cali_weight) ], settings->userSetting.cali_weight );

  showDigitfun(  settings, 215 );
  memset( settings->showDigit.preValue, '\0', 10 );
  memset( settings->showDigit.currentValue, '\0', 10 );


  Enable_Text_Font()
  delete[] settings->userInput.userInputArray;

}

void showCaliWeight( class settings *settings )
{
  String CalibratedWeight = String(NULL);

  while ( 1 )
  {
    if ( caliButtonTouch() == CALI_WEIGHT )
    {
      if ( strlen( settings->userSetting.noLoad_count) && strlen(settings->userSetting.load_count) && strlen(settings->userSetting.cali_weight) )
      {
        // 1. Send calibartion command
        char buf[50] = {0};
        strcat(buf, "\2CAL#1,");
        strcat(buf, settings->userSetting.noLoad_count);
        buf[ strlen(buf) ] = ',';
        strcat(buf, settings->userSetting.load_count);
        buf[ strlen(buf) ] = ',';
        strcat(buf, settings->userSetting.cali_weight);
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
      settings->settingscalibrationImage();

      // 4.start showing data coming from machine untill submit button is not pressed.
      memset( settings->showDigit.preValue, '\0', 10 );
      settings->showDigit.currentValue[0] = '0';
      strcpy( & settings->showDigit.currentValue[1], settings->userSetting.noLoad_count );
      showDigitfun(  settings, 65 );

      memset( settings->showDigit.preValue, '\0', 10 );
      settings->showDigit.currentValue[0] = '0';
      strcpy( & settings->showDigit.currentValue[1], settings->userSetting.load_count );
      showDigitfun(  settings, 140 );

      memset( settings->showDigit.preValue, '\0', 10 );
      memset( settings->showDigit.currentValue, '\0', 10 );
      memset(  settings->showDigit.currentValue, '0', 8 - strlen( settings->userSetting.cali_weight ) );
      strcpy( &settings->showDigit.currentValue[ 8 - strlen( settings->userSetting.cali_weight) ], settings->userSetting.cali_weight );

      showDigitfun(  settings, 215 );
      memset( settings->showDigit.preValue, '\0', 10 );
      memset( settings->showDigit.currentValue, '\0', 10 );

      // 5. Show data untill submit button is not pressed
      char temp[10];
      int8_t dotpos = 0;
      while ( caliButtonTouch() != SUBMIT )
      {
HERE:
        if ( Serial2.available() > 5 )
        {

          CalibratedWeight =  Serial2.readStringUntil('=');

          strcpy(  settings->showDigit.currentValue, CalibratedWeight.c_str() );
          settings->showDigit.currentValue[strlen(  settings->showDigit.currentValue) - 2] = '\0';

          //6. remove dot from Array

          dotpos = 0;
          memset(temp, '\0', 10);
          while ( settings->showDigit.currentValue[dotpos] != '.') {
            if ( ++dotpos >= 10 ) goto HERE;
          }
          settings->showDigit.dotpos = dotpos;
          strncpy(temp,  settings->showDigit.currentValue, dotpos );
          strcat(temp, & settings->showDigit.currentValue[++dotpos] );

          memset( settings->showDigit.currentValue, '\0', 10);
          settings->showDigit.currentValue[0] = '0';
          strcpy( & settings->showDigit.currentValue[1], temp );

          realTimeWeight(  settings, 290 );

        }//end-if
        yield();
      }//end-while

      // 6. back to menu screen
      Enable_Text_Font()
      tft.fillScreen(TFT_BLACK);
      settings->settingPageImage();
      FsettingIndexBlankFullCircle();
      settings->basesetting.fullBlankFlag = 1;
      settings->drawSettingString(  );
      FsettingIndexfillCircle(  settings->basesetting.dotIdx );
      return;
    }
    else if ( caliButtonTouch() == SUBMIT  )
    {
      Enable_Text_Font()
      tft.fillScreen(TFT_BLACK);
      settings->settingPageImage();
      FsettingIndexBlankFullCircle();
      settings->basesetting.fullBlankFlag = 1;
      settings->drawSettingString(  );
      FsettingIndexfillCircle( settings->basesetting.dotIdx );
      return;
    }//end-else if
    yield();
  }//end-while
}//end-showCaliWeight


int8_t autoZero( class settings *settings  )
{
  tft.fillScreen(TFT_BLACK);
  settings->settingPageImage();
  char option[5][10] = {"0 OFF" , "1 Div", "2 Div", "3 Div", "4 Div"};
  int8_t Size = 5;

  for (int8_t idx = 0; idx < ( Size ); ++idx )
  {
    tft.setCursor( settings->basesetting.xText, ( 85 + 54 * idx ) );
    tft.print( option[idx] );
  }


  String tempStr = settings->_getAutoZero();
  if ( tempStr == option[0]) settings->userSetting.autoZero = 0;
  else if ( tempStr == option[1]) settings->userSetting.autoZero = 1;
  else if ( tempStr == option[2]) settings->userSetting.autoZero = 2;
  else if ( tempStr == option[3]) settings->userSetting.autoZero = 3;
  else if ( tempStr == option[4]) settings->userSetting.autoZero = 4;


  int8_t temp = handleDotsubMenu( settings, Size, settings->userSetting.autoZero );
  settings->userSetting.autoZero =  ( temp == -1 ) ? settings->userSetting.autoZero : temp;

  tempStr = settings->_getWeighingUnit();
  if ( tempStr == "kg" ) settings->userSetting.WeigingUnits_kg = 1;
  else if ( tempStr == "gm" ) settings->userSetting.WeigingUnits_gm = 1;
  else if ( tempStr == "Tons" ) settings->userSetting.WeigingUnits_t = 1;
  else if ( tempStr == "Ounces(oz)" ) settings->userSetting.WeigingUnits_oz = 1;
  else if ( tempStr == "Pounds(lb)" ) settings->userSetting.WeigingUnits_lb = 1;

  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );
  return 1;
}

int8_t calibrateSetup( class settings *settings )
{
  settings->settingscalibrationImage();

  if ( readNoLoadcount( settings ) )
  {
    readLoadcount( settings );
    showCaliWeight( settings );
  }
  else
  {
    Enable_Text_Font()
    tft.fillScreen(TFT_BLACK);
    settings->settingPageImage();
    FsettingIndexBlankFullCircle();
    settings->basesetting.fullBlankFlag = 1;
    settings->drawSettingString(  );
    FsettingIndexfillCircle( settings->basesetting.dotIdx );
  }
  return 1;
}

int8_t decimalSetup(  class settings *settings )
{
  settings->userInput.userInputArray_Size = 17; // This Size must be (16 + user requirements)
  settings->userInput.userInputArray = new char[settings->userInput.userInputArray_Size];
  settings->init( );

  settings->userInput.numericSwitchFlag = 1; // only Numeric values are allowed

  // read previous Decimal from file system
  String temp = settings->_getDecimal();
  strcpy( settings->userSetting.decimalValue, temp.c_str() );
  SPL("decimalSetup : " + String(temp) );
  settings->takeUserInput(  settings->userSetting.decimalValue );
  strcpy( settings->userSetting.decimalValue ,   &settings->userInput.userInputArray[15] );

  if ( settings->userSetting.decimalValue[ strlen( settings->userSetting.decimalValue ) - 1 ] == '_' )
    settings->userSetting.decimalValue[ strlen( settings->userSetting.decimalValue ) - 1 ] = '\0';

  //  SPL("decimalValue : " + String( settings->userSetting.decimalValue ));
  Enable_Text_Font()
  tft.fillScreen(TFT_BLACK);
  settings->settingPageImage();
  FsettingIndexBlankFullCircle();
  settings->basesetting.fullBlankFlag = 1;
  settings->drawSettingString(  );
  FsettingIndexfillCircle( settings->basesetting.dotIdx );

  delete[] settings->userInput.userInputArray;
  return 1;
}



int8_t eValueSetup(  class settings *settings )
{
  settings->userInput.userInputArray_Size = 17; // This Size must be (16 + user requirements)
  settings->userInput.userInputArray = new char[settings->userInput.userInputArray_Size];
  settings->init( );

  settings->userInput.numericSwitchFlag = 1; // only Numeric values are allowed

    // read previous capacity from file system
    String temp = settings->_getEvalue();
    strcpy( settings->userSetting.eValue, temp.c_str() );
 
  settings->takeUserInput( settings->userSetting.eValue );
  strcpy( settings->userSetting.eValue , &settings->userInput.userInputArray[15] );

  if ( settings->userSetting.eValue[ strlen( settings->userSetting.eValue ) - 1 ] == '_' )
    settings->userSetting.eValue[ strlen( settings->userSetting.eValue ) - 1 ] = '\0';

    SPL("evalue : " + String( settings->userSetting.eValue));
  Enable_Text_Font()
  tft.fillScreen(TFT_BLACK);
  settings->settingPageImage();
  FsettingIndexBlankFullCircle();
  settings->basesetting.fullBlankFlag = 1;
  settings->drawSettingString(  );
  FsettingIndexfillCircle( settings->basesetting.dotIdx );

  delete[] settings->userInput.userInputArray;

  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );
  return 1;
}


int8_t capacitySetup( class settings *settings )
{
  settings->userInput.userInputArray_Size = 25; // This Size must be (15 + user requirements)
  settings->userInput.userInputArray = new char[settings->userInput.userInputArray_Size];
  settings->init(  );

  settings->userInput.numericSwitchFlag = 1; // only Numeric values are allowed
  {
    // read previous capacity from file system
    String temp = settings->_getCapacity();
    strcpy( settings->userSetting.scaleCapacity, temp.c_str() );
    SPL("capacity :-->> " + String( settings->userSetting.scaleCapacity ) );
  }

  settings->takeUserInput( settings->userSetting.scaleCapacity );
  strcpy( settings->userSetting.scaleCapacity ,   &settings->userInput.userInputArray[15] );

  if ( settings->userSetting.scaleCapacity[ strlen( settings->userSetting.scaleCapacity ) - 1 ] == '_' )
    settings->userSetting.scaleCapacity[ strlen( settings->userSetting.scaleCapacity ) - 1 ] = '\0';

  Enable_Text_Font()
  tft.fillScreen(TFT_BLACK);
  settings->settingPageImage();
  FsettingIndexBlankFullCircle();
  settings->basesetting.fullBlankFlag = 1;
  settings->drawSettingString(  );
  FsettingIndexfillCircle(  settings->basesetting.dotIdx );

  delete[]  settings->userInput.userInputArray;
  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );
  return 1;
}


int8_t clockSetup(  class settings *settings )
{
  char temp[10];
  settings->userInput.userInputArray_Size = 10;
  settings->userInput.userInputArray = temp;
  settings->init( );

  settings->userInput.fixFormatstruct->fixFormatFlag = 1;
  settings->userInput.fixFormatstruct->fixFormatArraySize = 15;
  settings->userInput.fixFormatstruct->fieldType = Date;

  strcpy( settings->userSetting.date, settings->_getDate().c_str() );
  Serial.println("Date : " + String(  settings->_getDate() ));
  settings->takeUserInput(  settings->userSetting.date );

  strcpy( settings->userSetting.date ,  settings->userInput.fixFormatstruct->fixFormatArray );
  Serial.println("World : " + String( settings->userSetting.date ));

  Enable_Text_Font()
  tft.fillScreen(TFT_BLACK);
  settings->settingPageImage();
  FsettingIndexBlankFullCircle();
  settings->basesetting.fullBlankFlag = 1;
  settings->drawSettingString( );
  FsettingIndexfillCircle( settings->basesetting.dotIdx );
  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );

  String tempStr = settings->_getWeighingUnit();

  if ( tempStr == "kg" ) settings->userSetting.WeigingUnits_kg = 1;
  else if ( tempStr == "gm" ) settings->userSetting.WeigingUnits_gm = 1;
  else if ( tempStr == "Tons" ) settings->userSetting.WeigingUnits_t = 1;
  else if ( tempStr == "Ounces(oz)" ) settings->userSetting.WeigingUnits_oz = 1;
  else if ( tempStr == "Pounds(lb)" ) settings->userSetting.WeigingUnits_lb = 1;
  
  return 1;
}


int8_t stabilityFilter( class settings *settings )
{
  tft.fillScreen(TFT_BLACK);
  settings->settingPageImage();
  char option[5][10] = {"Low" , "Medium", "Normal", "High", "Strong"};
  int8_t Size = 5;

  for (int8_t idx = 0; idx < ( Size ); ++idx )
  {
    tft.setCursor( settings->basesetting.xText, ( 85 + 54 * idx ) );
    tft.print( option[idx] );
  }
  {
    String temp = settings->_getStability();
    settings->userSetting.stabilityFilter = (temp == "Low") ? 0 : (temp == "Medium") ? 1 : (temp == "Normal") ? 2 : (temp == "High") ? 3 : (temp == "Strong") ? 4 : settings->userSetting.stabilityFilter;
  }

  int8_t temp = handleDotsubMenu( settings, Size, settings->userSetting.stabilityFilter );
  settings->userSetting.stabilityFilter =  ( temp == -1 ) ? settings->userSetting.stabilityFilter : temp;
  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );
  String tempStr = settings->_getWeighingUnit();
 if ( tempStr == "kg" ) settings->userSetting.WeigingUnits_kg = 1;
  else if ( tempStr == "gm" ) settings->userSetting.WeigingUnits_gm = 1;
  else if ( tempStr == "Tons" ) settings->userSetting.WeigingUnits_t = 1;
  else if ( tempStr == "Ounces(oz)" ) settings->userSetting.WeigingUnits_oz = 1;
  else if ( tempStr == "Pounds(lb)" ) settings->userSetting.WeigingUnits_lb = 1;
  return 1;
}


int8_t stopBits( class settings *settings )
{
  tft.fillScreen(TFT_BLACK);
  settings->settingPageImage();
  char option[2][10] = {"1 Bit" , "2 Bits"};
  int8_t Size = 2;

  for (int8_t idx = 0; idx < ( Size ); ++idx )
  {
    tft.setCursor( settings->basesetting.xText, ( 85 + 54 * idx ) );
    tft.print( option[idx] );
  }

  {
    String temp = settings->_getCOMPara( StopBits );
    settings->userSetting.stopBits = (temp == "1 bit") ? 0 : (temp == "2 bits") ? 1 :  settings->userSetting.stopBits ;
  }

  int8_t temp = handleDotsubMenu( settings, Size, settings->userSetting.stopBits );
  settings->userSetting.stopBits = ( temp == -1 ) ? settings->userSetting.stopBits : temp;
  /*
     Don't know why the value of decimalVlaue is corrupting. whenever I am changing any settings.
     So after doing this this problem is fixed some how.
  */
  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );
  String tempStr = settings->_getWeighingUnit();
  if ( tempStr == "kg" ) settings->userSetting.WeigingUnits_kg = 1;
  else if ( tempStr == "gm" ) settings->userSetting.WeigingUnits_gm = 1;
  else if ( tempStr == "Tons" ) settings->userSetting.WeigingUnits_t = 1;
  else if ( tempStr == "Ounces(oz)" ) settings->userSetting.WeigingUnits_oz = 1;
  else if ( tempStr == "Pounds(lb)" ) settings->userSetting.WeigingUnits_lb = 1;
  return 1;
}


int8_t paritySetting( class settings *settings )
{
  tft.fillScreen(TFT_BLACK);
  settings->settingPageImage();
  char option[3][10] = {"None" , "Odd", "Even"};
  int8_t Size = 3;

  for (int8_t idx = 0; idx < ( Size ); ++idx )
  {
    tft.setCursor( settings->basesetting.xText, ( 85 + 54 * idx ) );
    tft.print( option[idx] );
  }

  {
    String temp = settings->_getCOMPara( Parity );
    settings->userSetting.parity = (temp == "None") ? 0 : (temp == "odd") ? 1 : (temp == "Even") ? 2 : settings->userSetting.parity;
  }

  int8_t temp = handleDotsubMenu( settings, Size, settings->userSetting.parity );
  settings->userSetting.parity = ( temp == -1 ) ? settings->userSetting.parity : temp;
  /*
    Don't know why the value of decimalVlaue is corrupting. whenever I am changing any settings.
    So after doing this this problem is fixed some how.
  */
  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );
  String tempStr = settings->_getWeighingUnit();
  if ( tempStr == "kg" ) settings->userSetting.WeigingUnits_kg = 1;
  else if ( tempStr == "gm" ) settings->userSetting.WeigingUnits_gm = 1;
  else if ( tempStr == "Tons" ) settings->userSetting.WeigingUnits_t = 1;
  else if ( tempStr == "Ounces(oz)" ) settings->userSetting.WeigingUnits_oz = 1;
  else if ( tempStr == "Pounds(lb)" ) settings->userSetting.WeigingUnits_lb = 1;
  return 1;
}


int8_t dataBits( class settings *settings )
{
  tft.fillScreen(TFT_BLACK);
  settings->settingPageImage();
  char option[2][10] = {"8 bits" , "7 bits"};
  int8_t Size = 2;

  for (int8_t idx = 0; idx < ( Size ); ++idx )
  {
    tft.setCursor( settings->basesetting.xText, ( 85 + 54 * idx ) );
    tft.print( option[idx] );
  }

  {
    String temp = settings->_getCOMPara( DataBits );
   
    settings->userSetting.dataBits = (temp == "8 bits") ? 0 : (temp == "7 bits") ? 1 :  settings->userSetting.dataBits;
  }

  int8_t temp = handleDotsubMenu( settings, Size, settings->userSetting.dataBits );
  settings->userSetting.dataBits = ( temp == -1 ) ? settings->userSetting.dataBits : temp;
  /*
    Don't know why the value of decimalVlaue is corrupting. whenever I am changing any settings.
    So after doing this this problem is fixed some how.
  */
  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );
  String tempStr = settings->_getWeighingUnit();
 
  if ( tempStr == "kg" ) settings->userSetting.WeigingUnits_kg = 1;
  else if ( tempStr == "gm" ) settings->userSetting.WeigingUnits_gm = 1;
  else if ( tempStr == "Tons" ) settings->userSetting.WeigingUnits_t = 1;
  else if ( tempStr == "Ounces(oz)" ) settings->userSetting.WeigingUnits_oz = 1;
  else if ( tempStr == "Pounds(lb)" ) settings->userSetting.WeigingUnits_lb = 1;
  
  return 1;
}


int8_t handleDotsubMenu( class settings *settings, int8_t Size, int8_t dotIdx )
{

  FsettingIndexBlankFullCircle()
  FsettingIndexfillCircle( dotIdx )
  while ( 1 )
  {
    switch (  settings->ButtonTouch() )
    {
      case Down :
        ++dotIdx;
        if ( dotIdx >= Size )
        {
          dotIdx = Size - 1;
        }
        else
        {
          FsettingIndexBlankFullCircle()
          FsettingIndexfillCircle( dotIdx )
        }
        delay(80);
        break;
      case UP :
        --dotIdx;
        if ( dotIdx < 0 ) dotIdx = 0;
        else
        {
          FsettingIndexBlankFullCircle()
          FsettingIndexfillCircle( dotIdx )
        }
        delay(80);
        break;
      case ENT :
        settings->settingPageImage();
        FsettingIndexBlankFullCircle()
        settings->basesetting.fullBlankFlag = 1;
        settings->drawSettingString( );
        FsettingIndexfillCircle( settings->basesetting.dotIdx )
        return dotIdx;
        break;
      case ESC :
        settings->settingPageImage();
        FsettingIndexBlankFullCircle()
        settings->basesetting.fullBlankFlag = 1;
        settings->drawSettingString( );
        FsettingIndexfillCircle( settings->basesetting.dotIdx )
        return -1;
        break;
    }
    yield();
  }//end-while
}


int8_t setBaudrate( class settings *settings  )
{
  tft.fillScreen(TFT_BLACK);
  settings->settingPageImage();
  int baud[4] = {2400, 4800, 9600, 115200};
  int8_t Size = sizeof(baud) / sizeof(baud[0]);

  for (int8_t idx = 0; idx < ( Size ); ++idx )
  {
    tft.setCursor( settings->basesetting.xText, ( 85 + 54 * idx ) );
    tft.print( baud[idx] );
  }
  int defaultValueIdx = 0;
  String temp =  settings->_getCOMPara( BAUDRATE );

  settings->userSetting.Baudrate = ( temp == "2400" ) ? 2400 : ( temp == "4800" ) ? 4800 : ( temp == "9600" ) ? 9600 : ( temp == "115200" ) ? 115200 : settings->userSetting.Baudrate;


  switch ( settings->userSetting.Baudrate )
  {
    case 2400   :    settings->userSetting.Baudrate  = 0; break;
    case 4800   :    settings->userSetting.Baudrate  = 1; break;
    case 9600   :    settings->userSetting.Baudrate  = 2; break;
    case 115200 :    settings->userSetting.Baudrate  = 3; break;
    default     :    break;
  }

  switch ( handleDotsubMenu( settings, Size, settings->userSetting.Baudrate ) )
  {
    case 0   :    settings->userSetting.Baudrate  = 0; break;
    case 1   :    settings->userSetting.Baudrate  = 1; break;
    case 2   :    settings->userSetting.Baudrate  = 2; break;
    case 3   :    settings->userSetting.Baudrate  = 3; break;
    default  :    break;
  }
  /*
      Don't know why the value of decimalVlaue is corrupting. whenever I am changing any settings.
      So after doing this this problem is fixed some how.
  */
  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );
  String tempStr = settings->_getWeighingUnit();
  
  if ( tempStr == "kg" ) settings->userSetting.WeigingUnits_kg = 1;
  else if ( tempStr == "gm" ) settings->userSetting.WeigingUnits_gm = 1;
  else if ( tempStr == "Tons" ) settings->userSetting.WeigingUnits_t = 1;
  else if ( tempStr == "Ounces(oz)" ) settings->userSetting.WeigingUnits_oz = 1;
  else if ( tempStr == "Pounds(lb)" ) settings->userSetting.WeigingUnits_lb = 1;
  return 1;
}




int8_t scaleInfo( class settings *settings )
{
  tft.fillScreen(TFT_BLACK);
  tft.drawString( settings->userSetting.scaleInfo[0], 30, 80, 1);
  tft.drawString( settings->userSetting.scaleInfo[1], 30, 120, 1);
  delay(5000);
  settings->settingPageImage();
  FsettingIndexBlankFullCircle();
  settings->basesetting.fullBlankFlag = 1;
  settings->drawSettingString( );
  FsettingIndexfillCircle( settings->basesetting.dotIdx )
  /*
    Don't know why the value of decimalVlaue is corrupting. whenever I am changing any settings.
    So after doing this this problem is fixed some how.
  */
  return 1;
}



int8_t weighingUnit_lb( class settings *settings )
{
  enORDis( settings );
  settings->userSetting.WeigingUnits_lb = ( settings->_getWeighingUnit() == "Pounds(lb)" ? 1 : 0 );


  int8_t temp = handleDotsubMenu( settings, 2, settings->userSetting.WeigingUnits_lb );
  settings->userSetting.WeigingUnits_lb  = ( temp == -1 ) ?  settings->userSetting.WeigingUnits_lb : temp;
  if (  settings->userSetting.WeigingUnits_lb )
  {
    settings->userSetting.WeigingUnits_kg = 0;
    settings->userSetting.WeigingUnits_gm = 0;
    settings->userSetting.WeigingUnits_t = 0;
    settings->userSetting.WeigingUnits_oz = 0;
  }
  /*
     Don't know why the value of decimalVlaue is corrupting. whenever I am changing any settings.
     So after doing this this problem is fixed some how.
  */
  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );
  return 1;
}


int8_t weighingUnit_oz( class settings *settings )
{
  enORDis( settings );
  settings->userSetting.WeigingUnits_oz = (settings->_getWeighingUnit() == "Ounces(oz)" ? 1 : 0 );

  int8_t temp = handleDotsubMenu( settings, 2, settings->userSetting.WeigingUnits_oz );
  settings->userSetting.WeigingUnits_oz  = ( temp == -1 ) ?  settings->userSetting.WeigingUnits_oz : temp;
  if (  settings->userSetting.WeigingUnits_oz )
  {
    settings->userSetting.WeigingUnits_kg = 0;
    settings->userSetting.WeigingUnits_gm = 0;
    settings->userSetting.WeigingUnits_t = 0;
    //    settings->userSetting.WeigingUnits_oz = 0;
    settings->userSetting.WeigingUnits_lb = 0;
  }
  /*
     Don't know why the value of decimalVlaue is corrupting. whenever I am changing any settings.
     So after doing this this problem is fixed some how.
  */
  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );
  return 1;
}


int8_t weighingUnit_t( class settings *settings )
{
  enORDis( settings );
  settings->userSetting.WeigingUnits_t = ( settings->_getWeighingUnit() == "Tons" ? 1 : 0 );

  int8_t temp = handleDotsubMenu( settings, 2, settings->userSetting.WeigingUnits_t );
  settings->userSetting.WeigingUnits_t  = ( temp == -1 ) ?  settings->userSetting.WeigingUnits_t : temp;
  if (  settings->userSetting.WeigingUnits_t )
  {
    settings->userSetting.WeigingUnits_kg = 0;
    settings->userSetting.WeigingUnits_gm = 0;
    settings->userSetting.WeigingUnits_oz = 0;
    settings->userSetting.WeigingUnits_lb = 0;
  }
  /*
     Don't know why the value of decimalVlaue is corrupting. whenever I am changing any settings.
     So after doing this this problem is fixed some how.
  */
  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );
  return 1;
}


int8_t weighingUnit_gm(  class settings *settings )
{
  enORDis( settings );
  settings->userSetting.WeigingUnits_gm = ( settings->_getWeighingUnit() == "gm" ? 1 : 0 );


  int8_t temp = handleDotsubMenu( settings, 2, settings->userSetting.WeigingUnits_gm );
  settings->userSetting.WeigingUnits_gm  = ( temp == -1 ) ?  settings->userSetting.WeigingUnits_gm : temp;
  if (  settings->userSetting.WeigingUnits_gm )
  {
    settings->userSetting.WeigingUnits_kg = 0;
    settings->userSetting.WeigingUnits_t = 0;
    settings->userSetting.WeigingUnits_oz = 0;
    settings->userSetting.WeigingUnits_lb = 0;
  }
  /*
      Don't know why the value of decimalVlaue is corrupting. whenever I am changing any settings.
      So after doing this this problem is fixed some how.
  */
  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );
  return 1;
}


int8_t weighingUnit_kg( class settings *settings )
{
  enORDis( settings );

  settings->userSetting.WeigingUnits_kg = ( settings->_getWeighingUnit() == "kg" ? 1 : 0 );

  int8_t temp = handleDotsubMenu( settings, 2, settings->userSetting.WeigingUnits_kg );
  settings->userSetting.WeigingUnits_kg  = ( temp == -1 ) ? settings->userSetting.WeigingUnits_kg : temp;
  if (  settings->userSetting.WeigingUnits_kg )
  {
    settings->userSetting.WeigingUnits_gm = 0;
    settings->userSetting.WeigingUnits_t  = 0;
    settings->userSetting.WeigingUnits_oz = 0;
    settings->userSetting.WeigingUnits_lb = 0;
  }
  /*
    Don't know why the value of decimalVlaue is corrupting. whenever I am changing any settings.
    So after doing this this problem is fixed some how.
  */
  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );

  return 1;
}


void enORDis( class settings *settings )
{
  tft.fillScreen(TFT_BLACK);
  settings->settingPageImage();
  char option[2][10] = {"Disable", "Enable"};
  int8_t Size = 2;

  for ( int8_t idx = 0; idx < ( Size ); ++idx )
  {
    tft.setCursor( settings->basesetting.xText, ( 85 + 54 * idx ) );
    tft.print( option[idx] );
  }
}


int8_t weighingMode( class settings *settings )
{
  enORDis( settings );
  String str =  settings->_getApplication();
  settings->userSetting.weighingMode = ( str.indexOf('0') != -1 ) ? 1 : 0;

  int8_t temp =  handleDotsubMenu( settings, 2, settings->userSetting.weighingMode );
  settings->userSetting.weighingMode = (temp == -1) ? settings->userSetting.weighingMode : temp;

  /*
     Don't know why the value of decimalVlaue is corrupting. whenever I am changing any settings.
     So after doing this this problem is fixed some how.
  */
  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );
   String tempStr = settings->_getWeighingUnit();
  SPL("setbaud->weghtUnit : " + String(tempStr) );
  if ( tempStr == "kg" ) settings->userSetting.WeigingUnits_kg = 1;
  else if ( tempStr == "gm" ) settings->userSetting.WeigingUnits_gm = 1;
  else if ( tempStr == "Tons" ) settings->userSetting.WeigingUnits_t = 1;
  else if ( tempStr == "Ounces(oz)" ) settings->userSetting.WeigingUnits_oz = 1;
  else if ( tempStr == "Pounds(lb)" ) settings->userSetting.WeigingUnits_lb = 1;

  return 1;
}


int8_t countingMode( class settings *settings )
{
  enORDis( settings );
  String str =  settings->_getApplication();
  settings->userSetting.countingMode = ( str.indexOf('1') != -1 ) ? 1 : 0;

  int8_t temp =  handleDotsubMenu( settings, 2, settings->userSetting.countingMode );
  settings->userSetting.countingMode = (temp == -1) ? settings->userSetting.countingMode : temp;

  /*
    Don't know why the value of decimalVlaue is corrupting. whenever I am changing any settings.
    So after doing this this problem is fixed some how.
  */
  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );
    String tempStr = settings->_getWeighingUnit();
  
  if ( tempStr == "kg" ) settings->userSetting.WeigingUnits_kg = 1;
  else if ( tempStr == "gm" ) settings->userSetting.WeigingUnits_gm = 1;
  else if ( tempStr == "Tons" ) settings->userSetting.WeigingUnits_t = 1;
  else if ( tempStr == "Ounces(oz)" ) settings->userSetting.WeigingUnits_oz = 1;
  else if ( tempStr == "Pounds(lb)" ) settings->userSetting.WeigingUnits_lb = 1;
  return 1;
}

int8_t checkWeighing( class settings *settings )
{
  enORDis( settings );
  String str =  settings->_getApplication();
  settings->userSetting.checkWeighing = ( str.indexOf('2') != -1  ) ? 1 : 0;

  int8_t temp =  handleDotsubMenu( settings, 2, settings->userSetting.checkWeighing );
  settings->userSetting.checkWeighing = (temp == -1) ? settings->userSetting.checkWeighing : temp;

  /*
     Don't know why the value of decimalVlaue is corrupting. whenever I am changing any settings.
     So after doing this this problem is fixed some how.
  */
  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );
   String tempStr = settings->_getWeighingUnit();
 
  if ( tempStr == "kg" ) settings->userSetting.WeigingUnits_kg = 1;
  else if ( tempStr == "gm" ) settings->userSetting.WeigingUnits_gm = 1;
  else if ( tempStr == "Tons" ) settings->userSetting.WeigingUnits_t = 1;
  else if ( tempStr == "Ounces(oz)" ) settings->userSetting.WeigingUnits_oz = 1;
  else if ( tempStr == "Pounds(lb)" ) settings->userSetting.WeigingUnits_lb = 1;
  return 1;
}

int8_t priceComputing( class settings *settings )
{
  enORDis( settings );
  String str =  settings->_getApplication();
  settings->userSetting.pricecomputing = ( str.indexOf('3') != -1  ) ? 1 : 0;

  int8_t temp =  handleDotsubMenu( settings, 2, settings->userSetting.pricecomputing );
  settings->userSetting.pricecomputing = (temp == -1) ? settings->userSetting.pricecomputing : temp;

  /*
    Don't know why the value of decimalVlaue is corrupting. whenever I am changing any settings.
    So after doing this this problem is fixed some how.
  */
  strcpy( settings->userSetting.decimalValue, settings->_getDecimal().c_str() );
   String tempStr = settings->_getWeighingUnit();
 
  if ( tempStr == "kg" ) settings->userSetting.WeigingUnits_kg = 1;
  else if ( tempStr == "gm" ) settings->userSetting.WeigingUnits_gm = 1;
  else if ( tempStr == "Tons" ) settings->userSetting.WeigingUnits_t = 1;
  else if ( tempStr == "Ounces(oz)" ) settings->userSetting.WeigingUnits_oz = 1;
  else if ( tempStr == "Pounds(lb)" ) settings->userSetting.WeigingUnits_lb = 1;
  return 1;
}
