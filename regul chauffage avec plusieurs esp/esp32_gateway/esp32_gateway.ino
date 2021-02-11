//
//
//  ESP 32 Wemos gateway accessPoint Wifi
//
//

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "ntp.hpp"
#include "webRequest.hpp"
#include "globalDatas.hpp"
#include "meteo.hpp"

#define LOOP_DELAY 10

// Replace with your network credentials
//const char* ssid     = "gateway-chauffage";
//const char* password = "0296911369";
//const char* password = "";

#define     DELAY_REFRESH_AFFICHAGE_DATAS   1000*2      //2s
unsigned long nbMillisecondDisplayDatas = 0;

#define     DELAY_REFRESH_OPENWEATHERMAP   1000*10      //10s
unsigned long nbMillisecondOpenweathermap = 0;

// Set web server port number to 80
WiFiServer server(80);


// Variable to store the HTTP request
//String header;

WiFiClient client;

//=================================================
//
//      setup
//
//=================================================
void setup() { 
    // initialize serial communication
    //Serial.begin(115200);
    //Serial.begin(230400);
    Serial.begin(115200);
    int timeoutInitSerial = 100;
    while (timeoutInitSerial-- > 0)
    {
        if (Serial)
            break;
        delay(10);
    }
    delay(1000);
    Serial.println("Serial initialized");    
    Serial.println("+-------------------------------+");
    Serial.println("+                               +");
    Serial.println("+      WeMos ESP32 Gateway      +");
    Serial.println("+                               +");
    Serial.println("+-------------------------------+");

    strcpy(donneesGlobales.wifiSsid, "gateway-chauffage");
    strcpy(donneesGlobales.wifiPwd, "0296911369");

    initGlobalDatas();

    Serial.print("Setting soft-AP ... ");
    char buffer [100];
    sprintf("Demarrage du point d'acces Wifi ssid=%s, pwd=%d", donneesGlobales.wifiSsid, donneesGlobales.wifiPwd); Serial.println(buffer);
    boolean result = WiFi.softAP(donneesGlobales.wifiSsid, donneesGlobales.wifiPwd, 1, false, 8);
    if(result == true)
    {
        Serial.println("Ready");
        char buffer[50];
        IPAddress ip = WiFi.softAPIP();
        sprintf(donneesGlobales.ipGateway, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
        strcpy(donneesGlobales.nomGateway, "gateway");
    }
    else
    {
        Serial.println("Failed!");
        while(1)    delay(1000);
    }

    server.begin(); 

    Serial.print("Adresse IP de la Gateway : ");
    Serial.println(donneesGlobales.ipGateway);
    initNtp();
}

//=================================================
//
//      loop
//
//=================================================
void loop(){

    if (donneesGlobales.nbClientsConnectes != WiFi.softAPgetStationNum()){
        Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
        donneesGlobales.nbClientsConnectes = WiFi.softAPgetStationNum();
    }
    client = server.available();
    if (client){
        char adresseIpClient[20];
        IPAddress ipClient = client.remoteIP();
        sprintf(adresseIpClient,"%d.%d.%d.%d",ipClient[0],ipClient[1],ipClient[2],ipClient[3]);
        analyseRequete(client,adresseIpClient);
    }
    refreshNtp();

    nbMillisecondDisplayDatas = millis();     // on desactive le refresh pour le moment
    if ((millis() - nbMillisecondDisplayDatas) >= DELAY_REFRESH_AFFICHAGE_DATAS){
        nbMillisecondDisplayDatas = millis();
        afficheDatas();
    }
   
    //nbMillisecondOpenweathermap = millis();     // on desactive le refresh pour le moment
    if ((millis() - nbMillisecondOpenweathermap) >= DELAY_REFRESH_OPENWEATHERMAP){
        nbMillisecondOpenweathermap = millis();
        refreshMeteoInfos();
    }
   
    delay(LOOP_DELAY);
}

