//
//
//  api_capteurs.cpp
//
//
#include <Arduino.h>
#include <string.h>

#include "api_capteurs.hpp"
#include "wifiTools.hpp"


#define LOOP_DELAY 10

// definition des delais de rafraichissement des differentes donnees (en ms)
// il ne faut pas que deux delay aient la meme valeur
#define     DELAY_REFRESH_TEST_WIFI         1000*10     // 10s
#define     DELAY_REFRESH_DONNEE_CHAUFFAGE  1000*17/10  // 1,7s
#define     DELAY_REQUETE_ACTIVE                1000*60  // 1mn

unsigned long nbMillisecondUpdateWifi = 0;
unsigned long nbMillisecondUpdateDonneesChauffage = 0;
unsigned long nbMillisecondRequeteActive = 0;

capteursDatas mesDonneesCapteurs;
bool uneRequeteDejaActive = false;

//=========================================
//
//          afficheDatas
//
//=========================================
void afficheDatas(void){
    printf("api_capteurs.cpp => afficheDatas \n");
    char tmp[200];
    char status[10];
    Serial.println("+------------------+--------------------+");
    Serial.println("|  nom             |       valeur       |");
    Serial.println("+------------------+--------------------+");
    sprintf(tmp,   "| consigne         |  %10d        |", mesDonneesCapteurs.consigne);
    Serial.println(tmp);
    sprintf(tmp,   "| puiss chauffage  |  %10d        |", mesDonneesCapteurs.puissanceChauffage);
    Serial.println(tmp);
    if (mesDonneesCapteurs.chauffageOnOff) strcpy(status,"ON"); else strcpy(status,"OFF");
    sprintf(tmp,   "| chauffage On/Off |  %10s        |", status);
    Serial.println(tmp);
    Serial.println("+------------------+--------------------+");
    sprintf(tmp,   "| Ssid             |%17s   |", mesDonneesCapteurs.wifiSsid);
    Serial.println(tmp);
    sprintf(tmp,   "| Passwd           |%17s   |", mesDonneesCapteurs.wifiPwd);
    Serial.println(tmp);
    sprintf(tmp,   "| esp name         |%17s   |", mesDonneesCapteurs.espName);
    Serial.println(tmp);
    if (mesDonneesCapteurs.WifiConnected) strcpy(status,"true"); else strcpy(status,"false");
    sprintf(tmp,   "| wifi connected   |  %10s        |", status);
    Serial.println(tmp);
    sprintf(tmp,   "| IP gateway       |%17s   |", mesDonneesCapteurs.ipGateway);
    Serial.println(tmp);
    sprintf(tmp,   "| IP locale        |%17s   |", mesDonneesCapteurs.ipLocale);
    Serial.println(tmp);
    Serial.println("+------------------+--------------------+");
}

//=========================================
//
//          setGatewayrequest
//
//=========================================
bool setGatewayrequest(char *requete){
    char tmp[200];
    char tmp2[200];
    //Serial.print("setGatewayrequest => etat wificonnected avant envoi : ");
    //isWifiConnected();
    if (isWifiConnected()){
        //Serial.print("client.connect sur ");
        //Serial.println(mesDonneesCapteurs.gateway.toString());
        bool result = wifiClient.connect(mesDonneesCapteurs.gateway,80);
        if (result){
            sprintf(tmp, "GET /%s HTTP/1.1\n\n", requete);
            wifiClient.println(tmp);
            //sprintf(tmp2, "setGatewayrequest => requete <%s> envoyee", tmp); Serial.println(tmp2);
            uneRequeteDejaActive = true;
        } else {
            char buffer[100];
            //sprintf(buffer, "L'envoi de la requete %s/%s a echoue",mesDonneesCapteurs.ipGateway, requete );
            //Serial.println(buffer);
        }
        //isWifiConnected();
        //Serial.print("setGatewayrequest => etat wificonnected apres envoi : ");
        //Serial.println(mesDonneesCapteurs.WifiConnected);
        return true;
    } else {
        sprintf(tmp, "envoi requete %s impossible => wifi non connecte", requete); Serial.println(tmp);
        return false;
    }
}

//=========================================
//
//          initApi
//
//=========================================
void initApi(void){
    Serial.println("api_capteurs.cpp => init api");

    afficheDatas();
    char buffer[100];
    Serial.println("api_capteurs.cpp => envoi du nom de l'esp a la gateway");
    sprintf(buffer, "setName?nom=%s", mesDonneesCapteurs.espName);
    if (setGatewayrequest(buffer)){
        Serial.println("api_capteurs.cpp => envoi OK");
    } else {
        Serial.println("api_capteurs.cpp => envoi KO");
    }

    delay(1000);
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
        Serial.print("getGatewayResponse => nb car dans la reponse : ");
        Serial.println(wifiClient.available());
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
    
    if (strcmp(variable, "consigne") == 0){
        //sprintf(chaine, "api.cpp/actualiseVariable => mise a jour consigne <%s=%s>", variable, valeur);
        //Serial.println(chaine);
        int tmp = atoi(valeur);
        if (tmp != mesDonneesCapteurs.consigne){
            mesDonneesCapteurs.consigne = tmp;
            Serial.print("api.c / actualiseVariable => consigne mise a jour : "); Serial.println(valeur);
            mesDonneesCapteurs.parametresModifies = true;
        }
    } else if (strcmp(variable, "puissanceChauffage") == 0){
        //Serial.println("mise a jour puissanceChauffage");
        int tmp = atoi(valeur);
        if (tmp != mesDonneesCapteurs.puissanceChauffage){
            mesDonneesCapteurs.puissanceChauffage = tmp;
            Serial.print("api.c / actualiseVariable => puissanceChauffage mise a jour : "); Serial.println(valeur);
            mesDonneesCapteurs.refreshMesures = true;
        }
    } else if (strcmp(variable, "chauffageOnOff") == 0){
        //sprintf(chaine, "api.cpp/actualiseVariable => mise a jour chauffageOnOff <%s=%s>", variable, valeur);
        //Serial.println(chaine);
        bool tmp;
        if (strcmp(valeur, "OFF") == 0) tmp = false; else tmp = true;
        if (tmp != mesDonneesCapteurs.chauffageOnOff){
            mesDonneesCapteurs.chauffageOnOff = tmp;
            Serial.print("api.c / actualiseVariable => chauffageOnOff mise a jour : "); Serial.println(valeur);
            mesDonneesCapteurs.parametresModifies = true;
        }
    } else {
        sprintf(chaine, "api.c / actualiseVariable => couple variable=valeur invalide : <%s>=<%s>", variable, valeur); Serial.println(chaine);
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
    
    if (!uneRequeteDejaActive){
        //--------------
        // refresh wifi
        //--------------    
        int refreshWifi = DELAY_REFRESH_TEST_WIFI;
        if (!isWifiConnected()) {
            refreshWifi = 5000;
        }
        //nbMillisecondUpdateWifi = millis();     // on desactive le refresh pour le moment
        if ((millis() - nbMillisecondUpdateWifi) >= refreshWifi){     // update cnx wifi
            //Serial.println();
            Serial.println("------------------- requete  wifi-------------------");
            //Serial.println("api.cpp->updateDatas => refresh wifi");
            nbMillisecondUpdateWifi = millis();
            bool tmp = isWifiConnected();
            if (tmp != mesDonneesCapteurs.WifiConnected) {
                mesDonneesCapteurs.WifiConnected = tmp;
                mesDonneesCapteurs.parametresModifies = true;
                /*Serial.print("api.cpp => updateDatas : changement d'etat du wifi -> ");
                if (tmp){
                    Serial.println("true");
                } else {
                    Serial.println("false");
                }*/
            }
            if (!tmp) {
                initWifi(false);
                char buffer[100];
                sprintf(buffer, "setName?nom=capteurs");
                setGatewayrequest(buffer);
                delay(1000);
            }
            //Serial.println("api.cpp => uodateDatas ");
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
            Serial.println("------------------- requete chauffage -----------");
            //Serial.println("api.cpp->updateDatas => refresh donnees chauffage");
            setGatewayrequest("getInfoChauffage");
            delay(100);
        }
    }

    // test si le flag requete active est bloque
    // si plus d'une minute avec requete active a true => on debloque
    if (uneRequeteDejaActive){
        //nbMillisecondRequeteActive = millis();     // on desactive le refresh pour le moment
        if ((millis() - nbMillisecondRequeteActive) >= DELAY_REQUETE_ACTIVE){
            nbMillisecondRequeteActive = millis();
            Serial.println("------------------- deblocage requete active -----------");
            uneRequeteDejaActive = false;
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
