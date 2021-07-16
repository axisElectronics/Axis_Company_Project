
#ifndef _PRICE_H
#define _PRICE_H


#include "commonGlobal.h"
#include "InterruptHandler.h"


extern void createThread();
extern void vTask_SerialButtonMonitor( void * pvParameters );

enum {
  PRICE_perPcs = 1,
  PRICE_Total = 0,
  PRICE_NetWeight = 2,
};

class price : public WeighingHandle
{
  private :
  public :
    uint8_t updateField = PRICE_NetWeight;
    String temp;
    char asciiTemp[10];
    char asciiValues[3][10];
    char preAsciiValues[3][10];
    double numericValues[3];

    String _readbuf( );
    void onScreen(uint8_t showMeAt );
    void _rawCalculation();

    bool printStringPrice( );
    bool priceStripImage();

 
  
    class WeighingHandle commFun;
    class userKeyBoard kbd;

    void  _start();
    void priceHandler();
    int8_t activityBasedOnTouch();
    void unitePrice();
    void updateTotalPriceWindow();
    
};




int8_t  price ::  activityBasedOnTouch() {
  char src[12] = {0};
  int8_t tempDot = showDigits.dotPosition;

  switch (  getactiveMachineKeyMapping() ){
    case map_CmdESC   : break; pressed = 0; return -1;
    case map_CmdUnits : break; // as per requirement
    case map_CmdPrint : break; // as per requirement
    case map_CmdZero  : CMD_ZERODATA break;
    case map_CmdTare  : CMD_AUTOTARE break;
    case open_unitPrice : break; goto UNIT_PRICE;
  }//end-switch

 // if ( ( pressed = tft.getTouch(&xAxis, &yAxis) ) == 0 ) return 0;
    if ( pressed == 0 ) return 0;

  if ( pressed ) {
    pressed = 0;

    if ( Taretouch_Auto ) {
      START_SERIAL2
      CMD_AUTOTARE
    }
    else if ( Zerotouch ) {
      SPL("Zero Touch...!!!");
      START_SERIAL2
      CMD_ZERODATA
    }
    else if ( ESC_Touch ) {
      SPL("ESCTouch...\n");
      return -1;
    }
    else if ( Field_Two_Touch  ){
      UNIT_PRICE:
      
      STOP_SERIAL2
      START_SERIAL1

      kbd.userInput.userInputArray_Size = 25;
      kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];
      kbd.init(  );
      kbd.userInput.numericSwitchFlag = 1;
      strcpy( kbd.userInput.fieldLable, "unit Price" );
      strcpy( kbd.userInput.unitbuf, "Rs" );     
      kbd.takeUserInput( NULL );
      SPL("1-keyboard : " + String( kbd.userInput.userInputArray ) );

      updateField = PRICE_perPcs;
      strcpy( asciiTemp,  kbd.userInput.userInputArray );
      _rawCalculation();

      // Load screen images again.
      START_SERIAL2
      STOP_SERIAL1
         
      initTFTHandler();
      printStringPrice();

      // SHOW DATA ON SCREEN
      strcpy( preAsciiValues[PRICE_Total] , "ABCDEFGH" );
      strcpy( preAsciiValues[PRICE_NetWeight] , "ABCDEFGH" );
       
      onScreen(PRICE_perPcs);
      onScreen(PRICE_NetWeight);
      onScreen(PRICE_Total);    
      updateField = PRICE_NetWeight;   
              
      delete[]  kbd.userInput.userInputArray;
      START_SERIAL1
    }
  }//end-if
  return 0;
}



void  price :: updateTotalPriceWindow() {

  sprintf( asciiValues[PRICE_Total], "%lf",  ( numericValues[PRICE_Total] ) );
   
  temp = String( asciiValues[PRICE_Total] );
  uint8_t tempDotPos = temp.indexOf('.');
  char tempArray[15];
  int8_t leadingZero = 0;

  if ( tempDotPos < 7 ) {
    // tempArray have data without dot.
    memset(tempArray, '\0', 15);
    memset(tempArray, '0', 6);
    for(uint8_t i = 0, j=0; i < 7; ++i ){
       if( asciiValues[PRICE_Total][i] != '.' )
          tempArray[j++] = asciiValues[PRICE_Total][i];
    }
    for(uint8_t i = strlen(tempArray)-1; i >= 2; --i ){
        if(tempArray[i] == '0' )
            tempArray[i] = '\0';
        else break;
    }
    memset(showDigits.currentValue, '0', 7);   
    strcpy( &showDigits.currentValue[7-strlen(tempArray)],  tempArray );
    showDigits.currentValue[7] = '\0';
  } else return;


  //1. check with previous Value
  if ( !strcmp( showDigits.preValue[0] , showDigits.currentValue ) )  return;
     SPL("currentValue : " + String(showDigits.currentValue) );
  //   SPL("preValue[0] : " + String(showDigits.preValue[0]) );

  leadingZero = 7 -  strlen(tempArray);
//  if( tempDotPos == 1 )
    tempDotPos = leadingZero + (tempDotPos - 1 );
//  else if( tempDotPos == 2 )
//    tempDotPos = leadingZero + 1;
    
  // 2. Set Font Size & Style
  showDigits.digitFontSize  = 1;
  showDigits.digitFontStyle = LED7SEG_STD40;
  tft.setTextSize( showDigits.digitFontSize );
  tft.setFreeFont( (const GFXfont *)showDigits.digitFontStyle );


  //3. draw blank rectangle only those digits which is different from previous Value.

  static int8_t cnt_1 = 0;
  if ( ( cnt_1++) > 20 )
  {
    for (uint8_t idx = 0; idx < 7; ++idx )
    {
      if ( showDigits.preValue[0][idx] !=  showDigits.currentValue[idx] )
      {
        tft.fillRect( 15  + ( idx * 65),  58,   62,  90,  TFT_BLACK);
        if ( cnt_1 > 22 ) cnt_1 = 0;
      }

    }
  }

  //4.draw blank digits
  tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
  // IN PLACE OF leading Zeros we are drawing blank digits.
  for (uint8_t idx = 0; idx < leadingZero; ++idx )
  {
    tft.drawChar( '8', 15 + ( idx * 65), 130,  1);
  }
  for (uint8_t idx = leadingZero; idx < 7; ++idx )
  {
    if ( showDigits.preValue[0][idx] !=  showDigits.currentValue[idx] )
    {
      tft.drawChar( '8', 15 + ( idx * 65), 130,  1);
    }
  }

  //5. remove all previous dots and redraw at given position;
  for (uint8_t idx = 0; idx < 6; ++idx )
  {
    tft.fillCircle( 73 + ( idx * 65 ), 138, 6, TFT_BLACK);
  }
  // draw dot at given position
   tft.fillCircle( 73 + ( tempDotPos * 65), 138, 6, TFT_RED );



  //5. draw Digits only, Those digits which is different from previous Value.
  // front digit
  tft.setTextColor(showDigits.showDigitColorWindowOne, TFT_BLACK);
  for (uint8_t idx = leadingZero ; idx < 7; ++idx )
  {
    if ( showDigits.preValue[0][idx] !=  showDigits.currentValue[idx] )
    {
      tft.drawChar( showDigits.currentValue[idx], 15 + ( idx * 65), 130,  1);
    }
  }

  // end : update preVlaue
  strcpy( showDigits.preValue[0], showDigits.currentValue );
  strcpy( showDigits.preValue[2], "ABCDEFGH" );
#if 0
SP("asiiValue[PRICE_Total] : "); SPL(asciiValues[PRICE_Total]);
SP("tempArray : "); SPL( tempArray );
SP("showDigits.currentValue : "); SPL(showDigits.currentValue );
#endif


}//end




void price :: unitePrice(){
  

 temp = String( asciiValues[PRICE_perPcs] );
  uint8_t tempDotPos = temp.indexOf('.');
  uint8_t starttempDotPos= tempDotPos;
  char tempArray[15], modified[15];
  int8_t leadingZero = 0;
  
  if ( tempDotPos < 7 ) {
    memset(tempArray, '0', 10);
   strcpy(tempArray, asciiValues[PRICE_perPcs]);  
   // eliminating tariling Zero
   for(int8_t idx= strlen(tempArray)-1 ; idx > tempDotPos+1; --idx ){
    if( tempArray[idx] == '0' ) tempArray[idx] = '\0'; 
    else break;
   }
   
   // find dot postion from back side
   tempDotPos = 0;
   for(int8_t idx = strlen(tempArray)-1; idx >= 0; --idx ){
    if( tempArray[idx] != '.' ) tempDotPos++; 
    else if( tempArray[idx] == '.' ) break;
   }
  
  //set dot new position
   tempDotPos = 6 - tempDotPos;

  //eliminating dot & storing data in reverse order
   memset(showDigits.currentValue, '0', 10);
   int8_t len = strlen(tempArray)-1;   
   for(int8_t idx = 0, j=7-len; idx <= len; ++idx ){
    if( tempArray[idx] != '.' ) showDigits.currentValue[j++] = tempArray[idx];; 
   }
    showDigits.currentValue[7] = '\0';
  } else return;

////===========================================================//  

  //1. check with previous Value
  if ( !strcmp( showDigits.preValue[1] , showDigits.currentValue ) )  return;

  // find Leading Zeros
  for(uint8_t i = 0; i < tempDotPos; i++ ){
    if( showDigits.currentValue[i] == '0' ) leadingZero++;
    else break;
  }
  
   if( ( numericValues[PRICE_perPcs] ) == 0.0 ){
    tempDotPos = leadingZero = 5;
  }

 
  // 1. Set Font Size
  showDigits.digitFontSize  = 1;
  showDigits.digitFontStyle = LED7SEG_STD18;
  tft.setTextSize( showDigits.digitFontSize );
  tft.setFreeFont( (const GFXfont *)showDigits.digitFontStyle );
  
    //eliminate leading Zeros.
    tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
    for (uint8_t idx = 0; idx < leadingZero; ++idx ){
        tft.drawChar(  '8', 15 + ( idx * 31) , 210,  1);
    }
  

  tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
  for (uint8_t idx = 0; idx < 7; ++idx ){
    if ( showDigits.preValue[1][idx] !=  showDigits.currentValue[idx] ){
      tft.fillRect( 15  + ( idx * 31),  180,   30,  40,  TFT_BLACK );
      tft.drawChar(  '8', 15 + ( idx * 31) , 210,  1);
    }
  }


  //5. remove all previous dots and redraw at given position;
  for (uint8_t idx = 0; idx < 6; ++idx ){
    tft.fillCircle( 41 + ( idx * 31), 216, 3, TFT_BLACK);
  }
  
  int8_t dotPosition = tempDotPos;
  tft.fillCircle( 41 + ( dotPosition * 31), 216, 3, TFT_RED );

  //6. draw Digits only,, Those digits which is different from previous Value.
  tft.setTextColor( TFT_GREEN, TFT_BLACK );
  for (uint8_t idx = leadingZero; idx < 7; ++idx ){
    if ( showDigits.preValue[1][idx] !=  showDigits.currentValue[idx] ){
      tft.drawChar(  showDigits.currentValue[idx], 15 + ( idx * 31) , 210,  1);
    }
  }

   // end : update preVlaue
  strcpy( showDigits.preValue[1], showDigits.currentValue );

}//end-unitPrice




void   price :: onScreen(uint8_t showMeAt ) {
  bufferWithoutDot( showDigits.currentValue,  asciiValues[showMeAt] );
  showDigits.currentValue[6] = '\0';

  switch ( showMeAt ) {
    case PRICE_perPcs      :  unitePrice();    break;
    case PRICE_Total       :  updateTotalPriceWindow(); /* windowOne(); */  break;
    case PRICE_NetWeight   :  windowThree();   break;
  }
}


/*
   NOte : There are two parameter which is need to be care.Before calling this function these two field must be updated.
          1 >> updateField -->> this is the field which you want to update
          2 >> asciiTemp -->> this the temprary Array variable which you want to update.
*/
void   price ::_rawCalculation() {
  uint8_t dotpos;
  
  if ( !strcmp( preAsciiValues[updateField] , asciiTemp) ) return;
  dotpos = 0;
  numericValues[updateField] = strtod(asciiTemp, NULL);
   yield();
  // convert double values into equivalent ASCII values
  memset( asciiValues[updateField], '\0', 10);
  sprintf( asciiValues[updateField], "%lf", numericValues[updateField]);

  // Adjust dot postion in Array
  findDotPosition( asciiValues[updateField], dotpos);
  // check valid Dot position.[user set this during calibration]
  // Bug Fix : showDigits.dotPosition was corrupting again & again.
  showDigits.dotPosition = _getDecimal().c_str()[0] - '0';

  memset(asciiTemp, '0', 10);
  for (int8_t i = 0, j = ( 6 - showDigits.dotPosition - dotpos); ( i < 7 ) && ( j < 7 ) ; i++, j++ ) {
    asciiTemp[ j ] = asciiValues[updateField][i];
  }
  asciiTemp[7] = '\0';

  strcpy( asciiValues[updateField], asciiTemp );
  asciiValues[updateField][7] = '\0';
  //save current value for later use
  strcpy( preAsciiValues[updateField] , asciiValues[updateField]);

//main calculation
  if( updateField == PRICE_perPcs ){ 
    SP("new UPdate : ");
    SPL( asciiValues[updateField] );
  }

 yield();
  if( ( updateField == PRICE_NetWeight ) || ( updateField == PRICE_perPcs ) ){
    numericValues[PRICE_Total] = numericValues[PRICE_NetWeight] * numericValues[PRICE_perPcs];
    updateField = PRICE_Total;
    memset( asciiTemp, '\0', 10);
    sprintf( asciiTemp, "%lf", numericValues[updateField]);
     yield();
    _rawCalculation();
     updateField = PRICE_NetWeight;
  }  

  
  updateField = PRICE_NetWeight;
 yield();
}//end-_rawCalcualtion()



void  price :: priceHandler() {
  memset(asciiTemp, '\0', 10);
  strcpy( asciiTemp, _readbuf( ).c_str() );  asciiTemp[7] = '\0';
  if ( strlen(asciiTemp) > 5 ) {
    _rawCalculation();

    onScreen( PRICE_NetWeight );
    onScreen(PRICE_Total);
    onScreen(PRICE_perPcs);  
  }
   DateNTimeHandler();
    yield();
}


String price :: _readbuf( ) {
  temp =  "";
HERE :
  // 1. Get data from weighing machine
  if ( Serial2.available() > 0 )
    temp = Serial2.readStringUntil('=');

  //2. check, is String temp contains <STX> <ETX> ??
  int8_t stx = temp.indexOf(2);
  if ( stx >= 0 )
  {
    char ch = temp[stx + 1];
    switch (ch)
    {
      case '1'://Zero Command Response
      //  tareWeight = 0.0;
        break;
      case 'T':
        // Nothing to do.
        break;

      default : break;
    }
    temp = "";
    goto HERE;
  }

  handleFlags( (char *)temp.c_str() );
  return temp;
}







void  price :: _start() {
  char timeArray[10];
  char preTime[10];
   TaskHandle_t xHandle;
   
  initTFTHandler ( );
  printStringPrice();
  showDigits.dotPosition = _getDecimal().c_str()[0] - 48;;

   updateField = PRICE_Total;
  strcpy(asciiTemp, "0.0" );
  _rawCalculation();

  updateField = PRICE_perPcs;
  strcpy(asciiTemp, "0.0" );
  _rawCalculation();

  onScreen(PRICE_perPcs);
  onScreen(PRICE_Total);

  updateField = PRICE_NetWeight;

  pressed = 0;
  TouchInterrupt_ptr = myfastfunction;
  ATTACH_TOUCH_INTERRUPT
  ISR_dtech = 0;


  // make a thread which will take care all keypad data.
  createThread();
}






bool  price :: priceStripImage()
{
  tft.setSwapBytes(true);
  tft.pushImage(10, 0, PriceComputingModeStripWidth, PriceComputingModeStripHeight, PriceComputingModeStrip );
}


bool  price :: printStringPrice( ){
   String weightUnit = _getWeighingUnit();
  priceStripImage();
  tft.setTextSize( 1 );
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

 char dateArray[10];
  parshDateformat(dateArray, getRTCDate().c_str() );

  tft.setFreeFont( (const GFXfont *)EUROSTILE_B13 );
  tft.setCursor(18, 50);  tft.print("TOTAL Price");
  //  tft.setCursor(400, 50);  tft.print(weightUnit);

  tft.setFreeFont( (const GFXfont *)EUROSTILE_B10 );
  tft.setCursor(18, 173);  tft.print("Unit Price");
  tft.setCursor(180, 173);   tft.print(weightUnit);

  tft.setCursor(260, 173);  tft.print("Total WT");
  tft.setCursor(420, 173);  tft.print(weightUnit);

 // Bottom strip flags.
  tft.setFreeFont( (const GFXfont *)EUROSTILE_B7 );
  tft.setTextColor(TFT_BLACK, TFT_BLACK);
  tft.setCursor(5, 310);
  tft.print( "Model: " + String("TAX")\
            + " Max: " + String( 30 ) + String( weightUnit) \
            + " Min: " + String( 10 ) + String( weightUnit) \
            + " e: " + String(1) + String("g") \
            + " class: " + String("ll") \
            + "  " + String(dateArray)); 
}

#endif
