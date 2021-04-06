
#ifndef _UserInputKeyBoard_H
#define _UserInputKeyBoard_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <SPI.h>
#include <TFT_eSPI.h>
#include <String.h>
#include <vector>

#include "mplus_1m_bold20pt7b.h" 

using namespace std; 

#define mplus_B20   &mplus_1m_bold20pt7b 


int8_t _findkeypadTouch( uint16_t xAxis, uint16_t yAxis);
void printCharArray( char printChar, int8_t idx );
char findChar(int8_t idx);


class userKeyBoard
{
  public :
  userKeyBoard( );
  ~userKeyBoard( );

};


#endif
