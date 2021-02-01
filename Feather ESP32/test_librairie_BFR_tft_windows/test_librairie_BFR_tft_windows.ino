//
//
//  FeatherOscilloscope.ino
//
//

#include <Arduino.h>
#include "BFR_tft_windows.hpp"
#include "monitor.hpp"

#define OSCILLATEUR_INPUT   10
#define LOOP_DELAY 1
#define REFRESH_MESURE_TEXTE    100

BFR_tft_windows_manager tftDisplay;

/*
DisplayCadreClass cadreTitre = DisplayCadreClass();
DisplayCadreClass cadreMenu = DisplayCadreClass();
DisplayCadreClass cadreMesure = DisplayCadreClass();
DisplayCadreClass cadreCourbes = DisplayCadreClass();
DisplayCadreClass cadreCourbes1 = DisplayCadreClass();

int valeurMesure;
int echelleAxeX = 10;
int echelleAxeY = 20;
int loopDisplayMesureTexte;
*/

//=================================================
//
//      setup
//
//=================================================
void setup() {        
    // initialize serial communication
    Serial.begin(115200);
    int timeoutInitSerial = 100;
    while (timeoutInitSerial-- > 0)
    {
        if (Serial)
            break;
        delay(10);
    }
    delay(1000);
    Serial.println("Serial initialized");

    Serial.println("=========================");
    Serial.println("|    begin of setup     |");
    Serial.println("=========================");
    tftDisplay.init();

    //int tailleEcranX = tftDisplay.getSizeX();
    //int tailleEcranY = tftDisplay.getSizeY();

    //------------------------------------
    // creation des differents cadres
    //------------------------------------

    //------------------------------------
    //   init cadre titre
    //------------------------------------
    BFR_Window windowTitle = tftDisplay.newWindow(0, 0, 300, 100, _RED, _BLACK, _WHITE, "test Bruno");
    windowTitle.addText("titre", 2, 0, 3);
    windowTitle.addText("Test 1", 2, 1, 3);
    windowTitle.showWindows();
    BFR_Window windowFenetre1 = tftDisplay.newWindow(0, 150, 200, 100, _YELLOW, _BLACK, _WHITE, "fenetre1");
    windowFenetre1.addText("fenetre 1", 2, 0, 3);
    windowFenetre1.addText("Test 2 tres long 123456", 2, 1, 3);
    windowFenetre1.showWindows();
    tftDisplay.DisplayWindowsList();

/*
    //------------------------------------
    //   init cadre affichage des infos 
    //------------------------------------
    cadreMenu.init(tft, 0, 234, (tailleEcranX / 2) - 2, 60, _RED, _BLACK, _WHITE, CADRE_MENU, "Menu");
    cadreMenu.addText("Parametres", 1, 0, 5);
    cadreMenu.addText("Axe X (ms) : ", 1, 1, 0);
    cadreMenu.addVariable(&echelleAxeX, 1, 1, 14);
    cadreMenu.addText("Axe Y (mV) : ", 1, 2, 0);
    cadreMenu.addVariable(&echelleAxeY, 1, 2, 14);
    tftDisplay.addCadre(&cadreMenu);

    //------------------------------------
    //   init cadre affichage des mesures 
    //------------------------------------
    cadreMesure.init(tft,(tailleEcranX / 2) + 2, 234, (tailleEcranX / 2) - 2, 60, _RED, _BLACK, _WHITE, CADRE_MESURES_TEXTE, "Mesures");
    cadreMesure.addText("Mesure", 1, 0, 5);
    cadreMesure.addText("Voie 1 : ", 1, 1, 0);
    cadreMesure.addVariable(&valeurMesure, 1, 1, 9);
    tftDisplay.addCadre(&cadreMesure);

    //------------------------------------
    //   init cadre affichage des signaux
    //------------------------------------
    cadreCourbes.init(tft, 0, 32, tailleEcranX, 200, _YELLOW, _BLUE, _RED, CADRE_MESURES_GRAPH, "Graphique");
    tftDisplay.addCadre(&cadreCourbes);
*/

    Serial.println("=========================");
    Serial.println("|    end of setup       |");
    Serial.println("=========================");

}

//=================================================
//
//      loop
//
//=================================================

void loop(){
    readKeybord();
    tftDisplay.refresh();

    delay(LOOP_DELAY);
}
