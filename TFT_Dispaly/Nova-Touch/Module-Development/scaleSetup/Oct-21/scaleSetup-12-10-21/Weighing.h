
#ifndef _WEIGHT_H
#define _WEIGHT_H

#include "commonGlobal.h"
#include "InterruptHandler.h"
#include "prtLabel.h"


extern class WeighingHandle Wtft;

typedef enum e_Weightkey{
  GROSS = 0,
  NET   = 1,
  TARE  = 2
};

//#define  GROSS  0
//#define  NET    1
//#define  TARE   2



class weight : public WeighingHandle {
  private :
    char dateArray[10];
    uint8_t digitSpeed = 0;
    uint8_t updateField = GROSS;
    String temp;
    char asciiTemp[10];
    char asciiValues[3][10];
    char preAsciiValues[3][10];
    double numericValues[3];

    String _readbuf( );
    void onScreen(uint8_t showMeAt );
    void _rawCalculation();

    bool weightStripImage();
    bool printStringWeight( );
    void  handleTareCommand( char *Weight  );


  public :
    class WeighingHandle commFun;
    class userKeyBoard kbd;

    void  _start();
    void weightHandler();
    int8_t activityBasedOnTouch();

    int8_t printerLevel();
};




void weight :: _rawCalculation() {

  uint8_t dotpos;
  if ( !strcmp( preAsciiValues[updateField] , asciiTemp) ) return;
  dotpos = 0;
  numericValues[updateField] = strtod(asciiTemp, NULL);

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
  if ( updateField == NET ) {
    numericValues[GROSS] = numericValues[NET] + numericValues[TARE];
    updateField = GROSS;
    memset( asciiTemp, '\0', 10);
    sprintf( asciiTemp, "%lf", numericValues[updateField]);
    _rawCalculation();
    updateField = NET;
  }

}


void  weight :: weightHandler() {
  memset(asciiTemp, '\0', 10);
  strcpy( asciiTemp, _readbuf( ).c_str() );  asciiTemp[7] = '\0';

  if ( strlen(asciiTemp) > 5 ) {
    _rawCalculation();

    onScreen( GROSS );
    if ( ++digitSpeed > 3 ) {
      onScreen(NET);
      digitSpeed = 0;
    }
  }
  DateNTimeHandler();
  esp_task_wdt_reset();
}



void batteryAnimation(int blevel){
    while(1){
    uint16_t linecolor[4]={TFT_RED, TFT_YELLOW, TFT_GREEN, TFT_BLACK};
    int8_t color=0;
    
    for( int xline=253, yline=5; xline < 295; ++xline ){
      if( ( xline % 253 ) == 14 ){ 
        color++;    
      }
     tft.drawFastVLine(xline ,yline, 16, linecolor[color]);
      delay(10);
      yield();
    }
 
 
    yield();
  }//end-while(1)
}//batteryAnimation()


void weight:: _start() {

  char timeArray[10];
  char preTime[10];

  initTFTHandler ( );
  printStringWeight( );

  
  batteryAnimation(100);



  uint8_t tempdot = _getDecimal().c_str()[0];
  showDigits.dotPosition = tempdot - '0';

  updateField = GROSS;
  memset( asciiTemp, '\0', 10);
  strcpy(asciiTemp , "0.0");
  _rawCalculation();
  onScreen( GROSS );

  updateField = TARE;
  _rawCalculation();
  onScreen( TARE );

  updateField = NET;
  _rawCalculation();
  onScreen( NET );

  // get TARE data from Machine
  CMD_GETTARE
  _readbuf( );

  pressed = 0;


  TouchInterrupt_ptr = myfastfunction;

#ifdef ATTACH_INTER
  ATTACH_TOUCH_INTERRUPT
#elif
  ISR_Called = 1;
#endif

  ISR_dtech = 0;
  
//  Wtft.popupButton(Xesc,  Ybutton, TFT_GREEN);
//  Wtft.popupButton(Xunit, Ybutton, TFT_GREEN);
////  Wtft.popupButton(Xprint,Ybutton, TFT_GREEN);
//  Wtft.popupButton(Xzero, Ybutton, TFT_GREEN);
//  Wtft.popupButton(Xtare, Ybutton, TFT_GREEN);
}



void   weight :: onScreen(uint8_t showMeAt ) {
  bufferWithoutDot( showDigits.currentValue,  asciiValues[showMeAt] );
  showDigits.currentValue[6] = '\0';

  switch ( showMeAt ) {
    case GROSS      :  windowTwo(); break;
    case NET        :  windowOne();  break;
    case TARE       :  windowThree(); break;
  }
}


int8_t  weight :: printerLevel() {
  static int snum = 0;
  //1. Start Serial-0 port
 // Serial.println("Printer has been touched...!!!");
  
  // Update Printer Variables
  
    Wtft.updateLableVar( DATE , dateArray );
    Wtft.updateLableVar( TIME , preTime );
     
    Wtft.updateLableVar( Lvar_GROSS , asciiValues[GROSS]);
    Wtft.updateLableVar( Lvar_TARE , asciiValues[TARE]);
    Wtft.updateLableVar( Lvar_NET , asciiValues[NET]);

    Wtft.updateLableVar( PRODUCT_NAME, (char *)Wtft._getProductName().c_str() );
    Wtft.updateLableVar( Operator_Name, (char *)Wtft._getOperatorName().c_str() );
    Wtft.updateLableVar( SHIFT, (char *)Wtft._getShift().c_str() );
  

  //check which printer is enabled....
  int8_t checkprtLable;
  for(int8_t i=0; i < 5; ++i){
    if( Wtft._getPrtResponse(i) == '1' ){
      checkprtLable = i;
    }
  }
  
  // sample bill print Format -1
  switch(checkprtLable){
    case 0:
        SP( getHeader() );
        SPL();
        SP("S.no         : "); SPL( snum++ );
        SP("Date         : "); SPL( Wtft.showMeLableVar( DATE ) );
        SP("Time         : "); SPL( Wtft.showMeLableVar( TIME ) );
        SP("Product Code : "); SPL( Wtft.showMeLableVar( PRODUCT_NAME ));
        SP("NetWeight    : "); SPL( Wtft.showMeLableVar(Lvar_NET)  );
        SP("TareWeight   : "); SPL( Wtft.showMeLableVar(Lvar_TARE) );
        SP("GrossWeight  : "); SPL( Wtft.showMeLableVar(Lvar_GROSS) );
        SP("Remark       : "); SPL();
        SP("Shift        : "); SPL( Wtft.showMeLableVar(SHIFT) );
    
        SP( getFooter() );
        SPL();
        SPL();
        SPL();
 /* ===================================================================== */
   // sample bill print Format -2
  /*
    static int count=1, sample2=1;
    //Header part must print Once
    if( sample2 ){
    SP( getHeader() );
    SPL();

    #define adjustSpace(xx) for(uint8_t i=0; i < ( 15-strlen(xx) ); i++ ) SP(' ');

    SP("Product  : "); SP( Wtft.showMeLableVar(PRODUCT_NAME) );    adjustSpace( (  Wtft.showMeLableVar(PRODUCT_NAME) ) );     SP("Operator Name : "); SPL(Wtft.showMeLableVar(Operator_Name));
    SP("Batch No : "); SP("123456789"); adjustSpace("123456789");  SP("Line No       : "); SPL("1");
    SP("Date     : "); SP(dateArray);   adjustSpace(dateArray);    SP("Shift         : "); SPL(Wtft.showMeLableVar(SHIFT));
    char tempbuff[] = "==================================================";
    //SPL( strlen(tempbuff) );//max length is 50
    SPL(tempbuff);
    SPL("S.no         Time           Gross Weight  ");
    SPL(tempbuff);
    sample2 =0;
    }

    //printable data
    if( count < 6 ){
    SP(snum++);  SP("            ");
    SP(preTime); SP("            ");
    SPL( Wtft.showMeLableVar(2) );
    count++;
    }else{
    count = 1;
    sample2 = 1;
    }


    // footer part must print after 5 print has been done.
    if( sample2 ){
    SP( getFooter() );
    SPL();
    SPL();
    SPL();
}
*/


    break;

    case 1 :
    
      /* 1 -> It is tested on Gainscha Printer using ZPL printer Label.
         2 -> Here we are replacing variables with our machine real time values.
         3 -> use this code to check,
      
             Wtft.printLebel();
      */
        // sample print lable format -3
         Wtft.printLebel();
    break;
  }
 



}



int8_t weight :: activityBasedOnTouch() {
  char src[12] = {0};

  switch (  getactiveMachineKeyMapping() ){
    case map_CmdESC   : pressed = 0; return -1;
    case map_CmdUnits : break; // as per requirement
    case map_CmdPrint : printerLevel(); break; // as per requirement
    case map_CmdZero  : CMD_ZERODATA break;
    case map_CmdTare  : CMD_AUTOTARE break;
    case open_tareWin : goto TARE_WEIGHT;
  }//end-switch

  // if ( ( pressed = tft.getTouch(&xAxis, &yAxis) ) == 0 ) return 0;

  if ( pressed == 0 ) return 0;

  if ( pressed )
  {
    //  SPL("pressed : " + String(pressed));
    pressed = 0;

    if ( ESC_Touch ){
      Wtft.popupButton(Xesc, Ybutton, TFT_RED);
      delay(500); 
     // SPL("ESCTouch...>>X:  " + String(xAxis) + "Y : " + String(yAxis) );
      xAxis = 0;
      yAxis = 0;
      STOP_SERIAL2
      EMPTY_SERIALBUFFER
      pressed = 0;

#ifdef ATTACH_INTER
      ISR_Called = 0;
      detachInterrupt(GPIOPin_TIRQ);
      ISR_dtech = 1;
#endif
     
      Wtft.popupButton(Xesc, Ybutton, TFT_GREEN);
      return -1;
    }
    
    else if ( Taretouch_Auto ){
      Wtft.popupButton(Xtare, Ybutton, TFT_RED);
      CMD_AUTOTARE
      delay(250);
      Wtft.popupButton(Xtare, Ybutton, TFT_BLACK);
      return 0;
    }
    else if ( Zerotouch ){
       Wtft.popupButton(Xzero, Ybutton, TFT_RED);
      CMD_ZERODATA
      delay(250);
       Wtft.popupButton(Xzero, Ybutton, TFT_BLACK);
      return 0;
    }
    else if ( Field_three_Touch  )
    {
TARE_WEIGHT :

      STOP_SERIAL2
      START_SERIAL1
      kbd.userInput.userInputArray_Size = 25;
      kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];
      kbd.init( );
      kbd.userInput.numericSwitchFlag = 1;
      strcpy( kbd.userInput.fieldLable, "TARE" );
      strcpy( kbd.userInput.unitbuf,  _getWeighingUnit().c_str() );
      kbd.takeUserInput(  NULL );
      SPL("keyboard : " + String( kbd.userInput.userInputArray ) );
      START_SERIAL2
      char cmd[20] = "\2T#";
      strcat(cmd, kbd.userInput.userInputArray );
      cmd[strlen(cmd)] = 3;
      Serial2.print(cmd);
      //  SPL("Manual Tare : " + String(cmd) );
      initTFTHandler();
      printStringWeight( );
      CMD_GETTARE
      _readbuf( );
      delete[]  kbd.userInput.userInputArray;
    }
    else if ( PRT_Touch ) {
      //Serial.printf("printer xAxis : %ld, yAxis : %ld\n", xAxis, yAxis);
     Wtft.popupButton(Xprint, Ybutton, TFT_RED);
      printerLevel();
      delay(500);
     Wtft.popupButton(Xprint, Ybutton, TFT_BLACK);
    }

  }//end-if
  return 0;
}



String  weight :: _readbuf( ) {
  temp =  "";
HERE :
  // 1. Get data from weighing machine
  if ( Serial2.available() > 12  )
  {
    temp = Serial2.readStringUntil('=');
  }

  //2. check, is String temp contains <STX> <ETX> ??
  int8_t stx = temp.indexOf(2);
  if ( stx >= 0 ) {
    char ch = temp[stx + 1];
    switch (ch)
    {
      case '1':
        numericValues[GROSS] = 0.00;
        numericValues[NET] = 0.00;
        numericValues[TARE] = 0.00;
        handleTareCommand( "0.00"  );
        break;
      case 'T':
        handleTareCommand( (char *)temp.c_str() );
        break;

      default : break;
    }
    temp = "";
    goto HERE;
  }
  if (  temp.length() > 7 ) {
    handleFlags( (char *)temp.c_str() );
  }

  return temp;
}



bool   weight :: weightStripImage() {
  tft.setSwapBytes(true);
  tft.pushImage(10, 0, WeightModeStripWidth, WeightModeStripHeight, WeightModeStrip );
}

bool  weight :: printStringWeight( ) {
  String weightUnit = _getWeighingUnit();
  weightStripImage();
 // SPL("weightUnit : " + String(weightUnit) );
  tft.setTextSize( 1 );


//  SPL("getDate+  : " + getRTCDate() );
  parshDateformat(dateArray,  getRTCDate().c_str() );
 // SPL("parseDate  : " + String( dateArray ) );

  // Window -1
  tft.setFreeFont( (const GFXfont *)EUROSTILE_B13 );
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(18, 50);  tft.print("NET WT");
  tft.setCursor(400, 50);  tft.print(weightUnit);

  // Window -2
  tft.setFreeFont( (const GFXfont *)EUROSTILE_B10 );
  tft.setCursor(18, 173);  tft.print("GROSS WT");
  tft.setCursor(180, 173);   tft.print(weightUnit);

  // Window -3
  tft.setCursor(260, 173);  tft.print("TARE WT");
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



void weight :: handleTareCommand( char *Weight  ) {
  String cmdBuf = "";
  String temp = Weight;

  int8_t stx = temp.indexOf(2);
  int8_t etx = temp.indexOf(3);

  for (int8_t leftIdx = stx + 3; leftIdx < etx; ++leftIdx )
  {
    cmdBuf += temp[leftIdx];
  }

  updateField = TARE;
  memset( asciiTemp, '\0', 10);
  strcpy(asciiTemp , cmdBuf.c_str());
  _rawCalculation();
  onScreen( TARE );
  updateField = NET;
}


#endif
