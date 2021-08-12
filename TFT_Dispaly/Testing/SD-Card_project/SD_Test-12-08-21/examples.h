
#include "headerFiles.h"

extern TFT_eSPI tft;

/*<-------------- File Read & Write Examples ------------>*/ 
void example_1(){
char str[4096];

memset(str, '\0', 4096);

uint32_t timeout = millis();
/* unComment any line to check the result in SD card.*/
//checkFile("/test.txt");
//readFile("/test.txt", str );
//writeFile("/test_1.txt", str);
//copyFile("/test_1.txt", "/walk.h");
Serial.printf("Timout : %ld\n", millis()-timeout);
delay(1000);
}


/*<-------------- GIF Examples ------------>*/ 
void example_2(){  
 uint16_t w = 0, h = 0;
 char imagebuff[50];
 
tft.fillScreen(TFT_BLACK);
 
uint32_t timeout = millis();
for(uint8_t i=1; i < 30; i++ )
{ 
  sprintf(imagebuff, "/walk/%d.jpg", i);
//  Serial.printf("==>> %s\n", imagebuff );
  TJpgDec.getSdJpgSize(&w, &h, imagebuff );
// Draw the image, top left at 0,0
  TJpgDec.drawSdJpg(0, 0, imagebuff );
yield();
}
 Serial.printf("Timeout : %ld\n", ( millis()-timeout ));

}


/*<-------------- IMAGES Examples ------------>*/ 
void example_3(){
   uint16_t w = 0, h = 0;
  char imagePathBuff[50] = "/images/flower.jpg";
  tft.fillScreen(TFT_BLACK);
  uint32_t timeout = millis();
  TJpgDec.getSdJpgSize(&w, &h, imagePathBuff );
  // Draw the image, top left at 0,0
  TJpgDec.drawSdJpg(0, 0, imagePathBuff );
  Serial.printf("Timeout : %ld\n", ( millis()-timeout ));
}
