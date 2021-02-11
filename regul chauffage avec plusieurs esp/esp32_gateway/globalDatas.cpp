//
//
//  globalDatas.cpp
//
//

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "globalDatas.hpp"
#include "temperature.hpp"

typeGlobalDatas donneesGlobales;

//=========================================
//
//          initGlobalDatas
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
    //char nomGateway;
    //ipAfficheur;
    //nomAfficheur;
    //ipPilotageChaudiere;
    //nomPilotageChaudiere;

}


//=========================================
//
//          afficheDatas
//
//=========================================
void afficheDatas(void){
    // on affiche rien si en mode simulation
    if (donneesGlobales.modeCalculTemperature == 2){
        getTemperature();
        return;
    } 

    printf("globalDatas.cpp => afficheDatas \n");
    char tmp[200];
    char status[10];
    Serial.println("+------------------+--------------------+");
    Serial.println("|  nom             |       valeur       |");
    Serial.println("+------------------+--------------------+");
    sprintf(tmp,   "| consigne         |  %10d        |", donneesGlobales.consigne);
    Serial.println(tmp);
    sprintf(tmp,   "| puiss chauffage  |  %10d        |", donneesGlobales.puissanceChauffage);
    Serial.println(tmp);
    if (donneesGlobales.chauffageOnOff) strcpy(status,"ON"); else strcpy(status,"OFF");
    sprintf(tmp,   "| chauffage On/Off |  %10s        |", status);
    Serial.println(tmp);
    Serial.println("+------------------+--------------------+");
    sprintf(tmp,   "| temperature int  |  %10d        |", getTemperature());
    Serial.println(tmp);
    sprintf(tmp,   "| temperature ext  |  %10d        |", donneesGlobales.temperatureExterieure);
    Serial.println(tmp);
    Serial.println("+------------------+--------------------+");
    sprintf(tmp,   "| Ssid             |%17s   |", donneesGlobales.wifiSsid);
    Serial.println(tmp);
    sprintf(tmp,   "| Passwd           |%17s   |", donneesGlobales.wifiPwd);
    Serial.println(tmp);
    sprintf(tmp,   "| IP gateway       |%17s   |", donneesGlobales.ipGateway);
    Serial.println(tmp);
    sprintf(tmp,   "| nb clients       |  %10d        |", donneesGlobales.nbClientsConnectes);
    Serial.println(tmp);
    sprintf(tmp,   "| IP Afficheur     |%17s   |", donneesGlobales.ipAfficheur);
    Serial.println(tmp);
    sprintf(tmp,   "| nom Afficheur    |%17s   |", donneesGlobales.nomAfficheur);
    Serial.println(tmp);
    sprintf(tmp,   "| IP Capteurs      |%17s   |", donneesGlobales.ipPilotageChaudiere);
    Serial.println(tmp);
    sprintf(tmp,   "| nom Capteurs     |%17s   |", donneesGlobales.nomPilotageChaudiere);
    Serial.println(tmp);
    Serial.println("+------------------+--------------------+");
    char modeSimulation [20];
    switch(donneesGlobales.modeCalculTemperature){
        case 1: strcpy(modeSimulation,"1:rampe"); break;
        case 2: strcpy(modeSimulation,"2:simul"); break;
        case 3: strcpy(modeSimulation,"3:mesure"); break;
        default : strcpy(modeSimulation,"inconnu");
    }
    sprintf(tmp,   "| mode simulation  |  %10s        |", modeSimulation);
    Serial.println(tmp);
    if (donneesGlobales.modeVerbose) strcpy(status,"true"); else strcpy(status,"false");
    sprintf(tmp,   "| mode verbose     |  %10s        |", status);
    Serial.println(tmp);
    Serial.println("+------------------+--------------------+");
}

