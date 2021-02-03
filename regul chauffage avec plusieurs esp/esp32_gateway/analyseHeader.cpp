//
//
//  analyseHeader.cpp
//
//

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "ntp.hpp"
#include "temperature.hpp"
#include "configPage.hpp"
#include "globalDatas.hpp"

String htmlStyleTable  = "HTTP/1.1 200 OK Content-Type: text/html<!DOCTYPE HTML><style> div {width: 100%;} table, th, td {padding: 10px;border: 1px solid black;border-collapse: collapse;}</style>";
String htmlTitre       = "<h1>gateway chauffage</h1>";
String htmlHeader      = htmlStyleTable + "<body><div>" + htmlTitre ;
String htmlFooter      = "</div></body></html>";

#define TAILLE_MESSAGE  1000
String message = "";

//=================================================
//
//      buildMessage
//
//=================================================
void sendMessage(WiFiClient client, String texte){
    client.println(texte);
    delay(2);
}
void sendHtmlHeader(WiFiClient client){
    client.println("HTTP/1.1 200 OK ");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<style type=\"text/css\">");
    client.println("    div {width: 100%;} ");
    client.println("    table, th, td {padding: 10px;border: 1px solid black;border-collapse: collapse;}");
    client.println("</style>");
    client.println("<html>");
    client.println("<head>");
    client.println("    <title>Gateway chauffage</title>");
    client.println("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1, minimum-scale=1, maximum-scale=1, user-scalable=no\" />");
    client.println("    <meta charset=\"utf-8\" />");
    client.println("    <link rel=\"icon\" href=\"data:,\">");
    client.println("</head>");
    client.println("<body>");
    client.println("<div>");

}

//=================================================
//
//      analyseHeader
//
//=================================================
void analyseHeader(WiFiClient client, String header){
    char tmp[200];
    message = "";
    //Serial.println("analyseHeader : " + header);
    if (header.indexOf("GET /getNtp") >= 0) {
        refreshNtpNow();
        delay(500);
        Serial.println("requete get Ntp traitee");
        message = "second=" + String(donneesGlobales.second);
        sendMessage(client, message);
        message = "minute=" + String(donneesGlobales.minute);
        sendMessage(client, message);
        message = "heure=" + String(donneesGlobales.heure);
        sendMessage(client, message);
        message = "jour=" + String(donneesGlobales.jour);
        sendMessage(client, message);
        message = "jourSemaine=" + String(donneesGlobales.jourSemaine);
        sendMessage(client, message);
        message = "mois=" + String(donneesGlobales.mois);
        sendMessage(client, message);
        message = "annee=" + String(donneesGlobales.annee);
        sendMessage(client, message);
    } else if (header.indexOf("GET /getTemperature") >= 0) {
        Serial.println("requete get temperature traitee");
        message = "temperature=" + String(getTemperature());
        sendMessage(client, message);
    } else if (header.indexOf("GET /config") >= 0) {
        Serial.println("requete config");
        sendConfigPage(client, header);
    } else if (header.indexOf("GET /swichChauffageOnOff") >= 0) {
        donneesGlobales.chauffageOnOff = !donneesGlobales.chauffageOnOff;
        sendConfigPage(client, header);
    } else if (header.indexOf("GET /incrementeConsigne") >= 0) {
        donneesGlobales.consigne++;
        sendConfigPage(client, header);
    } else if (header.indexOf("GET /decrementeConsigne") >= 0) {
        donneesGlobales.consigne--;
        sendConfigPage(client, header);
    } else if (header.indexOf("GET / ") >= 0) {
        Serial.println("requete get vide traitee");
        client.println("ca marche");
    } else {
        client.println("404 page inconnue");
    }
}


