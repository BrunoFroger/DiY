//
//
//  BFR_Window.cpp
//
//

#include "BFR_tft_windows.hpp"

//*********************************************
//*
//*       Constructor
//*
//*********************************************
BFR_Window::BFR_Window(){
}

//*********************************************
//*
//*       Destructor
//*
//*********************************************
BFR_Window::~BFR_Window()
{
}

//*********************************************
//*
//*       init
//*
//*********************************************
void BFR_Window::init(tftType *tft, int originX, int originY, int sizeX, int sizeY, int colorwindow, int colorBackground, int colorText, char *name){
    Serial.println("BFR_Window : init ---------- debut -------------");
    this->tft = tft;
    this->originX = originX;
    this->originY = originY;
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->colorwindow = colorwindow;
    this->colorBackground = colorBackground;
    this->colorText = colorText;
    this->refresh = true;
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
    for (int i = 0 ; i < NB_MAX_VARIABLES ; i++){
        this->tblVariables[i].variable = NULL;
        this->tblVariables[i].posX = 0;
        this->tblVariables[i].posY = 0;
        this->tblVariables[i].size = 0;
    }
    
    Serial.print("BFR_Window : init : init de la fenetre ");
    Serial.print(this->name);
    Serial.println();
    
    showWindows();
    fillBackground();
    Serial.println("BFR_Window : init ---------- fin -------------");
}

//*********************************************
//*
//*       showCaracteristique
//*
//*********************************************
void BFR_Window::showCaracteristique(){
    Serial.println("BFR_Window : showCaracteristique ---------- debut -------------");
    Serial.print("caracteristique de la fenetre :");
    Serial.println(this->name);
    Serial.print("  - posX = ");
    Serial.println(this->originX);
    Serial.print("  - posY = ");
    Serial.println(this->originY);
    Serial.print("  - sizeX = ");
    Serial.println(this->sizeX);
    Serial.print("  - sizeY = ");
    Serial.println(this->sizeY);
    Serial.print("  - nbLignesTexte = ");
    Serial.println(this->nbLigneText);
    Serial.println("BFR_Window : showCaracteristique ---------- fin -------------");
}

//*********************************************
//*
//*       refresh
//*
//*********************************************
void BFR_Window::refreshWindow(){       
    if (this->refresh){
        Serial.println("BFR_Window : refreshWindow ---------- debut -------------");
        Serial.print("BFR_Window => refresh de la fenetre <");
        Serial.print(this->name);
        Serial.print("> ");
        Serial.println();
        printLignes();
        this->refresh = false;
        Serial.println("BFR_Window : refreshWindow ---------- fin -------------");
    } 
    /*
    if (this->cadreType == CADRE_MESURES_GRAPH && mesure.isMesureDisponible()){
        this->refresh = true;
    }
    if (this->refresh){
        
        Serial.print("DisplayCadreClass => refresh du cadre <");
        Serial.print(this->name);
        Serial.print("> ");
        Serial.print(" de type ");
        Serial.println(this->cadreType);
        
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
    }    */
}

//*********************************************
//*
//*       showWindows
//*
//*********************************************
void BFR_Window::showWindows(){
    Serial.println("BFR_Window : showWindows ---------- debut -------------");
    char tmp[100];
    sprintf(tmp, "BFR_Window : showWindows => affiche la fenetre %s en %d, %d ; de taille %d,%d", name, originX, originY, sizeX, sizeY);
    Serial.println(tmp);
    tft->drawRect(originX, originY, sizeX, sizeY, colorwindow);
    this->refreshWindow();
    Serial.println("BFR_Window : showWindows ---------- fin -------------");
}

//*********************************************
//*
//*       fillBackground
//*
//*********************************************
void BFR_Window::fillBackground(){
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
void BFR_Window::setTextSize(int size){
    this->textSize = size;
}

//*********************************************
//*
//*       printTexte
//*
//*********************************************
void BFR_Window::printTexte(char *texte, int size, int posX, int posY){
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
void BFR_Window::printLignes(){
    Serial.println("BFR_Window : printLignes ---------- debut -------------");
    Serial.print("fenetre  = ");
    Serial.println(this->name);
    Serial.print("nb lignes = ");
    Serial.println(this->nbLigneText);
    if (this->nbLigneText > NB_LIGNE_TEXTE){
        this->nbLigneText = NB_LIGNE_TEXTE;
        Serial.println("ERROR : nbLignesTexte too big !! troncate it ");
    } else {
        for (int i = 0 ; i < this->nbLigneText ; i++){
            Serial.print("Ligne n° ");
            Serial.print(i);
            Serial.print(" => ");
            Serial.print(this->tblText[i].text);
            Serial.println();
            printTexte(this->tblText[i].text, this->tblText[i].size, this->tblText[i].posX, this->tblText[i].posY);
        }
    }
    Serial.println("BFR_Window : printLignes ---------- fin -------------");
}

//*********************************************
//*
//*       printVar
//*
//*********************************************
void BFR_Window::printVar(int *ptrVar, int size, int posX, int posY){
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
void BFR_Window::printVariables(){
    for (int i = 0 ; i < this->nbLigneVar ; i++){
        printVar(this->tblVariables[i].variable, this->tblVariables[i].size, this->tblVariables[i].posX, this->tblVariables[i].posY);
    }
}

//*********************************************
//*
//*       getName
//*
//*********************************************
char *BFR_Window::getName(){
    return this->name;
}

//*********************************************
//*
//*       addTexte
//*
//*********************************************
void BFR_Window::addText(char *texte, int size, int ligne, int colonne){
    Serial.println("BFR_Window : addText ---------- debut -------------");
    if (strlen(texte) > MAX_TEXT_SIZE){
        Serial.println("ERROR => text size too long");
    } else if (nbLigneText > NB_LIGNE_TEXTE) {
        Serial.println("ERROR : nb max lines reach");
        nbLigneText = NB_LIGNE_TEXTE;
    } else {
        int posX = originX + (size * 6 * colonne) + 4;
        int posY = originY + (size * 10 * ligne) + 6;
        Serial.print("Dans la fenetre : ");
        Serial.print(this->name);
        Serial.print(", ajout d'une ligne a l'index : ");
        Serial.print(this->nbLigneText);
        Serial.print(" => ");
        Serial.print(texte);
        Serial.println();
        strcpy(this->tblText[this->nbLigneText].text,texte);
        this->tblText[this->nbLigneText].posX = posX;
        this->tblText[this->nbLigneText].posY = posY;
        this->tblText[this->nbLigneText].size = size;
        this->nbLigneText++;
        this->refresh = true;
        showCaracteristique();
    }
    Serial.println("BFR_Window : addText ---------- fin -------------");
}

//*********************************************
//*
//*       addVariable
//*
//*********************************************
void BFR_Window::addVariable(int *ptrVar, int size, int ligne, int colonne){
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
void BFR_Window::drawLine(int x0, int y0, int x1, int y1){
    tft->drawLine(this->originX + y0 + 2, this->originY + this->sizeY - y0 - 2, this->originX + x1 + 2, this->originY + this->sizeY - y1 - 2,this->colorText);
} 


/*******************************
int posX = 2;
int posY = 2;
int lastMesure;
int newMesure;
//=================================================
//
//      displayCourbes
//
//=================================================
void BFR_Window::displayCourbes(){

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
*************************/

//=================================================
//
//      activateRefresh
//
//=================================================
void BFR_Window::activateRefresh(){
    this->refresh = true;
}