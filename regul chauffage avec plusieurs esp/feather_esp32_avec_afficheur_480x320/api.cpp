//
//
//  api.cpp
//
//

#include <Arduino.h>
#include <string.h>

#include "api.hpp"
#include "wifiTools.hpp"



int second = 0;
int minute = 25;
int heure = 12;
int jour = 17;
int jourSemaine;
int mois = 2;
int annee = 2020;
unsigned long nbMillisecondUpdateHeure = 0;
unsigned long nbMillisecondUpdateWifi = 0;
char dayOfWeek[7][5] = {"dim", "lun", "mar", "mer", "jeu", "ven", "sam"};
bool datasUpated = false;
chauffageDatas mesDonneesApi;


//=========================================
//
//          afficheDatas
//
//=========================================
void afficheDatas(void){
    printf("api.cpp => afficheDatas (%s) \n", mesDonneesApi.heureFormatee);
    char tmp[200];
    char status[10];
    Serial.println("+------------------+--------------------+");
    Serial.println("|  nom             |       valeur       |");
    Serial.println("+------------------+--------------------+");
    sprintf(tmp,   "| heure formatee   |     %10s     |", mesDonneesApi.heureFormatee);
    Serial.println(tmp);
    sprintf(tmp,   "| date formatee    | %17s  |", mesDonneesApi.dateFormatee);
    Serial.println(tmp);
    sprintf(tmp,   "| consigne         |  %10d        |", mesDonneesApi.consigne);
    Serial.println(tmp);
    sprintf(tmp,   "| temperature      |  %10d        |", mesDonneesApi.temperatureMesuree);
    Serial.println(tmp);
    if (mesDonneesApi.chauffageOnOff) strcpy(status,"ON"); else strcpy(status,"OFF");
    sprintf(tmp,   "| chauffage On/Off |  %10s        |", status);
    Serial.println(tmp);
    if (mesDonneesApi.WifiConnected) strcpy(status,"true"); else strcpy(status,"false");
    sprintf(tmp,   "| wifi connected   |  %10s        |", status);
    Serial.println(tmp);
    Serial.println("+------------------+--------------------+");
}

//=========================================
//
//          initApi
//
//=========================================
void initApi(void){
    Serial.println("api.cpp => init api");
    mesDonneesApi.heureModifiee = true;
    mesDonneesApi.dateModifiee = true;
    mesDonneesApi.parametresModifies = true;
    mesDonneesApi.WifiConnected = isWifiConnected();
    mesDonneesApi.temperatureMesuree = 205;
    mesDonneesApi.temperatureMesureeModifiee = true;
    afficheDatas();
}

//=========================================
//
//          resetDataAvailables
//
//=========================================
/*chauffageDatas *getDatas(void){
    printf("api.cpp => getDatas\n");
    afficheDatas();
    return (&mesDonneesApi);
}*/
void resetDataAvailables(void){
    datasUpated = false;
}

//=========================================
//
//          isDatasAvailables
//
//=========================================
bool isDatasAvailables(void){
    return datasUpated;
}

//=========================================
//
//          updateHeure
//
//=========================================
void updateHeure(void){
    int dernierJour = 31;

    if ((millis() - nbMillisecondUpdateHeure) >= 1000){
        nbMillisecondUpdateHeure = millis();

        //Serial.println("Update Heure");

        second++;
        mesDonneesApi.heureModifiee = true;

        if (second >= 60){
            second = 0;
            minute++;
        }
        if (minute >= 60){
            minute = 0;
            heure++;
        }
        if (heure >= 24){
            heure = 0;
            jour++;
            mesDonneesApi.dateModifiee = true;
        }
        switch (mois){  
            case 4:
            case 6:
            case 9:
            case 11:    
                dernierJour = 30;
                break;      
            case 2:
                if ((annee % 4) != 0){
                    dernierJour = 29;
                } else {
                    dernierJour = 28;
                }
                break;
            default:
                dernierJour = 31;
        }
        if (jour > dernierJour){
            jour = 1;
            mois++;
        }
        if (mois > 12){
            mois = 0;
            annee++;
        }
        sprintf(mesDonneesApi.heureFormatee, "%2d:%02d:%02d", heure, minute, second);
        sprintf(mesDonneesApi.dateFormatee, "%s %2d/%02d/%2d", dayOfWeek[jourSemaine], jour, mois, annee);
        datasUpated = true;
    }
}

//=========================================
//
//          updateDatas
//
//=========================================
void updateDatas(void){
    updateHeure();

    //if ((millis() - nbMillisecondUpdateWifi) >= 5*60*1000){     // update tt les 5 min
    if ((millis() - nbMillisecondUpdateWifi) >= 30*1000){     // update tt les 30sec
        nbMillisecondUpdateWifi = millis();
        bool tmp = isWifiConnected();
        if (!tmp) {
            initWifi(true);
        }
        //Serial.println("api.cpp => uodateDatas ");
        afficheDatas();
        if (tmp != mesDonneesApi.WifiConnected) {
            mesDonneesApi.parametresModifies = true;
            Serial.print("api.cpp => updateDatas : changement d'etat du wifi -> ");
            if (tmp){
                Serial.println("true");
            } else {
                Serial.println("false");
            }
            mesDonneesApi.WifiConnected = tmp;
        }
    }
}