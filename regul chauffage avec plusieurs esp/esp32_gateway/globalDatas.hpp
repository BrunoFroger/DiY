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
    int temperature = 180;
    int temperatureExterieure = 150;

    // parametres de chauffage
    int consigne;
    bool chauffageOnOff;
    int puissanceChauffage;

    // parametres reseau
    char wifiSsid[50];
    char wifiPwd[50];
    int nbClientsConnectes = 0;
    char ipGateway[50];
    char nomGateway[50];
    char ipAfficheur[50];
    char nomAfficheur[50];
    char ipPilotageChaudiere[50];
    char nomPilotageChaudiere[50];

    // autres parametres
    bool modeVerbose = false;
    int modeCalculTemperature = 1;
} typeGlobalDatas;


extern typeGlobalDatas donneesGlobales;

extern void initGlobalDatas(void);
extern void afficheDatas(void);

#endif