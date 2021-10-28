
#ifndef _ERROR_H
#define  _ERROR_H


#include "headerFiles.h"
#include "commonGlobal.h"

extern class TFT_eSPI tft;

typedef enum errType{
  OVERLOAD = 0,
  UNDER_WEIGHT
};

class classError : public WeighingHandle {

private:
// private variable
double _machineMaxCalibration=0;

// private module
void _erashDigits(char *arr){
  //1. make setup
  tft.setTextSize( 1 );
  tft.setFreeFont( LED7SEG_STD40 );
  
  //2. erash all previous data
  for (uint8_t idx = 0; idx < 7; ++idx ){    
    tft.fillRect( 15  + ( idx * 65),  58,   62,  90,  TFT_BLACK);
 }

  //3. remove all previous dots and redraw at given position;
  for (uint8_t idx = 0; idx < 6; ++idx ){
    tft.fillCircle( 73 + ( idx * 65 ), 138, 6, TFT_BLACK);
}
  //4. draw character
  // char arr[10] = "--O L--";
  for (uint8_t idx = 0; idx < 7; ++idx )
    tft.drawChar( arr[idx], 15 + ( idx * 65), 130,  1);

  delay(1000);

  //5. erash all previous data
  for (uint8_t idx = 0; idx < 7; ++idx ){    
    tft.fillRect( 15  + ( idx * 65),  58,   62,  90,  TFT_BLACK);
 }

  //6. Draw background digit
  tft.setTextColor( makeCustomColor(10, 10, 10), TFT_BLACK );
  for (uint8_t idx = 0; idx < 7; ++idx ){
      tft.drawChar( '8', 15 + ( idx * 65), 130,  1);
  }//end-for
  
}//end-erashDigits
void _showError(errType err){

  switch(err){
   case OVERLOAD :
    _erashDigits("--O L--");
//     _erashDigits("ABCDEFG");
    break;

   case UNDER_WEIGHT :
    _erashDigits("--U L--");
    break;

    default : return;
  }
    memset( showDigits.preValue[0], '\0', sizeof(showDigits.preValue[0]));
    strcpy( showDigits.preValue[0], "ABCDEFGH");
    strcpy( showDigits.currentValue, "000000");
    windowOne( );
    strcpy( showDigits.preValue[0], "ABCDEFGH");
}
double _checkMachineCapacity(){
  //1. get saved machine capacity
 //  Serial.println("errInit ==>> " + _getCapacity() );
  //2. store into error class private varibale
  _machineMaxCalibration = strtod( _getCapacity().c_str(), NULL );
  _machineMaxCalibration /= 1000;
}//end-machineCapacity

public :

void errInit(){
  _checkMachineCapacity();
}//end-errInit
int8_t checkError(double grossWeight){
  if( grossWeight > _machineMaxCalibration ){
        _showError(OVERLOAD);
        //we have to remove later this command.
        CMD_ZERODATA 
        return 1;
  }//end-if
  


return 0;
}//end-checkError


};//end-class classError


#endif
