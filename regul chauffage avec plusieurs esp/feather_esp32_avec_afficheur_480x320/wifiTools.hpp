// ESP32_horloge
// wifiTools.hpp
// (c) B. Froger 2020

#ifndef __WIFITOOLS
#define __WIFITOOLS

#include <WiFi.h>

extern WiFiClient wifiClient;
//extern WiFiServer wifiServer;
//extern IPAddress gatewayIp;

extern void initWifi(bool silence);
extern char *getSsid();
extern char *getIp();
extern boolean isWifiConnected(bool silence);

#endif