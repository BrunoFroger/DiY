//
//
//  analyseHeader.hpp
//
//

#include <Arduino.h>

#ifndef __ANALYSEHEADER
#define __ANALYSEHEADER

extern String htmlHeader;
extern String htmlFooter;

extern void analyseHeader(WiFiClient client, String header, char *adresseIpClient);
extern void sendMessage(WiFiClient client, String texte);
extern void sendHtmlHeader(WiFiClient client);

#endif