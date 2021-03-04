//
//
//  ESP 32 Feather avec afficheur 480 x 320.ino
//
//


#include <Arduino.h>
#include <SPI.h>

#include "api.hpp"
#include "wifiTools.hpp"
#include "display.hpp"
#include "encoder.hpp"
#include "sdcard.hpp"


//#define LOOP_DELAY 5
#define LOOP_DELAY 10
#define REFRESH_DELAY_AFFICHAGE_DATAS   1000*2

int nbMillisecondAffichageDatas = 0;
int lastEncoderValue = 0;

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
    Serial.println();
    char buffer[100];
    sprintf(buffer, "Serial initialized in %d ms", (100-timeoutInitSerial)); Serial.println(buffer);    
    Serial.println("+----------------------------------+");
    Serial.println("+                                  +");
    Serial.println("+      Feather ESP32 Affichage     +");
    Serial.println("+                                  +");
    Serial.println("+----------------------------------+");
    Serial.println("|        debut setup               |");
    Serial.println("+----------------------------------+");

    strcpy(mesDonneesApi.wifiSsid, "gateway-chauffage");
    strcpy(mesDonneesApi.wifiPwd, "0296911369");
    strcpy(mesDonneesApi.espName, "Afficheur");

    initTft();
    initWifi(false);
    initApi();
    initEncoder();
    initSdcard();

    Serial.println("+----------------------------------+");
    Serial.println("|        fin de setup              |");
    Serial.println("+----------------------------------+");
  
}
 
//int cpt = 0;
//=================================================
//
//      loop
//
//=================================================
void loop(){ 
    //Serial.println("boucle principale");

    refreshDisplay();
    updateDatas();
    checkEncoder();

    nbMillisecondAffichageDatas = millis();     // on desactive le refresh pour le moment
    if ((millis() - nbMillisecondAffichageDatas) >= REFRESH_DELAY_AFFICHAGE_DATAS){
        afficheDatas();
        nbMillisecondAffichageDatas = millis(); 
    }

    delay(LOOP_DELAY);
    
}
