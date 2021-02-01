//
//
//  TftDisplay.cpp
//
//


#include "TftDisplay.hpp"
#include "displayCadre.hpp"

int nbItemMenu = 2;
char itemMenu[][20] = {"parametres", "set "};

boolean refreshTitle = true;
boolean refreshMenu = true;
boolean refreshCadreCourbes = true;
boolean refreshMesures = true;
boolean refreshAxes = true;
boolean refreshPoints = true;
int lastRefreshPoints = millis();
int cptMesures=0;

char texte[100];

int nbColor = 6;
int color[] = {_WHITE, _YELLOW, _BLACK, _RED, _GREEN, _BLUE};
char textColor[][20] = {"blanc", "jaune", "noir", "rouge", "vert", "bleu"};

//*********************************************
//*
//*       Constructor
//*
//*********************************************
TftDisplayClass::TftDisplayClass()
{

}

//*********************************************
//*
//*       Destructor
//*
//*********************************************
TftDisplayClass::~TftDisplayClass()
{
}

//=================================================
//
//      initTft
//
//=================================================
void TftDisplayClass::initTft(){
    Serial.println("init TFT");

    tft.begin();

    // read diagnostics (optional but can help debug problems)
    uint8_t x = tft.readcommand8(_RDMODE);
    Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(_RDMADCTL);
    Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);

    delay(1000);
    /*
    if (!ts.begin()) {
        Serial.println("Couldn't start touchscreen controller");
    }else{
        Serial.println("Touchscreen started");
    }*/

    tft.fillScreen(_BLACK);
    tft.setRotation(TFT_ROTATION);
    if (TFT_ROTATION == 2 || TFT_ROTATION == 0){
        //mode portrait
        this->tailleEcranX=tft.width();
        this->tailleEcranY=tft.height();
    }else{
        //mode portrait
        this->tailleEcranY=tft.width();
        this->tailleEcranX=tft.height();
    }
    Serial.print("taille X = ");
    Serial.println(tailleEcranX);
    Serial.print("taille Y = ");
    Serial.println(tailleEcranY);
    Serial.println("TFT display Init : fin");

    /*
    for (int i = 0 ; i < NB_MAX_CADRES ; i++){
        lstCadresPtr[index] = NULL;
    }
    */
}

//=================================================
//
//      refresh
//
//=================================================
void TftDisplayClass::refresh(){
    //Serial.println("TftDisplayClass : refresh ------------------------------");
    int index = 0;
    while (this->lstCadresPtr[index] != NULL){
        lstCadresPtr[index]->refreshCadre();
        index++;
    }
}

//=================================================
//
//      addCadre
//
//=================================================
void TftDisplayClass::addCadre(DisplayCadreClass *cadrePtr){

    int index = 0;
    while (lstCadresPtr[index] != NULL){
        index++;
        if (index > NB_MAX_CADRES){
            Serial.println("ERREUR : trop de cadres créés");
            return;
        }
    }
    lstCadresPtr[index] = cadrePtr;
}

//=================================================
//
//      getTft
//
//=================================================
tftType *TftDisplayClass::getTft(){
    return &this->tft;
}

//=================================================
//
//      getSizeX
//
//=================================================
int TftDisplayClass::getSizeX(){
    return this->tailleEcranX;
}

//=================================================
//
//      getSizeY
//
//=================================================
int TftDisplayClass::getSizeY(){
    return this->tailleEcranY;
}

//=================================================
//
//      afficheListeCadres
//
//=================================================
void TftDisplayClass::afficheListeCadres(){
    //Serial.println("tftDisplay Refresh()");
    int index = 0;
    Serial.println("TftDisplayClass : afficheListeCadres ------------------------------");
    DisplayCadreClass *tmp;
    while (index < NB_MAX_CADRES){
        tmp = this->lstCadresPtr[index];
        if (tmp == NULL){
            return;
        }
        tmp->showCaracteristique(index);
        index++;
    }
}
