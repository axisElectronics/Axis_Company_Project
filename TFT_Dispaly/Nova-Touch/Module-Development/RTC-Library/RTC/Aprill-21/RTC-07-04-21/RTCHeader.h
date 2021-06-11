
#ifndef RTCHeader_H
#define RTCHeader_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Arduino.h"
#include <iostream>
#include <Wire.h>
#include <ErriezDS1307.h>

using namespace std; 

#define SPLErr(x)  Serial.println("Error : " + String(x) )



class customRTC
{
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
