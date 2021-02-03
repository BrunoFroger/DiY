// chauffage
// display.cpp
// (c) B. Froger 2020

#include <Arduino.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include <Wire.h>      // this is needed even tho we aren't using it
#include <Adafruit_HX8357.h>
#include <Adafruit_STMPE610.h>


#include "chauffage.hpp"
#include "api.hpp"
#include "wifiTools.hpp"

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

// The STMPE610 uses hardware SPI on the shield, and #8
#define STMPE_CS 32
//Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS   15
#define TFT_DC   33
#define SD_CS    14

#define TFT_RST -1

Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 3


#define LOOP_REFRESH_POINTS     1000
#define NB_STORED_VALUES    200
#define _WHITE HX8357_WHITE
#define _BLACK HX8357_BLACK
#define _BLUE HX8357_BLUE
#define _GREEN HX8357_GREEN
#define _RED HX8357_RED
#define _YELLOW HX8357_YELLOW

// orientation de l'ecran
// 2  portrait (prise en bas)
// 0  portrait (prise en haut)
// 1  paysage (prise a gauche)
// 3  paysage (prise a gauche)
#define SCREEN_ORIENTATION 1

int nbColor = 6;
int color[] = {_WHITE, _YELLOW, _BLACK, _RED, _GREEN, _BLUE};
char textColor[][20] = {"blanc", "jaune", "noir", "rouge", "vert", "bleu"};

int nbItemMenu = 2;
char itemMenu[][20] = {"parametres", "set "};

boolean refreshTitle = true;
boolean refreshMesures = true;

char texte[100];

int tblValues[NB_STORED_VALUES];

int couleurTexte = _WHITE;
int couleurFond = _BLACK;
char oldHeure[50];
char oldDate[50];
char oldConsigne[50];
char oldChauffageOnOff[50];
char oldWifiConnected[50];
char oldTemperatureMesuree[50];

//=================================================
//
//      displayTitle
//
//=================================================
void displayTitle(char *title){
    if (refreshTitle){
        Serial.print("displayTitle : ");
        Serial.println(title);
        tft.setCursor(15,0);
        tft.setTextColor(_RED);
        tft.setTextSize(3); 
        tft.println(title);
        refreshTitle = false;
    }
}

//=================================================
//
//      displayMenu
//
//=================================================
void displayParametres(){
    if (mesDonneesApi.parametresModifies){
        mesDonneesApi.parametresModifies = false;
        //Serial.println("displayParametres");
        int colorCadre = _YELLOW;
        int originX = 5;
        int originY = (tft.height() / 2) + 40;
        int tailleX = tft.width() / 2 - 15;
        int tailleY = 100;
        //sprintf(texte, "orig X=%d, orig Y=%d, taille X=%d, taille Y=%d", originX, originY, tailleX, tailleY);
        //Serial.println(texte);
        tft.drawRect(originX, originY, tailleX, tailleY, colorCadre);

        // display text
        int numLigne = 1;
        int textSize = 2;
        char tmp[50];

        originX = originX + 10;
        originY = originY + (10 * numLigne++);
        tft.setCursor(originX, originY);
        tft.setTextSize(textSize); 
        tft.println("Parametres");
        
        originY = originY + (textSize * 10);
        tft.setCursor(originX, originY);
        tft.print("Consigne : ");
        tft.setTextColor(_BLACK);
        tft.println(oldConsigne);
        tft.setTextColor(_WHITE);
        sprintf(tmp,"Consigne : %d", mesDonneesApi.consigne);
        tft.setCursor(originX, originY);
        tft.print(tmp);
        strcpy(oldConsigne, tmp);

        originY = originY + (textSize * 10);
        tft.setCursor(originX, originY);
        tft.print("Chauffage: ");
        tft.setTextColor(_BLACK);
        tft.println(oldChauffageOnOff);
        if (mesDonneesApi.chauffageOnOff){
            sprintf(tmp, "Chauffage: ON");
        } else {
            sprintf(tmp, "Chauffage: OFF");
        }
        tft.setTextColor(_WHITE);
        tft.setCursor(originX, originY);
        tft.println(tmp);
        strcpy(oldChauffageOnOff,tmp);

        originY = originY + (textSize * 10);
        tft.setCursor(originX, originY);
        tft.setTextColor(_BLACK);
        tft.println(oldWifiConnected);
        if (mesDonneesApi.WifiConnected){
            sprintf(tmp, "Wifi     : Oui");
        } else {
            sprintf(tmp, "Wifi     : Non");
        }
        tft.setTextColor(_WHITE);
        tft.setCursor(originX, originY);
        tft.println(tmp);
        strcpy(oldWifiConnected,tmp);
    }
}

//=================================================
//
//      displayMesures
//
//=================================================
void displayMesures(){
    if (refreshMesures){
        //Serial.println("displayMesures");
        int colorCadre = _WHITE;
        int originX = tft.width() / 2 + 10;;
        int originY = (tft.height() / 2) + 40;
        int tailleX = tft.width() / 2 - 15;
        int tailleY = 100;
        //sprintf(texte, "orig X=%d, orig Y=%d, taille X=%d, taille Y=%d", originX, originY, tailleX, tailleY);
        //Serial.println(texte);
        tft.drawRect(originX, originY, tailleX, tailleY, colorCadre);
        refreshMesures = false;

        // display text
        int numLigne = 1;
        int textSize = 2;
        tft.setCursor(originX + 10,originY + (10 * numLigne++));
        tft.setTextSize(textSize); 
        tft.println("Mesures");
        tft.setCursor(originX + 10,originY + (textSize * 10 * numLigne++));
        tft.println("Temp   : ");
        tft.setCursor(originX + 10,originY + (textSize * 10 * numLigne++));
        tft.println("Puiss  : ");
    }
}

//=================================================
//
//      displayValeurs
//
//=================================================
void displayValeurs(){
    bool execModule = mesDonneesApi.heureModifiee || 
                    mesDonneesApi.dateModifiee ||
                    mesDonneesApi.temperatureMesureeModifiee;

    if (execModule){
        //Serial.println("displayValeurs");
        int colorCadre = _GREEN;
        int originX = 5;
        int originY = 30;
        int tailleX = tft.width() - 10;
        int tailleY = tft.height() / 2;
        //sprintf(texte, "orig X=%d, orig Y=%d, taille X=%d, taille Y=%d", originX, originY, tailleX, tailleY);
        //Serial.println(texte);
        tft.drawRect(originX, originY, tailleX, tailleY, colorCadre);
        //tft.fillRect(originX+1, originY+1, tailleX-1, tailleY-1, couleurFond);

        // display text
        int numLigne = 1;
        int tmpX = originX + 10;
        int tmpY = originY + (10 * numLigne++);

        int textSize = 4;
        tft.setTextSize(textSize); 
        tft.setCursor(tmpX,tmpY);
        if (mesDonneesApi.heureModifiee){
            //Serial.println("display.cpp => displayValeurs : display heure");
            tft.setTextColor(_BLACK);
            tft.print(oldHeure);
            tft.setTextColor(_WHITE);
            tft.setCursor(tmpX,tmpY);
            strcpy(oldHeure,mesDonneesApi.heureFormatee);
            tft.println(oldHeure); 
            mesDonneesApi.heureModifiee = false;
        }

        tmpY = tmpY + ((textSize / 2) *  10 * numLigne++);
        textSize = 3;
        tft.setTextSize(textSize); 
        tft.setCursor(tmpX,tmpY);
        if (mesDonneesApi.dateModifiee){
            //Serial.println("display.cpp => displayValeurs : display date");
            tft.setTextColor(_BLACK);
            tft.print(oldDate);
            tft.setTextColor(_WHITE);
            tft.setCursor(tmpX,tmpY);
            strcpy(oldDate,mesDonneesApi.dateFormatee);
            tft.println(oldDate);
            mesDonneesApi.dateModifiee = false;
        }

        tmpY = tmpY + ((textSize / 2) *  10 * numLigne++);
        textSize = 2;
        tft.setTextSize(textSize); 
        tft.setCursor(tmpX,tmpY);
        tft.println("taux d'humidite   : ");


        tmpX = originX + 280;
        tmpY = originY + 38;
        textSize = 8;
        char tmp[50];
        tft.setTextSize(textSize); 
        tft.setCursor(tmpX,tmpY);
        if (mesDonneesApi.temperatureMesureeModifiee){
            //Serial.println("display.cpp => displayValeurs : temperature mesuree");
            tft.setTextColor(_BLACK);
            tft.print(oldTemperatureMesuree);
            tft.setTextColor(_WHITE);
            tft.setCursor(tmpX,tmpY);
            int temperature = mesDonneesApi.temperatureMesuree;
            sprintf(tmp, "%2d.%d", temperature/10, temperature%10);
            strcpy(oldTemperatureMesuree,tmp);
            tft.println(oldTemperatureMesuree);
            mesDonneesApi.temperatureMesureeModifiee = false;
        }

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

    // read diagnostics (optional but can help debug problems)
    uint8_t x = tft.readcommand8(HX8357_RDPOWMODE);
    Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(HX8357_RDMADCTL);
    Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(HX8357_RDCOLMOD);
    Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(HX8357_RDDIM);
    Serial.print("Image Format: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(HX8357_RDDSDR);
    Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
    
    Serial.println(F("Benchmark                Time (microseconds)"));
    
    tft.setRotation(SCREEN_ORIENTATION);
    tft.fillScreen(HX8357_BLACK);
    
    /*
    if (!ts.begin()) {
        Serial.println("Couldn't start touchscreen controller");
    }else{
        Serial.println("Touchscreen started");
    }
    */

    couleurTexte = _WHITE;
    tft.setTextColor(couleurTexte);
    couleurFond = _BLACK;
    //tft.setBackgroundColor(couleurFond);

    strcpy(oldHeure,"");
    strcpy(oldDate,"");

    strcpy(oldHeure, "");
    strcpy(oldDate, "");
    strcpy(oldChauffageOnOff, "");
    strcpy(oldWifiConnected, "");
    
}




//=================================================
//
//      refreshDisplay
//
//=================================================
void refreshDisplay(void){
    if (isDatasAvailables()){
        resetDataAvailables();
        displayTitle("Chauffage");
        displayValeurs();
        displayParametres();
        displayMesures();
    }
}
