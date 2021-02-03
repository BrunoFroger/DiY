//
//
//  configPage.hpp
//
//

#ifndef __CONFIGPAGE
#define __CONFIGPAGE


#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

extern void sendConfigPage(WiFiClient client, String header);

#endif
