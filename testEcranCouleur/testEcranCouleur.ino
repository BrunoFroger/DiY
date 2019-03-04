//
//
//  testEcranCouleur.ino
//
//


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
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 9
#define TFT_DC 10
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 3


#define LOOP_DELAY 10
#define LOOP_REFRESH_POINTS     1000
#define NB_STORED_VALUES    200
#define _WHITE ILI9341_WHITE
#define _BLACK ILI9341_BLACK
#define _BLUE ILI9341_BLUE
#define _GREEN ILI9341_GREEN
#define _RED ILI9341_RED
#define _YELLOW ILI9341_YELLOW

int nbColor = 6;
int color[] = {_WHITE, _YELLOW, _BLACK, _RED, _GREEN, _BLUE};
char textColor[][20] = {"blanc", "jaune", "noir", "rouge", "vert", "bleu"};

int nbItemMenu = 2;
char itemMenu[][20] = {"parametres", "set "};

boolean refreshTitle = true;
boolean refreshMenu = true;
boolean refreshCadreCourbes = true;
boolean refreshMesures = true;
boolean refreshAxes = true;
boolean refreshPoints = true;
int lastRefreshPoints = millis();

char texte[100];

int tblValues[NB_STORED_VALUES];

//=================================================
//
//      displayTitle
//
//=================================================
void displayTitle(){
    if (refreshTitle){
        Serial.println("displayTitle");
        tft.setCursor(15,0);
        tft.setTextColor(_RED);
        tft.setTextSize(3); 
        tft.println("OSCILLOSCOPE");
        refreshTitle = false;
    }
}

//=================================================
//
//      displayMenu
//
//=================================================
void displayMenu(){
    if (refreshMenu){
        Serial.println("displayMenu");
        int colorCadre = _YELLOW;
        int originX = 5;
        int originY = (tft.height() / 2) + 40;
        int tailleX = tft.width() / 2 - 15;
        int tailleY = 100;
        sprintf(texte, "orig X=%d, orig Y=%d, taille X=%d, taille Y=%d", originX, originY, tailleX, tailleY);
        Serial.println(texte);
        tft.drawRect(originX, originY, tailleX, tailleY, colorCadre);
        refreshMenu = false;

        // display text
        int numLigne = 1;
        tft.setCursor(originX + 10,originY + (10 * numLigne++));
        tft.setTextSize(1); 
        tft.println("Parametres");
        tft.setCursor(originX + 10,originY + (10 * numLigne++));
        tft.println("Axe X : ");
        tft.setCursor(originX + 10,originY + (10 * numLigne++));
        tft.println("Axe Y : ");
    }
}

//=================================================
//
//      displayMesures
//
//=================================================
void displayMesures(){
    if (refreshMesures){
        Serial.println("displayMesures");
        int colorCadre = _WHITE;
        int originX = tft.width() / 2 + 10;;
        int originY = (tft.height() / 2) + 40;
        int tailleX = tft.width() / 2 - 15;
        int tailleY = 100;
        sprintf(texte, "orig X=%d, orig Y=%d, taille X=%d, taille Y=%d", originX, originY, tailleX, tailleY);
        Serial.println(texte);
        tft.drawRect(originX, originY, tailleX, tailleY, colorCadre);
        refreshMesures = false;

        // display text
        int numLigne = 1;
        tft.setCursor(originX + 10,originY + (10 * numLigne++));
        tft.setTextSize(1); 
        tft.println("Mesures");
        tft.setCursor(originX + 10,originY + (10 * numLigne++));
        tft.println("Axe X : ");
        tft.setCursor(originX + 10,originY + (10 * numLigne++));
        tft.println("Axe Y : ");
    }
}

//=================================================
//
//      displayCourbes
//
//=================================================
void displayCourbes(){
    if (refreshCadreCourbes){
        Serial.println("displayCourbes");
        int colorCadre = _GREEN;
        int originX = 5;
        int originY = 30;
        int tailleX = tft.width() - 10;
        int tailleY = tft.height() / 2;
        sprintf(texte, "orig X=%d, orig Y=%d, taille X=%d, taille Y=%d", originX, originY, tailleX, tailleY);
        Serial.println(texte);
        tft.drawRect(originX, originY, tailleX, tailleY, colorCadre);

        refreshCadreCourbes = false;
    }

    // display Axes
    if (refreshAxes){
        Serial.println("displayAxes");
        int colorAxes = _RED;
        int originX = 10;
        int originY = 35;
        int lengthX = 150;
        int lengthY = 220;
        //int finY = tft.height() / 2 - 15;
        sprintf(texte, "orig X=%d, orig Y=%d, lengthX=%d, lengthY=%d", originX, originY, lengthX, lengthY);
        Serial.println(texte);
        tft.drawFastVLine(originX, originY, lengthX, colorAxes);
        tft.drawFastHLine(originX, originY + lengthX, lengthY, colorAxes);
        refreshAxes = false;
    }

    // display points
    if (refreshPoints){
        int originX = 15;
        int originY = 40;
        int lengthX = 145;
        int lengthY = 215;
        int colorPoints = _BLUE;
        Serial.println("displayPoints");
        tft.fillRect(originX, originY, lengthY, lengthX, _BLACK);
        for (int i=1 ; i<NB_STORED_VALUES ; i++){
            int x0 = originX + i-1;
            int y0 = originY + lengthX - tblValues[i-1];
            int x1 = originX + i;
            int y1 = originY + lengthX - tblValues[i];
            tft.drawLine(x0,y0,x1,y1,colorPoints);
        }

        refreshPoints = false;
    }
}

//=================================================
//
//      initTft
//
//=================================================
void initTft(){
    Serial.println("Touch Paint!");

    tft.begin();

    // read diagnostics (optional but can help debug problems)
    uint8_t x = tft.readcommand8(ILI9341_RDMODE);
    Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(ILI9341_RDMADCTL);
    Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(ILI9341_RDPIXFMT);
    Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(ILI9341_RDIMGFMT);
    Serial.print("Image Format: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(ILI9341_RDSELFDIAG);
    Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 

    
    if (!ts.begin()) {
        Serial.println("Couldn't start touchscreen controller");
    }else{
        Serial.println("Touchscreen started");
    }
    
}

//=================================================
//
//      setup
//
//=================================================
void setup() {        
    // initialize serial communication
    Serial.begin(38400);
    int timeoutInitSerial = 100;
    while (timeoutInitSerial-- > 0)
    {
        if (Serial)
            break;
        delay(10);
    }
    Serial.println("Serial initialized");

    initTft();

    tft.setRotation(2);
    tft.fillScreen(ILI9341_BLACK);

    for (int i=0 ; i<NB_STORED_VALUES ; i++ ){
        tblValues[i] = 50 + (i%50);
    }

    Serial.println("end of setup");
  
}
 

//=================================================
//
//      loop
//
//=================================================
void loop(){ 
    //Serial.println("boucle principale");

    displayTitle();
    displayCourbes();
    displayMenu();
    displayMesures();

    if ((millis() - lastRefreshPoints) > LOOP_REFRESH_POINTS ){
        refreshPoints=true;
        lastRefreshPoints = millis();
    }

    delay(LOOP_DELAY);

}