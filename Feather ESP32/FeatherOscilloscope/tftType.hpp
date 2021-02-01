//
//
//  tftType.hpp
//
//

// uncomment the define corresponding to the type of tft used by the project
// if you want to use another type of tft create a new ifdef/endif bloc 
#define __Adafruit_HXHX8357     //Adafruit Featherwing 3"5 480x320
//#define __Adafruit_ILI9341      //Adafruit Featherwing 2"8 240x320

#ifdef __Adafruit_HXHX8357
    #include <Adafruit_HX8357.h>
    typedef Adafruit_HX8357 tftType;
    #define _WHITE HX8357_WHITE
    #define _BLACK HX8357_BLACK
    #define _BLUE HX8357_BLUE
    #define _GREEN HX8357_GREEN
    #define _RED HX8357_RED
    #define _YELLOW HX8357_YELLOW
    #define _RDMODE HX8357_RDPOWMODE
    #define _RDMADCTL HX8357_RDMADCTL
#endif

#ifdef __Adafruit_STMPE610
    #include <Adafruit_ILI9341.h>
    typedef Adafruit_ILI9341 tftType;
    #define _WHITE ILI9341_WHITE
    #define _BLACK ILI9341_BLACK
    #define _BLUE ILI9341_BLUE
    #define _GREEN ILI9341_GREEN
    #define _RED ILI9341_RED
    #define _YELLOW ILI9341_YELLOW
    #define _RDMODE ILI9341_RDMODE
    #define _RDMADCTL ILI9341_RDMADCTL
#endif

#include <Adafruit_STMPE610.h>