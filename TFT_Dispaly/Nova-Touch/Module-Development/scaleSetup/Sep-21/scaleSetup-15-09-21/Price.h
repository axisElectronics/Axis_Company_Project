
#ifndef _PRICE_H
#define _PRICE_H


#include "commonGlobal.h"
#include "InterruptHandler.h"


extern void createThread();
extern void vTask_SerialButtonMonitor( void * pvParameters );

enum {
  PRICE_perPcs = 1,
  PRICE_Total = 0,
  PRICE_NetWeight = 2
};

class price : public WeighingHandle{
  private :
  char asciiTemp_perUnit[10];
  uint8_t digitSpeed;

 
  
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

     int8_t printerLevel();

};

#define  on_Screen( PRICE_NetWeight ) _rawCalculation()
#define on_Screen(PRICE_Total)  updateTotalPriceWindow()

extern  char  preBuff[10], Serialtemp[10];
extern  uint8_t  DigitAfterDot;
extern  bool noDotFound;
extern bool userInputAtRunTime;
extern  TaskHandle_t xHandle;
extern SemaphoreHandle_t xSemaphore_Lock;
extern  double value,  temp, preValue;
extern QueueHandle_t xQueue1, xQueue2;

int8_t  price ::  printerLevel(){
  //1. Start Serial-0 port
  Serial.println("Printer has been touched...!!!"); 
}

int8_t  price ::  activityBasedOnTouch() {
  char src[12] = {0};
  int8_t tempDot = showDigits.dotPosition;

  switch (  getactiveMachineKeyMapping() ) {
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
    else if ( Field_Two_Touch  ) {
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
      strcpy( asciiValues[updateField],  kbd.userInput.userInputArray );
      numericValues[updateField] = strtod( asciiValues[updateField], NULL);

      //enable Serial ports
      START_SERIAL2
      STOP_SERIAL1
      // Load screen images again.
      initTFTHandler();
      printStringPrice();

      // SHOW DATA ON SCREEN
      onScreen(PRICE_perPcs);
      onScreen(PRICE_NetWeight);
      onScreen(PRICE_Total);
      updateField = PRICE_NetWeight;

      delete[]  kbd.userInput.userInputArray;
      START_SERIAL1
    }
    else if( PRT_Touch ){
      //Serial.printf("printer xAxis : %ld, yAxis : %ld\n", xAxis, yAxis);
      printerLevel();
    }

  
  }//end-if
  return 0;
}



void  price :: updateTotalPriceWindow() {

// Do nothing untill Weight is stable
 // if( ( flags & 0x04 ))
  {
    
  numericValues[PRICE_Total] = numericValues[PRICE_NetWeight] * numericValues[PRICE_perPcs] ;
  sprintf( asciiValues[PRICE_Total], "%lf",  ( numericValues[PRICE_Total] ) );

  if ( strcmp(  preAsciiValues[PRICE_Total] , asciiValues[PRICE_Total] ) == 0) return;
  strcpy(  preAsciiValues[PRICE_Total] , asciiValues[PRICE_Total] );

  ////===========================================================//
  // Below code is responsiable for manipulating Data
  ////===========================================================//

  uint8_t tempDotPos = 0;
  char tempArray[15];
  int8_t leadingZero = 0;

  // find Dot position
  for (uint8_t i = 0; i < 7; ++i) {
    if (asciiValues[PRICE_Total][i] == '.') {
      tempDotPos = i;
      break;
    }
  }

  if ( tempDotPos < 6 ) {
    memset(tempArray, '\0', 15);

    for (uint8_t i = 0; i < tempDotPos; ++i ) {
      tempArray[i] = asciiValues[PRICE_Total][i];
    }
    tempArray[ tempDotPos + 0 ] = asciiValues[PRICE_Total][ tempDotPos + 1 ];
    tempArray[ tempDotPos + 1 ] = asciiValues[PRICE_Total][ tempDotPos + 2 ];
    tempArray[ tempDotPos + 2 ] = '\0';

    memset(showDigits.currentValue, '0', 7);
    strcpy( &showDigits.currentValue[7 - strlen(tempArray)],  tempArray );
    showDigits.currentValue[7] = '\0';
  }
  else if ( tempDotPos >= 6 ) {
    SP("tempDotPos : "); SPL(tempDotPos);
    return;
  }

  ////===========================================================//
  // Below code is responsiable for drawing Digits on Screen
  ////===========================================================//
 static uint8_t skipcnt = 20, drawRectCnt=0;
    
  //1. check with previous Value
  if ( !strcmp( showDigits.preValue[0] , showDigits.currentValue ) )  return;

  
  
  leadingZero = 7 -  strlen(tempArray);
  tempDotPos = 4;

  // 2. Set Font Size & Style
  showDigits.digitFontSize  = 1;
  showDigits.digitFontStyle = LED7SEG_STD40;
  tft.setTextSize( showDigits.digitFontSize );
  tft.setFreeFont( (const GFXfont *)showDigits.digitFontStyle );


  //3. draw blank rectangle only those digits which is different from previous Value.
   if ( drawRectCnt++ > 20 ){
      for (uint8_t idx = 0; idx < 7; ++idx ){
        if ( showDigits.preValue[0][idx] !=  showDigits.currentValue[idx] ){
          tft.fillRect( 15  + ( idx * 65),  58,   62,  90,  TFT_BLACK);   
        }//end-if
      }//end-for
      drawRectCnt= 0;
   }//end-if
 
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
  }//end-stable Flag
}//end


static uint32_t Timeout = 0;
void price :: unitePrice() {


  if ( xSemaphoreTake( xSemaphore_Lock, ( TickType_t )1 ) == pdTRUE ) {
    // SPL("Sema is taken by unitPrice");
    int8_t leadingZero = 0;
    uint8_t setDot = 0;
    uint8_t bufflen = 0;
    char *buff = NULL;
    static char currentValue[10];

    if( xQueue1 != NULL )
   {
    memset(asciiTemp_perUnit, '0', 10);
      if( xQueueReceive( xQueue1,
                         &( asciiTemp_perUnit ),
                         ( TickType_t ) 10 ) == pdPASS )
      {
         /* *pxRxedPointer now points to xMessage. */
       //  SP("asciiTemp_perUnit_InMain : "); SPL(asciiTemp_perUnit);
        if( strcmp( asciiTemp_perUnit, "0.000000" ) == 0 ){
           memset(currentValue, '0', 7 );
          leadingZero = 7 - 3;
          goto   EXIT_ZERO;
        }

      }
   }
   else return;
     
    for (int8_t i = 0; i < 7; ++i ) {
      if ( asciiTemp_perUnit[i] == '.' ) {
        setDot = i;
        break;
      }
    }
FIXDATA :
    if ( userInputAtRunTime == true ) {
      
      if ( ( setDot >= 0 && setDot < 6 ) ) {
       buff = new char[10];
       memset(buff, '0', 7);
        for (uint8_t i = 0; i < setDot; ++i ) {
            buff[i] = asciiTemp_perUnit[i];
        }
          buff[setDot + 0] = asciiTemp_perUnit[setDot + 1];
          buff[setDot + 1] = asciiTemp_perUnit[setDot + 2];
          buff[setDot + 2] = '\0';
          if ( DigitAfterDot == 2 ) {
            userInputAtRunTime = false;
          }
        }//end-setDot(valid)
        else if (setDot >= 6) {
        SP("setDot Error : "); SPL(setDot);
        value = 0;
        memset(asciiTemp_perUnit, '0', 7);
        memset(currentValue, '0', 7 );
        leadingZero = 7 - 3;
        goto   EXIT_ZERO;
      }

        if ( ( strlen(buff) == 0 ) && setDot) {
          SPL("Data has been Corrput..");
          if( buff != NULL )
              delete(buff);
          preValue = 0;
          goto EXIT;
        }
        
        bufflen = strlen(buff);
        if (bufflen != 0 ){
        memset(currentValue, '\0', sizeof(currentValue) );
        memset(currentValue, '0', 7 );
        strcpy( &currentValue[7 - strlen(buff)], buff );
        }
        delete(buff);
      }//end-setDot
     


      {
        ////===========================================================//
        // Below code is responsiable for drawing Digits on Screen.
        // Important Things
        // 1 > dot position
        // 2 > leading Zeros
        ////===========================================================//

        //1. check with previous Value
        if ( !strcmp( showDigits.preValue[1] , currentValue ) ) {
          goto EXIT;
        }
        
       // Return if asciiValue trunc to Zero 
       if( strcmp( currentValue, "0000000" ) == 0 ){
        //SPL("return ");
        // end : update preVlaue
        strcpy( showDigits.preValue[1], currentValue ); 
        yield();
        goto EXIT;    
       }
           
        leadingZero = 7 - bufflen; 
        
        EXIT_ZERO :        
        setDot = 4;
        strcpy( showDigits.preValue[1] , "ABCDEFGH" );
      //  SPL("currentValue >>: " + String(currentValue) );

        // 1. Set Font Size
        showDigits.digitFontSize  = 1;
        showDigits.digitFontStyle = LED7SEG_STD18;
        tft.setTextSize( showDigits.digitFontSize );
        tft.setFreeFont( (const GFXfont *)showDigits.digitFontStyle );

        //eliminate leading Zeros.
        tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
        for (uint8_t idx = 0; idx < leadingZero; ++idx ) {
          tft.drawChar(  '8', 15 + ( idx * 31) , 210,  1);
        }


        tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
        for (uint8_t idx = 0; idx < 7; ++idx ) {
          if ( showDigits.preValue[1][idx] != currentValue[idx] )
          {
            tft.fillRect( 15  + ( idx * 31),  180,   30,  40,  TFT_BLACK );
            tft.drawChar(  '8', 15 + ( idx * 31) , 210,  1);
          }
        }


        //5. remove all previous dots and redraw at given position;
        for (uint8_t idx = 0; idx < 6; ++idx ) {
          tft.fillCircle( 41 + ( idx * 31), 216, 3, TFT_BLACK);
        }
     
          // Draw dot at given position.
          tft.fillCircle( 41 + ( ( setDot ) * 31), 216, 3, TFT_RED );
     

        //6. draw Digits only,, Those digits which is different from previous Value.
        tft.setTextColor( TFT_GREEN, TFT_BLACK );
        for (uint8_t idx = leadingZero; idx < 7; ++idx ) {
          if ( showDigits.preValue[1][idx] !=  currentValue[idx] )
          {
            tft.drawChar(  currentValue[idx], 15 + ( idx * 31) , 210,  1);
          }
        }

        // end : update preVlaue
        strcpy( showDigits.preValue[1], currentValue );

        if ( userInputAtRunTime == false) {
          if ( xHandle != NULL ) {
            SPL("TaskSuspend ....!!!");
         //   vTaskSuspend( xHandle );
         //   xHandle = NULL;
            // Draw dot at given position.
            tft.fillCircle( 41 + ( ( setDot ) * 31), 216, 3, TFT_RED );
         //   vTaskResume( xHandle );
          }
        }
      }

EXIT :
    xSemaphoreGive( xSemaphore_Lock );
  }//end-if(sema_lock)
  yield();
}//end-unitPrice




void   price :: onScreen(uint8_t showMeAt ) {

  switch ( showMeAt ) {
    case PRICE_perPcs      :  unitePrice();    break;
    case PRICE_Total       :  updateTotalPriceWindow();  break;
    case PRICE_NetWeight   :  _rawCalculation();    break;
  }
}




/*
   NOte : There are two parameter which is need to be care.Before calling this function these two field must be updated.
          1 >> updateField -->> this is the field which you want to update
          2 >> asciiTemp -->> this the temprary Array variable which you want to update.
*/
void   price ::_rawCalculation() {

  // SPL("Step-1");
 // SP("ascii value : "); SPL( asciiValues[PRICE_NetWeight] );
  memset(showDigits.currentValue, '0', 7);
  for (uint8_t i = 0, j = 0; i < 7; i++ ) {
    if (  asciiValues[PRICE_NetWeight][i] != '.' )
      showDigits.currentValue[j++] =  asciiValues[PRICE_NetWeight][i];
  }
  showDigits.currentValue[7] = '\0';
  // Below function is responsibale for showing Digits on Screen
 // SP(" showDigits.currentValue : ");SPL( showDigits.currentValue);
  windowThree();

  // update values for later uses
  numericValues[PRICE_NetWeight] =  strtod(asciiValues[ PRICE_NetWeight ], NULL);
  // SPL("Step-2");
  yield();
}//end-_rawCalcualtion()



void  price :: priceHandler() {
 
  strcpy( asciiValues[ PRICE_NetWeight ], _readbuf( ).c_str() );
  asciiValues[ PRICE_NetWeight ][7] = '\0';
//  SP("ascii value : "); SPL( asciiValues[PRICE_NetWeight] );
  
  if ( ( strlen(asciiValues[ PRICE_NetWeight ]) > 5 ) ) {  
    _rawCalculation();
    onScreen(PRICE_NetWeight);

    if( ++digitSpeed > 2 ){
 
      digitSpeed = 0;     
      onScreen(PRICE_Total);
   
  }//end-digitSpeed
  }
  onScreen(PRICE_perPcs);
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

//  SP("readbuff temp : "); SPL( temp );
  return temp;
}







void  price :: _start() {
  char timeArray[10];
  char preTime[10];
  TaskHandle_t xHandle;

  initTFTHandler ( );
  printStringPrice();
  showDigits.dotPosition = _getDecimal().c_str()[0] - 48;;

  class command cmd;
  if( cmd.sendCommand( LOWDATA ) ){
    SPL("LOW Precision has been set...!!!");
  }else{
    SPL("LOW Precision has not set...!!!");
  }



  // Reset All BUffers
  char dummyBuff[10] = "ABCDEFGH";
  strcpy(asciiTemp, dummyBuff);
  for (uint8_t i = 0; i < 3; i++ ) {
    strcpy(asciiValues[i], dummyBuff);
    strcpy(preAsciiValues[i], dummyBuff);
  }

  /*
    onScreen(PRICE_NetWeight);
    onScreen(PRICE_perPcs);
    onScreen(PRICE_Total);
  */
  updateField = PRICE_NetWeight;

  pressed = 0;
  TouchInterrupt_ptr = myfastfunction;
  ATTACH_TOUCH_INTERRUPT
  ISR_dtech = 0;


  // make a thread which will take care all keypad data.
  createThread();

  SPL("STEP-1");
}






bool  price :: priceStripImage() {
  tft.setSwapBytes(true);
  tft.pushImage(10, 0, PriceComputingModeStripWidth, PriceComputingModeStripHeight, PriceComputingModeStrip );
}


bool  price :: printStringPrice( ) {
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
