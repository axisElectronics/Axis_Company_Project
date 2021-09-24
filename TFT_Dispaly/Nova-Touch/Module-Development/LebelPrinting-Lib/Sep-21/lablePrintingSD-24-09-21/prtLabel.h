
#ifndef __printer_label
#define __printer_label


#include <stdint.h>
#include <string>
#include <Arduino.h>
#include <iterator>
#include <vector>
#include <algorithm>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

using namespace std;




#define SP Serial.print
#define SPL Serial.println

typedef enum {
  SERIAL_NUM_DEFAULT,
  SERIAL_NUM_ONE,
  SERIAL_NUM_TWO
} enum_serial_num;

typedef enum {
  BAUD_RATE_9600 = 9600,
  BAUD_RATE_115200 = 115200
} enum_baud_rate;

typedef enum {
  prt_RXD1 = 9,
  prt_TXD1 = 10,
  prt_RXD2 = 16,
  prt_TXD2 = 17,
} enum_serial_pins;

typedef struct {
  uint8_t serial_num; // 0: Default, 1: Serial1 2: Serial2 enable
} global_config_t;

typedef struct {
  int key;
  void *value;
  char *ptr_next;
} keyValue;


typedef enum {
  //GENERIC
  DATE = 0,
  TIME = 1,
  // WEIGHT MACHINE
  Lvar_GROSS = 2,
  Lvar_TARE = 3,
  Lvar_NET = 4,
  // COUNT MACHINE
  T_COUNT = 5,
  U_WEIGHT = 6,
  HRES_WEIGHT = 7,
  LRES_WEIGHT = 8,
  // PRICE WEIGHT
  T_PRICE = 9,
  U_PRICE = 10,
  //CHECK MACHINE
  L_WEIGHT = 11,
  H_WEIGHT = 12,
  // MISCELLANEOUS
  USER = 13,
  PRODUCT_NAME = 14,
  Batch_No = 15,
  Operator_Name = 16,
  LineNo = 17,
  SHIFT = 18,

  //TOTAL NUMBER OF VARIABLES
  PRT_MAX_VAR = 19, // IT MUST BE UPDATE IF VARIABLES INCREASE OR DECREASE
  PRT_VAR_SPACE = 20 // IT IS ARRAY SIZE
} enum_LableVar;




class prtLabel {
  private :
   
    global_config_t prtconfig;

    String temp = "^XA\n\
                ^SZ2^JMA\n\
                ^MCY^PMN\n\
                ^PW779\n\
                ~JSN\n\
                ^JZY\n\
                ^LH0,0^LRN\n\
                ^XZ\n\
                ^XA\n\
                ^FT42,441\n\
                ^CI0\n\
                ^A0N,51,98^FDBOX^FS\n\
                ^FT258,163\n\
                ^A0N,20,27^FDBatch No.: $$$$$$$$$0,$$$$$$$$$1^FS\n\
                ^FT512,161\n\
                ^A0N,14,19^FDOperator : $$$$$$$$$2^FS\n\
                ^FT258,197\n\
                ^A0N,14,19^FDDate : 26/11/20^FS\n\
                ^FT307,265\n\
                ^A0N,31,42^FDCHEMICAL^FS\n\
                ^FT557,268\n\
                ^A0N,31,42^FDWEIGHT^FS\n\
                ^FT327,310\n\
                ^A0N,20,27^FDDCR - $$$$$$$$$3^FS\n\
                ^FT589,310\n\
                ^A0N,20,27^FD39.94^FS\n\
                ^FT332,351\n\
                ^A0N,20,27^FDPM - 40^FS\n\
                ^FT595,351\n\
                ^A0N,20,27^FD8.36^FS\n\
                ^FT31,507\n\
                ^A0N,42,57^FD001/100^FS\n\
                ^FO261,228\n\
                ^GB458,337,3^FS\n\
                ^FO261,283\n\
                ^GB454,0,3^FS\n\
                ^FO529,228\n\
                ^GB0,339,3^FS\n\
                ^FO72,32\n\
                ^BQN,2,6^FDLA,100012238100^FS\n\
                ^FT45,237\n\
                ^A0N,37,31^FD100012238100^FS\n\
                ^FT587,197\n\
                ^A0N,14,19^FDTime : 12:20:47^FS\n\
                ^FT327,396\n\
                ^A0N,20,27^FDDCR - 45^FS\n\
                ^FT332,440\n\
                ^A0N,20,27^FDPM - 40^FS\n\
                ^FT589,396\n\
                ^A0N,20,27^FD39.94^FS\n\
                ^FT595,440\n\
                ^A0N,20,27^FD8.36^FS\n\
                ^FT327,486\n\
                ^A0N,20,27^FDDCR - 45^FS\n\
                ^FT332,530\n\
                ^A0N,20,27^FDPM - 40^FS\n\
                ^FT589,486\n\
                ^A0N,20,27^FD39.94^FS\n\
                ^FT595,530\n\
                ^A0N,20,27^FD8.36^FS\n\
                ^FT247,55\n\
                ^A0N,23,44^FDPOLYMER FOR 238100CFMB^FS\n\
                ^FT244,106\n\
                ^A0N,23,44^FDPOLYMER FOR 238100CFMB^FS\n\
                ^FT611,161\n\
                ^A0N,14,19^FDSumit Kumar ^FS\n\
                ^PQ1,0,1,Y\n\
                ^XZ\n";


    /* Total varibales need to be replaced into Labels. */
    int8_t intVar;
    char labelVaribales[PRT_MAX_VAR][PRT_VAR_SPACE];
    String tempstr;
    bool printFlag = 1;
    String line; 
   
    /* @ whichVarNeedReplacement () :
          > this funcation will return varibale number, presented in your label.
          >
    */
    String whichVarNeedReplacement(  ) {
      String idxstr = "$$$$$$$$$";

      int subStringIdx = tempstr.indexOf(idxstr);
      if ( subStringIdx == -1 ) { // it means there is no variable which needs to be replaced.
        return "PRINT";
      }

      // There is varibale which need to be replaced in Label.
      char temp = tempstr.charAt( (subStringIdx + 9 ) );
      // it can be replaced with switch cases.
      if ( isDigit( temp ) ) {
        idxstr += temp;
        intVar = temp - '0';
        temp = tempstr.charAt( (subStringIdx + 10 ) );
        if ( isDigit( temp ) ) {
          idxstr += temp;
          intVar = ( intVar * 10 )+ ( temp - '0' );
        }
      }
     
      return idxstr;
    }



    String whichVarNeedReplacementSD(  ) {
      String idxstr = "$$$$$$$$$";

      int subStringIdx = line.indexOf(idxstr);
      if ( subStringIdx == -1 ) { // it means there is no variable which needs to be replaced.
        return "PRINT";
      }

      // There is varibale which need to be replaced in Label.
      char temp = line.charAt( (subStringIdx + 9 ) );
      // it can be replaced with switch cases.
      if ( isDigit( temp ) ) {
        idxstr += temp;
        intVar = temp - '0';
        temp = line.charAt( (subStringIdx + 10 ) );
        if ( isDigit( temp ) ) {
          idxstr += temp;
          intVar = ( intVar * 10 )+ ( temp - '0' );
        }
      }   
      return idxstr;
    }

     void startSDCard();

    // =================================================
  public :
    // =================================================
     String billStr[50];
     String header[5];
     String footer[5];

    prtLabel(){
      Serial.begin(BAUD_RATE_9600);
      prtconfig.serial_num = SERIAL_NUM_DEFAULT;  
    }
    
    prtLabel(int SERIAL_NUM, int BAUD_RATE, bool sdFlag = 1) {

      switch (SERIAL_NUM) {
        case SERIAL_NUM_DEFAULT :
          Serial.begin(BAUD_RATE);
          prtconfig.serial_num = SERIAL_NUM_DEFAULT;
          break;
        case SERIAL_NUM_ONE :
          Serial1.begin(BAUD_RATE, SERIAL_8N1, prt_RXD1, prt_TXD1);
          prtconfig.serial_num = SERIAL_NUM_ONE;
          break;
        case SERIAL_NUM_TWO :
          Serial2.begin(BAUD_RATE, SERIAL_8N1, prt_RXD2, prt_TXD2);
          prtconfig.serial_num = SERIAL_NUM_ONE;
          break;
        default :
          Serial.println("Please set any serial of esp32...!!!");
      }
      
      if( sdFlag )
        startSDCard();
        
    }


    void readLabelFile(const char *path, char *userBuff);
   
    void  updateLableVar(enum_LableVar varNum, char *varData);
    char * showMeLableVar(int varNum);
    void  printLebel( );
    void  printLebelSD( char *labelFilePath );
    
};



#endif
