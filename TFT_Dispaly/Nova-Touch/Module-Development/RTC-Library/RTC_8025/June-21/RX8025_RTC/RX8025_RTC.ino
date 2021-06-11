#include <TimeLib.h>
#include <Wire.h>
#include "RX8025_RTC.h"
#define SECOND_ADJUSTMENT 16

RX8025_RTC rtc;
tmElements_t tm;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  //コンパイル時刻にセットの場合、以下をコメント解除
  initDateTime();
  rtc.write(tm);

  Serial.println("RTC start >.....!!!");
}

void loop() {
  tmElements_t tm2;
  tm2 = rtc.read();
  char s[20];
 // sprintf(s,"%d/%d/%d %d:%d:%d",tmYearToCalendar(tm2.Year),tm2.Month,tm2.Day,tm2.Hour,tm2.Minute,tm2.Second);
   sprintf(s,"%d/%d/%d %d:%d:%d", tm2.Day, tm2.Month, tmYearToCalendar(tm2.Year), tm2.Hour, tm2.Minute, tm2.Second);
  Serial.println(String(s));
  delay(1000);
}

String updateDateTime()
{
  tmElements_t tm2;
  tm2 = rtc.read();
  char s[20];
  sprintf(s,"%d/%d/%d %d:%d:%d", tm2.Day, tm2.Month, tmYearToCalendar(tm2.Year), tm2.Hour, tm2.Minute, tm2.Second);
 // Serial.println(String(s));
  
return  String(s);
}

bool initDateTime(){
  const char* monthNames[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
  char mon[12];
  int Year, Month, Day, Hour, Minute, Second;

  if(sscanf(__DATE__,"%s %d %d",mon, &Day, &Year) != 3){
    return false;
  }
  if(sscanf(__TIME__,"%d:%d:%d",&Hour, &Minute, &Second) != 3){
    return false;
  }

  uint8_t idx;
  Month = 0;
  for(idx = 0; idx < 12; idx++){
    if(strcmp(mon, monthNames[idx]) == 0){
      Month = idx + 1;
      break;
    }
  }
  if(Month == 0){
      return false;
  }
  Second += SECOND_ADJUSTMENT;
  tm.Year = CalendarYrToTm(Year);
  tm.Month = Month;
  tm.Day = Day;
  tm.Hour = Hour;
  tm.Minute = Minute;
  tm.Second = Second;
  return true;
}
