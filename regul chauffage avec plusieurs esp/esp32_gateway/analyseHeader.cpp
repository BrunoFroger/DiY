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

String htmlHeader = "<body><h1>gateway chauffage</h1>";
String htmlFooter = "</body></html>";

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
        //client.println(htmlHeader);
        //sprintf(tmp, "reponse getNtp");
        //client.println(tmp);
        message = "second=" + String(second);
        sendMessage(client, message);
        message = "minute=" + String(minute);
        sendMessage(client, message);
        message = "heure=" + String(heure);
        sendMessage(client, message);
        message = "jour=" + String(jour);
        sendMessage(client, message);
        message = "jourSemaine=" + String(jourSemaine);
        sendMessage(client, message);
        message = "mois=" + String(mois);
        sendMessage(client, message);
        message = "annee=" + String(annee);
        sendMessage(client, message);
        /*
        sprintf(tmp, "second=%d",second);
        client.println(tmp);
        sprintf(tmp, "minute=%d",minute);
        client.println(tmp);
        sprintf(tmp, "heure=%d",heure);
        client.println(tmp);
        sprintf(tmp, "jour=%d",jour);
        client.println(tmp);
        sprintf(tmp, "jourSemaine=%d",jourSemaine);
        client.println(tmp);
        sprintf(tmp, "mois=%d",mois);
        client.println(tmp);
        sprintf(tmp, "annee=%d",annee);
        client.println(tmp);
        */
        //client.println(htmlFooter);
    } else if (header.indexOf("GET /getTemperature") >= 0) {
        Serial.println("requete get temperature traitee");
        message = "temperature=" + String(getTemperature());
        sendMessage(client, message);
        //client.println(htmlHeader);
        //sprintf(tmp, "reponse getTemperature");
        //client.println(tmp);
        //sprintf(tmp, "temperature=%d",getTemperature());
        //client.println(tmp);
        //client.println(htmlFooter);
    } else if (header.indexOf("GET / ") >= 0) {
        Serial.println("requete get vide traitee");
        //client.println(htmlHeader);
        client.println("ca marche");
        //client.println(htmlFooter);
    } else {
        //client.println(htmlHeader);
        client.println("404 page inconnue");
        //client.println(htmlFooter);
    }
    //client.println();
}


