//
//
//  displayCadre.cpp
//
//

#include "displayCadre.hpp"
#include "TftDisplay.hpp"
#include <Adafruit_ILI9341.h>
#include "displayCadre.hpp"
#include "mesure.hpp"

extern MesureClass mesure;

//*********************************************
//*
//*       Constructor
//*
//*********************************************
DisplayCadreClass::DisplayCadreClass(){
}

//*********************************************
//*
//*       Destructor
//*
//*********************************************
DisplayCadreClass::~DisplayCadreClass()
{
}

//*********************************************
//*
//*       init
//*
//*********************************************
void DisplayCadreClass::init(tftType *tft, int originX, int originY, int sizeX, int sizeY, int colorCadre, int colorBackground, int colorText, int typeCadre, char *name){
    this->tft = tft;
    this->originX = originX;
    this->originY = originY;
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->colorCadre = colorCadre;
    this->colorBackground = colorBackground;
    this->colorText = colorText;
    this->refresh = true;
    this->cadreType = typeCadre;
    strcpy(this->name,name); 

    // init tableau des chaine de texte
    this->nbLigneText=0;
    for (int i = 0 ; i < NB_LIGNE_TEXTE ; i++){
        strcpy(this->tblText[i].text,"");
        this->tblText[i].posX = 0;
        this->tblText[i].posY = 0;
        this->tblText[i].size = 0;
    }
    // init tableau des variables
    this->nbLigneVar=0;
    for (int i = 0 ; i < NB_LIGNE_TEXTE ; i++){
        this->tblVariables[i].variable = NULL;
        this->tblVariables[i].posX = 0;
        this->tblVariables[i].posY = 0;
        this->tblVariables[i].size = 0;
    }
    /*
    Serial.print("DisplayCadreClass : init : init du cadre ");
    Serial.print(this->name);
    Serial.print(" de type ");
    Serial.print(this->cadreType);
    Serial.println();
    */
    displayCadre();
    fillBackground();
}

//*********************************************
//*
//*       getCadreType
//*
//*********************************************
int DisplayCadreClass::getCadreType(){
    return this->cadreType;
}

//*********************************************
//*
//*       showCaracteristique
//*
//*********************************************
void DisplayCadreClass::showCaracteristique(int index){
    Serial.print("caracteristique du cadre <");
    Serial.print(this->name);
    Serial.print("> type = ");
    Serial.print(this->getCadreType());
    Serial.print(" index = ");
    Serial.print(index);
    Serial.println();
}

//*********************************************
//*
//*       refresh
//*
//*********************************************
void DisplayCadreClass::refreshCadre(){
    
    if (this->cadreType == CADRE_MESURES_GRAPH && mesure.isMesureDisponible()){
        this->refresh = true;
    }
    if (this->refresh){
        /*
        Serial.print("DisplayCadreClass => refresh du cadre <");
        Serial.print(this->name);
        Serial.print("> ");
        Serial.print(" de type ");
        Serial.println(this->cadreType);
        */
        switch (this->cadreType) {
            case CADRE_TITRE:
                printLignes();
                break;
            case CADRE_MENU:
                printLignes();
                printVariables();
                break;
            case CADRE_TABLEAU:
                break;
            case CADRE_MESURES_TEXTE:
                printLignes();
                printVariables();
                break;
            case CADRE_MESURES_GRAPH:
                if (mesure.isMesureDisponible()){
                    displayCourbes();
                }
                break;
            default:
                Serial.print("DisplayCadreClass => ERREUR : type de cadre inconnu : ");
                Serial.println(this->cadreType);
        }
        this->refresh = false;
    }    
}

//*********************************************
//*
//*       displayCadre
//*
//*********************************************
void DisplayCadreClass::displayCadre(){
    //char tmp[100];
    //sprintf(tmp, "affiche le cadre en %d, %d ; de taille %d,%d", originX, originY, sizeX, sizeY);
    //Serial.println(tmp);
    tft->drawRect(originX, originY, sizeX, sizeY, colorCadre);
}

//*********************************************
//*
//*       fillBackground
//*
//*********************************************
void DisplayCadreClass::fillBackground(){
    //char tmp[100];
    //sprintf(tmp, "affiche le cadre en %d, %d ; de taille %d,%d", originX, originY, sizeX, sizeY);
    //Serial.println(tmp);
    tft->fillRect(originX + 1, originY + 1, sizeX - 2, sizeY - 2, colorBackground);
}


//*********************************************
//*
//*       setTextSize
//*
//*********************************************
void DisplayCadreClass::setTextSize(int size){
    this->textSize = size;
}

//*********************************************
//*
//*       printTexte
//*
//*********************************************
void DisplayCadreClass::printTexte(char *texte, int size, int posX, int posY){
    tft->setCursor(posX, posY);
    tft->setTextSize(size); 
    tft->setTextColor(this->colorText);
    tft->print(texte);
}

//*********************************************
//*
//*       printLignes
//*
//*********************************************
void DisplayCadreClass::printLignes(){
    for (int i = 0 ; i < this->nbLigneText ; i++){
        printTexte(this->tblText[i].text, this->tblText[i].size, this->tblText[i].posX, this->tblText[i].posY);
    }
}

//*********************************************
//*
//*       printVar
//*
//*********************************************
void DisplayCadreClass::printVar(int *ptrVar, int size, int posX, int posY){
    tft->setCursor(posX, posY);
    tft->setTextSize(size); 
    tft->setTextColor(this->colorText, this->colorBackground);
    tft->println(*ptrVar);
}

//*********************************************
//*
//*       printVariables
//*
//*********************************************
void DisplayCadreClass::printVariables(){
    for (int i = 0 ; i < this->nbLigneVar ; i++){
        printVar(this->tblVariables[i].variable, this->tblVariables[i].size, this->tblVariables[i].posX, this->tblVariables[i].posY);
    }
}

//*********************************************
//*
//*       addTexte
//*
//*********************************************
void DisplayCadreClass::addText(char *texte, int size, int ligne, int colonne){
    int posX = originX + (size * 6 * colonne) + 4;
    int posY = originY + (size * 10 * ligne) + 6;

    strcpy(this->tblText[this->nbLigneText].text,texte);
    this->tblText[this->nbLigneText].posX = posX;
    this->tblText[this->nbLigneText].posY = posY;
    this->tblText[this->nbLigneText].size = size;
    this->nbLigneText++;
    this->refresh = true;
}

//*********************************************
//*
//*       addVariable
//*
//*********************************************
void DisplayCadreClass::addVariable(int *ptrVar, int size, int ligne, int colonne){
    int posX = originX + (size * 6 * colonne) + 4;
    int posY = originY + (size * 10 * ligne) + 6;

    this->tblVariables[this->nbLigneVar].variable = ptrVar;
    this->tblVariables[this->nbLigneVar].posX = posX;
    this->tblVariables[this->nbLigneVar].posY = posY;
    this->tblVariables[this->nbLigneVar].size = size;
    this->nbLigneVar++;
    this->refresh = true;
}

//*********************************************
//*
//*       drawLine
//*
//*********************************************
void DisplayCadreClass::drawLine(int x0, int y0, int x1, int y1){
    tft->drawLine(this->originX + y0 + 2, this->originY + this->sizeY - y0 - 2, this->originX + x1 + 2, this->originY + this->sizeY - y1 - 2,this->colorText);
} 

int posX = 2;
int posY = 2;
int lastMesure;
int newMesure;
//=================================================
//
//      displayCourbes
//
//=================================================
void DisplayCadreClass::displayCourbes(){

    // display points
    //Serial.print("DisplayCadreClass => displayPoints");
    char tmp[100];

    int x0, x1, y0, y1;
    while (mesure.isMesureDisponible()){
        lastMesure = mesure.getLastMesure();
        newMesure = mesure.getMesure();
        x0 = originX + posX - 1;
        y0 = originY + sizeY - lastMesure;
        x1 = originX + posX;
        y1 = originY + sizeY - newMesure;
        tft->drawLine(x0,y0,x1,y1,this->colorText);
    }
    posX++;
    if (posX > sizeX - 2){
        posX = 2;
        fillBackground();
    }
    refresh = false;
}


//=================================================
//
//      activateRefresh
//
//=================================================
void DisplayCadreClass::activateRefresh(){
    this->refresh = true;
}