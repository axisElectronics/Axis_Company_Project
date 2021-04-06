
#include "commonGlobal.h"

#define SPLErr(x)  Serial.println("Error : " + String(x) )

// Create RTC object
ErriezDS1307 rtc;



/******************************************************************
   @intitRTC() :
   (#) initilize RTC module.
                                                                 
 *******************************************************************/


void WeighingHandle ::intitRTC()
{
  // Initialize TWI with a 100kHz (default)
  Wire.begin();
  Wire.setClock(400000); // Initialize TWI with a 400kHz

  // Initialize RTC
  while (!rtc.begin()) {
    Serial.println(F("RTC not found"));
    delay(3000);
  }

  // Check oscillator status
  if (!rtc.isRunning()) {
    // Error: RTC oscillator stopped. Date/time cannot be trusted.
    // Set new date/time before reading date/time.
    SPLErr("oscillatorproblem");
    // Enable oscillator
    rtc.clockEnable(true);
  }
}//end-initRTC()


void WeighingHandle :: updateRTCVariable()
{
   // Read time from RTC
  if (!rtc.getTime(&RTCHandler.hour, &RTCHandler.minute, &RTCHandler.second)) {
    // Error: RTC read failed
    SPLErr("gettime failed");
  }

  Serial.println("Time = " + String(RTCHandler.hour) + ": " + String(RTCHandler.minute) + ": " + String( RTCHandler.second ) );

}

void  WeighingHandle :: setRTCValue()
{
  // Write time to RTC
if (!rtc.setTime(RTCHandler.hour, RTCHandler.minute,  RTCHandler.second )) {
    // Error: Set time failed
     SPLErr("settime failed");
} 
}
