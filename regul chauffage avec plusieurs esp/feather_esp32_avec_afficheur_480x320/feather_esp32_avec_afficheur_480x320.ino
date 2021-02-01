//
//
//  ESP 32 Fether avec afficheur 480 x 320.ino
//
//


#include <Arduino.h>

#include "chauffage.hpp"
#include "api.hpp"
#include "wifiTools.hpp"
#include "display.hpp"


#define LOOP_DELAY 10

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

    delay(LOOP_DELAY);

}
