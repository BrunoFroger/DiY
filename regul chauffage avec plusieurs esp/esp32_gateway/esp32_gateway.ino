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

#define LOOP_DELAY 10

// Replace with your network credentials
const char* ssid     = "gateway-chauffage";
const char* password = "0296911369";


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

    initGlobalDatas();

    Serial.print("Setting soft-AP ... ");
    boolean result = WiFi.softAP(ssid, password);
    if(result == true)
    {
        Serial.println("Ready");
        donneesGlobales.ipGateway = WiFi.softAPIP();
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
        //Serial.println("requete client recue");
        analyseRequete(client);
    }
    refreshNtp();
    delay(100);
}

