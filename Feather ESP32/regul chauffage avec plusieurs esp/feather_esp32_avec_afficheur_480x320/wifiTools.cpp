// ESP32_horloge
// wifiTools.cpp
// (c) B. Froger 2020


#include <Arduino.h>
#include <string.h>
#include <WiFi.h>

#include "wifiTools.hpp"

char ipAdress[20];
char wifiSsid[25];
char wifiPassword[50];
int cptTryWifi = 0;
boolean wifiConnected = false;
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
//          initWifi
//
//=========================================
void initWifi(void){    // init wifi connection

    //Serial.println("initWifi => debut");
    delay(100);
    //Serial.println("initWifi => check wifi status");
    if (WiFi.status() == WL_NO_SHIELD){
        Serial.println("initWifi => ERROR : No shield detected !!");
        return;
    }
    //Serial.println("initWifi => a shield is detected");
    delay(1000);
    //Serial.println("initWifi => set wifi mode to WIFI_STA : OK");
    deconnecteWifi();
    delay(1000);
    cptTryWifi = 0;

    // TODO mettre a jour avec les identifiants du serveur web central
    strcpy(wifiSsid,"TP-LINK_88E2");
    strcpy(wifiPassword, "74546007");
    
    // Connect to WiFi network
    Serial.print("Connecting to ");
    Serial.println(wifiSsid);
    WiFi.begin(wifiSsid, wifiPassword);
    int cpt=0;
    int cpt2=0;
    while (WiFi.status() != WL_CONNECTED) {  //Attente de la connexion
        delay(500);
        //char tmp[10];
        //sprintf(tmp,"%d,",cpt);
        //Serial.print(tmp);
        Serial.print(".");   //Typiquement 5 à 10 points avant la connexion
        if (cpt++ >= 5){
            Serial.println();
            cpt=0;
            WiFi.begin(wifiSsid, wifiPassword);
        }
        if (cpt2++ > 20){
            cpt2=0;
            cpt=0;
        }
    }
    Serial.println("");
    wifiConnected = true;
    Serial.println("WiFi connected");

    // Print the IP address
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    IPAddress tmpIp = WiFi.localIP();
    sprintf(ipAdress,"%d.%d.%d.%d",tmpIp[0],tmpIp[1],tmpIp[2],tmpIp[3]);
    //Serial.print(WiFi.localIP());
    Serial.print(ipAdress);
    Serial.println("/");  //Utiliser cette URL sous Firefox de preference à Chrome

    //Serial.println("initWifi => fin");
}

//=========================================
//
//          getSsid
//
//=========================================
char *getSsid(){
    return wifiSsid;
}

//=========================================
//
//          getIp
//
//=========================================
char *getIp(){
    return ipAdress;
}

//=========================================
//
//          isWifiConnected
//
//=========================================
boolean isWifiConnected(){
    return wifiConnected;
}
