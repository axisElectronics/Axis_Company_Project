
#ifndef __headerFile_h
#define __headerFile_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <SPI.h>
#include "TFT_eSPI.h"
//#include <String.h>
#include <cstring>
#include <vector>
#include "Arduino.h"
#include <iostream>
// Include the jpeg decoder library
#include <TJpg_Decoder.h>
#include <XPT2046_Touchscreen.h>
#include <Wire.h>
#include <ErriezDS1307.h>
#include <array>

using namespace std; 

/* Custom Library used for this project. Please use always latest one*/
#include "customFontHeaders.h" 
#include "customImages.h"
#include "settings.h"
#include "UserInputKeyBoard.h"
#include "command.h"
#include "DisplayInit.h"

#include "freertos/xtensa_context.h"
#include "esp_panic.h"
#include "sdkconfig.h"
#include "soc/soc.h"
#include "soc/dport_reg.h"
#include "soc/gpio_reg.h"
#include <esp_task_wdt.h>

//enum pin{
//  RXD2 = 16,
//  TXD2 = 17,
//  RXD1 = 32,
//  TXD1 = 33,
//}SERIAL_PIN;




#define SPL Serial.println  
#define SP Serial.print

#endif
