

#ifndef SEPL328_KEYPAD_H
#define SEPL328_KEYPAD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Serial_1 will be used for getting data from Arduino NANO for Keypad data.
#define RXD1 32
#define TXD1 33

#define map_Zero  0
#define map_One   1
#define map_Two   2
#define map_Three 3
#define map_Four  4
#define map_Five  5
#define map_Six   6
#define map_Seven 7
#define map_Eight 8
#define map_Nine  9


#define map_CmdEnter  -1
#define map_CmdDot    -2
#define map_CmdClear  -3
#define map_CmdDelete -4
#define map_CmdBack   -5
#define map_CmdMode_Switch -6

#define map_UP    1
#define map_Down  2
#define map_Enter 3
#define map_ESC   4

/*
  #define map_CmdZero
  #define map_CmdTare
  #define map_CmdPrint
  #define map_CmdMode


  #define map_Cmd_mPluse
  #define map_Total
  #define map_QTY
  #define map_PLU

*/

class SEPL328_customKeypad
{
  private:
    char key[2];
    int8_t keyNo;
    char mappedKey;

  public :
    void intiSEPL328_Keypad() {
      Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1); // keypad
    }
    void stopSEPL328_Keypad() {
      Serial1.end();
    }


    int8_t getKeyPressed() {
      if (Serial1.available() > 0 ) {
        int8_t buffBytes = Serial1.readBytes(key, 2);
        Serial1.read();
        Serial1.flush();
        //        if ( buffBytes != 0 )
        //        {
        //          Serial.print("rawkey : "); Serial.println(key);
        //        }
        sscanf(key, "%d", &keyNo);
        return keyNo;
      }
      return 0;
    }



    int8_t getMappedKey(  ) {

      if ( getKeyPressed() != 0 )
      {
        switch ( keyNo )
        {
          case 1 : return map_Seven;
          case 2 : return map_Eight;
          case 3 : return map_Nine;
          case 6 : return map_Four;
          case 7 : return map_Five;
          case 8 : return map_Six;
          case 11 : return map_One;
          case 12 : return map_Two;
          case 13 : return map_Three;
          case 16 : return map_CmdDot;
          case 17 : return map_Zero;
          case 18 : return map_CmdEnter;
          case 19 : return map_CmdDelete;
          case 20 : return map_CmdBack;
          case 15 : return map_CmdClear;
          case 14 : return map_CmdMode_Switch;
          default :
            Serial1.print("keyNo : "); Serial.println(keyNo);
        }//end-switch
      }//end-if
      return -20; // keep running
    }//end-getmappedKey


    int8_t navigationKeyMapping() {
      
      if ( getKeyPressed() != 0 )
      {
        switch ( keyNo )
        {
          case 5  : return map_UP;
          case 10 : return map_Down;
          case 18 : return map_Enter;
          case 20 : return map_ESC;
        }//end-switch
      }//end-if
     
      return 5;
    }//emd-naviagtionKeyMapping

};//end-class


#endif
