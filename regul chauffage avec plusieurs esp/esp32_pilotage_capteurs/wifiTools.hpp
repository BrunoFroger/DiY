// ESP32_horloge
// wifiTools.hpp
// (c) B. Froger 2020

#ifndef __WIFITOOLS
#define __WIFITOOLS

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

typedef struct{
    char ssid[50];
    char password[50];
} TypeWifiNetwork;

extern TypeWifiNetwork tblWifiNetwork[10];

extern WiFiClient wifiClient;
extern WiFiServer wifiServer;
extern IPAddress gatewayIpAddress;

extern void initWifi(bool silence);
extern char *getSsid();
extern char *getIp();
extern boolean isWifiConnected();

#endif