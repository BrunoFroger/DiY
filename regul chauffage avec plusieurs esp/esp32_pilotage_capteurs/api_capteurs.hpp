//
//
//  api_capteurs.hpp
//
//

#ifndef __API_CAPTEURS
#define __API_CAPTEURS

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
} capteursDatas;

extern capteursDatas mesDonneesCapteurs;

extern void updateDatas(void);
extern void initApi(void);
extern void afficheDatas(void);

#endif