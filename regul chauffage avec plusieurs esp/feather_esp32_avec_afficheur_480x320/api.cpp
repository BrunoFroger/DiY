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
// il ne faut pas que deux delay aient la meme valeur
#define     DELAY_REFRESH_TEST_WIFI     1000*30         // 30s
#define     DELAY_REFRESH_NTP           1000*60*60*2    // 2H
//#define     DELAY_REFRESH_TEMPERATURE   1000*8        // 8s
#define     DELAY_REFRESH_TEMPERATURE   1000*25/10      // 2,5s
//#define     DELAY_REFRESH_DONNEE_CHAUFFAGE  1000*10   // 10s
#define     DELAY_REFRESH_DONNEE_CHAUFFAGE  1000*17/10  // 1,7s

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
unsigned long nbMillisecondUpdateDonneesChauffage = 0;
char dayOfWeek[7][5] = {"dim", "lun", "mar", "mer", "jeu", "ven", "sam"};
bool datasUpated = false;
chauffageDatas mesDonneesApi;
bool uneRequeteDejaActive = false;

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
    Serial.println("+------------------+--------------------+");
    sprintf(tmp,   "| consigne         |  %10d        |", mesDonneesApi.consigne);
    Serial.println(tmp);
    sprintf(tmp,   "| puiss chauffage  |  %10d        |", mesDonneesApi.puissanceChauffage);
    Serial.println(tmp);
    if (mesDonneesApi.chauffageOnOff) strcpy(status,"ON"); else strcpy(status,"OFF");
    sprintf(tmp,   "| chauffage On/Off |  %10s        |", status);
    Serial.println(tmp);
    Serial.println("+------------------+--------------------+");
    sprintf(tmp,   "| temperature      |  %10d        |", mesDonneesApi.temperatureMesuree);
    Serial.println(tmp);
    Serial.println("+------------------+--------------------+");
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
    mesDonneesApi.refreshMesures = true;

    afficheDatas();
    char buffer[100];
    Serial.println("envoi du nom de l'esp a la gateway");
    sprintf(buffer, "setName?nom=afficheur");
    setGatewayrequest(buffer);
}

//=========================================
//
//          resetDataAvailables
//
//=========================================
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
        sprintf(mesDonneesApi.heureFormatee, "%02d:%02d:%02d", heure, minute, second);
        //Serial.println(mesDonneesApi.heureFormatee);
        sprintf(mesDonneesApi.dateFormatee, "%s %02d/%02d/%2d", dayOfWeek[jourSemaine], jour, mois, annee);
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
            //if ((car >= '0') && (car <='z')){
                response[idx++] = car;
                if (idx >= TAILLE_REPONSE) {
                    Serial.print("Erreur : on depasse la taille de reponse possible (%d)");
                    Serial.println(TAILLE_REPONSE);
                    return NULL;
                }
            //}
        }
        response[idx] = '\0';
        /*Serial.println("Resultat de la requete : ");
        Serial.print("<");
        Serial.print(response);
        Serial.print(">");
        Serial.println();*/
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
    char chaine[200];
    //sprintf(chaine,"\napi.c / actualiseVariable => donnees avant traitement <%s> = <%s>", variable, valeur); Serial.println(chaine);

    // suppression des caracteres indelicat dans variable
    int i,j = 0;
    for (i = 0 ; i < strlen(variable) ; i++){
        if ((variable[i] >= '0') && (variable[i] <= 'z')){
            chaine[j++] = variable[i];
        }
    }
    chaine[j] = '\0';
    // suppression des caracteres indelicat dans valeur
    j=0;
    strcpy(variable, chaine);
    for (i = 0 ; i < strlen(valeur) ; i++){
        if ((valeur[i] >= '0') && (valeur[i] <= 'z')){
            chaine[j++] = valeur[i];
        }
    }
    chaine[j] = '\0';
    strcpy(valeur, chaine);

    //sprintf(chaine,"api.c / actualiseVariable => test data recuee => <%s> = <%s>", variable, valeur); Serial.println(chaine);
    
    if (strcmp(variable, "temperature") == 0){
        //sprintf(chaine, "api.c / actualiseVariable => mise a jour temperature : %d -> %s", mesDonneesApi.temperatureMesuree, valeur); Serial.println(chaine);
        int tmp = atoi(valeur);
        if (tmp != mesDonneesApi.temperatureMesuree){
            mesDonneesApi.temperatureMesuree = tmp;
            mesDonneesApi.temperatureMesureeModifiee = true;
            mesDonneesApi.refreshMesures = true;
            Serial.print("api.c / actualiseVariable => temperature mise a jour : "); Serial.println(mesDonneesApi.temperatureMesuree);
        }
    } else if (strcmp(variable, "heure") == 0){
        //Serial.println("mise a jour heure");
        int tmp = atoi(valeur);
        if (tmp != heure){
            heure = tmp;
            Serial.print("api.c / actualiseVariable => heure mise a jour : "); Serial.println(valeur);
            mesDonneesApi.heureModifiee = true;
        }
    } else if (strcmp(variable, "minute") == 0){
        //Serial.println("mise a jour minute");
        int tmp = atoi(valeur);
        if (tmp != minute){
            minute = tmp;
            Serial.print("api.c / actualiseVariable => minute mise a jour : "); Serial.println(valeur);
            mesDonneesApi.heureModifiee = true;
        }
    } else if (strcmp(variable, "second") == 0){
        //Serial.println("mise a jour second");
        int tmp = atoi(valeur);
        if (tmp != second){
            second = tmp;
            Serial.print("api.c / actualiseVariable => second mise a jour : "); Serial.println(valeur);
            mesDonneesApi.heureModifiee = true;
        }
    } else if (strcmp(variable, "annee") == 0){
        //Serial.println("mise a jour annee");
        int tmp = atoi(valeur);
        if (tmp != annee){
            annee = tmp;
            Serial.print("api.c / actualiseVariable => annee mise a jour : "); Serial.println(valeur);
            mesDonneesApi.dateModifiee = true;
        }
    } else if (strcmp(variable, "mois") == 0){
        //Serial.println("mise a jour mois");
        int tmp = atoi(valeur);
        if (tmp != mois){
            mois = tmp;
            Serial.print("api.c / actualiseVariable => mois mise a jour : "); Serial.println(valeur);
            mesDonneesApi.dateModifiee = true;
        }
    } else if (strcmp(variable, "jour") == 0){
        //Serial.println("mise a jour jour");
        int tmp = atoi(valeur);
        if (tmp != jour){
            jour = tmp;
            Serial.print("api.c / actualiseVariable => jour mise a jour : "); Serial.println(valeur);
            mesDonneesApi.dateModifiee = true;
        }
    } else if (strcmp(variable, "jourSemaine") == 0){
        //Serial.println("mise a jour jourSemaine");
        int tmp = atoi(valeur);
        if (tmp != jourSemaine){
            jourSemaine = tmp;
            Serial.print("api.c / actualiseVariable => jourSemaine mise a jour : "); Serial.println(valeur);
            mesDonneesApi.dateModifiee = true;
        }
    } else if (strcmp(variable, "consigne") == 0){
        //sprintf(chaine, "api.cpp/actualiseVariable => mise a jour consigne <%s=%s>", variable, valeur);
        //Serial.println(chaine);
        int tmp = atoi(valeur);
        if (tmp != mesDonneesApi.consigne){
            mesDonneesApi.consigne = tmp;
            Serial.print("api.c / actualiseVariable => consigne mise a jour : "); Serial.println(valeur);
            mesDonneesApi.parametresModifies = true;
        }
    } else if (strcmp(variable, "puissanceChauffage") == 0){
        //Serial.println("mise a jour puissanceChauffage");
        int tmp = atoi(valeur);
        if (tmp != mesDonneesApi.puissanceChauffage){
            mesDonneesApi.puissanceChauffage = tmp;
            Serial.print("api.c / actualiseVariable => puissanceChauffage mise a jour : "); Serial.println(valeur);
            mesDonneesApi.refreshMesures = true;
        }
    } else if (strcmp(variable, "chauffageOnOff") == 0){
        //sprintf(chaine, "api.cpp/actualiseVariable => mise a jour chauffageOnOff <%s=%s>", variable, valeur);
        //Serial.println(chaine);
        bool tmp;
        if (strcmp(valeur, "OFF") == 0) tmp = false; else tmp = true;
        if (tmp != mesDonneesApi.chauffageOnOff){
            mesDonneesApi.chauffageOnOff = tmp;
            Serial.print("api.c / actualiseVariable => chauffageOnOff mise a jour : "); Serial.println(valeur);
            mesDonneesApi.parametresModifies = true;
        }
    } else {
        sprintf(chaine, "api.c / actualiseVariable => couple variable=valeur invalide : <%s>=<%s>", variable, valeur); Serial.println(chaine);
        refreshNecessaire(variable);
    }
    //if (datasModified) afficheDatas();
}

//=========================================
//
//          updateDatas
//
//=========================================
void updateDatas(void){
    char response[TAILLE_REPONSE];

    updateHeure();
    

    if (!uneRequeteDejaActive){
        //--------------
        // refresh wifi
        //--------------
        //nbMillisecondUpdateWifi = millis();     // on desactive le refresh pour le moment
        if ((millis() - nbMillisecondUpdateWifi) >= DELAY_REFRESH_TEST_WIFI){     // update cnx wifi
            //Serial.println();
            //Serial.println("------------------- requete  wifi-------------------");
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
                uneRequeteDejaActive = true;
                delay(1000);
            }
            //Serial.println("api.cpp => uodateDatas ");
        } 
    }

    if (!uneRequeteDejaActive){
        //--------------
        // refresh NTP
        //--------------
        //nbMillisecondUpdateNTP = millis();     // on desactive le refresh pour le moment
        //if (annee < 2000) setGatewayrequest("getNtp");
        int refreshNtp = DELAY_REFRESH_NTP;
        if (annee < 2020) {
            refreshNtp = 5000;
        }
        if ((millis() - nbMillisecondUpdateNTP) >= refreshNtp){     // update ntp
            nbMillisecondUpdateNTP = millis();
            //Serial.println();
            //Serial.println("------------------- requete  ntp -------------------");
            //Serial.println("api.cpp->updateDatas => refresh ntp");
            setGatewayrequest("getNtp");
            uneRequeteDejaActive = true;
            delay(1000);
        } 
    }

    if (!uneRequeteDejaActive){
        //--------------
        // refresh Temp
        //--------------
        //nbMillisecondUpdateTemperature = millis();     // on desactive le refresh pour le moment
        if ((millis() - nbMillisecondUpdateTemperature) >= DELAY_REFRESH_TEMPERATURE){
            nbMillisecondUpdateTemperature = millis();
            //Serial.println();
            //Serial.println("------------------- requete temperature ------------");
            //Serial.println("api.cpp->updateDatas => refresh temperature");
            setGatewayrequest("getTemperature");
            uneRequeteDejaActive = true;
            delay(100);
        } 
    }

    if (!uneRequeteDejaActive){
        //------------------
        // refresh chauffage
        //------------------
        //nbMillisecondUpdateDonneesChauffage = millis();     // on desactive le refresh pour le moment
        if ((millis() - nbMillisecondUpdateDonneesChauffage) >= DELAY_REFRESH_DONNEE_CHAUFFAGE){
            nbMillisecondUpdateDonneesChauffage = millis();
            //Serial.println();
            //Serial.println("------------------- requete chauffage -----------");
            //Serial.println("api.cpp->updateDatas => refresh donnees chauffage");
            setGatewayrequest("getInfoChauffage");
            uneRequeteDejaActive = true;
            delay(100);
        }
    }

    //------------------
    // analyse reponse
    //------------------
    getGatewayResponse(response);
    if (strlen(response) > 0){
        //Serial.println("----------------------------------------------");
        //Serial.println("api.c / decomposition de la reponse : debut");
        char tmp[200];
        //sprintf(tmp, "api.c => reponse a decomposer <%s>", response); Serial.println(tmp);
        char variable[50], valeur[50];
        strcpy(variable, "");
        strcpy(valeur, "");
        int i, idxV = 0;
        bool isVariable = true;
        while (i <= strlen(response)){
            //sprintf(tmp, "analyse du caractere <%x>:<%c>", response[i], response[i]); Serial.println(tmp);
            if (isVariable) {       // on recupere les caracteres du nom de variable
                if (response[i] == '='){
                    isVariable = false;
                    //Serial.print("variable = "); Serial.println(variable);
                    strcpy(valeur, "");
                    idxV = 0;
                } else {
                    if (response[i] != '\n'){
                        variable[idxV++] = response[i];
                        variable[idxV] = '\0';
                    }
                }
            } else {        // on recupere les caracteres de la valeur
                if ((response[i] == '\n') || (response[i] == '\0')){
                    isVariable = true;
                    //Serial.print("valeur = "); Serial.println(valeur);
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
        delay(100);
        //sprintf(tmp, "api.c => decomposition de la reponse : fin ");
        //Serial.println(tmp);
    } else{
        uneRequeteDejaActive = false;
    }
}
