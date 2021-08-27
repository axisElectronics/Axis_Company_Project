#ifndef customFontHeaders_h
#define customFontHeaders_h



/* 
	@I uaed this utility to convert fonts 
	https://rop.nl/truetype2gfx/
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <SPI.h>
#include <TFT_eSPI.h>
#include <String.h>
#include <vector>

#include "Free_Fonts.h"

#include "mplus_1m_regular13pt7b.h" 
#include "mplus_1m_regular8pt7b.h"

#include "LED7SEG_Standard40pt7b.h"
#include "LED7SEG_Standard20pt7b.h"
#include "LED7SEG_Standard10pt7b.h"
#include "LED7SEG_Standard18pt7b.h"

#include "EurostileBold20pt7b.h"
#include "EurostileBold13pt7b.h"
#include "EurostileBold10pt7b.h"
#include "EurostileBold7pt7b.h"

using namespace std; 

#define mplus_R13   &mplus_1m_regular13pt7b 
#define mplus_R8   &mplus_1m_regular8pt7b

#define LED7SEG_STD40   &LED7SEG_Standard40pt7b
#define LED7SEG_STD20   &LED7SEG_Standard20pt7b
#define LED7SEG_STD10   &LED7SEG_Standard10pt7b
#define LED7SEG_STD18   &LED7SEG_Standard18pt7b
#define EUROSTILE_B20   &EurostileBold20pt7b

#define EUROSTILE_B13   &EurostileBold13pt7b
#define EUROSTILE_B10   &EurostileBold10pt7b 
#define EUROSTILE_B7    &EurostileBold7pt7b 

#endif
