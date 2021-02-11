//
//
//  ESP 32 Fether avec afficheur 480 x 320.ino
//
//


#include <Arduino.h>

#include "api.hpp"
#include "wifiTools.hpp"
#include "display.hpp"


#define LOOP_DELAY 10
#define REFRESH_DELAY_AFFICHAGE_DATAS   1000*2

int nbMillisecondAffichageDatas = 0;

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
    Serial.println("+      Feather ESP32 Affichage  +");
    Serial.println("+                               +");
    Serial.println("+-------------------------------+");

    strcpy(mesDonneesApi.wifiSsid, "gateway-chauffage");
    strcpy(mesDonneesApi.wifiPwd, "0296911369");
    strcpy(mesDonneesApi.espName, "Afficheur");

    Serial.println("---------------------------");
    Serial.println("start initTft");
    initTft();
    Serial.println("start initTft");

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

    refreshDisplay();
    updateDatas();

    //nbMillisecondAffichageDatas = millis();     // on desactive le refresh pour le moment
    if ((millis() - nbMillisecondAffichageDatas) >= REFRESH_DELAY_AFFICHAGE_DATAS){
        afficheDatas();
        nbMillisecondAffichageDatas = millis(); 
    }

    delay(LOOP_DELAY);

}
