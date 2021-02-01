//
//
//  monitor.cpp
//
//

#include <string.h>
#include "BFR_tft_windows.hpp"

char carLu;
char commande[50];
String cde;
int indexCarLu=0;

extern BFR_tft_windows_manager tftDisplay;

//*********************************************
//*
//*         help
//*
//*********************************************
void help(){
    Serial.println("Commandes disponibles : ");
    Serial.println("- help : cette aide");
    Serial.println("- refresh : actualise les fenetres");
    Serial.println("- rotate : fait une rotation de l'ecran");
    Serial.println("- list : ");
    Serial.println("    - windows : liste infos sur les fenetres crees");
    Serial.println("- new : ");
    Serial.println("    - window posX posY sizeX sizeY couleurCadre couleurBackground couleurTexte nomFenetre : cree une fenetre");

}

//*********************************************
//*
//*         cdeNew
//*
//*********************************************
void cdeNew(String subCommand){
    Serial.print("Commandes new : <");
    Serial.print(subCommand);
    Serial.print(">");
    Serial.println();

    if (subCommand.startsWith("window")){
        Serial.print("creation d'une fenetre avec les parametres : ");
        Serial.print(subCommand);
        Serial.println();
    } else {
        Serial.print("ERREUR => <");
        Serial.print(subCommand);
        Serial.println("> => sous commande inconnue");
        help();
    }
}

//*********************************************
//*
//*         cdeList
//*
//*********************************************
void cdeList(String subCommand){
    Serial.print("Commandes list : <");
    Serial.print(subCommand);
    Serial.print(">");
    Serial.println();

    if (subCommand.equals("windows")){
        tftDisplay.DisplayWindowsList();
    } else {
        Serial.print("ERREUR => <");
        Serial.print(subCommand);
        Serial.println("> => sous commande inconnue");
        help();
    }
}

//*********************************************
//*
//*         analyzeCommand
//*
//*********************************************
void analyzeCommand(String cde){
    cde=cde.substring(0,cde.length()-1);
    int index = cde.indexOf(' ');
    String subCommand="";
    if (index != -1){
        subCommand = cde.substring(index+1,cde.length());
    }
    if (cde.startsWith("new")){
        cdeNew(subCommand);
    } else if (cde.startsWith("list")){
        cdeList(subCommand);
    } else if (cde.equals("help")){
        help();
    } else if (cde.equals("rotate")){
        tftDisplay.rotate();
        tftDisplay.forceRefresh();
    } else if (cde.equals("refresh")){
        tftDisplay.forceRefresh();
    } else {
        Serial.print("ERREUR => <");
        Serial.print(cde);
        Serial.println("> => commande inconnue");
        help();
    }
}

//*********************************************
//*
//*         readKeybord
//*
//*********************************************
void readKeybord(){    
    while (Serial.available() > 0){
        carLu = Serial.read();
        commande[indexCarLu++] = carLu;
        commande[indexCarLu]='\0';
        if (carLu == '\n'){
            // retour chariot saisi
            // on analyse la commande
            cde = String(commande);
            // cde.toLowerCase();
            // suppression des doublons de blancs
            while (cde.indexOf("  ") > 0){
                cde.replace("  ", " ");
            }
            cde.toCharArray(commande,50);
            Serial.print(commande);
            analyzeCommand(String(commande));
            indexCarLu=0;
            commande[indexCarLu]='\0';
        }
    }
}