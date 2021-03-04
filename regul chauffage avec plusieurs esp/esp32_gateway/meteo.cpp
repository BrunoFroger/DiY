//
//
//  meteo.cpp
//
//

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


WiFiClient meteoClient;

//=========================================
//
//          refreshMeteoInfos
//
//=========================================
void refreshMeteoInfos(){
    // recuppere les informations meteo sur openweathermap
    meteoClient.connect("api.openweathermap.org/data/2.5/weather?q=Louannec&units=metric&lang=fr&appid=d5661c9ed3cb8f728b90243ebb890806",80);
    delay(100);
    int idx = 0;
    Serial.println("resultat de la requete auprès de openweathermap :");
    char buffer[5000]; // buffer de collecte des donnees transmises par openweatehrmap
    while (meteoClient.available() > 0){ // double boucle afin de tolerer les latences reseau
        while (meteoClient.available() > 0){
            // des donnees a recuperer 
            char car = meteoClient.read();
            Serial.print(car);
            buffer[idx++] = car;
        }
        delay(100); // dans le cas ou les datas arriveraient pas toutes en meme temps
    }
    Serial.println();
}
