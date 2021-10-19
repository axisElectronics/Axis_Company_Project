
#include "settings.h"


#define x1NOLOAD  250
#define x2NOLOAD  471
#define y1NOLOAD  0
#define y2NOLOAD  55 

#define xNOLOAD  ( ( xAxis >= x1NOLOAD ) && ( xAxis <= x2NOLOAD ) )
#define yNOLOAD  ( ( yAxis >= y1NOLOAD  ) && ( yAxis <= y2NOLOAD ) )
/**********************************************************************************************/

#define x1LOAD  250
#define x2LOAD  471
#define y1LOAD  84
#define y2LOAD  153

#define xLOAD  ( ( xAxis >= x1LOAD ) && ( xAxis <= x2LOAD ) )
#define yLOAD  ( ( yAxis >= y1LOAD  ) && ( yAxis <= y2LOAD ) )
/**********************************************************************************************/

#define x1CALI_WEIGHT  250
#define x2CALI_WEIGHT  471
#define y1CALI_WEIGHT  176
#define y2CALI_WEIGHT  238

#define xCALI_WEIGHT  ( ( xAxis >= x1CALI_WEIGHT ) && ( xAxis <= x2CALI_WEIGHT ) )
#define yCALI_WEIGHT  ( ( yAxis >= y1CALI_WEIGHT  ) && ( yAxis <= y2CALI_WEIGHT ) )
/**********************************************************************************************/

#define x1SUBMIT  250
#define x2SUBMIT  471
#define y1SUBMIT  255
#define y2SUBMIT  320

#define xSUBMIT  ( ( xAxis >= x1SUBMIT ) && ( xAxis <= x2SUBMIT ) )
#define ySUBMIT  ( ( yAxis >= y1SUBMIT  ) && ( yAxis <= y2SUBMIT ) )
/**********************************************************************************************/
enum {
  NOLOAD      = 1,
  LOAD        = 2,
  CALI_WEIGHT = 3,
  SUBMIT      = 4
};

/*
  #define NOLOAD       1
  #define LOAD         2
  #define CALI_WEIGHT  3
  #define SUBMIT       4
*/

class customRTC myRTC;

#define RXD2 16
#define TXD2 17

#define CMD_STARTDATA   {           \
    String StartCMD = "\2DT#1\3";   \
    Serial2.print( StartCMD );      \
  }

#define CMD_STOPDATA {            \
    String StopCMD = "\2DT#0\3";  \
    Serial2.print(StopCMD);       \
  }



/******************************************************************
   @makeCustomColor :
   (#) we can make custom color accordingly changed the value of RGB color.
 *                                                                *
 *******************************************************************/
#define makeCustomColor( red, green , blue ) (((red & 0xf8)<<8) + ((green & 0xfc)<<3) + (blue>>3))



int8_t saveAsDefault(  class settings *settings  ) {

   enORDis( settings );
   int temp = handleDotsubMenu( settings, 2, 0 );  
   if( temp ){

    // Show dummy Progress bar
    tft.fillScreen(TFT_BLACK);
    tft.drawRoundRect(50, 150, 300, 40, 10, TFT_RED);
    for(int16_t slider=0; slider < 300; ++slider ){
      tft.fillRoundRect(55, 153, slider, 35, 5, TFT_GREEN);
      delay(10);
      yield();
    }//end-for
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
   }//end-if

  Enable_Text_Font()
  tft.fillScreen(TFT_BLACK);
  settings->settingPageImage();
  FsettingIndexBlankFullCircle();
  settings->basesetting.fullBlankFlag = 1;
  settings->drawSettingString(  );
  FsettingIndexfillCircle( settings->basesetting.dotIdx );
  return 1;
}


void  settings :: importDefualtSettings() {




}









int8_t factoryDefault(  class settings *settings )
{
   enORDis( settings );
   int temp = handleDotsubMenu( settings, 2, 0 );  
   if( temp )
    return settings->factoryDefault();
   
  return 1;
}



void realTimeWeight( class settings *settings, int yPos )
{

  SPL("realTime : " + String( settings->showDigit.currentValue ) );

  // 1. check input data must not same as previous
  if ( !strcmp( settings->showDigit.preValue,  settings->showDigit.currentValue ) ) return;

  // 2. Set color and digit size
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








int8_t caliButtonTouch() {
  uint16_t xAxis, yAxis, threshold = 1000;
  if ( tft.getTouch(&xAxis, &yAxis, threshold) )
  {
    if (xNOLOAD && yNOLOAD ) return NOLOAD; //NOLOAD
    else if (xLOAD && yLOAD ) return LOAD;
    else if (xCALI_WEIGHT && yCALI_WEIGHT ) {
      Serial.println("CALI_WEIGHT Touch :\nxAxis --->>> " + String(xAxis) + "\nyAxis --->>> " + String(yAxis) );
      return CALI_WEIGHT;
    }
    else if ( xSUBMIT && ySUBMIT ) return SUBMIT;
    else
      Serial.println("ELSE Touch :\nxAxis --->>> " + String(xAxis) + "\nyAxis --->>> " + String(yAxis) );
  }
  return 5;
}





int8_t readNoLoadcount( class settings *settings ) {
  String noLoad_count  = String(NULL);

  // 1. send command to machine so that machine can send NO-LOAD counts.
  // three value required for this command.
  // i.e <STX>CAL#0,capacity,decimal,eValue<ETX>

  strcpy(settings->userSetting.scaleCapacity, settings->_getCapacity().c_str() );
  strcpy(settings->userSetting.decimalValue, settings->_getDecimal().c_str() );
  strcpy(settings->userSetting.eValue, settings->_getEvalue().c_str() );

  SPL("scaleCapacity : " +  String( settings->userSetting.scaleCapacity ) );
  SPL("decimalValue : " +  String( settings->userSetting.decimalValue ) );
  SPL("eValue : " +  String( settings->userSetting.eValue ) );
  //  SPL(" ");
  //  SPL("getscaleCapacity : " +  String( settings->_getCapacity() ) );
  //  SPL("getdecimalValue : " +  String( settings-> _getDecimal() ) );
  //  SPL("geteValue : " +  String( settings->_getEvalue() ) );
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Weight Mechine
  if ( strlen( settings->userSetting.scaleCapacity ) && strlen(settings->userSetting.decimalValue) && strlen(settings->userSetting.eValue) )
  {
    char buf[50] = {0};
    char cmdResponse[10];
    memset(cmdResponse, '\0', 3);
    strcat(buf, "\2CAL#0,");
    strcat(buf, settings->userSetting.scaleCapacity);
    buf[ strlen(buf) ] = ',';
    strcat(buf, settings->userSetting.decimalValue);
    buf[ strlen(buf) ] = ',';
    strcat(buf, settings->userSetting.eValue);
    buf[ strlen(buf) ] = '\3';

    // 1st stop data comming from machine
    uint32_t timeout = millis();
 /*
    while ( 1 ) {

      if ( Serial2.available() > 0 ) {
        Serial2.readBytes(cmdResponse, 2);
        if ( !strcmp(cmdResponse, "OK") ) {
          Serial.printf("cmdResponse : %s\n" , cmdResponse );
          memset(cmdResponse, '\0', 10);
          break;
        }
      }

      if ( ( millis() - timeout ) > 1000 ) {
        timeout = millis();
        CMD_STOPDATA
      }
      yield();
    }//end-while

*/
    //2. wait till you received "OK" from machine
    // I found that we are receving response too early.
    //  Machine should not send continuous data untill i ask.
    // Ask neeraj sir to modified
    while ( 1 ){
      Serial2.readBytes(cmdResponse, 2);
      Serial.println("cmdResponse : " + String(cmdResponse));
      if ( !strcmp(cmdResponse, "OK") )  break;

      if ( ( millis() - timeout ) > 1000 ) {
        timeout = millis();
        Serial.println("Command--->>>> : " + String(buf));
        Serial2.write(buf); // NO-Load Command
      }
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
      Serial.println("noLoad_count : " + String(noLoad_count) );
      yield();
    }//end-if


    memset(  settings->showDigit.currentValue, '\0', 10);
    memset( settings->showDigit.preValue, '\0', 10);

    // Serial.println("NOload : " + String( noLoad_count ) );
    strcpy( settings->userSetting.noLoad_count, noLoad_count.c_str() );
    settings->userSetting.noLoad_count[ strlen( settings->userSetting.noLoad_count ) - 2 ] = '\0';
    // Serial.println("NOload : " + String(  settings->userSetting.noLoad_count ) );
    return 1;
  }//end-if

  return 0;
}



void readLoadcount( class settings *settings ) {

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

  // Serial.println("Load : " + String( load_count ) );
  strcpy(  settings->userSetting.load_count, load_count.c_str() );
  settings->userSetting.load_count[ strlen( settings->userSetting.load_count ) - 2 ] = '\0';
  // Serial.println("Load : " + String(  settings->userSetting.load_count ) );

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


void showCaliWeight( class settings *settings ) {
  String CalibratedWeight = String(NULL);
  String temp = "";
  unsigned int tout = millis();
  char cmdResponse[3];
  int8_t responseCount = 0;

  while ( 1 )
  {
    if ( caliButtonTouch() == CALI_WEIGHT )
    {
      if ( strlen( settings->userSetting.noLoad_count) && strlen(settings->userSetting.load_count) && strlen(settings->userSetting.cali_weight) )
      {
        // Start Serial 2
        Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Weight Mechine

        // 1. Send calibartion command
        char buf[50] = {0};
        strcat(buf, "\2CAL#1,");
        strcat(buf, settings->userSetting.noLoad_count);
        buf[ strlen(buf) ] = ',';
        strcat(buf, settings->userSetting.load_count);
        buf[ strlen(buf) ] = ',';
        strcat(buf, settings->userSetting.cali_weight);
        buf[ strlen(buf) ] = '\3';

BACK :
        responseCount = 0;
        Serial.println("Command--->>>> : " + String(buf));
        Serial2.print(buf); // Calibartion command

        //2. wait till you received "OK" from machine
        while ( 1 )
        {
          Serial2.readBytes(cmdResponse, 2);
          Serial.println("cmdResponse : " + String(cmdResponse));
          if ( !strcmp(cmdResponse, "OK") )  break;
          if ( responseCount > 5 ) goto BACK;

          responseCount++;
          yield();
        }//end-while
        //        while (1)
        //        {
        //          if ( Serial2.available() > 0 )
        //          {
        //            temp += Serial2.readStringUntil('K');
        //            break;
        //          }
        //
        //          if ( ( millis() - tout ) > 1000 )
        //          {
        //            tout = millis();
        //            //   Serial.println("Command--->>>> : " + String(buf));
        //            Serial2.print(buf);
        //          }
        //          yield();
        //        }//end-while

        //   SPL("Response : " + String(temp) );
        //    SPL("Calibartin request has been confirmed...!!!");


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
      // Request for continuous data
      String StartCMD = "\2DT#1\3";
      Serial2.print( StartCMD );

      while ( caliButtonTouch() != SUBMIT )
      {
HERE:
        // ASK for data
        //             String onDemandData = "\2W\3";
        //             Serial2.print(onDemandData);
        //             SPL("Data ON Depamd : " + String( Serial2.readStringUntil('=') ));

        if ( Serial2.available() > 5 )
        {

          CalibratedWeight =  Serial2.readStringUntil('=');

          strcpy(  settings->showDigit.currentValue, CalibratedWeight.c_str() );
          settings->showDigit.currentValue[strlen(  settings->showDigit.currentValue) - 2] = '\0';

          //6. remove dot from Array

          dotpos = 0;
          memset(temp, '\0', 10);
          while ( settings->showDigit.currentValue[dotpos] != '.') {
            if ( ++dotpos >= 10 ) {

              goto HERE;
            }
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


int8_t autoZero( class settings *settings  ) {
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



int8_t calibrateSetup( class settings *settings ) {
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




int8_t decimalSetup(  class settings *settings ) {
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

  // set decimal data into SPIFFS memory
  settings->_setDecimal( settings->userSetting.decimalValue[0] );

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



int8_t eValueSetup(  class settings *settings ) {
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

  // set evalue into SPIFFS memory
  settings->_setEvalue( settings->userSetting.eValue[0] ) ;

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


int8_t capacitySetup( class settings *settings ) {
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

  // set capacity as default in parmanet memory
  settings->_setCapacity( settings->userSetting.scaleCapacity );


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




int8_t TimeSetup(  class settings *settings ) {
  bool errFlag=0;
  // =============================================
  //  Take user Input for Date and update into
  //  SPIFFS memory.
  // =============================================

  char temp[10];
  settings->userInput.userInputArray_Size = 10;
  settings->userInput.userInputArray = temp;
  settings->init( );

  settings->userInput.fixFormatstruct->fixFormatFlag = 1;
  settings->userInput.fixFormatstruct->fixFormatArraySize = 15;
  settings->userInput.fixFormatstruct->fieldType = Time;

  // get SPIFFS time data

  settings->takeUserInput(  settings->userSetting.TimeArray );

  strcpy( settings->userSetting.TimeArray ,  settings->userInput.fixFormatstruct->fixFormatArray );
  // Serial.println("Time : " + String( settings->userSetting.TimeArray )); // new updated date value

  // =============================================
  // update RTC with new value
  // please check date formate
  // =============================================

  // date string must be 8 character long
  if ( strlen( settings->userSetting.TimeArray ) == 8 ) {
    //check date must be digit
    if ( isdigit( settings->userSetting.TimeArray[0])    \
         && isdigit( settings->userSetting.TimeArray[1])  \
         && isdigit( settings->userSetting.TimeArray[3])  \
         && isdigit( settings->userSetting.TimeArray[4] ) \
         && isdigit( settings->userSetting.TimeArray[6])  \
         && isdigit( settings->userSetting.TimeArray[7] ) \
       ) {

      if ( ( settings->userSetting.TimeArray[0] >= '0' ) \
           &&  ( settings->userSetting.TimeArray[0] <= '2' ) ) {

        if ( ( settings->userSetting.TimeArray[0] == '1' ) \
             && settings->userSetting.TimeArray[1] > '2' ) {
          SPL("Error 1: Time must be 00...12 !!!" );
          errFlag=1;
        }
        else if ( ( settings->userSetting.TimeArray[3] > '5' ) ) {
          SPL("Error 2: minute must be 00...59 !!!" );
          errFlag=1;
        }
        else if ( settings->userSetting.TimeArray[6] > '5' ) {
          SPL("Error 3: second must be 00...59 !!!" );
          errFlag=1;
        }
        else {
          myRTC.RTCHandler.hour    = ( settings->userSetting.TimeArray[0] - '0' ) * 10 + ( settings->userSetting.TimeArray[1] - '0' );
          myRTC.RTCHandler.minute  = ( settings->userSetting.TimeArray[3] - '0' ) * 10 + ( settings->userSetting.TimeArray[4] - '0' );
          myRTC.RTCHandler.second  = ( settings->userSetting.TimeArray[6] - '0' ) * 10 + ( settings->userSetting.TimeArray[7] - '0' );
          // Serial.println("Time ===>>> " + String( myRTC.RTCHandler.hour ) + ": " + String( myRTC.RTCHandler.minute ) + ": " + String( myRTC.RTCHandler.second ) );
          myRTC.setRTCTime();

          strcpy( settings->userScaleSetup.timeArr, settings->userSetting.TimeArray );
          settings->_setTime();
        }
      } else{  SPL("Error 1: Time must be 00...12 !!!" ); errFlag=1; }
    } else{ SPL("Error 5: check Time must be digit"); errFlag=1; }
  } else{ SPL("Error 6: check Time length"); errFlag=1; }

  // save time it has already done in previous logic.
  // My next target to modify this _setTime() to _setTime(char *time)
  // Leave it is as for now.
  


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

int8_t DateSetup(  class settings *settings ) {
  bool errFlag=0;
  // =============================================
  //  Take user Input for Date and update into
  //  SPIFFS memory.
  // =============================================

  char temp[10];
  settings->userInput.userInputArray_Size = 10;
  settings->userInput.userInputArray = temp;
  settings->init( );

  settings->userInput.fixFormatstruct->fixFormatFlag = 1;
  settings->userInput.fixFormatstruct->fixFormatArraySize = 15;
  settings->userInput.fixFormatstruct->fieldType = Date;

  strcpy( settings->userSetting.date, settings->_getDate().c_str() );
  // Serial.println("preStoreDate : " + String(  settings->_getDate() )); // get previous set value which is stored into memory

  settings->takeUserInput(  settings->userSetting.date );

  strcpy( settings->userSetting.date ,  settings->userInput.fixFormatstruct->fixFormatArray );
  // Serial.println("World : " + String( settings->userSetting.date )); // new updated date value

  // =============================================
  // update RTC with new value
  // please check date formate
  // =============================================

  // date string must be 8 character long
  if ( strlen( settings->userSetting.date ) == 8 ) {
    //check date must be digit
    if ( isdigit( settings->userSetting.date[0])    \
         && isdigit( settings->userSetting.date[1])  \
         && isdigit( settings->userSetting.date[3])  \
         && isdigit( settings->userSetting.date[4] ) \
         && isdigit( settings->userSetting.date[6])  \
         && isdigit( settings->userSetting.date[7] ) \
       ) {

      if ( ( settings->userSetting.date[0] >= '0' ) \
           &&  ( settings->userSetting.date[0] <= '3' ) ) {

        if ( ( settings->userSetting.date[0] == '3' ) \
             && settings->userSetting.date[1] >= '2' ) {
          SPL("Error 1: date must be 0..31 !!!" );
          errFlag=1;
        }
        else if ( ( settings->userSetting.date[3] == '1' ) \
                  && ( settings->userSetting.date[4] > '2' ) ) {
          SPL("Error 2: month must be 1..12 !!!" );
        }
        else if ( settings->userSetting.date[6] < '2' ) {
          SPL("Error 3: year must be greater then 2020 !!!" );
        }
        else {
          myRTC.RTCHandler.mday = ( settings->userSetting.date[0] - '0' ) * 10 + ( settings->userSetting.date[1] - '0' );
          myRTC.RTCHandler.mon  = ( settings->userSetting.date[3] - '0' ) * 10 + ( settings->userSetting.date[4] - '0' );
          myRTC.RTCHandler.year = 2000 + ( settings->userSetting.date[6] - '0' ) * 10 + ( settings->userSetting.date[7] - '0' );

          //   Serial.println("Date ===>>> " + String( myRTC.RTCHandler.mday ) + ": " + String( myRTC.RTCHandler.mon ) + ": " + String( myRTC.RTCHandler.year ) );
          myRTC.setRTCDate();
          strcpy( settings->userScaleSetup.dateArr, settings->userSetting.date );
          settings->_setDate(settings->userScaleSetup.dateArr);
        }
      } else{ SPL("Error 2: date must be 0..31 !!!");  errFlag=1; }
    } else{ SPL("Error 3: check date must be digit");  errFlag=1; }
  } else{ SPL("Error 4: check date length");  errFlag=1; }


  // if all right then save the data
  if( !errFlag ){
    settings->_setDate( settings->userSetting.date );
  }
  else{
    settings->_setDate( (char *)settings->_getDate().c_str() );
  }
  
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

  // =========================================
  //  Bug Fix : variable curruption
  // =========================================
  String tempStr = settings->_getWeighingUnit();

  if ( tempStr == "kg" ) settings->userSetting.WeigingUnits_kg = 1;
  else if ( tempStr == "gm" ) settings->userSetting.WeigingUnits_gm = 1;
  else if ( tempStr == "Tons" ) settings->userSetting.WeigingUnits_t = 1;
  else if ( tempStr == "Ounces(oz)" ) settings->userSetting.WeigingUnits_oz = 1;
  else if ( tempStr == "Pounds(lb)" ) settings->userSetting.WeigingUnits_lb = 1;

  return 1;
}




int8_t stabilityFilter( class settings * settings ) {
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


int8_t stopBits( class settings * settings ) {
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


int8_t paritySetting( class settings * settings ) {
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


int8_t dataBits( class settings * settings ) {
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


int8_t handleDotsubMenu( class settings * settings, int8_t Size, int8_t dotIdx ) {

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


int8_t setBaudrate( class settings * settings  ) {
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




int8_t scaleInfo( class settings * settings ) {
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



int8_t weighingUnit_lb( class settings * settings )
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


int8_t weighingUnit_oz( class settings * settings ) {
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


int8_t weighingUnit_t( class settings * settings ) {
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


int8_t weighingUnit_gm(  class settings * settings ) {
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


int8_t weighingUnit_kg( class settings * settings ) {
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


void enORDis( class settings * settings ) {
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


int8_t weighingMode( class settings * settings ) {
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


int8_t countingMode( class settings * settings ) {
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

int8_t checkWeighing( class settings * settings ) {
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

int8_t priceComputing( class settings * settings ) {
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


/*<! ==================================================== >*/

extern class headerInput headerStr;
int8_t addHeaderOne( class settings *settings ){ 
    char buff[70]; 
    memset(buff, '\0', sizeof(buff)); 
    Serial.println(settings->_getHeader(0));
    strcpy(buff, (char *)settings->_getHeader(0).c_str()); 
    headerStr.takeStringInput( buff, 70 );   
    strcpy( settings->userScaleSetup.saveHeader[0] , buff );
    settings->saveAsDefault();
//    Serial.println( String( settings->_getHeader(0) ) );
  switch( settings->author){
    case 0:  
        Enable_Text_Font()
        tft.fillScreen(TFT_BLACK);
        settings->settingPageImage();
        FsettingIndexBlankFullCircle();
        settings->basesetting.fullBlankFlag = 1;
        settings->drawSettingString(  );
        FsettingIndexfillCircle( settings->basesetting.dotIdx );
        break;
    case 1:
        Enable_Text_Font()
        tft.fillScreen(TFT_BLACK);
        settings->settingPageImage();
        FsettingIndexBlankFullCircle();
        break;
            
  }//end-switch
  return 1;
}

/*<! ==================================================== >*/

int8_t addHeaderTwo( class settings *settings ){
    char buff[70]; 
    memset(buff, '\0', sizeof(buff)); 
    Serial.println(settings->_getHeader(1));
    strcpy(buff, (char *)settings->_getHeader(1).c_str()); 
    headerStr.takeStringInput( buff, 70 );   
    strcpy( settings->userScaleSetup.saveHeader[1] , buff );
    settings->saveAsDefault();

  switch( settings->author){
    case 0:  
        Enable_Text_Font()
        tft.fillScreen(TFT_BLACK);
        settings->settingPageImage();
        FsettingIndexBlankFullCircle();
        settings->basesetting.fullBlankFlag = 1;
        settings->drawSettingString(  );
        FsettingIndexfillCircle( settings->basesetting.dotIdx );
        break;
    case 1:
        Enable_Text_Font()
        tft.fillScreen(TFT_BLACK);
        settings->settingPageImage();
        FsettingIndexBlankFullCircle();
        break;
            
  }//end-switch
  return 1;
  
}

/*<! ==================================================== >*/

int8_t addHeaderThree( class settings *settings ){
    char buff[70]; 
    memset(buff, '\0', sizeof(buff)); 
    Serial.println(settings->_getHeader(2));
    strcpy(buff, (char *)settings->_getHeader(2).c_str()); 
    headerStr.takeStringInput( buff, 70 );   
    strcpy( settings->userScaleSetup.saveHeader[2] , buff );
    settings->saveAsDefault();
    
 
  switch( settings->author){
    case 0:  
        Enable_Text_Font()
        tft.fillScreen(TFT_BLACK);
        settings->settingPageImage();
        FsettingIndexBlankFullCircle();
        settings->basesetting.fullBlankFlag = 1;
        settings->drawSettingString(  );
        FsettingIndexfillCircle( settings->basesetting.dotIdx );
        break;
    case 1:
        Enable_Text_Font()
        tft.fillScreen(TFT_BLACK);
        settings->settingPageImage();
        FsettingIndexBlankFullCircle();
        break;
            
  }//end-switch
  return 1;

}

/*<! ==================================================== >*/

int8_t addHeaderFour( class settings *settings ){
    char buff[70]; 
    memset(buff, '\0', sizeof(buff)); 
    Serial.println(settings->_getHeader(3));
    strcpy(buff, (char *)settings->_getHeader(3).c_str()); 
    headerStr.takeStringInput( buff, 70 );   
    strcpy( settings->userScaleSetup.saveHeader[3] , buff );
    settings->saveAsDefault();
    

  switch( settings->author){
    case 0:  
        Enable_Text_Font()
        tft.fillScreen(TFT_BLACK);
        settings->settingPageImage();
        FsettingIndexBlankFullCircle();
        settings->basesetting.fullBlankFlag = 1;
        settings->drawSettingString(  );
        FsettingIndexfillCircle( settings->basesetting.dotIdx );
        break;
    case 1:
        Enable_Text_Font()
        tft.fillScreen(TFT_BLACK);
        settings->settingPageImage();
        FsettingIndexBlankFullCircle();
        break;
            
  }//end-switch
  return 1;

}

/*<! ==================================================== >*/

int8_t addHeaderFive( class settings *settings ){
    char buff[70]; 
    memset(buff, '\0', sizeof(buff)); 
    Serial.println(settings->_getHeader(4));
    strcpy(buff, (char *)settings->_getHeader(4).c_str()); 
    headerStr.takeStringInput( buff, 70 );   
    strcpy( settings->userScaleSetup.saveHeader[4] , buff );
    settings->saveAsDefault();
         

  switch( settings->author){
    case 0:  
        Enable_Text_Font()
        tft.fillScreen(TFT_BLACK);
        settings->settingPageImage();
        FsettingIndexBlankFullCircle();
        settings->basesetting.fullBlankFlag = 1;
        settings->drawSettingString(  );
        FsettingIndexfillCircle( settings->basesetting.dotIdx );
        break;
    case 1:
        Enable_Text_Font()
        tft.fillScreen(TFT_BLACK);
        settings->settingPageImage();
        FsettingIndexBlankFullCircle();
        break;
            
  }//end-switch
  return 1;

}



int8_t addFooterOne( class settings *settings ){

    char buff[70]; 
    memset(buff, '\0', sizeof(buff)); 
    Serial.println(settings->_getFooter(0));
    strcpy(buff, (char *)settings->_getFooter(0).c_str()); 
    headerStr.takeStringInput( buff, 70 );   
    strcpy( settings->userScaleSetup.saveFooter[0] , buff );
    settings->saveAsDefault(); 

  switch( settings->author){
    case 0:  
        Enable_Text_Font()
        tft.fillScreen(TFT_BLACK);
        settings->settingPageImage();
        FsettingIndexBlankFullCircle();
        settings->basesetting.fullBlankFlag = 1;
        settings->drawSettingString(  );
        FsettingIndexfillCircle( settings->basesetting.dotIdx );
        break;
    case 1:
        Enable_Text_Font()
        tft.fillScreen(TFT_BLACK);
        settings->settingPageImage();
        FsettingIndexBlankFullCircle();
        break;
            
  }//end-switch
  return 1;

}


int8_t addFooterTwo( class settings *settings ){
    char buff[70]; 
    memset(buff, '\0', sizeof(buff)); 
    Serial.println(settings->_getFooter(1));
    strcpy(buff, (char *)settings->_getFooter(1).c_str()); 
    headerStr.takeStringInput( buff, 70 );   
    strcpy( settings->userScaleSetup.saveFooter[1] , buff );
    settings->saveAsDefault();   

  switch( settings->author){
    case 0:  
        Enable_Text_Font()
        tft.fillScreen(TFT_BLACK);
        settings->settingPageImage();
        FsettingIndexBlankFullCircle();
        settings->basesetting.fullBlankFlag = 1;
        settings->drawSettingString(  );
        FsettingIndexfillCircle( settings->basesetting.dotIdx );
        break;
    case 1:
        Enable_Text_Font()
        tft.fillScreen(TFT_BLACK);
        settings->settingPageImage();
        FsettingIndexBlankFullCircle();
        break;
            
  }//end-switch
  return 1;
}


int8_t addFooterThree( class settings *settings ){
    char buff[70]; 
    memset(buff, '\0', sizeof(buff)); 
    Serial.println(settings->_getFooter(2));
    strcpy(buff, (char *)settings->_getFooter(2).c_str()); 
    headerStr.takeStringInput( buff, 70 );   
    strcpy( settings->userScaleSetup.saveFooter[2] , buff );
    settings->saveAsDefault(); 

  switch( settings->author){
    case 0:  
        Enable_Text_Font()
        tft.fillScreen(TFT_BLACK);
        settings->settingPageImage();
        FsettingIndexBlankFullCircle();
        settings->basesetting.fullBlankFlag = 1;
        settings->drawSettingString(  );
        FsettingIndexfillCircle( settings->basesetting.dotIdx );
        break;
    case 1:
        Enable_Text_Font()
        tft.fillScreen(TFT_BLACK);
        settings->settingPageImage();
        FsettingIndexBlankFullCircle();
        break;
            
  }//end-switch
  return 1;

}



int8_t addProductName( class settings *settings ){


    char buff[70]; 
    memset(buff, '\0', sizeof(buff)); 
    Serial.println(settings-> _getProductName() );
    strcpy(buff, (char *)settings-> _getProductName().c_str() ); 
    headerStr.takeStringInput( buff, 70 );   
    strcpy( settings->userScaleSetup.saveProductName , buff );
    settings->saveAsDefault(); 

  switch( settings->author ){
    case 0:
      Enable_Text_Font()
      tft.fillScreen(TFT_BLACK);
      settings->settingPageImage();
      FsettingIndexBlankFullCircle();
      settings->basesetting.fullBlankFlag = 1;
      settings->drawSettingString(  );
      FsettingIndexfillCircle( settings->basesetting.dotIdx );
      break;

    case 1:
      Enable_Text_Font()
      tft.fillScreen(TFT_BLACK);
      settings->settingPageImage();      
      break;
  
  }//end-switch

  return 1;
}





int8_t addOperatorName( class settings *settings ){

    char buff[70];
    settings->takeStringInput(buff, 70);    
    strcpy( settings->userScaleSetup.saveOperatorName , buff     );  
    settings->saveAsDefault();
  
  SPL("addOperatorName ==>> " + settings->_getOperatorName() );
  
  Enable_Text_Font()
  tft.fillScreen(TFT_BLACK);
  settings->settingPageImage();
  FsettingIndexBlankFullCircle();
  settings->basesetting.fullBlankFlag = 1;
  settings->drawSettingString(  );
  FsettingIndexfillCircle( settings->basesetting.dotIdx );
  return 1;
}



int8_t addShift( class settings *settings ){

  char buff[70];
  settings->takeStringInput(buff, 70);    
  strcpy( settings->userScaleSetup.saveShift , buff     );  
  settings->saveAsDefault();

  SPL("addShift ==>> " + settings->_getShift() );
  
  Enable_Text_Font()
  tft.fillScreen(TFT_BLACK);
  settings->settingPageImage();
  FsettingIndexBlankFullCircle();
  settings->basesetting.fullBlankFlag = 1;
  settings->drawSettingString(  );
  FsettingIndexfillCircle( settings->basesetting.dotIdx );
  return 1;
}
