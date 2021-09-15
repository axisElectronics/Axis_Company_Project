
#ifndef __printer_label
#define __printer_label


#include <stdint.h>
#include <string>

#include <Arduino.h>

#include <iterator>
#include <vector>
#include <algorithm>


using namespace std;


#define SP Serial.print
#define SPL Serial.println

typedef enum {
  SERIAL_NUM_DEFAULT,
  SERIAL_NUM_ONE,
  SERIAL_NUM_TWO
}enum_serial_num;

typedef enum {
 BAUD_RATE_9600 = 9600,
 BAUD_RATE_115200 = 115200
}enum_baud_rate;

typedef enum {
  RXD1 = 9,
  TXD1 = 10,
  RXD2 = 16,
  TXD2 = 17,
}enum_serial_pins;

typedef struct {
  uint8_t serial_num; // 0: Default, 1: Serial1 2: Serial2 enable
}global_config_t;

typedef struct {
   int key;
   void *value;
   char *ptr_next;
}keyValue;

class prtLabel {
private :
  
global_config_t prtconfig;

public :

  prtLabel(int SERIAL_NUM, int BAUD_RATE){
    switch(SERIAL_NUM){
      case SERIAL_NUM_DEFAULT : 
        Serial.begin(BAUD_RATE);
        prtconfig.serial_num = SERIAL_NUM_DEFAULT;
        break;
      case SERIAL_NUM_ONE :
        Serial1.begin(BAUD_RATE, SERIAL_8N1, RXD1, TXD1); 
        prtconfig.serial_num = SERIAL_NUM_ONE;     
        break;
      case SERIAL_NUM_TWO :
        Serial2.begin(BAUD_RATE, SERIAL_8N1, RXD2, TXD2);
        prtconfig.serial_num = SERIAL_NUM_ONE;
        break;
      default :
        Serial.println("Please set any serial of esp32...!!!");
    }
  }


  void printLebel( ){
    char arr[20] = "Hello I am vivek";
    keyValue Map;

    Map.key = 1;
    Map.value = (char *)arr;
    
  }
};



#endif
