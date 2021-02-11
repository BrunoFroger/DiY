//
//
//  configPage.cpp
//
//

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "analyseHeader.hpp"
#include "globalDatas.hpp"

WiFiClient localClient;

//=========================================
//
//          sendLigne
//
//=========================================
void sendLigne (String ligne){
    sendMessage(localClient, ligne);
}

//=========================================
//
//          sendConfigPage
//
//=========================================
void sendConfigPage(WiFiClient client, String header){
    String message;

    localClient = client;
    char tmp[200];
    //char tmp2[25];

    sendHtmlHeader(client);

    sendLigne("<h1>Gateway chauffage</h1>");
    sendLigne("<br>");
    sendLigne("<p>Donnees NTP (derniere mise a jour)</p>");
    sendLigne("<p><a href=\"/updateNtp\">update NTP</a></p>");
    sendLigne("<table>");
        sendLigne("<tr>");
            sendLigne("<th>variable</th>");
            sendLigne("<th>valeur</th>");
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>annee</td>");
            sprintf(tmp, "<td>%d</td>", donneesGlobales.annee);
            sendLigne(tmp);
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>mois</td>");
            sprintf(tmp, "<td>%d</td>", donneesGlobales.mois);
            sendLigne(tmp);
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>jour</td>");
            sprintf(tmp, "<td>%d</td>", donneesGlobales.jour);
            sendLigne(tmp);
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>joursemaine</td>");
            sprintf(tmp, "<td>%d</td>", donneesGlobales.jourSemaine);
            sendLigne(tmp);
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>heure</td>");
            sprintf(tmp, "<td>%d</td>", donneesGlobales.heure);
            sendLigne(tmp);
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>minute</td>");
            sprintf(tmp, "<td>%d</td>", donneesGlobales.minute);
            sendLigne(tmp);
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>second</td>");
            sprintf(tmp, "<td>%d</td>", donneesGlobales.second);
            sendLigne(tmp);
        sendLigne("</tr>");
    sendLigne("</table>");

    //-----------------------------
    // table des donnees chauffage
    //-----------------------------
    sendLigne("<br>");
    sendLigne("<p>Donnees chauffage</p>");
    sendLigne("<table>");
        sendLigne("<tr>");
            sendLigne("<th>variable</th>");
            sendLigne("<th>valeur</th>");
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>consigne</td>");
            sprintf(tmp, "<td>%d.%d</td>", donneesGlobales.consigne/10, donneesGlobales.consigne%10);
            sendLigne(tmp);
            sendLigne("<td><a href=\"/incrementeConsigne\">+</td>");
            sendLigne("<td><a href=\"/decrementeConsigne\">-</td>");
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>temperature mesureee</td>");
            sprintf(tmp, "<td>%d.%d</td>", donneesGlobales.temperature/10, donneesGlobales.temperature%10);
            sendLigne(tmp);
            sendLigne("<td></td>");
            sendLigne("<td></td>");
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>chauffage on/off</td>");
            if (donneesGlobales.chauffageOnOff){
                sendLigne("<td><a href= \"swichChauffageOnOff\">ON</td>");
            } else {
                sendLigne("<td><a href= \"swichChauffageOnOff\">OFF</td>");
            }
            sendLigne("<td></td>");
            sendLigne("<td></td>");
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>puissance chauffage</td>");
            sprintf(tmp, "<td>%d</td>", donneesGlobales.puissanceChauffage); sendLigne(tmp);
            sendLigne("<td><a href=\"/incrementeChauffage\">+</td>");
            sendLigne("<td><a href=\"/decrementeChaufage\">-</td>");
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>mode verbose on/off</td>");
            if (donneesGlobales.modeVerbose){
                sendLigne("<td><a href= \"swichVerbose\">ON</td>");
            } else {
                sendLigne("<td><a href= \"swichVerbose\">OFF</td>");
            }
            sendLigne("<td></td>");
            sendLigne("<td></td>");
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>mode calcul temperature</td>");
            char mode[20];
            switch(donneesGlobales.modeCalculTemperature){
                case 1: strcpy(mode,"rampe"); break;
                case 2: strcpy(mode,"simul"); break;
                case 3: strcpy(mode,"mesure"); break;
                default : strcpy(mode,"");
            }
            sprintf(tmp, "<td>%s</td>", mode); sendLigne(tmp);
            sendLigne("<td><a href= \"incrementeModeCalculTemperature\">+</td>");
            sendLigne("<td><a href= \"derementeModeCalculTemperature\">-</td>");
        sendLigne("</tr>");
    sendLigne("</table>");

    //-----------------------------
    // table des donnees reseau
    //-----------------------------
    sendLigne("<br>");
    sendLigne("<p>Donnees reseau</p>");
    sendLigne("<table>");
        sendLigne("<tr>");
            sendLigne("<th>variable</th>");
            sendLigne("<th>valeur</th>");
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>nb clients connectes</td>");
            sprintf(tmp, "<td>%d</td>", donneesGlobales.nbClientsConnectes);
            sendLigne(tmp);
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>Ssid</td>");
            sprintf(tmp, "<td>%s</td>", donneesGlobales.wifiSsid);
            sendLigne(tmp);
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>Password</td>");
            sprintf(tmp, "<td>%s</td>", donneesGlobales.wifiPwd);
            sendLigne(tmp);
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>ipGateway</td>");
            sprintf(tmp, "<td>%s</td>", donneesGlobales.ipGateway);
            sendLigne(tmp);
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>nom gateway</td>");
            sprintf(tmp, "<td>%s</td>", donneesGlobales.nomGateway);
            sendLigne(tmp);
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>ipAfficheur</td>");
            sprintf(tmp, "<td>%s</td>", donneesGlobales.ipAfficheur);
            sendLigne(tmp);
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>nom afficheur</td>");
            sprintf(tmp, "<td>%s</td>", donneesGlobales.nomAfficheur);
            sendLigne(tmp);
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>ipPilotage chaudiere</td>");
            sprintf(tmp, "<td>%s</td>", donneesGlobales.ipPilotageChaudiere);
            sendLigne(tmp);
        sendLigne("</tr>");
        sendLigne("<tr>");
            sendLigne("<td>nom pilotage chaudiere</td>");
            sprintf(tmp, "<td>%s</td>", donneesGlobales.nomPilotageChaudiere);
            sendLigne(tmp);
        sendLigne("</tr>");
    sendLigne("</table>");
    sendLigne(htmlFooter);
}