//
//
//  TftDisplay.hpp
//
//


#ifndef __TFT_DISPLAY_HPP
#define __TFT_DISPLAY_HPP

#include "displayCadre.hpp"

#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include <Wire.h>      // this is needed even tho we aren't using it
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

// The STMPE610 uses hardware SPI on the shield, and #8
#define STMPE_CS 6

// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 9
#define TFT_DC 10

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 3


#define LOOP_REFRESH_POINTS     1000
#define _WHITE ILI9341_WHITE
#define _BLACK ILI9341_BLACK
#define _BLUE ILI9341_BLUE
#define _GREEN ILI9341_GREEN
#define _RED ILI9341_RED
#define _YELLOW ILI9341_YELLOW

#define TFT_ROTATION    2
#define NB_MAX_CADRES   10

class TftDisplayClass
{
    public:
    TftDisplayClass(void);
    ~TftDisplayClass();
    void initTft();
    void refresh();
    void addCadre(DisplayCadreClass *cadrePtr);
    Adafruit_ILI9341 *getTft();
    int getSizeX();
    int getSizeY();
    void afficheListeCadres();


    private:
    int tailleEcranX;
    int tailleEcranY;
    DisplayCadreClass *lstCadresPtr[NB_MAX_CADRES];
    Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);    
    Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);
};

#endif