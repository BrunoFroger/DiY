//
//
//  api_capteurs.hpp
//
//

#ifndef __API_CAPTEURS
#define __API_CAPTEURS

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

typedef struct {
    // requete donnees de chauffage
    int consigne;
    bool consigneModifiee;
    int puissanceChauffage;
    // requete temperature
    bool parametresModifies;
    bool chauffageOnOff;
    bool WifiConnected;
    bool refreshMesures;
    // donnees reseau
    char wifiSsid[50];
    char wifiPwd[50];
    char espName[50];
    IPAddress gateway;
    char ipGateway[25];
    char ipLocale[25];
} capteursDatas;

extern capteursDatas mesDonneesCapteurs;

extern void updateDatas(void);
extern void initApi(void);
extern void afficheDatas(void);

#endif