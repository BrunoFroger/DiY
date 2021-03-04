// ESP32_horloge
// wifiTools.cpp
// (c) B. Froger 2020


#include <Arduino.h>
#include <string.h>
#include <WiFi.h>

#include "wifiTools.hpp"
#include "api.hpp"

//char ipAdress[20];
//char wifiSsid[25];
//char wifiPassword[50];
int cptTryWifi = 0;
boolean wifiConnected = false;
boolean wifiFound = false;
WiFiClient wifiClient;
WiFiServer wifiServer(80);//Ecouter le port 80
IPAddress gatewayIp;

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
//          initWifi
//
//=========================================
void initWifi(bool silence){    // init wifi connection

    if (!silence){
        Serial.println("initWifi => debut");
    } else {
        Serial.println("try wifi ....");
    }
    delay(100);
    //Serial.println("initWifi => check wifi status");
    /*if (WiFi.status() == WL_NO_SHIELD){
        Serial.println("initWifi => ERROR : No shield detected !!");
        return;
    }*/
    //Serial.println("initWifi => a shield is detected");
    delay(1000);
    //Serial.println("initWifi => set wifi mode to WIFI_STA : OK");
    deconnecteWifi();
    delay(1000);
    cptTryWifi = 0;

    // TODO mettre a jour avec les identifiants du serveur web central
    
    // Connect to WiFi network
    if (!silence) Serial.print("Connecting to ");
    if (!silence) Serial.println(mesDonneesApi.wifiSsid);
    WiFi.begin(mesDonneesApi.wifiSsid, mesDonneesApi.wifiPwd);
    int cpt=0;
    int cpt2=0;
    while (WiFi.status() != WL_CONNECTED) {  //Attente de la connexion
        delay(500);
        //Serial.print("wifiStatus = "); Serial.println(WiFi.status());
        //char tmp[10];
        //sprintf(tmp,"%d,",cpt);
        //Serial.print(tmp);
        WiFi.begin(mesDonneesApi.wifiSsid, mesDonneesApi.wifiPwd);
        if (!silence) Serial.print(".");   //Typiquement 5 à 10 points avant la connexion
        if (cpt++ >= 5){
            if (!silence) Serial.println();
            cpt=0;
        }
        if (cpt2++ > 20){
            break;
        }
    }
    if (cpt2 > 20){
        if (!silence) Serial.println("Wifi non connecte");
    } else {
        Serial.println("");
        wifiConnected = true;
        if (!silence) Serial.println("WiFi connected");
    }

    // Print the IP address
    if (!silence) Serial.print("Use this URL to connect: ");
    if (!silence) Serial.print("http://");
    IPAddress tmpIp = WiFi.localIP();
    sprintf(mesDonneesApi.ipLocale,"%d.%d.%d.%d",tmpIp[0],tmpIp[1],tmpIp[2],tmpIp[3]);
    //Serial.print(WiFi.localIP());
    if (!silence) Serial.print(mesDonneesApi.ipLocale);
    if (!silence) Serial.println("/");  //Utiliser cette URL sous Firefox de preference à Chrome

    //IPAddress gatewayIp = WiFi.gatewayIP();
    gatewayIp = WiFi.gatewayIP();
    sprintf(mesDonneesApi.ipGateway,"%d.%d.%d.%d",gatewayIp[0],gatewayIp[1],gatewayIp[2],gatewayIp[3]);
    if (!silence) Serial.print("La gateway est : ");
    if (!silence) Serial.println(mesDonneesApi.ipGateway);

    if (!silence) Serial.println("initWifi => fin");
}

//=========================================
//
//          getSsid
//
//=========================================
char *getSsid(){
    return mesDonneesApi.wifiSsid;
}

//=========================================
//
//          getIp
//
//=========================================
char *getIp(){
    return mesDonneesApi.ipLocale;
}

//=========================================
//
//          isWifiConnected
//
//=========================================
boolean isWifiConnected(){
    if (WiFi.status() == WL_CONNECTED) {
        //Serial.println("wifiTools.cpp => isWifiConnected : TRUE");
        wifiConnected = true;
    } else {
        //Serial.println("wifiTools.cpp => isWifiConnected : FALSE");
        wifiConnected = false;
    }
    //wifiConnected = (WiFi.status() == WL_CONNECTED);
    return wifiConnected;
}
