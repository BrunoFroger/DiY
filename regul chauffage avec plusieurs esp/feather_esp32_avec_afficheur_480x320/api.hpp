//
//
//  api.hpp
//
//

#ifndef __API
#define __API

typedef struct {
    // requete ntp
    char heureFormatee[50];
    bool heureModifiee;
    char dateFormatee[50];
    bool dateModifiee;
    // requete donnees de chauffage
    int consigne;
    bool consigneModifiee;
    int puissanceChauffage;
    // requete temperature
    bool parametresModifies;
    bool chauffageOnOff;
    int temperatureMesuree;
    bool temperatureMesureeModifiee;
    bool WifiConnected;
    bool refreshMesures;
    // donnees reseau
    char wifiSsid[50];
    char wifiPwd[50];
    char espName[50];
    char ipGateway[25];
    char ipLocale[25];
} chauffageDatas;

extern chauffageDatas mesDonneesApi;

//extern chauffageDatas *getDatas(void);
extern void updateDatas(void);
extern bool isDatasAvailables(void);
extern void resetDataAvailables(void);
extern void initApi(void);
extern void afficheDatas(void);
extern void setGatewayrequest(char *requete);

#endif