// ESP32_horloge
// BFR_PA_Wifi.cpp
// (c) B. Froger 2020

#include <Arduino.h>
#include <string.h>

#include "BFR_PA_Wifi.hpp"

//=========================================
//
//          Constructeur
//
//=========================================
BFR_PA_Wifi::BFR_PA_Wifi(){
    //init(DEFAULT_ACCESS_POINT_NAME);
}

//=========================================
//
//          getAccesPointName
//
//=========================================
String BFR_PA_Wifi::getSsid(){
    //Serial.println("+----------------------------------+");
    //Serial.println("BFR_PA_Wifi::getSsid => debut");
    return this->ssid;
    //Serial.println("BFR_PA_Wifi::getSsid => fin");
    //Serial.println("+----------------------------------+");
}

//=========================================
//
//          getAccesPointName
//
//=========================================
IPAddress BFR_PA_Wifi::getIp(){
    return adresseIp;
}

//=========================================
//
//          getAccesPointName
//
//=========================================
IPAddress BFR_PA_Wifi::getGateway(){
    return gateway;
}

//=========================================
//
//          getAccesPointName
//
//=========================================
IPAddress BFR_PA_Wifi::getnetMask(){
    return netMask;
}

//=========================================
//
//          buildIPAdress
//
//=========================================
IPAddress BFR_PA_Wifi::buildIPAdress(String adresse){
    IPAddress tmpAddr;
    char tmpNum[5] = "";
    int j = 0;
    int k = 0;
    for (int i = 0; i < adresse.length() ; i++){
        if (adresse[i] != '.'){
            tmpNum[j++] = adresse[i];
        } else {
            tmpAddr[k++] = atoi(tmpNum);
            j=0;
        }
        tmpNum[j] = '\0';
    }
    Serial.print("adresse IP => ");
    Serial.println(tmpAddr);
    return tmpAddr;
}

//=========================================
//
//          init
//
//=========================================
void BFR_PA_Wifi::init(){
    init(DEFAULT_ACCESS_POINT_NAME);
}
void BFR_PA_Wifi::init(String ssid){
    init(ssid,DEFAULT_ACCESS_POINT_PWD);
}
void BFR_PA_Wifi::init(String ssid, String pwd){
    Serial.println("+----------------------------------+");
    Serial.println("BFR_PA_Wifi::init => debut");
    Serial.print("      Nom du point d'acces = ");
    Serial.print(ssid);
    Serial.println();
    Serial.print("      Password du point d'acces = ");
    Serial.print(pwd);
    Serial.println();
    this->ssid = ssid;
    this->passord = pwd;
    WiFi.softAP(ssid,pwd);
    this->adresseIp = buildIPAdress("192.168.10.1");
    //this->gateway = "192.168.4.1";
    //this->netMask = "255.255.255.0";
    WiFi.softAPConfig(this->adresseIp, this->gateway, this->netMask);
    Serial.print("      IP du point d'acces => ");
    Serial.print(this->adresseIp);
    Serial.println();
    Serial.println("BFR_PA_Wifi::init => fin");
    Serial.println("+----------------------------------+");
}