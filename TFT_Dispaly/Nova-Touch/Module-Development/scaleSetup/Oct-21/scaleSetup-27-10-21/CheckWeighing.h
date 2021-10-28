

#ifndef __CHECK_H
#define __CHECK_H


#include "commonGlobal.h"
#include "InterruptHandler.h"


typedef enum e_CheckKey{
  Nor = 0,
  Hi  = 1,
  Low = 2,
  MAX = 0,
  CHECK_NetWeight = 1,
  MIN = 2
};


class checkWeighing: public WeighingHandle{
  private :
     char dateArray[10];
    int8_t flag = Nor;
    int8_t digitSpeed = 0;
    uint8_t updateField = CHECK_NetWeight;
    String temp;
    char asciiTemp[10];
    char asciiValues[3][10];
    char preAsciiValues[3][10];
    double numericValues[3], tareWeight = 0.0;

    String _readbuf( );
    void onScreen(uint8_t showMeAt );
    void _rawCalculation();

    bool printStringCHECK( );
    bool checkStripImage();
  
    
  public :
    class WeighingHandle commFun;
    class userKeyBoard kbd;

    void  _start();
    void checkHandler();
    int8_t activityBasedOnTouch();

      int8_t printerLevel();
};

int8_t checkWeighing :: printerLevel(){
   //1. Start Serial-0 port
   static int snum = 0;
  
  // Update Printer Variables 
    Wtft.updateLableVar( DATE , dateArray );
    Wtft.updateLableVar( TIME , preTime );
     
    Wtft.updateLableVar( L_WEIGHT , asciiValues[MIN]);
    Wtft.updateLableVar( H_WEIGHT , asciiValues[MAX]);
    Wtft.updateLableVar( Lvar_NET , asciiValues[CHECK_NetWeight]);

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
        SP("Low Weight   : "); SPL( Wtft.showMeLableVar(L_WEIGHT) );
        SP("High Weight  : "); SPL( Wtft.showMeLableVar(H_WEIGHT) );
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

int8_t  checkWeighing ::  activityBasedOnTouch() {
  char src[12] = {0};
  int8_t tempDot = showDigits.dotPosition;

  switch ( getactiveMachineKeyMapping() ) {
    case map_CmdESC   :  pressed = 0; return -1;
    case map_CmdUnits : break; // as per requirement
    case map_CmdPrint : break; // as per requirement
    case map_CmdZero  : CMD_ZERODATA break;
    case map_CmdTare  : CMD_AUTOTARE break;
    case open_minWin  :  updateField = MIN; goto HERE;
    case open_maxWin  :  updateField = MAX; goto HERE;
  }//end-switch

  // if ( ( pressed = ( ( pressed )? pressed : tft.getTouch(&xAxis, &yAxis) ) == 0 ) ) return 0;

  if ( ( pressed == 0 )  ) return 0;

  if ( pressed ) {
    pressed = 0;
    if ( Field_Two_Touch  ) {
      updateField = MAX;
    }
    else if ( Field_three_Touch ) {
      updateField = MIN;
    }
    else if ( Zerotouch )     {
      Wtft.popupButton(Xzero,Ybutton, TFT_RED); 
      CMD_ZERODATA
      delay(250);
      Wtft.popupButton(Xzero,Ybutton, TFT_BLACK); 
    }
    else if ( Taretouch_Auto ) {
      Wtft.popupButton(Xtare,Ybutton, TFT_RED); 
      CMD_AUTOTARE
      delay(250);
      Wtft.popupButton(Xtare,Ybutton, TFT_BLACK); 
    }
    else if ( ESC_Touch ) {
//      SPL("ESCTouch...\n");
      return -1;
    }
    else if( PRT_Touch ){
      Wtft.popupButton(Xprint,Ybutton, TFT_RED); 
      printerLevel();
      Wtft.popupButton(Xprint,Ybutton, TFT_BLACK); 
    }

  }//end-if( TFT_Touch )


  if ( ( updateField != CHECK_NetWeight ) ) {
HERE :

    STOP_SERIAL2
    START_SERIAL1
    kbd.userInput.userInputArray_Size = 25;
    kbd.userInput.userInputArray = new char[kbd.userInput.userInputArray_Size];
    kbd.init(  );
    kbd.userInput.numericSwitchFlag = 1;
    if ( updateField == MAX )
      strcpy( kbd.userInput.fieldLable, "MAX Weight" );
    else
      strcpy( kbd.userInput.fieldLable, "MIN Weight" );

    strcpy( kbd.userInput.unitbuf,  _getWeighingUnit().c_str() );

    kbd.takeUserInput( NULL );
    SPL("keyboard : " + String( kbd.userInput.userInputArray ) );

    if ( strlen( kbd.userInput.userInputArray ) > 0 ) {
      strcpy(asciiTemp, kbd.userInput.userInputArray );
      _rawCalculation();
    }
    STOP_SERIAL1
    START_SERIAL2

    initTFTHandler();
    printStringCHECK( );

    strcpy(preAsciiValues[CHECK_NetWeight], "ABCDEFGH");
    _rawCalculation();
    delay(100);
    onScreen(CHECK_NetWeight);
    onScreen(MAX);
    onScreen(MIN);

    delete[] kbd.userInput.userInputArray;
    updateField = CHECK_NetWeight;
    START_SERIAL1
  }//end-if( updateField )


  return 0;

}


void   checkWeighing :: onScreen(uint8_t showMeAt ) {
  bufferWithoutDot( showDigits.currentValue,  asciiValues[showMeAt] );
  showDigits.currentValue[6] = '\0';

  switch ( showMeAt ) {
    case CHECK_NetWeight:  windowOne(); break;
    case MAX            :  
    Serial.printf("max->%s\nnet->%s\nmin->%s",preAsciiValues[MAX], preAsciiValues[CHECK_NetWeight], preAsciiValues[MIN] );
    windowTwo(); break;
    case MIN            :  windowThree(); break;
  }
}


/*
   NOte : There are two parameter which is need to be care.Before calling this function these two field must be updated.
          1 >> updateField -->> this is the field which you want to update
          2 >> asciiTemp -->> this the temprary Array variable which you want to update.
*/
void   checkWeighing ::_rawCalculation() {

  uint8_t dotpos;
  static uint16_t preColorFlag;
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


  if ( ( numericValues[MAX] == 0.0 ) && ( numericValues[MIN] == 0.0 ) ) {
    showDigits.showDigitColorWindowOne = TFT_WHITE; //TFT_BLUE;
    if (preColorFlag !=  showDigits.showDigitColorWindowOne ) {
      strcpy( showDigits.preValue[0], "ABCDEFGH");
    }
    preColorFlag = TFT_WHITE; //TFT_BLUE;
  }

  else if( (numericValues[updateField] >=  numericValues[MIN]) && ( numericValues[MAX] == 0.0) ) {
    if (  showDigits.showDigitColorWindowOne != TFT_GREEN ) {
           showDigits.showDigitColorWindowOne = TFT_GREEN;
           if (preColorFlag !=  showDigits.showDigitColorWindowOne ) {
               strcpy( showDigits.preValue[0], "ABCDEFGH");
            }//end-if
      preColorFlag = TFT_GREEN;

      tft.setTextSize( 1 );
      tft.setTextColor( showDigits.showDigitColorWindowOne, TFT_BLACK);
      tft.setFreeFont( (const GFXfont *)EUROSTILE_B13 );
      tft.fillRect(220, 30, 35, 25, TFT_PURPLE );
      //      tft.drawString("OK", 220, 30, 1);
      tft.setCursor(220, 50);
      tft.print("OK");
    }//end-if  
  }//end-else-if

  else if( (numericValues[updateField] <=  numericValues[MIN]) && ( numericValues[MAX] == 0.0) ) {
    if (  showDigits.showDigitColorWindowOne != TFT_YELLOW ) {
           showDigits.showDigitColorWindowOne = TFT_YELLOW;
           if (preColorFlag !=  showDigits.showDigitColorWindowOne ) {
               strcpy( showDigits.preValue[0], "ABCDEFGH");
            }//end-if
      preColorFlag = TFT_YELLOW;

      tft.setTextSize( 1 );
      tft.setTextColor( showDigits.showDigitColorWindowOne, TFT_BLACK);
      tft.setFreeFont( (const GFXfont *)EUROSTILE_B13 );
      tft.fillRect(220, 30, 35, 25, TFT_PURPLE );
      //      tft.drawString("OK", 220, 30, 1);
      tft.setCursor(220, 50);
      tft.print("LO");
    }//end-if  
  }//end-else-if

  else if ( (numericValues[updateField] >=  numericValues[MIN]) && ( numericValues[updateField] <  numericValues[MAX] ) ) {
    if (  showDigits.showDigitColorWindowOne != TFT_GREEN ) {
      showDigits.showDigitColorWindowOne = TFT_GREEN;
      if (preColorFlag !=  showDigits.showDigitColorWindowOne ) {
        strcpy( showDigits.preValue[0], "ABCDEFGH");
      }
      preColorFlag = TFT_GREEN;
      tft.setTextSize( 1 );
      tft.setTextColor( showDigits.showDigitColorWindowOne, TFT_BLACK);
      tft.setFreeFont( (const GFXfont *)EUROSTILE_B13 );
      tft.fillRect(220, 30, 35, 25, TFT_PURPLE );
      //      tft.drawString("OK", 220, 30, 1);
      tft.setCursor(220, 50);
      tft.print("OK");
    }
  }
 
  else if (  numericValues[updateField] >  numericValues[MAX] ) {
    if (  showDigits.showDigitColorWindowOne != TFT_RED ) {
      showDigits.showDigitColorWindowOne = TFT_RED;
      if (preColorFlag !=  showDigits.showDigitColorWindowOne ) {
        strcpy( showDigits.preValue[0], "ABCDEFGH");
      }
      preColorFlag = TFT_RED;
      tft.setTextSize( 1 );
      tft.setTextColor( showDigits.showDigitColorWindowOne, TFT_BLACK);
      tft.setFreeFont( (const GFXfont *)EUROSTILE_B13 );
      tft.fillRect(220, 30, 35, 25, TFT_PURPLE );
      //      tft.drawString("HI ", 220, 30, 1);
      tft.setCursor(220, 50);
      tft.print("HI");
    }
  } else if ( numericValues[updateField] <=  numericValues[MIN] ) {
    if (  showDigits.showDigitColorWindowOne != TFT_YELLOW ) {
      showDigits.showDigitColorWindowOne = TFT_YELLOW;
      if (preColorFlag !=  showDigits.showDigitColorWindowOne ) {
        strcpy( showDigits.preValue[0], "ABCDEFGH");
      }
      preColorFlag = TFT_YELLOW;

      tft.setTextSize( 1 );
      tft.setTextColor( showDigits.showDigitColorWindowOne, TFT_BLACK);
      tft.setFreeFont( (const GFXfont *)EUROSTILE_B13 );
      tft.fillRect(220, 30, 35, 25, TFT_PURPLE  );
      //  tft.drawString("LO", 220, 30, 1);
      tft.setCursor(220, 50);
      tft.print("LO");
    }
  }

}//end-_rawCalcualtion()



void  checkWeighing :: checkHandler() {
  memset(asciiTemp, '\0', 10);
  strcpy( asciiTemp, _readbuf( ).c_str() );  asciiTemp[7] = '\0';
  if ( strlen(asciiTemp) > 5 ) {
    _rawCalculation();

    if ( ++digitSpeed > 2 ) {
      onScreen( CHECK_NetWeight );
      digitSpeed = 0;
    }
  }

  DateNTimeHandler();
}



String checkWeighing :: _readbuf( ) {
  temp =  "";
HERE :
  // 1. Get data from weighing machine
  //if( Serial2.available() > 0 )
  temp = Serial2.readStringUntil('=');

  //2. check, is String temp contains <STX> <ETX> ??
  int8_t stx = temp.indexOf(2);
  if ( stx >= 0 )
  {
    char ch = temp[stx + 1];
    switch (ch)
    {
      case '1'://Zero Command Response
        tareWeight = 0.0;
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




void  checkWeighing :: _start() {
  char timeArray[10];
  char preTime[10];

  initTFTHandler ( );
  printStringCHECK( );
  parshTimeFormat(timeArray, getRTCTime().c_str() )
  strcpy( preTime, timeArray );
  showDigits.dotPosition = _getDecimal().c_str()[0] - 48;

  updateField = MAX;
  strcpy(asciiTemp, "0.0" );
  _rawCalculation();

  updateField = MIN;
  strcpy(asciiTemp, "0.0" );
  _rawCalculation();

  onScreen(MAX);
  onScreen(MIN);

  updateField = CHECK_NetWeight;
  pressed = 0;
  TouchInterrupt_ptr = myfastfunction;
  ATTACH_TOUCH_INTERRUPT
  ISR_dtech = 0;

}




bool  checkWeighing :: checkStripImage() {
  tft.setSwapBytes(true);
  tft.pushImage( 10, 0, checkingModeStripWidth, checkingModeStripHeight, checkingModeStrip );
}



bool  checkWeighing :: printStringCHECK( ) {
  checkStripImage();
  String weightUnit = _getWeighingUnit();
  tft.setTextSize( 1 );
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  parshDateformat(dateArray, getRTCDate().c_str() );

  // Window-1
  tft.setFreeFont( (const GFXfont *)EUROSTILE_B13 );
  tft.setCursor(18, 50);  tft.print("TOTAL WT");
  tft.setCursor(400, 50);  tft.print(weightUnit);

  // Window-2
  tft.setFreeFont( (const GFXfont *)EUROSTILE_B10 );
  tft.setCursor(18, 173);  tft.print("MAX WT");
  tft.setCursor(180, 173);   tft.print(weightUnit);

  // window-3
  tft.setCursor(260, 173);  tft.print("MIN WT");
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
             + "  " + String( dateArray  ) );

  char timeArray[10];
  memset(timeArray, '\0', 10);
  parshTimeFormat(timeArray, getRTCTime() )
  tft.setCursor(400, 310);
  tft.print("  " + String(timeArray) + String("pm") );
}




#endif
