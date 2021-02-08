//
//
//  ESP 32 Wemos pilotages capteurs
//
//

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "wifiTools.hpp"
#include "api_capteurs.hpp"

#define LOOP_DELAY 100

// Replace with your network credentials
const char* ssid     = "gateway-chauffage";
const char* password = "0296911369";
const char* espName = "Capteurs";

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

    
    Serial.println("---------------------------");
    Serial.println("start initWifi");
    initWifi(false);
    Serial.println("end initWifi");
    

    initApi();

    Serial.println("end of setup");
  
}
 

//=================================================
//
//      loop
//
//=================================================
void loop(){ 
    //Serial.println("boucle principale");

    valeurLue = map(analogRead(entreeAnalogique),0,1024,10,30);
    sprintf(buffer, "valeur lue = %d", valeurLue); Serial.println(buffer);

    updateDatas();

    delay(LOOP_DELAY);

}

