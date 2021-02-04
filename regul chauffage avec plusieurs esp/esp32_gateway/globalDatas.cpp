//
//
//  globalDatas.cpp
//
//

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "globalDatas.hpp"

typeGlobalDatas donneesGlobales;

//=========================================
//
//          afficheDatas
//
//=========================================
void initGlobalDatas(void){
    // parametre temps
    donneesGlobales.second = 0;
    donneesGlobales.minute = 25;
    donneesGlobales.heure = 12;
    donneesGlobales.jour = 17;
    donneesGlobales.jourSemaine;
    donneesGlobales.mois = 2;
    donneesGlobales.annee = 2021;

    // parametres de chauffage
    donneesGlobales.consigne = 200;
    donneesGlobales.chauffageOnOff = false;
    donneesGlobales.puissanceChauffage = 0;

    // parametres reseau
    //ipGateway;
    //nomGateway;
    //ipAfficheur;
    //nomAfficheur;
    //ipPilotageChaudiere;
    //nomPilotageChaudiere;

}