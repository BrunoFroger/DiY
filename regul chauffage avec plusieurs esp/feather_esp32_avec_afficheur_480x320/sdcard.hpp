// chauffage
// sdcard.hpp
// (c) B. Froger 2020


#include <Arduino.h>
#include <SPI.h>
#include <SD.h>



#ifndef __SDCARD
#define __SDCARD

extern void initSdcard(void);
char *getSdcardInfo(char *valueName);


#endif