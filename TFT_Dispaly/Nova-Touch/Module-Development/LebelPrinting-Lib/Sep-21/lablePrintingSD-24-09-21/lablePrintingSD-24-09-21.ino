#include "prtLabel.h"
#include "UserInputKeyBoard.h"

TFT_eSPI tft = TFT_eSPI();

class userKeyBoard kbd;
class prtLabel printer(SERIAL_NUM_DEFAULT, BAUD_RATE_115200);




void setup() {

  //========================================================
  //         Serial Begin
  //========================================================
  Serial2.begin(BAUD_RATE_9600, SERIAL_8N1, prt_RXD2, prt_TXD2);
  Serial.begin(115200);
  
 
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  uint16_t calData[5] = { 250, 3591, 1154, 2671, 2 };
  tft.setTouch(calData);
  
  printer.header[0] = String( kbd.takeStringInput() );
  printer.header[1] = String( kbd.takeStringInput() );
  
  Serial.println("===### " + printer.header[0] );
  Serial.println("===>>> " + printer.header[1] );
  
}




void loop() {
   
}
