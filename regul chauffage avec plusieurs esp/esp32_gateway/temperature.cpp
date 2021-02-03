//
//
//  temperature.cpp
//
//

#include <Arduino.h>

#include "globalDatas.hpp"


//=========================================
//
//          getTemperature
//
//=========================================
int getTemperature(void){
    donneesGlobales.temperature = donneesGlobales.temperature + 1;
    if (donneesGlobales.temperature > 210) donneesGlobales.temperature = 200;
    Serial.print("getTemperature => ");
    Serial.println(donneesGlobales.temperature);
    return donneesGlobales.temperature;
}