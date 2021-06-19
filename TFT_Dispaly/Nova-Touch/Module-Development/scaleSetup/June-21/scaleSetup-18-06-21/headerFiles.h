
#ifndef __headerFile_h
#define __headerFile_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <SPI.h>
#include <TFT_eSPI.h>
#include <String.h>
#include <vector>
#include "Arduino.h"
#include <iostream>
// Include the jpeg decoder library
#include <TJpg_Decoder.h>
#include <XPT2046_Touchscreen.h>
#include <Wire.h>
#include <ErriezDS1307.h>

using namespace std; 

/* Custom Library used for this project. Please use always latest one*/
#include "customFontHeaders.h" 
#include "customImages.h"
#include "settings.h"
#include "UserInputKeyBoard.h"


#define SPL Serial.println  
#define SP Serial.print

#endif
