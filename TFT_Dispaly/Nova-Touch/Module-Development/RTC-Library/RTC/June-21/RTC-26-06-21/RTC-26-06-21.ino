
#include "RTCHeader.h"

class customRTC myRTC;

void setup()
{
  Serial.begin(9600);
  delay(1000);
  myRTC.initRTC();
/*
//   myRTC.RTCHandler.mday    = 31;
//   myRTC.RTCHandler.mon     = 12;
//   myRTC.RTCHandler.year    = 2021;
//   myRTC.RTCHandler.wday    = 6;
//   myRTC.setRTCDate();

//  myRTC.RTCHandler.mday    = 31;
//   myRTC.RTCHandler.mon     = 12;
//   myRTC.RTCHandler.year    = 2021;
//   myRTC.RTCHandler.wday    = 6;
//   myRTC.setRTCDate();

//   myRTC.RTCHandler.mday    = 31;
//   myRTC.RTCHandler.mon     = 12;
//   myRTC.RTCHandler.year    = 2021;
//   myRTC.RTCHandler.wday    = 6;
//   myRTC.setRTCDate();
   
  myRTC.RTCHandler.hour = 23;
  myRTC.RTCHandler.minute = 59;
  myRTC.RTCHandler.second = 45;
  myRTC.setRTCTime();
*/
}

void loop() {
  Serial.println("Time >>> " + myRTC.getRTCTime() );
  Serial.println("Date >>> " + myRTC. getRTCDate() );
}
