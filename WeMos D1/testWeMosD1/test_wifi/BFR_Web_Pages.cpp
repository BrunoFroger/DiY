// ESP32_horloge
// BFR_Web_Pages.cpp
// (c) B. Froger 2020

#include <Arduino.h>
#include "BFR_Web_Pages.hpp"
#include "BFR_serveurWeb.hpp"

String page;
extern BFR_PA_Wifi wifiAccessPoint;
extern BFR_serveurWeb myServerWeb;


//=========================================
//
//          web_Pages_Init
//
//=========================================
void web_Pages_Init(){
    page="";
}

//=========================================
//
//          web_Header
//
//=========================================
void web_Header(void){
    //Serial.println("BFR_Web_Pages : web_Header => debut"); 
    page = "";
    page += ""; //  do not forget this one
    page += "<!DOCTYPE HTML>\n";
    page += "<style type=\"text/css\">\n";
    page += "    div{\n";
    page += "        width:expression(document.body.clientWidth >= 1000? \"1000px\": \"auto\" );\n";
    page += "    }\n";
    page += "    table, th, td {\n";
    page += "        padding: 10px;\n";
    page += "        border: 1px solid black;\n";
    page += "        border-collapse: collapse;\n";
    page += "    }\n";
    page += "</style>\n";
    page += "<html>\n";
    page += "<head><title>WeMos D1 R1 (test Wifi)</title>\n";   //Pour l'onglet du navigateur
    page += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, minimum-scale=1, maximum-scale=1, user-scalable=no\" />\n";
    page += "<meta charset=\"utf-8\" />\n"; 
    //Pour eviter l'emission de  'GET /favicon.ico HTTP/1.1' à chaque requete
    page += "<link rel=\"icon\" href=\"data:,\">\n";
    page += "</head>\n";
    page += "<body>\n";
    page += "<div>\n";
    //Serial.println("BFR_Web_Pages : web_Header => fin"); 
}

//=========================================
//
//          web_Footer
//
//=========================================
void web_Footer(void){
    //Serial.println("BFR_Web_Pages :  : web_Footer => debut"); 
    page += "</div><br><br>\n";
    page += "</body>\n";
    page += "</html>\n";  //Fin de la page
    myServerWeb.send(page);
    //Serial.println(page);
    //Serial.println("BFR_Web_Pages : web_Footer => fin"); 
}

//=========================================
//
//          web_HomePage
//
//=========================================
void web_HomePage(){
    Serial.println("+----------------------------------+");
    Serial.println("BFR_Web_Pages : displayHomePage => debut");
    web_Header();
    page += "<h2>test_Wifi sur WeMos D1 R1 Mini</h2>\n";
    page += "<p>";
    page+= "<a href= \" /config \" >configuration</a>\n";
    page += "<p>";
    web_Footer();
    Serial.println("BFR_Web_Pages : displayHomePage => fin");
    Serial.println("+----------------------------------+");
}

//=========================================
//
//          web_ConfigPage
//
//=========================================
void web_ConfigPage(){
    Serial.println("+----------------------------------+");
    Serial.println("BFR_Web_Pages : web_ConfigPage => debut");
    web_Header();
    page += "<h2>Page de configuration</h2>\n";
    page += "<p>local IP = " + myServerWeb.getLocalIP();
    page += "<p>gateway  = " + myServerWeb.getGateway();
    page += "<p>ssid     = " + myServerWeb.getSsid();
    web_Footer();
    Serial.println("BFR_Web_Pages : web_ConfigPage => fin");
    Serial.println("+----------------------------------+");
}
