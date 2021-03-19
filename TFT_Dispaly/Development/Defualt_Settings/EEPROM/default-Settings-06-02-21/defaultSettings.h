#ifndef DefaultSetttings_H
#define DefaultSetttings_H

#include <EEPROM.h>


void setDefaultSettings();
void initEEPROM();




// define the number of bytes you want to access
#define EEPROM_SIZE    50

/*********************************************************************************
        +-+-+-+-+-+-+-+-+
        |7|6|5|4|3|2|1|0|
        +-+-+-+-+-+-+-+-+
  --------------------------------------------------
    @Application & Weighing Units :

        # Lower Nibbles[0-3] are for Application
        # Higher Nibbles[4-7] are for Weighing Units
 *********************************************************************************/
#define WeighingApplicationAddress  0
#define defaultApplication          ( 0x00 & 0x0F )
#define defaultWeighingUnit         ( 0x00 & 0xF0 ) //kg

/*********************************************************************************
        +-+-+-+-+-+-+-+-+
        |7|6|5|4|3|2|1|0|
        +-+-+-+-+-+-+-+-+
         | | | |
         | | | |---->>> data bit
         | | |------>>> Stop bit
         | |
         | |-------->>> Parity
  --------------------------------------------------
    @BaudRate :

        # Lower Nibbles[0-3] are for Baudrate.
        # Bit[4]--->>> data bit
        # Bit[5]--->>> Stop bit.
        # Bits[6-7]--->>> Parity.

 *********************************************************************************/
#define COMPortSettingAddress   1

#define defaultbaudRate ( 0x03 & 0x0F )
#define defaultDataBit  ( 0x00 & 0x10 )
#define defaultStopBit  ( 0x00 & 0x20 )
#define defaultParity   ( 0x00 & 0xC0 )

String _getBuadrate();
int8_t _setBuadrate(char *baudrate);
int8_t  _setDataBits(int8_t DataBits );
int8_t _getDataBits();
int8_t _setStopBits(int8_t StopBits );
int8_t _getStopBits();
String _getParityBits();
int8_t  _setParityBits(char *ParityBits );


/*********************************************************************************
       +-+-+-+-+-+-+-+-+
       |7|6|5|4|3|2|1|0|
       +-+-+-+-+-+-+-+-+
  --------------------------------------------------
   @Date :
       # Address[2] holds only dates. i.e 1-31
       # Address[3] holds only months. i.e 1-12
       # Address[4] holds only higher value of year.
       # Address[5] holds only Lower value of year.
         @ i.e
         year -->> 2021
           Higher -->> 20
           Higher -->> 21
*********************************************************************************/
#define dateAddress   0x02
#define monthAddress  0x03
#define yearAdressH   0x04
#define yearAdressL   0x05

String _getDate();
char *_setDate(char *date );

/*********************************************************************************
       +-+-+-+-+-+-+-+-+
       |7|6|5|4|3|2|1|0|
       +-+-+-+-+-+-+-+-+
  --------------------------------------------------
   @Calibaration :
     # capacity hold only 

*********************************************************************************/
#define eValueAddress      0x06
#define decimalAddress     0x07
#define capacityAddress    0x08
#define caliWeightAddress  0x12 // reg : 18

String _getCapacity();
char * _setCapacity( char *capacity );

int8_t _geteValue();
int8_t _seteValue( int8_t eValue );

int8_t _getdecimal();
int8_t _setdecimal( int8_t decimal );

String _getcaliWeight();
String _setcaliWeight();

#endif
