//
//
//  globalDatas.hpp
//
//

#ifndef __GLOBALDATAS
#define __GLOBALDATAS


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


typedef struct {
    // parametre temps
    int second = 0;
    int minute = 25;
    int heure = 12;
    int jour = 17;
    int jourSemaine;
    int mois = 2;
    int annee = 2010;

    // parametres temperature (en dixieme de degres)
    int temperature = 200;

    // parametres de chauffage
    int consigne;
    bool chauffageOnOff;

    // parametres reseau
    int nbClientsConnectes = 0;
    IPAddress ipGateway;
    char nomGateway[50];
    IPAddress ipAfficheur;
    char nomAfficheur[50];
    IPAddress ipPilotageChaudiere;
    char nomPilotageChaudiere[50];
} typeGlobalDatas;


extern typeGlobalDatas donneesGlobales;

extern void initGlobalDatas(void);


#endif