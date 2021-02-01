//
//
//  BFR_tft_windows.hpp
//
//

// this librairy enable you to custom windows on a tft display
// you can create windows, manage them
// inside windows, you can write text ou draw lines

// uncomment only one of the following define corresponding to the type of tft used by the project
// if you want to use another type of tft create a new ifdef/endif bloc with the sames definitions correspondances
#define __Adafruit_HXHX8357     //ADAfruit featherwing 3,5" 480x320
//#define __Adafruit_ILI9341     //ADAfruit featherwing 2,8" 240x320

#ifdef __Adafruit_HXHX8357      
    #include <Adafruit_HX8357.h>
    typedef Adafruit_HX8357 tftType;
    #define TFT_CS  15
    #define TFT_DC  33
    #define SD_CS    14
    #define STMPE_CS 32
    #define _WHITE HX8357_WHITE
    #define _BLACK HX8357_BLACK
    #define _BLUE HX8357_BLUE
    #define _GREEN HX8357_GREEN
    #define _RED HX8357_RED
    #define _YELLOW HX8357_YELLOW
    #define _RDMODE HX8357_RDPOWMODE
    #define _RDMADCTL HX8357_RDMADCTL
#endif

#ifdef __Adafruit_ILI9341
    #include <Adafruit_ILI9341.h>
    typedef Adafruit_ILI9341 tftType;
    #define TFT_CS 9
    #define TFT_DC 10
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

#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include <Wire.h>      // this is needed even tho we aren't using it


#ifndef __BFR_TFT_WINDOWS_HPP
    #define __BFR_TFT_WINDOWS_HPP

    //--------------------------------------------
    //      
    //      BFR_Window Class
    //      
    //--------------------------------------------

    #define NB_LIGNE_TEXTE      10
    #define NB_MAX_VARIABLES    10
    #define MAX_TEXT_SIZE       50

    struct dataText {
        char text[MAX_TEXT_SIZE];
        int posX;
        int posY;
        int size;
    };

    struct dataVariable {
        int *variable;
        int posX;
        int posY;
        int size;
    };

    class BFR_Window{
        public:

            BFR_Window();
            ~BFR_Window();
            int getWindowType();
            void init(tftType *tft, int originX, int originY, int sizeX, int sizeY, int colorCadre, int colorBackground, int colorText, char *name);
            void refreshWindow();
            void setTextSize(int size);
            void addText(char *texte, int size, int ligne, int colonne);
            void addVariable(int *ptrVar, int size, int ligne, int colonne);
            void showCaracteristique();
            void activateRefresh();
            void showWindows();
            void rotate();
            char *getName();
            void moveWindow(int newOriginX, int newOriginY);
            void reduceWindow();
            void restoreWindow();

        private:
            char name[30];
            int originX;
            int originY;
            int sizeX;
            int sizeY;
            boolean displaywindowBorder;
            int colorwindow;
            int colorBackground;
            int textSize;
            int colorText;
            tftType *tft;
            boolean refresh;
            boolean visibility;
            dataText tblText[NB_LIGNE_TEXTE];
            int nbLigneText;
            dataVariable tblVariables[NB_MAX_VARIABLES];
            int nbLigneVar;

            void printTexte(char *texte, int size, int ligne, int colonne);
            void printLignes();
            void printVar(int *ptrVar, int size, int ligne, int colonne);
            void printVariables();
            void drawLine(int x0, int y0, int x1, int y1);
            void displayCourbes();
            void fillBackground();
            void showWindow();

    };

    //--------------------------------------------
    //      
    //      BFR_tft_windows_manager Class
    //      
    //--------------------------------------------

    // This is calibration data for the raw touch data to the screen coordinates
    #define TS_MINX 150
    #define TS_MINY 130
    #define TS_MAXX 3800
    #define TS_MAXY 4000

/*
    // The STMPE610 uses hardware SPI on the shield, and #8
    #define STMPE_CS 32
    // The display also uses hardware SPI, plus #9 & #10
    #define TFT_CS   15
    #define TFT_DC   33
    #define SD_CS    14
*/

    // Size of the color selection boxes and the paintbrush size
    #define BOXSIZE 40
    #define PENRADIUS 3


    #define LOOP_REFRESH_POINTS     1000

    // definition of rotation
    // 0 : usb on top
    // 1 : usb on left
    // 2 : usb on bottom
    // 3 : usb on right
    #define TFT_ROTATION    3
    #define NB_MAX_WINDOWS  10
    class BFR_tft_windows_manager
    {
        public:
        BFR_tft_windows_manager(void);
        ~BFR_tft_windows_manager();
        void init();
        void forceRefresh();
        void refresh();
        void rotate(void);
        BFR_Window newWindow(int pos_x, int pos_y, int size_x, int size_y, int color_window, int color_background, int color_text, char *windoww_name);
        int getSizeX();
        int getSizeY();
        void DisplayWindowsList();


        private:
        int tailleEcranX;   // horizontal
        int tailleEcranY;   // vertical
        uint8_t rotation;
        BFR_Window *lstWindowsPtr[NB_MAX_WINDOWS];
        tftType tft = tftType(TFT_CS, TFT_DC);
        Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);
        void addWindow(BFR_Window *newWindow);
    };



#endif

