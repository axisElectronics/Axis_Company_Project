
#ifndef MACROS_H
#define MACROS_H


/******* MACROs declaration ********/
#define spacing 90
#define digitMargin 90
#define xdigitMargin digitMargin
#define ydigitMargin digitMargin

#define xClr 160
#define yClr 145
#define xOne   ( xClr + spacing)
#define yOne    yClr
#define xTwo    ( xOne + spacing )
#define yTwo    yOne
#define xThree  ( xTwo + spacing )
#define yThree  yTwo

#define xFour   xOne
#define yFour   60
#define xFive   ( xOne + spacing )
#define yFive   yFour
#define xSix  ( xTwo + spacing )
#define ySix    yFive

#define xSeven  xOne
#define ySeven  0
#define xEight ( xOne + spacing )
#define yEight  ySeven
#define xNine   ( xTwo + spacing )
#define yNine   yEight


#define xBack 2
#define yBack  239

#define xDel (160)
#define yDel  yBack

#define xDot  ( xDel + spacing )
#define yDot  yBack

#define xZero ( xDot + spacing )
#define yZero yBack

#define xEnt ( xZero + spacing )
#define yEnt yBack

#define zeroTouch      ( ( (xAxis > xZero  )   && (xAxis < ( xZero  + digitMargin ) ) )   && ( ( yAxis > yZero  ) && ( yAxis < ( yZero + ydigitMargin  ) ) ) )
#define oneTouch       ( ( (xAxis > xOne   )   && (xAxis < ( xOne   + digitMargin ) ) )   && ( ( yAxis > yOne   ) && ( yAxis < ( yOne + ydigitMargin   ) ) ) )
#define twoTouch       ( ( (xAxis > xTwo   )   && (xAxis < ( xTwo   + digitMargin ) ) )   && ( ( yAxis > yTwo   ) && ( yAxis < ( yTwo + ydigitMargin   ) ) ) )
#define threeTouch     ( ( (xAxis > xThree )   && (xAxis < ( xThree + digitMargin ) ) )   && ( ( yAxis > yThree ) && ( yAxis < ( yThree + ydigitMargin ) ) ) )
#define fourTouch      ( ( (xAxis > xFour  )   && (xAxis < ( xFour  + digitMargin ) ) )   && ( ( yAxis > yFour  ) && ( yAxis < ( yFour + ydigitMargin  ) ) ) )
#define fiveTouch      ( ( (xAxis > xFive  )   && (xAxis < ( xFive  + digitMargin ) ) )   && ( ( yAxis > yFive  ) && ( yAxis < ( yFive + ydigitMargin  ) ) ) )
#define sixTouch       ( ( (xAxis > xSix   )   && (xAxis < ( xSix   + digitMargin ) ) )   && ( ( yAxis > ySix   ) && ( yAxis < ( ySix + ydigitMargin   ) ) ) )
#define sevenTouch     ( ( (xAxis > xSeven )   && (xAxis < ( xSeven + digitMargin ) ) )   && ( ( yAxis > ySeven ) && ( yAxis < ( ySeven + ydigitMargin ) ) ) )
#define eightTouch     ( ( (xAxis > xEight )   && (xAxis < ( xEight + digitMargin ) ) )   && ( ( yAxis > yEight ) && ( yAxis < ( yEight + ydigitMargin ) ) ) )
#define nineTouch      ( ( (xAxis > xNine  )   && (xAxis < ( xNine  + digitMargin ) ) )   && ( ( yAxis > yNine  ) && ( yAxis < ( yNine + ydigitMargin  ) ) ) )
#define dotTouch       ( ( (xAxis > xDot   )   && (xAxis < ( xDot   + digitMargin ) ) )   && ( ( yAxis > yDot  ) && ( yAxis < ( yDot + ydigitMargin  ) ) ) )
#define delTouch       ( ( (xAxis > xDel   )   && (xAxis < ( xDel   + digitMargin ) ) )   && ( ( yAxis > yDel  ) && ( yAxis < ( yDel + ydigitMargin  ) ) ) )
#define clrTouch       ( ( (xAxis > xClr   )   && (xAxis < ( xClr   + digitMargin ) ) )   && ( ( yAxis > yClr  ) && ( yAxis < ( yClr + ydigitMargin  ) ) ) )
#define backTouch      ( ( (xAxis > xBack  )   && (xAxis < ( xBack  + 135         ) ) )   && ( ( yAxis > yBack ) && ( yAxis < ( yBack + ydigitMargin ) ) ) )
#define entTouch       ( ( (xAxis > xEnt   )   && (xAxis < ( xEnt   + digitMargin ) ) )   && ( ( yAxis > yEnt  ) && ( yAxis < ( yEnt + ydigitMargin  ) ) ) )

#define x1SW 65
#define x2SW 144
#define y1SW 153
#define y2SW 205

#define xSW ( ( xAxis >= x1SW ) && ( xAxis <= x2SW ) )
#define ySW ( ( yAxis >= y1SW ) && ( yAxis <= y2SW ) )
#define swTouch (xSW && ySW)

//===================================================================//

enum touchKey
{
  keepRunning = -20,
  sw = -6,
  back,
  del,
  clr,
  dot,
  ent,
  zero = 0,
  one,
  two,
  three,
  four,
  five,
  six,
  seven,
  eight,
  nine,
};



#define findkey zeroTouch ? zero : oneTouch ? one : twoTouch ? two : threeTouch ? three : fourTouch ? four : fiveTouch ? five :\
                  sixTouch ? six : sevenTouch ? seven : eightTouch ? eight : nineTouch ? nine : backTouch ? back :\
                  delTouch ? del : entTouch ? ent : dotTouch ? dot : clrTouch ? clr : swTouch ? sw : keepRunning;

//<<=============================================================>>//

// The TIRQ interrupt signal must be used for this example.
#define INT_STATUS  0x3FF44044
#define INT_CLEAR 0x3FF4404C
#define INT_SET 0x3FF44048


//<<=============================================================>>//

// Serial_1 will be used for getting data from Arduino NANO for Keypad data.
#define RXD1 32
#define TXD1 33

#endif
