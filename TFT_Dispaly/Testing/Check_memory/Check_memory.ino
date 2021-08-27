#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  
  Serial.printf("Total heap: %d\n", ESP.getHeapSize());
  Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
  Serial.printf("Total PSRAM: %d\n", ESP.getPsramSize());
  Serial.printf("Free PSRAM: %d\n", ESP.getFreePsram());
  Serial.printf("Total Flash Size: %d\n", ESP.getFlashChipSize());
    
//  log_d("Total heap: %d", ESP.getHeapSize());
//  log_d("Free heap: %d", ESP.getFreeHeap());
//  log_d("Total PSRAM: %d", ESP.getPsramSize());
//  log_d("Free PSRAM: %d", ESP.getFreePsram());
}

void loop() {}
