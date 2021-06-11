

#ifndef SEPL328_KEYPAD_H
#define SEPL328_KEYPAD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Serial_1 will be used for getting data from Arduino NANO for Keypad data.
#define RXD1 32
#define TXD1 33


#define map_One 1
#define map_Two 2
#define map_Three 3
#define map_Four 4
#define map_Five 5
#define map_Six 6
#define map_Seven 7
#define map_Eight 8
#define map_Nine 9
#define map_Zero 0
#define map_Dot
#define map_CmdClear
#define map_CmdZero
#define map_CmdTare
#define map_CmdPrint
#define map_CmdMode
#define map_Cmd_mPluse
#define map_Total
#define map_QTY
#define map_PLU


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
        Serial1.readBytes(key, 2);
        Serial1.read();
        ///need to flash out this it is giveing garbage data
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
        }//end-switch
      }//end-if
      return 0;
      }//end-getmappedKey
    };//end-class

#endif
