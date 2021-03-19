
#ifndef _UserInputKeyBoard_H
#define _UserInputKeyBoard_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <SPI.h>
#include <TFT_eSPI.h>
#include <String.h>
#include <vector>
using namespace std; 

int8_t _findkeypadTouch( uint16_t xAxis, uint16_t yAxis);

class userKeyBoard
{
  public :
  userKeyBoard( );
  ~userKeyBoard( );

};


#endif
