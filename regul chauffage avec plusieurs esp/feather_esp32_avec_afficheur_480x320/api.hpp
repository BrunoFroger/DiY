//
//
//  api.hpp
//
//

#ifndef __API
#define __API

typedef struct {
    char heureFormatee[50];
    bool heureModifiee;
    char dateFormatee[50];
    bool dateModifiee;
    int consigne;
    int temperatureMesuree;
    bool temperatureMesureeModifiee;
    bool consigneModifiee;
    bool parametresModifies;
    bool chauffageOnOff;
    bool WifiConnected;
} chauffageDatas;

extern chauffageDatas mesDonneesApi;

//extern chauffageDatas *getDatas(void);
extern void updateDatas(void);
extern bool isDatasAvailables(void);
extern void resetDataAvailables(void);
extern void initApi(void);

#endif