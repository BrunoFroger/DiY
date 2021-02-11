//
//
//  ESP 32 Wemos pilotages capteurs
//
//

#include <Arduino.h>
#include <string.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "wifiTools.hpp"
#include "api_capteurs.hpp"

#define LOOP_DELAY 100

#define     DELAY_REFRESH_AFFICHAGE_DATAS   1000*2      //2s
unsigned long nbMillisecondDisplayDatas = 0;

// Replace with your network credentials
//const char* ssid     = "gateway-chauffage";
//const char* password = "0296911369";
//const char* espName = "Capteurs";

const int entreeAnalogique = A0;
int valeurLue = 0;
char buffer[200];

//=================================================
//
//      setup
//
//=================================================
void setup() {        
    // initialize serial communication
    //Serial.begin(115200);
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
    Serial.println("+      WeMos ESP32 Capteurs     +");
    Serial.println("+                               +");
    Serial.println("+-------------------------------+");

    strcpy(mesDonneesCapteurs.wifiSsid, "gateway-chauffage");
    strcpy(mesDonneesCapteurs.wifiPwd, "0296911369");
    strcpy(mesDonneesCapteurs.espName, "Capteurs");
    
    Serial.println("+==========================+");
    Serial.println("|    start initWifi        |");
    Serial.println("+--------------------------+");
    initWifi(false);
    Serial.println("end initWifi");
    Serial.println("+==========================+");
    
    Serial.println("+==========================+");
    Serial.println("|    start initApi         |");
    Serial.println("+--------------------------+");
    initApi();
    Serial.println("end initApi");
    Serial.println("+==========================+");

    Serial.println("end of setup");
    Serial.println("---------------------------");
  
}
 

//=================================================
//
//      loop
//
//=================================================
void loop(){ 
    //Serial.println("boucle principale");

    //valeurLue = map(analogRead(entreeAnalogique),0,1024,10,30);
    //sprintf(buffer, "valeur lue = %d", valeurLue); Serial.println(buffer);

    updateDatas();

    //nbMillisecondDisplayDatas = millis();     // on desactive le refresh pour le moment
    if ((millis() - nbMillisecondDisplayDatas) >= DELAY_REFRESH_AFFICHAGE_DATAS){
        nbMillisecondDisplayDatas = millis();
        afficheDatas();
    }

    delay(LOOP_DELAY);

}

