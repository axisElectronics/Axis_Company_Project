

#include "RTCHeader.h"

// Create RTC object
#ifdef DS1307_RTC
ErriezDS1307 rtc; // rtc DS1307
#endif

#ifdef R8025_RTC
RX8025_RTC rtc;
#endif

/******************************************************************
   @intitRTC() :
   (#) initilize RTC module.
 *******************************************************************/
void customRTC ::initRTC()
{
  // Initialize TWI with a 100kHz (default)
  Wire.begin();
  Wire.setClock(400000); // Initialize TWI with a 400kHz

#ifdef DS1307_RTC
  // Initialize RTC
  if (!rtc.begin())
{
    Serial.println(F("RTC not found"));
    delay(3000);
  }

  // Check oscillator status
  if (!rtc.isRunning()) 
  {
    // Error: RTC oscillator stopped. Date/time cannot be trusted.
    // Set new date/time before reading date/time.
    SPLErr("oscillatorproblem");
  //   Enable oscillator
    rtc.clockEnable(true);
  }
 #endif 
 
}//end-initRTC()


String customRTC :: getRTCTime()
{
  String temp = "";
  // Read time from RTC

 #ifdef DS1307_RTC
     if (!rtc.getTime(&RTCHandler.hour, &RTCHandler.minute, &RTCHandler.second)) {
    // Error: RTC read failed
    SPLErr("gettime failed");
  }

    if (!rtc.getTime(&RTCHandler.hour, &RTCHandler.minute, &RTCHandler.second)) {
    // Error: RTC read failed
    SPLErr("gettime failed");
  }
  Serial.println("getTime = " + String(RTCHandler.hour) + ": " + String(RTCHandler.minute) + ": " + String( RTCHandler.second ) );

 #endif

 #ifdef R8025_RTC 
   tm2 = rtc.read(); 
  RTCHandler.hour =   tm2.Hour;
  RTCHandler.minute = tm2.Minute;
  RTCHandler.second = tm2.Second;
 #endif
  
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
 // Serial.println("setTime = " + String(RTCHandler.hour) + ": " + String(RTCHandler.minute) + ": " + String( RTCHandler.second ) );
 #ifdef DS1307_RTC
  if (!rtc.setTime( RTCHandler.hour, RTCHandler.minute,  RTCHandler.second )) {
    // Error: Set time failed
    SPLErr("settime failed");
  } 
  #endif

  #ifdef R8025_RTC 
  tm2.Hour   = RTCHandler.hour;
  tm2.Minute = RTCHandler.minute;
  tm2.Second = RTCHandler.second;
  rtc.write(tm2);
  #endif

  
}

  
/*******************************************************
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

*******************************************************/
bool customRTC :: setRTCDate()
{
  
  #ifdef DS1307_RTC
  struct tm tm2;
  tm2.tm_mday = RTCHandler.mday;
  tm2.tm_mon  = RTCHandler.mon - 1;
  tm2.tm_year = RTCHandler.year - 1900;
  tm2.tm_wday = RTCHandler.wday;  
  return rtc.write(&tm2); 
  #endif
  
  #ifdef R8025_RTC   
  Serial.println("In R8025...!!!!");
  tm2.Day    = RTCHandler.mday;
  tm2.Month  = RTCHandler.mon; 
  tm2.Year   = RTCHandler.year -1970;
  tm2.Wday   = RTCHandler.wday;
  return rtc.write(tm2);

  #endif

}



/*******************************************************!
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
*******************************************************/

String customRTC :: getRTCDate()
{
  

#ifdef DS1307_RTC
  // Read date/time from RTC
  struct tm tm2;
  if (!rtc.read(&tm2)) {
    return "Error : RTC date";
  }

  RTCHandler.mday  = tm2.tm_mday;
  RTCHandler.mon = tm2.tm_mon + 1;
  RTCHandler.year = tm2.tm_year + 1900;
  RTCHandler.wday = tm2.tm_wday;

#else
  tm2 = rtc.read(); 
  RTCHandler.mday  = tm2.Day;
  RTCHandler.mon = tm2.Month;
  RTCHandler.year = tm2.Year + 1970;
  RTCHandler.wday = tm2.Wday;
#endif


  String temp = "";

  temp += String( RTCHandler.mday  ) + "/";
  temp += String( RTCHandler.mon   ) + "/";
  temp += String( RTCHandler.year  ) + "-";
  temp += String( RTCHandler.wday  );
  return temp;
}
