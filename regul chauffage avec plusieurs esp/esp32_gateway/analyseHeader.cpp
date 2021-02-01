//
//
//  analyseHeader.cpp
//
//

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "ntp.hpp"

String httpResponse = "";
String htmlHeader = "<body><h1>gateway chauffage</h1>";
String htmlFooter = "</body></html>\n";



//=================================================
//
//      analyseHeader
//
//=================================================
void analyseHeader(WiFiClient client, String header){
    Serial.println("analyseHeader : " + header);
    httpResponse = "";
    if (header.indexOf("GET /getNtp") >= 0) {
        Serial.println("requete get Ntp");
        httpResponse = htmlHeader;
        httpResponse += "second=" + second;
        httpResponse += "minute=" + minute;
        httpResponse += "heure=" + heure;
        httpResponse += "jour=" + jour;
        httpResponse += "jourSemaine=" + jourSemaine;
        httpResponse += "mois=" + mois;
        httpResponse += "annee=" + annee;
        httpResponse += htmlFooter;
    } else if (header.indexOf("GET /getTemperature") >= 0) {
        Serial.println("requete get temperature");
    } 
    client.println(httpResponse);
}


