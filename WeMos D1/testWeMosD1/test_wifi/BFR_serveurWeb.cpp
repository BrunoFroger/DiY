// ESP32_horloge
// BFR_serveurWeb.cpp
// (c) B. Froger 2020


#include "BFR_serveurWeb.hpp"
#include "BFR_Web_Pages.hpp"

extern BFR_PA_Wifi wifiAccessPoint;
extern BFR_serveurWeb myServerWeb;


//=========================================
//
//          Constructeur
//
//=========================================
BFR_serveurWeb::BFR_serveurWeb(){
}

//=========================================
//
//          init
//
//=========================================
void BFR_serveurWeb::init(){
    Serial.println("+----------------------------------+");
    Serial.println("BFR_serveurWeb::init => debut");

    this->serveur.begin();
    Serial.println("        wifiServer started");

    Serial.println("        infos serveur");
    Serial.println("            Adresse IP = " + wifiAccessPoint->getIp());
    Serial.println("            Gateway    = " + wifiAccessPoint->getGateway());

    Serial.println("        activation des pages");
    serveur.on("/", web_HomePage);
    Serial.println("            web_HomePage activee");
    serveur.on("/config", web_ConfigPage);
    Serial.println("            web_ConfigPage activee");

    Serial.println("BFR_serveurWeb::init => fin");
    Serial.println("+----------------------------------+");
}


//=========================================
//
//          send
//
//=========================================
void BFR_serveurWeb::send(String page){
     serveur.send( 200, "text/html", page);
}


//=========================================
//
//          getLocalIP
//
//=========================================
String BFR_serveurWeb::getLocalIP(){
    return this->adresseIp;
}

//=========================================
//
//          getGateway
//
//=========================================
String BFR_serveurWeb::getGateway(){
    return this->gatewayIp;
}


//=========================================
//
//          getSsid
//
//=========================================
String BFR_serveurWeb::getSsid(){
    return this->ssid;
}


//=========================================
//
//          handleClient
//
//=========================================
void BFR_serveurWeb::handleClient(){
    serveur->handleClient();
}

