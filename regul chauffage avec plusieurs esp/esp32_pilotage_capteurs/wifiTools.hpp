// ESP32_horloge
// wifiTools.hpp
// (c) B. Froger 2020

#ifndef __WIFITOOLS
#define __WIFITOOLS

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

extern WiFiClient wifiClient;
extern WiFiServer wifiServer;
extern IPAddress gatewayIpAddress;

extern void initWifi(bool silence);
extern char *getSsid();
extern char *getIp();
extern boolean isWifiConnected();

#endif