
#include <Wire.h>
#include <ErriezDS1307.h>


intitRTC()
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

}
// Create RTC object
ErriezDS1307 rtc;

#define SPLErr(x)  Serial.println("Error : " + String(x) )

typedef struct {

  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
  uint8_t mday;
  uint8_t mon;
  uint16_t year;
  uint8_t wday;
//#define SPLErr(x)  Serial.println("Error : " + String(x) )
} RTC_t;

RTC_t dateTime;


void setup()
{
  Serial.begin(9600);
  delay(1000);

  /*

    // Write time to RTC
    if (!rtc.setTime(11, 55, 0)) {
      // Error: Set time failed
    SPLErr("timesetting");
    }

    // Write RTC date/time: 13:45:09  31 December 2019  2=Tuesday
    if (!rtc.setDateTime(13, 45, 9,  31, 12, 2019,  2) {
      // Error: RTC write failed
    }
  */

}

void loop() {
  // Read time from RTC
  if (!rtc.getTime(&hour, &minute, &second)) {
    // Error: RTC read failed
    SPLErr("gettime failed");
  }

  Serial.println("Time = " + String(hour) + ": " + String(minute) + ": " + String(second) );

}
