//
//
//  FeatherOscilloscope.ino
//
//

#include <Arduino.h>
#include "oscillateur.hpp"
#include "mesure.hpp"
#include "displayCadre.hpp"

#define OSCILLATEUR_INPUT   10
#define LOOP_DELAY 1
#define REFRESH_MESURE_TEXTE    100

TftDisplayClass tftDisplay;
OscillateurClass oscillateur; 
MesureClass mesure;
boolean oscillateurActif=false;

DisplayCadreClass cadreTitre = DisplayCadreClass();
DisplayCadreClass cadreMenu = DisplayCadreClass();
DisplayCadreClass cadreMesure = DisplayCadreClass();
DisplayCadreClass cadreCourbes = DisplayCadreClass();
DisplayCadreClass cadreCourbes1 = DisplayCadreClass();

int valeurMesure;
int echelleAxeX = 10;
int echelleAxeY = 20;
int loopDisplayMesureTexte;

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

    tftDisplay.initTft();
    if (oscillateurActif){
        oscillateur.init(OSCILLATEUR_INPUT);
    }
    mesure.init();

    Adafruit_ILI9341 *tft;
    tft = tftDisplay.getTft();
    int tailleEcranX = tftDisplay.getSizeX();
    int tailleEcranY = tftDisplay.getSizeY();

    //------------------------------------
    // creation des differents cadres
    //------------------------------------

    //------------------------------------
    //   init cadre titre
    //------------------------------------
    cadreTitre.init(tft, 0, 0, tailleEcranX, 30, _RED, _BLACK, _WHITE, CADRE_TITRE, "Titre");
    cadreTitre.addText("OSCILLOSCOPE", 2, 0, 3);
    tftDisplay.addCadre(&cadreTitre);

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

    Serial.println("=========================");
    Serial.println("     end of setup");
    Serial.println("=========================");
}

//=================================================
//
//      loop
//
//=================================================

void loop(){
    tftDisplay.refresh();

    loopDisplayMesureTexte += LOOP_DELAY;
    if (loopDisplayMesureTexte > REFRESH_MESURE_TEXTE){
        valeurMesure = mesure.getCurrentMesure();
        cadreMesure.activateRefresh();
        loopDisplayMesureTexte=0;
    }
    // provisoirement en abscence d'oscillateur, on declenche la mesure ici
    
    if (!oscillateurActif){
        mesure.fakeMesure();
    }
    delay(LOOP_DELAY);
}
