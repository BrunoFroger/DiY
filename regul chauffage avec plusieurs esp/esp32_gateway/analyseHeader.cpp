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
//      setName
//
//=================================================
void setName(String header, char *adresseIpClient){
    char buffer[200];
    char tmp[100];
    header.toCharArray(tmp,100);
    sprintf(buffer,"analyseHeader/setName => header = %s", tmp); Serial.println(buffer);
}

//=================================================
//
//      sendMessage
//
//=================================================
void sendMessage(WiFiClient client, String texte){
    client.println(texte);
    if (donneesGlobales.modeVerbose) Serial.println("analyseHeader/sendMessage => " + texte);
    delay(2);
}

//=================================================
//
//      sendHtmlHeader
//
//=================================================
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
void analyseHeader(WiFiClient client, String header, char *adresseIpClient){
    char tmp[200];
    message = "";
    //if (donneesGlobales.modeVerbose) Serial.println("analyseHeader : " + header);
    if (header.indexOf("GET /getNtp") >= 0) {
        refreshNtpNow();
        delay(500);
        if (donneesGlobales.modeVerbose) Serial.println("requete get Ntp traitee");
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
        if (donneesGlobales.modeVerbose) Serial.println("requete get temperature traitee");
        message = "temperature=" + String(getTemperature());
        sendMessage(client, message);
        if (donneesGlobales.modeVerbose) Serial.println(message);
    } else if (header.indexOf("GET /getInfoChauffage") >= 0) {
        if (donneesGlobales.modeVerbose) Serial.println("requete get info chauffage traitee");
        message = "puissanceChauffage=" + String(donneesGlobales.puissanceChauffage);
        sendMessage(client, message);
        message = "consigne=" + String(donneesGlobales.consigne);
        sendMessage(client, message);
        if (donneesGlobales.chauffageOnOff){
            message = "chauffageOnOff=ON";
        } else {
            message = "chauffageOnOff=OFF";
        }
        sendMessage(client, message);
    } else if (header.indexOf("GET /config") >= 0) {
        if (donneesGlobales.modeVerbose) Serial.println("requete config");
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
    } else if (header.indexOf("GET /incrementeChauffage") >= 0) {
        donneesGlobales.puissanceChauffage++;
        sendConfigPage(client, header);
    } else if (header.indexOf("GET /decrementeConsigne") >= 0) {
        donneesGlobales.consigne--;
        sendConfigPage(client, header);
    } else if (header.indexOf("GET /incrementeModeCalculTemperature") >= 0) {
        donneesGlobales.modeCalculTemperature++;
        if (donneesGlobales.modeCalculTemperature > 3) donneesGlobales.modeCalculTemperature = 1;
        sendConfigPage(client, header);
    } else if (header.indexOf("GET /derementeModeCalculTemperature") >= 0) {
        donneesGlobales.modeCalculTemperature--;
        if (donneesGlobales.modeCalculTemperature < 1) donneesGlobales.modeCalculTemperature = 3;
        sendConfigPage(client, header);
    } else if (header.indexOf("GET /updateNtp") >= 0) {
        refreshNtpNow();
        sendConfigPage(client, header);
    } else if (header.indexOf("GET / ") >= 0) {
        if (donneesGlobales.modeVerbose) Serial.println("requete get vide traitee");
        client.println("ca marche");
    } else if (header.indexOf("GET /swichVerbose") >= 0) {
        donneesGlobales.modeVerbose = !donneesGlobales.modeVerbose;
        sendConfigPage(client, header);
    } else if (header.indexOf("GET /setName") >= 0) {
        Serial.println("SetNom recu");
        setName(header, adresseIpClient);
    } else {
        client.println("404 page inconnue");
    }
}


