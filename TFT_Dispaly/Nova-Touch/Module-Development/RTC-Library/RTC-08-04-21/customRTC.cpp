

#include "RTCHeader.h"

// Create RTC object
ErriezDS1307 rtc;

/******************************************************************
   @intitRTC() :
   (#) initilize RTC module.
 *******************************************************************/
void customRTC ::initRTC()
{
  // Initialize TWI with a 100kHz (default)
  Wire.begin();
  Wire.setClock(400000); // Initialize TWI with a 400kHz

  // Initialize RTC
  if (!rtc.begin()) {
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


String customRTC :: getRTCTime()
{
  String temp = "";
  // Read time from RTC
  if (!rtc.getTime(&RTCHandler.hour, &RTCHandler.minute, &RTCHandler.second)) {
    // Error: RTC read failed
    SPLErr("gettime failed");
  }
  Serial.println("getTime = " + String(RTCHandler.hour) + ": " + String(RTCHandler.minute) + ": " + String( RTCHandler.second ) );
  if (  RTCHandler.hour > 12 ) {
    RTCHandler.hour -= 12;
    if ( RTCHandler.hour == 11 && RTCHandler.minute == 59 && RTCHandler.second == 59 ) {
      RTCHandler.mday++;
    }
  }

  temp += String( RTCHandler.hour ) + ":";
  temp += String( RTCHandler.minute)  + ":";
  temp += String( RTCHandler.second );
  return temp;
}

void  customRTC :: setRTCTime()
{
  // Write time to RTC
  Serial.println("setTime = " + String(RTCHandler.hour) + ": " + String(RTCHandler.minute) + ": " + String( RTCHandler.second ) );
  if (!rtc.setTime( RTCHandler.hour, RTCHandler.minute,  RTCHandler.second )) {
    // Error: Set time failed
    SPLErr("settime failed");
  }
}

/*
   \param mday
        Day of the month 1..31
   \param mon
        Month 1..12 (1=January)
   \param year
        Year 2000..2099
   \param wday
        Day of the week 0..6 (0=Sunday, .. 6=Saturday)
   \retval true
        Success.
   \retval false
        Set date/time failed.

*/
bool customRTC :: setRTCDate()
{
  struct tm dt;

  dt.tm_mday = RTCHandler.mday;
  dt.tm_mon  = RTCHandler.mon - 1;
  dt.tm_year = RTCHandler.year - 1900;
  dt.tm_wday = RTCHandler.wday;

  return rtc.write(&dt);
}



/*!
   \brief Get date time
   \param hour
        Hours 0..23
   \param min
        Minutes 0..59
   \param sec
        Seconds 0..59
   \param mday
        Day of the month 1..31
   \param mon
        Month 1..12 (1=January)
   \param year
        Year 2000..2099
   \param wday
        Day of the week 0..6 (0=Sunday, .. 6=Saturday)
   \retval true
        Success.
   \retval false
        Get date/time failed.
*/

String customRTC :: getRTCDate()
{
  struct tm dt;

  // Read date/time from RTC
  if (!rtc.read(&dt)) {
    return "Error : RTC date";
  }

  RTCHandler.mday  = dt.tm_mday;
  RTCHandler.mon = dt.tm_mon + 1;
  RTCHandler.year = dt.tm_year + 1900;
  RTCHandler.wday = dt.tm_wday;

  String temp = "";

  temp += String( RTCHandler.mday  ) + "/";
  temp += String( RTCHandler.mon   ) + "/";
  temp += String( RTCHandler.year  ) + "-";
  temp += String( RTCHandler.wday  );

  //  uint8_t datereg = 0x04;
  //  Serial.print(" readRegister >>> " );
  //  Serial.println(  readRegister(datereg) );

  return temp;
}
