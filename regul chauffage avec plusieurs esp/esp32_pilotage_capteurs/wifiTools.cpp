// ESP32_horloge
// wifiTools.cpp
// (c) B. Froger 2020


#include <Arduino.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "wifiTools.hpp"
#include "api_capteurs.hpp"

//char ipAdress[20];
//char wifiSsid[25];
//char wifiPassword[50];
int cptTryWifi = 0;
boolean wifiFound = false;
WiFiClient wifiClient;
WiFiServer wifiServer(80);//Ecouter le port 80

//=========================================
//
//          deconnecteWifi
//
//=========================================
void deconnecteWifi(){
    //Serial.println("deconnecteWifi => debut");
    if (WiFi.status() == WL_CONNECTED){
        //Serial.println("deconnecteWifi => try");
        delay(100);
        WiFi.disconnect();
        //Serial.println("deconnecteWifi => OK");
    }
    //Serial.println("deconnecteWifi => fin");
}

//=========================================
//
//          scanNetworks
//
//=========================================
void scanNetworks(void){    // search for availables Wifi Networks
    int nbSsid = WiFi.scanNetworks();
    if (nbSsid != -1){
        Serial.print(nbSsid);
        Serial.println(" wifi networks found");
        strcpy(wifiSsid,"");
        while (strcmp(wifiSsid,"") == 0){        
            for (int ssidNetwork = 0 ; ssidNetwork < nbSsid ; ssidNetwork++){
                Serial.print("  check network : ");
                Serial.print(WiFi.SSID(ssidNetwork));
                int availableSsidIndex = isAvailableAccesPoint(WiFi.SSID(ssidNetwork));
                if (availableSsidIndex != -1){
                    char tmp[30];
                    getSsid(availableSsidIndex).toCharArray(tmp,25);
                    //Serial.print(" on a trouve : ");
                    strcpy(wifiSsid,tmp);
                    //Serial.print(wifiSsid);
                    //Serial.print("/");
                    getPwd(availableSsidIndex).toCharArray(tmp,30);
                    strcpy(wifiPassword,tmp);
                    //Serial.print(wifiPassword);
                    Serial.println(" => OK");
                    wifiFound = true;
                    break;
                }
                Serial.println(" => NOK");
            }
            if (strcmp(wifiSsid,"") == 0){
                delay(2000);
                cptTryWifi++;
                Serial.printf("Essai %d de connexion Wifi\n",cptTryWifi);
                if (cptTryWifi > 2){
                    Serial.println("Aucun réseau détecté .. on passe");
                    wifiFound = false;
                    break;
                }
                Serial.println("No Wifi network found ==> rescan ......");
                nbSsid = WiFi.scanNetworks();
            }
        }
    }
}
//=========================================
//
//          initWifi
//
//=========================================
void initWifi(bool silence){    // init wifi connection

    if (!silence){
        Serial.println("initWifi => debut");
    } else {
        Serial.print("try wifi ....");
    }
    delay(100);
    //Serial.println("initWifi => check wifi status");
    if (WiFi.status() == WL_NO_SHIELD){
        Serial.println("initWifi => ERROR : No shield detected !!");
        return;
    }
    if (!silence) Serial.println("initWifi => a shield is detected");
    delay(1000);
    deconnecteWifi();
    delay(1000);
    cptTryWifi = 0;

    // TODO mettre a jour avec les identifiants du serveur web central
    //strcpy(wifiSsid,"gateway-chauffage");
    //strcpy(wifiPassword, "0296911369");
    //strcpy(wifiPassword, "");
    
    // Connect to WiFi network
    if (!silence) {
        Serial.print("Connecting to ");
        Serial.println(mesDonneesCapteurs.wifiSsid);
    }
    WiFi.mode(WIFI_STA);
    delay(100);
    WiFi.begin(mesDonneesCapteurs.wifiSsid, mesDonneesCapteurs.wifiPwd);
    int cpt=0;
    int cpt2=0;
    int connected = WiFi.status() != WL_CONNECTED;
    while (connected) {  //Attente de la connexion
        delay(500);
        char buffer[100];
        sprintf(buffer,"ssid = %s, pwd = %s", mesDonneesCapteurs.wifiSsid, mesDonneesCapteurs.wifiPwd); Serial.println(buffer);
        Serial.print("wifiStatus = "); Serial.println(connected);
        WiFi.begin(mesDonneesCapteurs.wifiSsid, mesDonneesCapteurs.wifiPwd);
        if (!silence) Serial.print(".");   //Typiquement 5 à 10 points avant la connexion
        if (cpt++ >= 5){
            if (!silence) Serial.println();
            cpt=0;
        }
        if (cpt2++ > 20){
            break;
        }
    connected = WiFi.status() != WL_CONNECTED;
    }
    Serial.println();

    if (cpt2 > 20){
        // ona  fait 20 tentatives
        // imposible de se connecter au wifi !
        if (!silence) {
            Serial.println();
            Serial.println("Wifi non connecte");
        } else {
            Serial.println(" => KO ; pas de wifi");
        }
        mesDonneesCapteurs.WifiConnected = false;
        return;
    } else {
        // on a reussit a se connecter au wifi
        Serial.println("");
        mesDonneesCapteurs.WifiConnected = true;
        if (!silence) {
            Serial.println("WiFi connecte");
        } else {
            Serial.println(" => OK");
        }
    }

    // Print the IP address
    if (!silence) Serial.print("ip locale = ");
    IPAddress tmpIp = WiFi.localIP();
    sprintf(mesDonneesCapteurs.ipLocale,"%d.%d.%d.%d",tmpIp[0],tmpIp[1],tmpIp[2],tmpIp[3]);
    if (!silence) Serial.print(mesDonneesCapteurs.ipLocale);

    //IPAddress gatewayIp = WiFi.gatewayIP();
    tmpIp = WiFi.gatewayIP();
    mesDonneesCapteurs.gateway = tmpIp;
    sprintf(mesDonneesCapteurs.ipGateway,"%d.%d.%d.%d",tmpIp[0],tmpIp[1],tmpIp[2],tmpIp[3]);
    if (!silence) Serial.print("La gateway est : ");
    if (!silence) Serial.println(mesDonneesCapteurs.ipGateway);

    if (!silence) Serial.println("initWifi => fin");
}

//=========================================
//
//          getSsid
//
//=========================================
char *getSsid(){
    return mesDonneesCapteurs.wifiSsid;
}

//=========================================
//
//          getIp
//
//=========================================
char *getIp(){
    return mesDonneesCapteurs.ipLocale;
}

//=========================================
//
//          isWifiConnected
//
//=========================================
boolean isWifiConnected(){
    if (WiFi.status() == WL_CONNECTED) {
        //Serial.println("wifiTools.cpp => isWifiConnected : TRUE");
        mesDonneesCapteurs.WifiConnected = true;
    } else {
        //Serial.println("wifiTools.cpp => isWifiConnected : FALSE");
        mesDonneesCapteurs.WifiConnected = false;
    }
    //wifiConnected = (WiFi.status() == WL_CONNECTED);
    return mesDonneesCapteurs.WifiConnected;
}
