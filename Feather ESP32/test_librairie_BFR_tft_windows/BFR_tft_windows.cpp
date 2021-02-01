//
//
//  BFR_tft_windows_manager.cpp
//
//

#include <Arduino.h>
#include "BFR_tft_windows.hpp"

boolean refreshTitle = true;
boolean refreshMenu = true;
boolean refreshCadreCourbes = true;
boolean refreshMesures = true;
boolean refreshAxes = true;
boolean refreshPoints = true;
int lastRefreshPoints = millis();

char texte[100];

int nbColor = 6;
int color[] = {_WHITE, _YELLOW, _BLACK, _RED, _GREEN, _BLUE};
char textColor[][20] = {"blanc", "jaune", "noir", "rouge", "vert", "bleu"};


//*********************************************
//*
//*       Constructor
//*
//*********************************************
BFR_tft_windows_manager::BFR_tft_windows_manager()
{
    Serial.println("create Windows manager");
}

//*********************************************
//*
//*       Destructor
//*
//*********************************************
BFR_tft_windows_manager::~BFR_tft_windows_manager()
{
    Serial.println("destroy windows Manager");
}

//=================================================
//
//      initTft
//
//=================================================
void BFR_tft_windows_manager::init(){
    Serial.println("BFR_tft_windows_manager : init ---------- debut -------------");

    tft.begin();

    // read diagnostics (optional but can help debug problems)
    uint8_t x = tft.readcommand8(_RDMODE);
    Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
    x = tft.readcommand8(_RDMADCTL);
    Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);

    delay(1000);
    
    if (!ts.begin()) {
        Serial.println("Couldn't start touchscreen controller");
    }else{
        Serial.println("Touchscreen started");
    }

    tft.fillScreen(_BLACK);
    this->rotation=TFT_ROTATION;
    tft.setRotation(this->rotation);
    Serial.println("BFR_tft_windows_manager : init ---------- fin -------------");

    
    for (int i = 0 ; i < NB_MAX_WINDOWS ; i++){
        lstWindowsPtr[i] = NULL;
    }
    
}

//=================================================
//
//      newWindow
//
//=================================================
BFR_Window BFR_tft_windows_manager::newWindow(int pos_x, int pos_y, int size_x, int size_y, int color_window, int color_background, int color_text, char *windoww_name){
    Serial.println("BFR_tft_windows_manager : newWindow ---------- debut -------------");
    // TODO creer la fenetre et la stocker dans la liste
    BFR_Window ptrWindow;
    ptrWindow.init(&tft, pos_x, pos_y, size_x, size_y, color_window, color_background, color_text, windoww_name);
    addWindow(&ptrWindow);
    Serial.println("BFR_tft_windows_manager : newWindow ---------- fin -------------");
    return ptrWindow;
}

//=================================================
//
//      forceRefresh
//
//=================================================
void BFR_tft_windows_manager::forceRefresh(){
    //Serial.println("BFR_tft_windows_manager : forceRefresh ---------- debut -------------");
    int index = 0;
    tft.fillScreen(_BLACK);
    while (this->lstWindowsPtr[index] != NULL){
        lstWindowsPtr[index]->showWindows();
        lstWindowsPtr[index]->activateRefresh();
        index++;
    }
    //Serial.println("BFR_tft_windows_manager : forceRefresh ---------- fin -------------");
}

//=================================================
//
//      refresh
//
//=================================================
void BFR_tft_windows_manager::refresh(){
    //Serial.println("BFR_tft_windows_manager : refresh ---------- debut -------------");
    int index = 0;
    while (this->lstWindowsPtr[index] != NULL){
        lstWindowsPtr[index]->refreshWindow();
        index++;
    }
    //Serial.println("BFR_tft_windows_manager : refresh ---------- fin -------------");
}

//=================================================
//
//      rotate
//
//=================================================
void BFR_tft_windows_manager::rotate(){
    Serial.println("BFR_tft_windows_manager : rotate ---------- debut -------------");
    this->rotation = (this->rotation + 1) % 4;
    Serial.print("rotate value : ");
    Serial.println(this->rotation);
    tft.setRotation(this->rotation);
    //if (this->rotation == 2 || this->rotation == 0){
        //mode portrait
        this->tailleEcranX=tft.width();
        this->tailleEcranY=tft.height();
    //}else{
        //mode paysage
        //this->tailleEcranY=tft.width();
        //this->tailleEcranX=tft.height();
    //}
    Serial.print("taille X = ");
    Serial.println(this->tailleEcranX);
    Serial.print("taille Y = ");
    Serial.println(this->tailleEcranY);
    Serial.println("BFR_tft_windows_manager : rotate ---------- fin -------------");
}

//=================================================
//
//      addWindow
//
//=================================================
void BFR_tft_windows_manager::addWindow(BFR_Window *newWindow){
    Serial.println("BFR_tft_windows_manager : addWindow ---------- debut -------------");
    int index = 0;
    while (lstWindowsPtr[index] != NULL){
        index++;
        if (index > NB_MAX_WINDOWS){
            Serial.println("ERREUR : too much windows created");
            Serial.println("BFR_tft_windows_manager : addWindow ---------- fin -------------");
            return;
        }
    }
    Serial.print("Windows <");
    Serial.print(newWindow->getName());
    Serial.print("> created at index ");
    Serial.print(index);
    Serial.println();
    lstWindowsPtr[index] = newWindow;
    Serial.println("BFR_tft_windows_manager : addWindow ---------- fin -------------");
}

/*
//=================================================
//
//      getTft
//
//=================================================
tftType *BFR_tft_windows_manager::getTft(){
    return &this->tft;
}*/

//=================================================
//
//      getSizeX
//
//=================================================
int BFR_tft_windows_manager::getSizeX(){
    return this->tailleEcranX;
}

//=================================================
//
//      getSizeY
//
//=================================================
int BFR_tft_windows_manager::getSizeY(){
    return this->tailleEcranY;
}

//=================================================
//
//      afficheListeCadres
//
//=================================================
void BFR_tft_windows_manager::DisplayWindowsList(){
    //Serial.println("tftDisplay Refresh()");
    int index = 0;
    Serial.println("BFR_tft_windows_manager : DisplayWindowsList ---------- debut -------------");
    BFR_Window *tmp;
    while (index < NB_MAX_WINDOWS){
        tmp = this->lstWindowsPtr[index];
        if (tmp == NULL){
            Serial.println("BFR_tft_windows_manager : DisplayWindowsList ---------- fin -------------");
            return;
        }
        tmp->showCaracteristique();
        index++;
    }
    Serial.println("BFR_tft_windows_manager : DisplayWindowsList ---------- fin -------------");
}
