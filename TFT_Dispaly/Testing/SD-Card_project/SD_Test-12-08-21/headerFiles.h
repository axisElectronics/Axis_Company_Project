
#ifndef __FILE_OPERATIONS_H
#define __FILE_OPERATIONS_H

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <TFT_eSPI.h> // Hardware-specific library

 // Include the jpeg decoder library
#include <TJpg_Decoder.h> 


void checkFile(const char *path);
void readFile(const char *path, char *userBuff);
void writeFile(const char *path, const char *userBuff);
void copyFile(char *destpath, const char *srcpath);
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap);
void showSdImageOnTFT(const char *imageSdpath );

void TFTinit();
void SDCardInit();
void startJPEGDecoder();

#endif
