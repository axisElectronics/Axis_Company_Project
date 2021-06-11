#include <Keypad.h>
#include <SoftwareSerial.h>

SoftwareSerial kpdSerial(9, 8); // RX, TX

const byte ROWS = 5; //four rows
const byte COLS = 4; //three columns

char keys[ROWS][COLS] = {
{1,  2,  3,  4},
  {5, 6,  7,  8},
  {9, 10, 11, 12},
  {13, 14, 15, 16},
  {17, 18, 19, 20}
};
byte rowPins[ROWS] = {2, 3, 5, 4, 6}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {10, 11, 12, 13}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

unsigned long loopCount = 0;
unsigned long timer_t = 0;

void setup(){
  Serial.begin(9600);
  kpdSerial.begin(9600);
  // Try playing with different debounceTime settings to see how it affects
  // the number of times per second your loop will run. The library prevents
  // setting it to anything below 1 millisecond.
  kpd.setDebounceTime(10);  // setDebounceTime(mS)
}

void loop(){
  char key = kpd.getKey();
  
  // Report the number of times through the loop in 1 second. This will give
  // you a relative idea of just how much the debounceTime has changed the
  // speed of your code. If you set a high debounceTime your loopCount will
  // look good but your keypresses will start to feel sluggish.
Serial.println("Hello.....!!!");
  if(key != NO_KEY)
  {
    uint8_t i  = 0;
    unsigned long tout = millis();
    while(millis() - tout < 50)
    {
      char x = kpd.getKey();
      if(x != NO_KEY)
      {
        i++;
        tout = millis();
      }
    }
//    Serial.println(String(key,DEC) + "\t Times: " + String(i));
//    Serial.println(String(key,DEC));
    kpdSerial.println(String(key,DEC));
    Serial.println("Hello.....!!!");
  }
}
