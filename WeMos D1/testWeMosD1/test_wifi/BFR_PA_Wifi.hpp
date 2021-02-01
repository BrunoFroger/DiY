// ESP32_horloge
// BFR_PA_Wifi.hpp
// (c) B. Froger 2020


#include <Arduino.h>
#include <string.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#ifndef __BFR_PA_WIFI
#define __BFR_PA_WIFI

#define DEFAULT_ACCESS_POINT_NAME   "WeMosD1R1_AP"
#define DEFAULT_ACCESS_POINT_PWD    "4587123609"

class BFR_PA_Wifi{

    public:
        // attributs

        //Methodes
            BFR_PA_Wifi();
            void init();
            void init(String ssid);
            void init(String ssid, String pwd);
            String getSsid();
            IPAddress getIp();
            IPAddress getGateway();
            IPAddress getnetMask();

    private:
        // attributs
            String ssid;
            String passord;
            int channel;
            boolean hidden;
            int max_connexions;
            IPAddress adresseIp;
            IPAddress gateway;
            IPAddress netMask;
        //Methodes
            IPAddress buildIPAdress(String adresse);

};

#endif