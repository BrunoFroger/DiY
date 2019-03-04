//
//
//  displayCadre.hpp
//
//

#include <arduino.h>
#include <Adafruit_ILI9341.h>

#ifndef __DISPLAYCADRE_HPP
#define __DISPLAYCADRE_HPP

#define CADRE_MENU    1
#define CADRE_TITRE    2
#define CADRE_TABLEAU   3
#define CADRE_MESURES_TEXTE 4
#define CADRE_MESURES_GRAPH 5
#define NB_LIGNE_TEXTE  10
#define NB_VARIABLES    10

struct dataText {
    char text[25];
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

class DisplayCadreClass{
    public:
        DisplayCadreClass();
        ~DisplayCadreClass();
        char name[30];
        int getCadreType();
        void init(Adafruit_ILI9341 *tft, int originX, int originY, int sizeX, int sizeY, int colorCadre, int colorBackground, int colorText, int typeCadre, char *name);
        void refreshCadre();
        void setTextSize(int size);
        void addText(char *texte, int size, int ligne, int colonne);
        void addVariable(int *ptrVar, int size, int ligne, int colonne);
        void showCaracteristique(int index);
        void activateRefresh();

    private:
        int originX;
        int originY;
        int sizeX;
        int sizeY;
        boolean displayCadreBorder;
        int colorCadre;
        int colorBackground;
        int textSize;
        int colorText;
        Adafruit_ILI9341 *tft;
        boolean refresh;
        dataText tblText[NB_LIGNE_TEXTE];
        int nbLigneText;
        dataVariable tblVariables[NB_VARIABLES];
        int nbLigneVar;
        int cadreType;

        void printTexte(char *texte, int size, int ligne, int colonne);
        void printLignes();
        void printVar(int *ptrVar, int size, int ligne, int colonne);
        void printVariables();
        void drawLine(int x0, int y0, int x1, int y1);
        void displayCourbes();
        void fillBackground();

        void displayCadre();

};

#endif