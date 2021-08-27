
// Serial_2 will be used for getting Weighing Scale data from machine.
#define RXD2 16
#define TXD2 17

void setup() {
 Serial.begin(9600); 
 Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // Weight Mechine
 delay(500);
}

void loop() {
  Serial.write("Hello World...!!!");
  Serial2.write("Hello World...!!!");
  if(Serial2.available() > 2 ){
    Serial.println("Data Received on Serial2...");
  }
}
