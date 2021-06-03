
#include "RTCHeader.h"

class customRTC myRTC;

void setup()
{
  Serial.begin(9600);
  delay(1000);

  myRTC.initRTC();
}

void loop() {
 myRTC.getRTCTime();
}
