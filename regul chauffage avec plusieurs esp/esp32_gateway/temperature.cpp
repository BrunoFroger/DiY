//
//
//  temperature.cpp
//
//

#include <Arduino.h>

#include "globalDatas.hpp"


//=========================================
//
//          getTemperatureSimulee
//
//=========================================
int getTemperatureSimulee(void){
    int perte = 1;
    int deltaPuissance;


    deltaPuissance = donneesGlobales.temperature - donneesGlobales.consigne;
    // on calcule le changement de puissance du chauffage
    If (deltaPuissance <= 0){
        // trop froid on allume la chaudiere
        donneesGlobales.puissanceChauffage = true;
        // on augmente la puissance de la chaudiere
        donneesGlobales.puissanceChauffage += deltapuissance;
        if (donneesGlobales.puissanceChauffage >= 100){
            donneesGlobales.puissanceChauffage = 100;
        }
    } else { // il fait trop chaud, on reduit le chauffage
        donneesGlobales.puissanceChauffage -= deltapuissance;
    }
}


//=========================================
//
//          getTemperatureMesuree
//
//=========================================
int getTemperatureMesuree(void){
}


//=========================================
//
//          int getTemperatureRampe(void){

//
//=========================================
int getTemperatureRampe(void){
    donneesGlobales.temperature = donneesGlobales.temperature + 1;
    if (donneesGlobales.temperature > 210) donneesGlobales.temperature = 200;
    //Serial.print("getTemperature => ");
    Serial.println(donneesGlobales.temperature);
    return donneesGlobales.temperature;
}


//=========================================
//
//          getTemperature
//
//=========================================
int getTemperature(void){
    int mode = 1;
    switch (mode){
        1:
            return getTemperatureRampe();
            break;
        2:
            return getTemeratureSimulee(void);
            break;
        3:
            return getTemeratureMesuree(void);
            break;
        default:
            Serial.println("mode de calcul de la temperature inconnu")
    }
}

