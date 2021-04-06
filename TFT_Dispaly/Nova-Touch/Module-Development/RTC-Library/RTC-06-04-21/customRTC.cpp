

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


void customRTC :: getRTCTime()
{
  // Read time from RTC
  if (!rtc.getTime(&RTCHandler.hour, &RTCHandler.minute, &RTCHandler.second)) {
    // Error: RTC read failed
    SPLErr("gettime failed");
  }

  Serial.println("Time = " + String(RTCHandler.hour) + ": " + String(RTCHandler.minute) + ": " + String( RTCHandler.second ) );

}

void  customRTC :: setRTCTime()
{
  // Write time to RTC
  if (!rtc.setTime(RTCHandler.hour, RTCHandler.minute,  RTCHandler.second )) {
    // Error: Set time failed
    SPLErr("settime failed");
  }
}

/*
 * \param mday
 *      Day of the month 1..31
 * \param mon
 *      Month 1..12 (1=January)
 * \param year
 *      Year 2000..2099
 * \param wday
 *      Day of the week 0..6 (0=Sunday, .. 6=Saturday)
 * \retval true
 *      Success.
 * \retval false
 *      Set date/time failed.
 *      
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
 * \brief Get date time
 * \param hour
 *      Hours 0..23
 * \param min
 *      Minutes 0..59
 * \param sec
 *      Seconds 0..59
 * \param mday
 *      Day of the month 1..31
 * \param mon
 *      Month 1..12 (1=January)
 * \param year
 *      Year 2000..2099
 * \param wday
 *      Day of the week 0..6 (0=Sunday, .. 6=Saturday)
 * \retval true
 *      Success.
 * \retval false
 *      Get date/time failed.
 */
 
bool customRTC :: getRTCDate()
{
  rtc.getDateTime( &RTCHandler.hour , &RTCHandler.minute , &RTCHandler.second , \
                          &RTCHandler.mday , &RTCHandler.mon , &RTCHandler.year, \
                          & RTCHandler.wday );

                          
}
