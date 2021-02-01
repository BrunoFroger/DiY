// ESP32_horloge
// BFR_serveurWeb.hpp
// (c) B. Froger 2020

#include <Arduino.h>
#include <string.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "BFR_PA_Wifi.hpp"

//using namespace std;

#ifndef __BFR_SERVEURWEB
#define __BFR_SERVEURWEB


class BFR_serveurWeb{

    public:
        // attributs

        //Methodes
            BFR_serveurWeb();
            void init();
            void send(String page);
            String getLocalIP();
            String getGateway();
            String getSsid();
            void handleClient();

    private:
        // attributs
            WiFiServer serveur();
            String ssid;
            String password;
            String adresseIp;
            String gatewayIp;


        //Methodes
            void displayHeader();
            void displayFooter();
            void displayHomePage();

};

#endif