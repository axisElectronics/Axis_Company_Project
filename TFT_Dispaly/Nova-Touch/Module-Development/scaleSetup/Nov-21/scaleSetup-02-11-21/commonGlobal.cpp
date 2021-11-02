
/*********************************************************************
   @FileName : commonGlobal.cpp

 **********************************************************************/
#include "commonGlobal.h"


#include "Price.h"
#include "checkWeighing.h"
#include "Counting.h"
#include "Weighing.h"
#include "InterruptHandler.h"

//class declarations
class userKeyBoard kbd;
class price priceObj;
class checkWeighing checkObj;
class countingMachine countM;
class weight weightObj;


/******* MACROs declaration ********/
#define spacing 90
#define digitMargin 90
#define xdigitMargin digitMargin
#define ydigitMargin digitMargin

#define xClr 160
#define yClr 145
#define xOne   ( xClr + spacing)
#define yOne    yClr
#define xTwo    ( xOne + spacing )
#define yTwo    yOne
#define xThree  ( xTwo + spacing )
#define yThree  yTwo

#define xFour   xOne
#define yFour   60
#define xFive   ( xOne + spacing )
#define yFive   yFour
#define xSix  ( xTwo + spacing )
#define ySix    yFive

#define xSeven  xOne
#define ySeven  0
#define xEight ( xOne + spacing )
#define yEight  ySeven
#define xNine   ( xTwo + spacing )
#define yNine   yEight


#define xBack 2
#define yBack  239

#define xDel (160)
#define yDel  yBack

#define xDot  ( xDel + spacing )
#define yDot  yBack

#define xZero ( xDot + spacing )
#define yZero yBack

#define xEnt ( xZero + spacing )
#define yEnt yBack

#define zeroTouch      ( ( (xAxis > xZero  )   && (xAxis < ( xZero  + digitMargin ) ) )   && ( ( yAxis > yZero  ) && ( yAxis < ( yZero + ydigitMargin  ) ) ) )
#define oneTouch       ( ( (xAxis > xOne   )   && (xAxis < ( xOne   + digitMargin ) ) )   && ( ( yAxis > yOne   ) && ( yAxis < ( yOne + ydigitMargin   ) ) ) )
#define twoTouch       ( ( (xAxis > xTwo   )   && (xAxis < ( xTwo   + digitMargin ) ) )   && ( ( yAxis > yTwo   ) && ( yAxis < ( yTwo + ydigitMargin   ) ) ) )
#define threeTouch     ( ( (xAxis > xThree )   && (xAxis < ( xThree + digitMargin ) ) )   && ( ( yAxis > yThree ) && ( yAxis < ( yThree + ydigitMargin ) ) ) )
#define fourTouch      ( ( (xAxis > xFour  )   && (xAxis < ( xFour  + digitMargin ) ) )   && ( ( yAxis > yFour  ) && ( yAxis < ( yFour + ydigitMargin  ) ) ) )
#define fiveTouch      ( ( (xAxis > xFive  )   && (xAxis < ( xFive  + digitMargin ) ) )   && ( ( yAxis > yFive  ) && ( yAxis < ( yFive + ydigitMargin  ) ) ) )
#define sixTouch       ( ( (xAxis > xSix   )   && (xAxis < ( xSix   + digitMargin ) ) )   && ( ( yAxis > ySix   ) && ( yAxis < ( ySix + ydigitMargin   ) ) ) )
#define sevenTouch     ( ( (xAxis > xSeven )   && (xAxis < ( xSeven + digitMargin ) ) )   && ( ( yAxis > ySeven ) && ( yAxis < ( ySeven + ydigitMargin ) ) ) )
#define eightTouch     ( ( (xAxis > xEight )   && (xAxis < ( xEight + digitMargin ) ) )   && ( ( yAxis > yEight ) && ( yAxis < ( yEight + ydigitMargin ) ) ) )
#define nineTouch      ( ( (xAxis > xNine  )   && (xAxis < ( xNine  + digitMargin ) ) )   && ( ( yAxis > yNine  ) && ( yAxis < ( yNine + ydigitMargin  ) ) ) )
#define dotTouch       ( ( (xAxis > xDot   )   && (xAxis < ( xDot   + digitMargin ) ) )   && ( ( yAxis > yDot  ) && ( yAxis < ( yDot + ydigitMargin  ) ) ) )
#define delTouch       ( ( (xAxis > xDel   )   && (xAxis < ( xDel   + digitMargin ) ) )   && ( ( yAxis > yDel  ) && ( yAxis < ( yDel + ydigitMargin  ) ) ) )
#define clrTouch       ( ( (xAxis > xClr   )   && (xAxis < ( xClr   + digitMargin ) ) )   && ( ( yAxis > yClr  ) && ( yAxis < ( yClr + ydigitMargin  ) ) ) )
#define backTouch      ( ( (xAxis > xBack  )   && (xAxis < ( xBack  + 135         ) ) )   && ( ( yAxis > yBack ) && ( yAxis < ( yBack + ydigitMargin ) ) ) )
#define entTouch       ( ( (xAxis > xEnt   )   && (xAxis < ( xEnt   + digitMargin ) ) )   && ( ( yAxis > yEnt  ) && ( yAxis < ( yEnt + ydigitMargin  ) ) ) )


/*********** ADDED BY VIPUL **********************/

/******* Class declaration ********/
extern TFT_eSPI tft;
//extern class DisplayInit distft;

/******* external function and variable **********/

/******* class functions and variable **********/

 char  WeighingHandle :: preTime[10] = {0};


/****************************************************************************
                    ---->> Common Funcations <<----
   @ handleFlags :
    - This funcation handles all common flags. when and which flag must be ON
      it take cares.
   ===============================================================================

   @ initWeighingTFT( ) :
    - This funcation setup TFT touch screen parameters and UART and their baudRate.
   ====================================================================================

   @ initTFTHandler ( ) :
    - This funcation is use to call Images which is necessary for basic funcationality.
      and update preVlaue with default "ABCDEFGH".
   =====================================================================================

 *****************************************************************************/
//===========================================================//
//  Global Resources of thread                              //
//===========================================================//

QueueHandle_t xQueue1, xQueue2;
SemaphoreHandle_t xSemaphore_Lock;
TaskHandle_t xHandle;

char  preBuff[10] =  "ABCDEFGH", Serialtemp[10];
uint8_t  setDot, DigitAfterDot = 0;
bool noDotFound = true;
bool userInputAtRunTime = true;
double value = 0, temp = 0, preValue = 0;

// Task to be created.
void vTask_SerialButtonMonitor( void * pvParameters ) {
  class userKeyBoard kbd;
  int8_t input = -20;
  uint32_t Timeout;
  uint8_t idx = 0;
  
  Timeout = millis();
  memset(Serialtemp, '\0', 10);

  for ( ;; ) {
 
    //==========================================
    //    Handle keypada data and calculation
    //===========================================

    if ( xSemaphoreTake( xSemaphore_Lock, ( TickType_t )1 ) == pdTRUE ) {
    
      input = kbd.getMappedKey(  );
      if (  input != -20 ) {

        switch (input) {
          case -1 :
            { //Enter Hit by user
              value = 0;
              DigitAfterDot = 0;
              noDotFound = true;
              userInputAtRunTime = true;
              memset(priceObj.asciiValues[PRICE_perPcs], '\0', 10);
              memset(priceObj.asciiValues[PRICE_perPcs], '0', 7);
              priceObj.numericValues[PRICE_perPcs] = 0.0;
            }
            break;
          case -2 :
            // Dot has been hit by user.
            noDotFound = 0;
            break;
           case 0 :
           case 1 :
           case 2 :
           case 3 :
           case 4 :
           case 5 :
           case 6 :
           case 7 :
           case 8 :
           case 9 :
            {
              // when there is not any dot
              if ( noDotFound == true )
                value = ( value * 10 ) + input;
              else {
                // when user has hitted dot once.
                if ( ( input == -20 ) || ( input == -2 ) ||  ( input == -1 ) ) {
                  // Do nothing
                  // this is for safe gaurd.
                }
                else {
                  DigitAfterDot++;
                  temp = (double)input / pow(10, DigitAfterDot );                 
                  value = ( value ) + ( temp );
                }//end-else
              }//end-else
            }//end-default
            break;
            
          default :
          SPL("Unknown KeyBoard Hit...!!!");
          break;
        }//end-switch
      }//end-valid input

      //===============================================
      // Calculate and update neccasary fields to see
      // unit price.
      //  1 > priceObj.asciiValues[PRICE_perPcs]
      //  2 >  priceObj.numericValues[PRICE_perPcs]
      //===============================================
      if ( ( value != preValue ) && userInputAtRunTime == true ) {
        sprintf( Serialtemp, "%lf",   value );

        if ( xQueue1 != NULL )
        {
          if ( xQueueSendToFront( xQueue1,
                                  ( void * ) &Serialtemp,
                                  ( TickType_t ) 10 ) != pdPASS ) {
            /* Failed to post the message, even after 10 ticks. */
            SPL("Failed to post the message, even after 10 ticks...");
          }
        }
        preValue = priceObj.numericValues[PRICE_perPcs] = value;
        input= -20; // keepRunning
      }

    // Unlock this Thread
      xSemaphoreGive( xSemaphore_Lock );
    }//end-if(sema_lock)

    //==============================================
    //        STOP this Thread for some Time
    //==============================================
    vTaskDelay(1 / portTICK_RATE_MS);
    yield();
  }

}//end

void createThread() {

  xTaskCreate(   vTask_SerialButtonMonitor,
                 "Price Input",
                 2048, // if get any issue please increase stack size to 4096
                 NULL,
                 1,
                 &xHandle );

  if ( ( xSemaphore_Lock =  xSemaphoreCreateBinary() ) == NULL ) {
    SPL("semaphore lock is not obtained ...!!!");
  } else {
//    SP("Sema is obtained ....!!! >> ");
    SPL( xSemaphoreGive( xSemaphore_Lock ) );
  }

  if ( ( xQueue1 = xQueueCreate( 1, sizeof( char )*10 ) ) == NULL ) {
    SPL("xQueue1 Error :");
  }
//
//  if ( ( xQueue2 = xQueueCreate( 1, sizeof( char )*10 ) ) == NULL ) {
//    SPL("xQueue2 Error :");
//  }

}




String  WeighingHandle :: handleFlags( char *Weight ) {

  flags = Weight[8];

  if ( flags != 0 && flags != '0' ) {
    //   SP("flag : "); SPL(flags);
    /* check Zero Bit */
    ( flags & 0x01 ) ? zeroSymbl(1) : zeroSymbl(0);
    /* check Tare Bit */
    (  flags & 0x02 ) ? tareweightSymbl(1) : tareweightSymbl(0);
    /* check stable Bit */
    ( flags & 0x04 ) ? stableSymbl(1) : stableSymbl(0);
    /* Check Negative Falg */
    if ( flags & 0x40 ) Weight[0] = '-';
  }
  return Weight;
}


bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // Return 1 to decode next block
  return 1;
}




void WeighingHandle :: initWeighingTFT( ) {
  Serial.begin(9600); // DEBUG WINDOW
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1); // Keypad interface
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Weight Mechine
  SPI.beginTransaction(SPISettings(80000000, MSBFIRST, SPI_MODE0));
  Serial2.setRxBufferSize(500);


  RpiDisplayInit(typeOne); 
  tft.fillScreen(TFT_BLACK);
/*
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  
  #ifdef ILI9486_DRIVER
//    uint16_t calData[5] = { 262, 3565, 1319, 2532, 2 };
     uint16_t calData[5] = { 292, 3504, 467, 3298, 2 };
  #endif
  
  #ifdef ILI9488_DRIVER
     uint16_t calData[5] = { 299, 3592, 256, 3464, 7 };
  #endif
  
  tft.setTouch(calData);
  */
  

  // Initialise SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    //    while (1) yield(); // Stay here twiddling thumbs waiting
  }
  Serial.println("\r\nInitialisation done.");
  tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)

  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);

  //RTC initilise
  initRTC();
}




void WeighingHandle :: initTFTHandler ( ) {
  weighingMode();
  batterySymbl(LOWER);
  stableSymbl(0);
  zeroSymbl(0);
  tareweightSymbl(0);

  strcpy( showDigits.preValue[0], "ABCDEFGH" );
  strcpy( showDigits.preValue[1], "ABCDEFGH" );
  strcpy( showDigits.preValue[2], "ABCDEFGH" );

}


void WeighingHandle :: _windowOne(char *preValue, char *currentValue){

// Serial.printf("===>>>>%s\n", currentValue);

 static uint8_t skipcnt = 20, drawRectCnt=0;
 int8_t leadingZero;  
  for(leadingZero=0; currentValue[leadingZero] == '0'; ++leadingZero)yield();
  
  if( showDigits.dotPosition != -1 )
    if( leadingZero > showDigits.dotPosition ) leadingZero = showDigits.dotPosition;
  else{
    if( leadingZero > 6 ) leadingZero = 6;
  }
 // Serial.printf("leadingZero = %d, showDigits.dotPosition= %d\n", leadingZero, showDigits.dotPosition );

  //2. draw blank rectangle only those digits which is different from previous Value.
   if ( drawRectCnt++ > 20 ){
      for (uint8_t idx = 0; idx < 7; ++idx ){
        if ( preValue[idx] !=  currentValue[idx] ){
          tft.fillRect( 15  + ( idx * 65),  58,   62,  90,  TFT_BLACK);   
        }//end-if
      }//end-for
      drawRectCnt= 0;
   }//end-if
 
  //3.draw blank digits
  // IN PLACE OF leading Zeros we are drawing blank digits.
  tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
  for (uint8_t idx = 0; idx < leadingZero; ++idx ){
    tft.drawChar( '8', 15 + ( idx * 65), 130,  1);
  }

  
  for (uint8_t idx = leadingZero; idx < 7; ++idx ){
    if ( preValue[idx] !=  currentValue[idx] ){
      tft.drawChar( '8', 15 + ( idx * 65), 130,  1);
    }
  }

  //4. draw Digits only, Those digits which is different from previous Value.
  // front digit
  tft.setTextColor(showDigits.showDigitColorWindowOne, TFT_BLACK);
  for (uint8_t idx = leadingZero ; idx < 7; ++idx ){
    if ( preValue[idx] !=  currentValue[idx] ){
      tft.drawChar( currentValue[idx], 15 + ( idx * 65), 130,  1);
    }
  }

  // end : update preVlaue
  if( !strcmp(preValue, "ABCDEFGH") && !strcmp(currentValue, "0000000") );
  else 
  strcpy( preValue, currentValue );

}//end- _window


void WeighingHandle :: windowOne(   ) {
  
  //1. check with previous Value
  if ( !strcmp( showDigits.preValue[0] , showDigits.currentValue ) )  return;

 // 2. Set Font Size & Style
  showDigits.digitFontSize  = 1;
  showDigits.digitFontStyle = LED7SEG_STD40;
  tft.setTextSize( showDigits.digitFontSize );
  tft.setFreeFont( (const GFXfont *)showDigits.digitFontStyle );

  _windowOne( showDigits.preValue[0] , showDigits.currentValue );

/*
 // Serial.printf("===>>>> %s\n", showDigits.currentValue);
 static uint8_t skipcnt = 20, drawRectCnt=0;
 int8_t leadingZero = 0;   
  for(leadingZero=0;  showDigits.currentValue[leadingZero] == '0'; ++leadingZero);
  if( leadingZero > 4) leadingZero=4;
 

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
  // IN PLACE OF leading Zeros we are drawing blank digits.
  tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
  for (uint8_t idx = 0; idx < leadingZero; ++idx ){
    tft.drawChar( '8', 15 + ( idx * 65), 130,  1);
  }

  
  for (uint8_t idx = leadingZero; idx < 7; ++idx ){
    if ( showDigits.preValue[0][idx] !=  showDigits.currentValue[idx] ){
      tft.drawChar( '8', 15 + ( idx * 65), 130,  1);
    }
  }

  //5. draw Digits only, Those digits which is different from previous Value.
  // front digit
  tft.setTextColor(showDigits.showDigitColorWindowOne, TFT_BLACK);
  for (uint8_t idx = leadingZero ; idx < 7; ++idx ){
    if ( showDigits.preValue[0][idx] !=  showDigits.currentValue[idx] ){
      tft.drawChar( showDigits.currentValue[idx], 15 + ( idx * 65), 130,  1);
    }
  }

  // end : update preVlaue
  if( !strcmp(showDigits.preValue[0], "ABCDEFGH") && !strcmp(showDigits.currentValue, "0000000") )
            ;
  else {
    strcpy( showDigits.preValue[0], showDigits.currentValue );
  } 
*/

}


void  WeighingHandle :: windowOne(int8_t tempDotPos  ){
  if( tempDotPos == -1 ) showDigits.dotPosition = -1;
  
  windowOne();
  
  // remove all previous dots and redraw at given position;
  for (uint8_t idx = 0; idx < 6; ++idx )
    tft.fillCircle( 73 + ( idx * 65 ), 138, 6, TFT_BLACK);
  
  // draw dot at given position
  if( tempDotPos != -1 )// if we donot want to use any dot.
    tft.fillCircle( 73 + ( tempDotPos * 65), 138, 6, TFT_RED );
}


void WeighingHandle :: _windowTwo(char *preValue, char *currentValue){

// Serial.printf("===>>>> %s\n", showDigits.currentValue);
 static uint8_t skipcnt = 20, drawRectCnt=0;
 int8_t leadingZero = 0;   
  for(leadingZero=0; currentValue[leadingZero] == '0'; ++leadingZero);
  if( leadingZero > showDigits.dotPosition ) leadingZero = showDigits.dotPosition;
 

  //3.draw blank digits
  // IN PLACE OF leading Zeros we are drawing blank digits.
  tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
  for (uint8_t idx = 0; idx < leadingZero; ++idx ){
   tft.fillRect( 15  + ( idx * 31),  180,   30,  40,  TFT_BLACK );
   tft.drawChar(  '8', 15 + ( idx * 31) , 210,  1);
  }

  
  for (uint8_t idx = leadingZero; idx < 7; ++idx ){
    if ( preValue[idx] !=  currentValue[idx] ){
      tft.drawChar(  '8', 15 + ( idx * 31) , 210,  1);
    }
  }

  //4. draw Digits only, Those digits which is different from previous Value.
  // front digit
  tft.setTextColor(showDigits.showDigitColorWindowOne, TFT_BLACK);
  for (uint8_t idx = leadingZero ; idx < 7; ++idx ){
    if ( preValue[idx] !=  currentValue[idx] ){
      tft.fillRect( 15  + ( idx * 31),  180,   30,  40,  TFT_BLACK );
      tft.drawChar( currentValue[idx], 15 + ( idx * 31) , 210,  1);
    }
  }

  // end : update preVlaue
  if( !strcmp(preValue, "ABCDEFGH") && !strcmp(currentValue, "0000000") );
  else 
    strcpy( preValue, currentValue );
  
}

void WeighingHandle :: windowTwo( ) {
 
 if ( !strcmp(  showDigits.preValue[1] , showDigits.currentValue ) ) return;

   // 1. Set Font Size
  showDigits.digitFontSize  = 1;
  showDigits.digitFontStyle = LED7SEG_STD18;
  tft.setTextSize( showDigits.digitFontSize );
  tft.setFreeFont( (const GFXfont *)showDigits.digitFontStyle );

  _windowTwo( showDigits.preValue[1] , showDigits.currentValue ); 
     
/*  
  int8_t leadingZero = 0;

  //3. check with previous Value
  if ( !strcmp(  showDigits.preValue[1] , showDigits.currentValue ) ) return;
  if ( strlen(showDigits.preValue[1]) > 6 ) strcpy(showDigits.preValue[1], "ABCDEFG");

  yield();
  
//    Bug Fix : showDigits.dotPosition was corrupting again again.
  
  showDigits.dotPosition = _getDecimal().c_str()[0] - '0';
  if (  !( showDigits.dotPosition < 5 &&  showDigits.dotPosition > 0 ) ) {
    SPL("Error : showDigits.dotPosition : " + String( showDigits.dotPosition ));
    return;
  }


  // 1. Set Font Size
  showDigits.digitFontSize  = 1;
  showDigits.digitFontStyle = LED7SEG_STD18;
  tft.setTextSize( showDigits.digitFontSize );
  tft.setFreeFont( (const GFXfont *)showDigits.digitFontStyle );

  //2. Add Leading Zero into current Value

  if (  showDigits.spclFlag )
  {
    showDigits.currentValue[6] = '0';
  }
  else
  {
    memmove( &showDigits.currentValue[1], &showDigits.currentValue[0], strlen(showDigits.currentValue) );
    showDigits.currentValue[0] = '0';
  }
  showDigits.currentValue[7] = '\0';

  eliminateLeadingZeros( showDigits.currentValue, ( 7 - showDigits.dotPosition ), leadingZero )

  //4. draw blank rectangle only those digits which is different from previous Value.
  tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
  for (uint8_t idx = 0; idx < 7; ++idx )
  {
    if ( showDigits.preValue[1][idx] !=  showDigits.currentValue[idx] )
    {
      tft.fillRect( 15  + ( idx * 31),  180,   30,  40,  TFT_BLACK );
      tft.drawChar(  '8', 15 + ( idx * 31) , 210,  1);
    }
    yield();
  }
  //5. remove all previous dots and redraw at given position;
  for (uint8_t idx = 0; idx < 6; ++idx )
  {
    tft.fillCircle( 41 + ( idx * 31), 216, 3, TFT_BLACK);
  }
  int8_t dotPosition = 6 - showDigits.dotPosition;
  tft.fillCircle( 41 + ( dotPosition * 31), 216, 3, TFT_RED );

  //6. draw Digits only,, Those digits which is different from previous Value.
  tft.setTextColor( TFT_GREEN, TFT_BLACK );
  for (uint8_t idx = leadingZero; idx < 7; ++idx ){
    if ( showDigits.preValue[1][idx] !=  showDigits.currentValue[idx] ){
      tft.drawChar(  showDigits.currentValue[idx], 15 + ( idx * 31) , 210,  1);
    }
    yield();
  }

  // end : update preVlaue
 // memset(showDigits.preValue[1], '\0', 7);
  strcpy( showDigits.preValue[1], showDigits.currentValue );
  showDigits.preValue[1][6] = '\0';
*/  

}

void WeighingHandle :: windowTwo(int8_t tempDotPos ) {
   windowTwo();
  // remove all previous dots and redraw at given position;
  for (uint8_t idx = 0; idx < 6; ++idx )
    tft.fillCircle( 41 + ( idx * 31), 216, 3, TFT_BLACK);
  
  // draw dot at given position
  if( tempDotPos != -1 )// if we donot want to use any dot.
     tft.fillCircle( 41 + ( tempDotPos * 31), 216, 3, TFT_RED );
}



/*
   Note : Three variables are important here which need to be care to show data on screen.
          1 >>  showDigits.currentValue -->> this array data must be without dot.
          2 >>  showDigits.preValue[2]  -->> it must be different from currentValues variable
          3 >>   showDigits.dotPosition -->> it will show dot position on screen.
*/


void WeighingHandle :: _windowThree(char *preValue, char *currentValue){

// Serial.printf("===>>>> %s\n", showDigits.currentValue);
 static uint8_t skipcnt = 20, drawRectCnt=0;
 int8_t leadingZero = 0;   
  for(leadingZero=0; currentValue[leadingZero] == '0'; ++leadingZero);
  if( leadingZero > showDigits.dotPosition ) leadingZero = showDigits.dotPosition;
 

  //3.draw blank digits
  // IN PLACE OF leading Zeros we are drawing blank digits.
  tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
  for (uint8_t idx = 0; idx < leadingZero; ++idx ){
      tft.fillRect( 249  + ( idx * 31),  180,   30,  40,  TFT_BLACK );
      tft.drawChar( '8', 249 + ( idx * 31) , 210,  1 );
  }

  
  for (uint8_t idx = leadingZero; idx < 7; ++idx ){
    if ( preValue[idx] !=  currentValue[idx] ){
       tft.drawChar( '8', 249 + ( idx * 31) , 210,  1 );
    }
  }

  //4. draw Digits only, Those digits which is different from previous Value.
  // front digit
  tft.setTextColor(showDigits.showDigitColorWindowOne, TFT_BLACK);
  for (uint8_t idx = leadingZero ; idx < 7; ++idx ){
    if ( preValue[idx] !=  currentValue[idx] ){
      tft.fillRect( 249  + ( idx * 31),  180,   30,  40,  TFT_BLACK );
       tft.drawChar( currentValue[idx], 249 + ( idx * 31) , 210,  1 );
    }
  }

  // end : update preVlaue
  if( !strcmp(preValue, "ABCDEFGH") && !strcmp(currentValue, "0000000") );
  else 
    strcpy( preValue, currentValue );
  
}


void WeighingHandle :: windowThree( ) {

   if ( !strcmp( showDigits.preValue[2] , showDigits.currentValue ) ) return;
    
  // 1. Set Font Size and style
  showDigits.digitFontSize  = 1;
  showDigits.digitFontStyle = LED7SEG_STD18;
  tft.setTextSize( showDigits.digitFontSize );
  tft.setFreeFont( (const GFXfont *)showDigits.digitFontStyle );

  _windowThree( showDigits.preValue[2] , showDigits.currentValue ); 

/*
  static int8_t cnt_3 = 3;
  int8_t leadingZero = 0;
  //  check with previous Value
  
     //  Bug Fix : showDigits.dotPosition was corrupting again again.
 
  if ( !strcmp( showDigits.preValue[2] , showDigits.currentValue ) ) return;

  showDigits.dotPosition = _getDecimal().c_str()[0] - '0';
  if (  !( showDigits.dotPosition < 5 &&  showDigits.dotPosition > 0 ) ) {
    SPL("Error : showDigits.dotPosition : " + String( showDigits.dotPosition ));
    return;
  }


  // 1. Set Font Size and style
  showDigits.digitFontSize  = 1;
  showDigits.digitFontStyle = LED7SEG_STD18;
  tft.setTextSize( showDigits.digitFontSize );
  tft.setFreeFont( (const GFXfont *)showDigits.digitFontStyle );

  // Add Leading Zero into current Value
  memmove( &showDigits.currentValue[1], &showDigits.currentValue[0], strlen(showDigits.currentValue) );
  showDigits.currentValue[0] = '0';
  showDigits.currentValue[7] = '\0';

  eliminateLeadingZeros( showDigits.currentValue, ( 7 - showDigits.dotPosition ), leadingZero )

  //4. draw blank rectangle only those digits which is different from previous Value.
  tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
  for (uint8_t idx = 0; idx < 7; ++idx )
  {
    if ( showDigits.preValue[2][idx] !=  showDigits.currentValue[idx] )
    {
      tft.fillRect( 249  + ( idx * 31),  180,   30,  40,  TFT_BLACK );
      tft.drawChar( '8', 249 + ( idx * 31) , 210,  1 );
    }

  }
  //5. remove all previous dots and redraw at given position;
  for (uint8_t idx = 0; idx < 6; ++idx )
  {
    tft.fillCircle( 275 + ( idx * 31), 216, 3, TFT_BLACK);
  }
  int8_t dotPosition = 6 - showDigits.dotPosition;
  tft.fillCircle( 275 + ( dotPosition * 31), 216, 3, TFT_RED );

  //6. draw Digits only,, Those digits which is different from previous Value.
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  for (uint8_t idx = leadingZero; idx < 7; ++idx )
  {
    if ( showDigits.preValue[2][idx] !=  showDigits.currentValue[idx] )
    {
      tft.drawChar(  showDigits.currentValue[idx], 249 + ( idx * 31) , 210,  1 );
    }
  }

  // end : update preVlaue
  strcpy( showDigits.preValue[2], showDigits.currentValue );
*/


}


void WeighingHandle :: windowThree(int8_t tempDotPos ) {
   windowThree();
  // remove all previous dots and redraw at given position;
  for (uint8_t idx = 0; idx < 6; ++idx )
   tft.fillCircle( 275 + ( idx * 31), 216, 3, TFT_BLACK);
  
  // draw dot at given position
  if( tempDotPos != -1 )// if we donot want to use any dot.
      tft.fillCircle( 275 + ( tempDotPos * 31), 216, 3, TFT_RED );
}



/********************************************************************************************************************************

   @ batterySymbl()  -- >> This Function is responsible for drawing battery Symbol
   @ stableSymbl()   -->>  This Function is reposible for drawing weight stable symbol
   @ tareweightSymbl -->>  This Function is reponsible for drawing tare weight symbol
   @ zeroSymbl       -->>  This Function is responsible for drawing Zero symbol.
   @ BootingImage()  -->>  This is booting image. It is doing Nothing but it looks good. It can be used later but not for now.
   @ startUPImage()  -->>  StartUP Image shows 4 Menu option to choose and accordingly program works.
   @ weighingMode()  -->>  This is base image which is used in all 4 Modes with title modification.
   @ numKeypad()     -->>  This is keypad image which works in all 4 Modes to get user data from screen when it touched by user.

 ********************************************************************************************************************************/

bool WeighingHandle :: batterySymbl(e_batterySymbol Mode){

  switch(Mode){
   case LOWER :
      tft.setSwapBytes(true);
      tft.pushImage(250, 2, bettry_symbWidth, bettry_symbHeight, bettry_symb );
      break;

  case UPPER:
      tft.setSwapBytes(true);
      tft.pushImage(230, 2, bettry_symbWidth, bettry_symbHeight, bettry_symb );
      break;
  }//end-switch
}//end-batterySymbol





bool  WeighingHandle :: stableSymbl(uint8_t flag){
  tft.setSwapBytes(true);
  if ( flag )
    tft.pushImage(310, 4, stable_symbWidth, stable_symbHeight, Stable_symbl );
  else
    tft.pushImage(310, 4, stable_symbWidth, stable_symbHeight, hideStable_symbl );

}


bool  WeighingHandle :: tareweightSymbl(uint8_t flag){
  tft.setSwapBytes(true);
  if ( flag )
    tft.pushImage(350, 4, taresymblWidth, taresymblHeight, Tare_symb );
  else
    tft.pushImage(350, 4, taresymblWidth, taresymblHeight, hideTare_symb );
}


bool WeighingHandle :: zeroSymbl(uint8_t flag){
  tft.setSwapBytes(true);

  if (flag)
    tft.pushImage(420, 4, zeroSymblWidth, zeroSymblHeight, ZeroSymbl );
  else
    tft.pushImage(420, 4, zeroSymblWidth, zeroSymblHeight, hideSymbolZero );
}

bool  WeighingHandle :: BootingImage() {
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  tft.pushImage(40, 50, bootWidth, bootHeight, Boot);

  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
}


/******************************************************************
   @ weighingMode()  -->>  This is base image which is used in
                           all 4 Modes with title modification.                                                                *
 *******************************************************************/
bool  WeighingHandle :: weighingMode(){
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  tft.pushImage(5, 22, WeighingModeWidth, WeighingModeHeight, WeighingMode );
}

bool WeighingHandle :: numKeypad(){
  //  tft.setSwapBytes(true);
  //  tft.pushImage(0, 3, NumkeypadWidth, NumkeypadHeight, Numkeypad );
}

bool WeighingHandle :: cross(uint16_t x, uint16_t y){
  //    tft.setSwapBytes(true);
  //    tft.pushImage(x, y, crossImageWidth, crossImageHeight, crossImage );
}

bool  WeighingHandle :: startUPImage() {
  tft.setSwapBytes(true);
  tft.pushImage(1, 1, startUPWidth, startUPHeight, StartUP );
}




/******************************************************************
 *                                                                *
 *                                                                *
 *******************************************************************/

int8_t  WeighingHandle :: startPriceComputing(){
  priceObj._start();
  while (1) {
 if( checkTouch(Mode) ){
  if( xHandle != NULL ) 
  vTaskDelete( xHandle );
  SPL("Task has been Deleted ...");
  xHandle = NULL;

   
  return -1;
 } 
    priceObj.priceHandler();
    yield();
  }
}

int8_t WeighingHandle :: startCheckWeighing(){

  checkObj._start();
  while (1) {
    touchHandler(Mode)
    checkObj.checkHandler();
    yield();
  }
}

int8_t  WeighingHandle :: startCountComputing() {
  countM._start();
  while (1) {
    touchHandler(Mode)
    countM.countHandler();
    yield();
  }
}//end-startCountingComputing

int8_t  WeighingHandle :: startWeighing(){
  weightObj._start();
  while (1) {
    touchHandler(Mode)
    weightObj.weightHandler();  
   yield();
  }
}


void   WeighingHandle :: batteryAnimation(int blevel){
  
  uint16_t linecolor[4] = {TFT_RED, TFT_YELLOW, TFT_GREEN, TFT_BLACK};
  int8_t color = 0;
  int xline = 233, yline = 5;

 if( blevel == -1 ){ 
  int xline2 = xline + 41;
    for ( int idx=xline; idx < xline2; ++idx ) {
        tft.drawFastVLine( idx , yline, 16, linecolor[3]);
        delay(10);
        yield();
      }//end-for 
      return;
 }//end-if(blevel)

   int colorCnt =  ( blevel > 50 ) ? 3 : ( blevel < 50 && blevel > 25 ) ? 2 : ( blevel < 25 ) ? 1 : -1;

  if( colorCnt == -1 ) return;
  
  START :
  while( color < colorCnt){
    for ( int idx = 0; idx < 15 ; ++idx ) {
        tft.drawFastVLine( ( xline + (14 * color) + idx ) , yline, 16, linecolor[color]);
        delay(10);
        yield();
      }//end-for
      color++;
      yield();
  }//end-while
   
}//batteryAnimation()




void  WeighingHandle :: DateNTimeHandler() {
  char timeArray[10];
 

  // write only when pre and current values of Time are different
  
  parshTimeFormat(timeArray, getRTCTime().c_str() ) 
  if ( strcmp( preTime, timeArray ) != 0 ) {
    tft.setFreeFont( (const GFXfont *)EUROSTILE_B7 );
    tft.setTextColor(TFT_BLACK, TFT_BLACK);
    tft.fillRect(400, 295, 50, 20, TFT_WHITE);
    tft.setCursor(400, 310);
  //  SP("timeArray : ");SPL(timeArray);
    tft.print("  " + String(timeArray) + String("pm") );
    strcpy( preTime, timeArray );
    memset(timeArray, '\0', 10);
  }
}


bool WeighingHandle::checkTouch(uint8_t touchMode ) {
  /* Return 0 : Keep Running
     Return 1 : Exit from this application.
  */
  noInterrupts();
  if ( findTouch() == -1 ) {
    return 1;
  }

  switch ( touchMode ) {

    case 2 :
      if ( weightObj.activityBasedOnTouch() == -1 ) {
        return 1;
      }
      break;

    case 3 :
      if ( countM.activityBasedOnTouch() == -1 ) {
        return 1;
      }
      break;

    case 5 :
      if ( checkObj.activityBasedOnTouch() == -1 ) {
        return 1;
      }
      break;

    case 4 :
      if ( priceObj.activityBasedOnTouch() == -1 ) {
        return 1;
      }
      break;


  }//end-switch
  if (  ISR_dtech  ) {
    ISR_dtech = 0;
    detachInterrupt(GPIOPin_TIRQ);
    ATTACH_TOUCH_INTERRUPT
  }
  interrupts();
  return 0;
}



void WeighingHandle:: popupButton(e_popupButton x, e_popupButton y,  uint16_t lineColor, uint8_t thickness){
  
  // Draw Rectange to show button has pressed OR not Pressed.
 for(uint8_t i=0; i < thickness; ++i )
  // tft.drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color)
  tft.drawRoundRect(x+i, y+i, 93-i*2, 67-i*2, 9, lineColor);


  
}



void WeighingHandle:: popupButton(int8_t Mode, uint16_t lineColor, uint8_t thickness){

// erash all previous Touch prints
 for(int i=0; i < thickness; ++i){
      tft.drawRect(22+i, 72+i, 106, 147, TFT_BLACK);  // weighing Scales
      tft.drawRect(131+i, 72+i, 106, 147, TFT_BLACK); // count Scales
      tft.drawRect(239+i, 72+i, 106, 147, TFT_BLACK); // price Scales 
      tft.drawRect(350+i, 72+i, 106, 147, TFT_BLACK); // check Scales 
      tft.drawRect(23+i, 225+i, 213, 60, TFT_BLACK); // check Scales    
  }//end-if


//enable new touch
  switch(Mode){
    case WeightMode:
     for(int i=0; i < thickness; ++i)
      tft.drawRect(22+i, 72+i, 106, 147, lineColor);  // weighing Scales
    break;

    case CountMode:
     for(int i=0; i < thickness; ++i)
      tft.drawRect(131+i, 72+i, 106, 147, lineColor); // count Scales
    break;

    case PriceMode:
       for(int i=0; i < thickness; ++i)
         tft.drawRect(239+i, 72+i, 106, 147, lineColor); // price Scales 
    break;

    case CheckMode:
       for(int i=0; i < thickness; ++i)
         tft.drawRect(350+i, 72+i, 106, 147, lineColor); // check Scales 
    break;

    case SettingMode:
       for(int i=0; i < thickness; ++i)
         tft.drawRect(23+i, 225+i, 213, 60, lineColor); // check Scales 
    break;    
  }

Mode = -1;
}//end-popButton
