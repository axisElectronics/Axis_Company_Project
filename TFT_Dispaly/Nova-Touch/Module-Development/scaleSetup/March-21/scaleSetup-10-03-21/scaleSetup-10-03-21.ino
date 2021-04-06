
#include "commonGlobal.h"

TFT_eSPI tft = TFT_eSPI();
class WeighingHandle Wtft;

//class settings set;
// this function determines the minimum of two numbers
#define minimum(a,b)     (((a) < (b)) ? (a) : (b))

//#define USE_SPI_BUFFER // Comment out to use slower 16 bit pushColor()

//====================================================================================
//   Opens the image file and prime the Jpeg decoder
//====================================================================================
void WeighingHandle :: drawJpeg(const char *filename, int xpos, int ypos) {

  Serial.println("===========================");
  Serial.print("Drawing file: "); Serial.println(filename);
  Serial.println("===========================");

  // Open the named file (the Jpeg decoder library will close it after rendering image)
  fs::File jpegFile = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
  //  File jpegFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
 
  if ( !jpegFile ) {
    Serial.print("ERROR: File \""); Serial.print(filename); Serial.println ("\" not found!");
    return;
  }

  // Use one of the three following methods to initialise the decoder:
  //boolean decoded = JpegDec.decodeFsFile(jpegFile); // Pass a SPIFFS file handle to the decoder,
  //boolean decoded = JpegDec.decodeSdFile(jpegFile); // or pass the SD file handle to the decoder,
  boolean decoded = JpegDec.decodeFsFile(filename);  // or pass the filename (leading / distinguishes SPIFFS files)
                                   // Note: the filename can be a String or character array type
  if (decoded) {
    // print information about the image to the serial port
    jpegInfo();

    // render the image onto the screen at given coordinates
    jpegRender(xpos, ypos);
  }
  else {
    Serial.println("Jpeg file format not supported!");
  }
}



//====================================================================================
//   Print information decoded from the Jpeg image
//====================================================================================
void WeighingHandle ::  jpegInfo() {

  Serial.println("===============");
  Serial.println("JPEG image info");
  Serial.println("===============");
  Serial.print  ("Width      :"); Serial.println(JpegDec.width);
  Serial.print  ("Height     :"); Serial.println(JpegDec.height);
  Serial.print  ("Components :"); Serial.println(JpegDec.comps);
  Serial.print  ("MCU / row  :"); Serial.println(JpegDec.MCUSPerRow);
  Serial.print  ("MCU / col  :"); Serial.println(JpegDec.MCUSPerCol);
  Serial.print  ("Scan type  :"); Serial.println(JpegDec.scanType);
  Serial.print  ("MCU width  :"); Serial.println(JpegDec.MCUWidth);
  Serial.print  ("MCU height :"); Serial.println(JpegDec.MCUHeight);
  Serial.println("===============");
  Serial.println("");
}


//====================================================================================
//   Decode and render the Jpeg image onto the TFT screen
//====================================================================================
void  WeighingHandle ::  jpegRender(int xpos, int ypos) {

  // retrieve infomration about the image
  uint16_t  *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;

  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  uint32_t min_w = minimum(mcu_w, max_x % mcu_w);
  uint32_t min_h = minimum(mcu_h, max_y % mcu_h);

  // save the current image block size
  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;

  // record the current time so we can measure how long it takes to draw an image
  uint32_t drawTime = millis();

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // read each MCU block until there are no more
#ifdef USE_SPI_BUFFER
  while( JpegDec.readSwappedBytes()){ // Swap byte order so the SPI buffer can be used
#else
  while ( JpegDec.read()) { // Normal byte order read
#endif
    // save a pointer to the image block
    pImg = JpegDec.pImage;

    // calculate where the image block should be drawn on the screen
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right and bottom edges
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;

    // calculate how many pixels must be drawn
    uint32_t mcu_pixels = win_w * win_h;

    // draw image MCU block only if it will fit on the screen
    if ( ( mcu_x + win_w) <= tft.width() && ( mcu_y + win_h) <= tft.height())
  {
#ifdef USE_SPI_BUFFER
      // Now set a MCU bounding window on the TFT to push pixels into (x, y, x + width - 1, y + height - 1)
      tft.setWindow(mcu_x, mcu_y, mcu_x + win_w - 1, mcu_y + win_h - 1);
      // Write all MCU pixels to the TFT window
      uint8_t *pImg8 = (uint8_t*)pImg;     // Convert 16 bit pointer to an 8 bit pointer
      tft.pushColors(pImg8, mcu_pixels*2); // Send bytes via 64 byte SPI port buffer
#else
      // Now set a MCU bounding window on the TFT to push pixels into (x, y, x + width - 1, y + height - 1)
      tft.setAddrWindow(mcu_x, mcu_y, mcu_x + win_w - 1, mcu_y + win_h - 1);
      // Write all MCU pixels to the TFT window
      while (mcu_pixels--) tft.pushColor(*pImg++);
#endif
    }

    else if ( ( mcu_y + win_h) >= tft.height()) JpegDec.abort();

  }

  // calculate how long it took to draw the image
  drawTime = millis() - drawTime; // Calculate the time it took

  // print the results to the serial port
  Serial.print  ("Total render time was    : "); Serial.print(drawTime); Serial.println(" ms");
  Serial.println("=====================================");

}


void WeighingHandle :: readyAxisScales()
{
  int Mode = 0;
  uint16_t xAxis, yAxis;

  initWeighingTFT( );

HERE :
  settingPageInit( ); // import default settings

  uint8_t tempdot = _getDecimal().c_str()[0];
  showDigits.dotPosition = tempdot - 48;
  
  String temp = _getApplication();

  if ( temp == "Weighing Mode" ) userSetting.weighingMode =   1; else userSetting.weighingMode =    0;
  if ( temp == "Counting Mode" ) userSetting.countingMode =   1; else userSetting.countingMode =    0;
  if ( temp == "Check Weighing") userSetting.checkWeighing =  1; else userSetting.checkWeighing =   0;
  if ( temp == "Price Weighing") userSetting.pricecomputing = 1; else userSetting.pricecomputing =  0;


 /*************************************************************************/
 /*************************************************************************/ 
// call SPIFFS imgae here.
    if ( !SPIFFS.begin(true) ) {
    Serial.println("SPIFFS Failed");
    return;
  } 

  if ( SPIFFS.exists("test.jpg") ) {
    Serial.println("test.jpg found on SD card.");
  }
  
  drawJpeg("/test.jpg", 0, 0);

  delay(5000);
    
  while(1) yield();
/*************************************************************************/
/*************************************************************************/
  Wtft.startUPImage();  
  while (1)
  {

    if ( tft.getTouch( &xAxis, &yAxis) )
    {
      if ( SettingTouchEnable() )
      {
        Mode = 1;
        break;
      }
      else if ( WeighingModeTouchEnable()  )
      {
        if ( userSetting.weighingMode ) {
          Mode = 2;
          break;
        }
      }
      else if ( CountingModeTouchEnable()  )
      {
        if ( userSetting.countingMode ) {
          Mode = 3;
          break;
        }
      }
      else if ( PriceingModeTouchEnable() )
      {
        if ( userSetting.pricecomputing ) {
          Mode = 4;
          break;
        }
      }
      else if ( CheckingModeTouchEnable() )
      {
        Mode = 5; //break;
      }

    }
    yield();
  }//end-while()

  switch (Mode)
  {
    case 1:
      {
        settingPageHandler( );
        SPL("I am Back my friends \n" );
        tft.fillScreen(TFT_BLACK);
        Mode = 0;
        goto HERE;
      }
      break;
    case 2:
      if ( startWeighing()  == -1 )
      {
        Mode = 0;
        goto HERE;
      }
      break;
    case 3:
      if ( userSetting.countingMode )
      {
        if ( startCountComputing() == -1 )
        {
          Mode = 0;
          goto HERE;
        }
      }
      break;
    case 4:
      if ( userSetting.pricecomputing )
      {
        if ( startPriceComputing() == -1 )
        {
          Mode = 0;
          goto HERE;
        }
      } break;
    case 5: break;
  }

}


void setup() {

  Wtft.readyAxisScales();

}

void loop() {
//  int Mode = 0;
//  uint16_t xAxis, yAxis;
//
//HERE :
//   Wtft.settingPageInit( ); // import default settings
//
//
//  uint8_t tempdot =  Wtft._getDecimal().c_str()[0];
//   Wtft.showDigits.dotPosition = tempdot - 48;
//
//  String temp =  Wtft._getApplication();
//
//  if ( temp == "Weighing Mode" )  Wtft.userSetting.weighingMode =   1; else  Wtft.userSetting.weighingMode =    0;
//  if ( temp == "Counting Mode" )  Wtft.userSetting.countingMode =   1; else  Wtft.userSetting.countingMode =    0;
//  if ( temp == "Check Weighing")  Wtft.userSetting.checkWeighing =  1; else  Wtft.userSetting.checkWeighing =   0;
//  if ( temp == "Price Weighing")  Wtft.userSetting.pricecomputing = 1; else  Wtft.userSetting.pricecomputing =  0;
//
//  Wtft.startUPImage();
//  while (1)
//  {
//
//    if ( tft.getTouch( &xAxis, &yAxis) )
//    {
//      if ( SettingTouchEnable() )
//      {
//        Mode = 1;
//        break;
//      }
//      else if ( WeighingModeTouchEnable()  )
//      {
//        if (  Wtft.userSetting.weighingMode ) {
//          Mode = 2;
//          break;
//        }
//      }
//      else if ( CountingModeTouchEnable()  )
//      {
//        if (  Wtft.userSetting.countingMode ) {
//          Mode = 3;
//          break;
//        }
//      }
//      else if ( PriceingModeTouchEnable() )
//      {
//        if (  Wtft.userSetting.pricecomputing ) {
//          Mode = 4;
//          break;
//        }
//      }
//      else if ( CheckingModeTouchEnable() )
//      {
//        Mode = 5; //break;
//      }
//
//    }
//    yield();
//  }//end-while()
//
//  switch (Mode)
//  {
//    case 1:
//      {
//         Wtft.settingPageHandler( );
//        SPL("I am Back my friends \n" );
//        tft.fillScreen(TFT_BLACK);
//        Mode = 0;
//        goto HERE;
//      }
//      break;
//    case 2:
//      if ( Wtft.startWeighing()  == -1 )
//      {
//        Mode = 0;
//        goto HERE;
//      }
//      break;
//    case 3:
//      if (  Wtft.userSetting.countingMode )
//      {
//        if ( Wtft.startCountComputing() == -1 )
//        {
//          Mode = 0;
//          goto HERE;
//        }
//      }
//      break;
//    case 4:
//      if (  Wtft.userSetting.pricecomputing )
//      {
//        if ( Wtft.startPriceComputing() == -1 )
//        {
//          Mode = 0;
//          goto HERE;
//        }
//      } break;
//    case 5: break;
//  }

}
