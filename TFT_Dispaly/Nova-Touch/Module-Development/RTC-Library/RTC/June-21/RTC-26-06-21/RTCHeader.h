
#ifndef RTCHeader_H
#define RTCHeader_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Arduino.h"
#include <iostream>

/**********************************************************
 * Please select RTC clock by uncomment proper MACRO
 * Given Below. It support 2 Real time clock.
 * 
 ***********************************************************/

//#define DS1307_RTC
#define R8025_RTC 1


// Enable RTC DS1307
#ifdef DS1307_RTC

#include <Wire.h>
#include <ErriezDS1307.h>

#endif

// Enable RTC R8025
#ifdef R8025_RTC

#include "RX8025_RTC.h"
#include <Wire.h>
#include <TimeLib.h>

#endif

using namespace std; 

#define SPLErr(x)  Serial.println("Error : " + String(x) )

/*****************************************************
typedef struct  { 
  uint8_t Second; 
  uint8_t Minute; 
  uint8_t Hour; 
  uint8_t Wday;   // day of week, sunday is day 1
  uint8_t Day;
  uint8_t Month; 
  uint8_t Year;   // offset from 1970; 
}   tmElements_t, TimeElements, *tmElementsPtr_t;
******************************************************/
#ifdef  DS1307_RTC
class customRTC : public ErriezDS1307
#endif

#ifdef R8025_RTC
class customRTC : public RX8025_RTC 
#endif
{
  private :
  tmElements_t tm2; 
  
  public :
 
  struct rtcvar {
      uint8_t hour;
      uint8_t minute;
      uint8_t second;
      
      uint8_t mday;
      uint8_t mon;
      uint16_t year;
      uint8_t wday;
    } RTCHandler;

  /*******************************************
                >>>>  RTC Module  <<<<<
   *******************************************/
     void initRTC();
     String getRTCTime();
     void setRTCTime();
     bool setRTCDate();
     String getRTCDate();
     
};


#endif
