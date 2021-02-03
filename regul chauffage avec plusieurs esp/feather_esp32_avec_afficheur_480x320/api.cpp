//
//
//  api.cpp
//
//

#include <Arduino.h>
#include <string.h>

#include "api.hpp"
#include "wifiTools.hpp"

// definition des delais de rafraichissement des differentes donnees (en ms)
#define     DELAY_REFRESH_TEST_WIFI     1000*30
#define     DELAY_REFRESH_NTP           1000*60*60*2
#define     DELAY_REFRESH_TEMPERATURE   1000*10

int second = 0;
int minute = 25;
int heure = 12;
int jour = 17;
int jourSemaine;
int mois = 12;
int annee = 2014;
unsigned long nbMillisecondUpdateHeure = 0;
unsigned long nbMillisecondUpdateWifi = 0;
unsigned long nbMillisecondUpdateNTP = 0;
unsigned long nbMillisecondUpdateTemperature = 0;
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
//          setGatewayrequest
//
//=========================================
void setGatewayrequest(char *requete){
    if (WiFi.status() == WL_CONNECTED){
        char tmp[100];
        char gatewayStringIp[30];
        gatewayIp.toString().toCharArray(gatewayStringIp,30);
        wifiClient.connect(gatewayIp,80);
        sprintf(tmp, "GET /%s HTTP/1.1", requete);
        wifiClient.println(tmp);
        wifiClient.println();
        //sprintf(tmp, "setGatewayrequest => requete <%s/%s> envoyee", gatewayStringIp, requete);
        //Serial.println(tmp);
    } else {
        Serial.print("envoi requete ");
        Serial.print(requete);
        Serial.println(" impossible => wifi non connecte");
    }
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
        //Serial.println("api.cpp->updateHeure");

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
            mesDonneesApi.dateModifiee = true;
        }
        if (mois > 12){
            mois = 0;
            annee++;
            mesDonneesApi.dateModifiee = true;
        }
        sprintf(mesDonneesApi.heureFormatee, "%2d:%02d:%02d", heure, minute, second);
        //Serial.println(mesDonneesApi.heureFormatee);
        sprintf(mesDonneesApi.dateFormatee, "%s %2d/%02d/%2d", dayOfWeek[jourSemaine], jour, mois, annee);
        datasUpated = true;
    }
}

#define TAILLE_REPONSE  5000
//========================================= 
//
//          getGatewayResponses
//
//=========================================
char *getGatewayResponse(char *response){
    delay(100);
    int idx = 0;
    strcpy(response, "");
    delay(100);
    if (wifiClient.available() > 0){
        //Serial.print("getGatewayResponse => nb car dans la reponse : ");
        //Serial.println(wifiClient.available());
        while (wifiClient.available() > 0){
            char car = wifiClient.read();
            //Serial.print(car);
            response[idx++] = car;
            if (idx >= TAILLE_REPONSE) {
                Serial.print("Erreur : on depasse la taille de reponse possible (%d)");
                Serial.println(TAILLE_REPONSE);
                return NULL;
            }
        }
        response[idx] = '\0';
        //Serial.println("Resultat de la requete : ");
        //Serial.print("<");
        //Serial.print(response);
        //Serial.print(">");
        //Serial.println();
    }
    return response;
}

//=========================================
//
//          refreshNecessaire
//
//=========================================
void refreshNecessaire(char *variable){
    if (strcmp(variable, "second") == 0){
        setGatewayrequest("getNtp");
    } else if (strcmp(variable, "minute") == 0){
        setGatewayrequest("getNtp");
    } else if (strcmp(variable, "heure") == 0){
        setGatewayrequest("getNtp");
    } else if (strcmp(variable, "jour") == 0){
        setGatewayrequest("getNtp");
    } else if (strcmp(variable, "jourSemaine") == 0){
        setGatewayrequest("getNtp");
    } else if (strcmp(variable, "mois") == 0){
        setGatewayrequest("getNtp");
    } else if (strcmp(variable, "annee") == 0){
        setGatewayrequest("getNtp");
    } else if (strcmp(variable, "temperature") == 0){
        setGatewayrequest("getTemperature");
    } 
}

//=========================================
//
//          actualiseVariable
//
//=========================================
void actualiseVariable(char *variable, char *valeur){
    //Serial.print("actualiseVariable => <");
    //Serial.print(variable);
    //Serial.print("> = <");
    //Serial.print(valeur);
    //Serial.print(">");
    //Serial.println();
    if (strcmp(variable, "temperature") == 0){
        //Serial.println("mise a jour temperature");
        int tmp = atoi(valeur);
        if (tmp != mesDonneesApi.temperatureMesuree){
            mesDonneesApi.temperatureMesuree = tmp;
            mesDonneesApi.temperatureMesureeModifiee = true;
            Serial.println("temperature mise a jour");
        }
    } else if (strcmp(variable, "heure") == 0){
        //Serial.println("mise a jour heure");
        int tmp = atoi(valeur);
        if (tmp != heure){
            heure = tmp;
            Serial.println("heure mise a jour");
            mesDonneesApi.heureModifiee = true;
        }
    } else if (strcmp(variable, "minute") == 0){
        //Serial.println("mise a jour minute");
        int tmp = atoi(valeur);
        if (tmp != minute){
            minute = tmp;
            Serial.println("minute mise a jour");
            mesDonneesApi.heureModifiee = true;
        }
    } else if (strcmp(variable, "second") == 0){
        //Serial.println("mise a jour second");
        int tmp = atoi(valeur);
        if (tmp != second){
            second = tmp;
            Serial.println("second mise a jour");
            mesDonneesApi.heureModifiee = true;
        }
    } else if (strcmp(variable, "annee") == 0){
        //Serial.println("mise a jour annee");
        int tmp = atoi(valeur);
        if (tmp != annee){
            annee = tmp;
            Serial.println("annee mise a jour");
            mesDonneesApi.dateModifiee = true;
        }
    } else if (strcmp(variable, "mois") == 0){
        //Serial.println("mise a jour mois");
        int tmp = atoi(valeur);
        if (tmp != mois){
            mois = tmp;
            Serial.println("mois mise a jour");
            mesDonneesApi.dateModifiee = true;
        }
    } else if (strcmp(variable, "jour") == 0){
        //Serial.println("mise a jour jour");
        int tmp = atoi(valeur);
        if (tmp != jour){
            jour = tmp;
            Serial.println("jour mise a jour");
            mesDonneesApi.dateModifiee = true;
        }
    } else if (strcmp(variable, "jourSemaine") == 0){
        //Serial.println("mise a jour jourSemaine");
        int tmp = atoi(valeur);
        if (tmp != jourSemaine){
            jourSemaine = tmp;
            Serial.println("jourSemaine mise a jour");
            mesDonneesApi.dateModifiee = true;
        }
    } else {
        Serial.print("couple variable=valeur invalide : <");
        Serial.print(variable);
        Serial.print(">=<");
        Serial.print(valeur);
        Serial.print(">");
        Serial.println();
        refreshNecessaire(variable);
    }
}

//=========================================
//
//          updateDatas
//
//=========================================
void updateDatas(void){
    char response[TAILLE_REPONSE];

    updateHeure();

    if ((millis() - nbMillisecondUpdateWifi) >= DELAY_REFRESH_TEST_WIFI){     // update cnx wifi
        //Serial.println("api.cpp->updateDatas => refresh wifi");
        nbMillisecondUpdateWifi = millis();
        bool tmp = isWifiConnected();
        if (tmp != mesDonneesApi.WifiConnected) {
            mesDonneesApi.WifiConnected = tmp;
            mesDonneesApi.parametresModifies = true;
            /*Serial.print("api.cpp => updateDatas : changement d'etat du wifi -> ");
            if (tmp){
                Serial.println("true");
            } else {
                Serial.println("false");
            }*/
        }
        if (!tmp) {
            initWifi(true);
            setGatewayrequest("getNtp");
            delay(1000);
        }
        //Serial.println("api.cpp => uodateDatas ");
        //afficheDatas();
    }

    //if (annee < 2000) setGatewayrequest("getNtp");
    int refreshNtp = DELAY_REFRESH_NTP;
    if (annee < 2020) {
        refreshNtp = 5000;
    }
    if ((millis() - nbMillisecondUpdateNTP) >= refreshNtp){     // update ntp
        //Serial.println("api.cpp->updateDatas => refresh ntp");
        nbMillisecondUpdateNTP = millis();
        setGatewayrequest("getNtp");
        delay(1000);
    }

    if ((millis() - nbMillisecondUpdateTemperature) >= DELAY_REFRESH_TEMPERATURE){     // update temperature
        //Serial.println("api.cpp->updateDatas => refresh temperature");
        nbMillisecondUpdateTemperature = millis();
        setGatewayrequest("getTemperature");
        delay(100);
    }


    getGatewayResponse(response);
    if (strlen(response) > 0){
        //Serial.println();
        //Serial.println("decomposition de la reponse : debut");
        char variable[50], valeur[50];
        strcpy(variable, "");
        strcpy(valeur, "");
        int i, idxV = 0;
        bool isVariable = true;
        while (i < strlen(response)){
            if (isVariable) {       // on recupere les caracteres du nom de variable
                if (response[i] == '='){
                    isVariable = false;
                    //Serial.print("variable = ");
                    //Serial.println(variable);
                    strcpy(valeur, "");
                    idxV = 0;
                } else {
                    if (response[i] != '\n'){
                        variable[idxV++] = response[i];
                        variable[idxV] = '\0';
                    }
                }
            } else {        // on recuperer les caracteres de la valeur
                if (response[i] == '\n'){
                    isVariable = true;
                    //Serial.print("valeur = ");
                    //Serial.println(valeur);
                    idxV = 0;
                    actualiseVariable(variable, valeur);
                    strcpy(variable, "");
                } else {
                    valeur[idxV++] = response[i];
                    valeur[idxV] = '\0';
                }
            }
            i++;
        }
        //Serial.println("decomposition de la reponse : fin");
    }
}
