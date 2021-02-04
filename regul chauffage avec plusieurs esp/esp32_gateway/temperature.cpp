//
//
//  temperature.cpp
//
//

#include <Arduino.h>

#include "globalDatas.hpp"

int coefPerteChauffage = 50;     // % de calcul de la perte de chauffage

//=========================================
//
//          getTemperatureSimulee
//
//=========================================
int getTemperatureSimulee(void){
    int deltaTemperature;
    int deltaPuissance;
    char tmp[200];

    Serial.println("------------------------------------");
    Serial.println("getTemperature => debut");
    deltaTemperature = donneesGlobales.consigne - donneesGlobales.temperature;
    Serial.print("deltaTemperature = ");
    Serial.println(deltaTemperature);
    deltaPuissance = deltaTemperature  * donneesGlobales.puissanceChauffage / 100;
    if (deltaPuissance == 0){
        deltaPuissance = 1;
    }
    // on calcule le changement de puissance du chauffage
    if (deltaTemperature >= 0){
        Serial.println("on augmente le chauffage");
        // trop froid on allume la chaudiere
        donneesGlobales.chauffageOnOff = true;
        // on augmente la puissance de la chaudiere
        Serial.print("deltaPuissance = ");
        Serial.println(deltaPuissance);
        donneesGlobales.puissanceChauffage += deltaPuissance;
        if (donneesGlobales.puissanceChauffage >= 100){
            donneesGlobales.puissanceChauffage = 100;
        }
    } else { // il fait trop chaud, on reduit le chauffage
        Serial.println("on diminue le chauffage");
        donneesGlobales.puissanceChauffage += deltaPuissance;
        if (donneesGlobales.puissanceChauffage <= 0){
            donneesGlobales.chauffageOnOff = false;
            donneesGlobales.puissanceChauffage = 0;
        }
    }

    // on applique le chaufage a la temperature
    int variationTemperature = donneesGlobales.puissanceChauffage / 10;

    // on applique la baisse de temperature vis a vis de l'exterieur
    int deltaTempExt = (donneesGlobales.temperature - donneesGlobales.temperatureExterieure) * coefPerteChauffage /100;
    variationTemperature -= deltaTempExt;
    if (donneesGlobales.temperature <= donneesGlobales.temperatureExterieure){
        donneesGlobales.temperature = donneesGlobales.temperatureExterieure;
    }

    donneesGlobales.temperature += variationTemperature;
    Serial.print("variation temperature : ");
    Serial.println(variationTemperature);

    sprintf(tmp,"getTemperature => on/off=%d, puissance=%d, temperature=%d", donneesGlobales.chauffageOnOff, donneesGlobales.puissanceChauffage, donneesGlobales.temperature);
    Serial.println(tmp);
    return donneesGlobales.temperature;
}


//=========================================
//
//          getTemperatureMesuree
//
//=========================================
int getTemperatureMesuree(void){
    return donneesGlobales.temperature;
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
    int mode = 2;
    switch (mode){
        case 1:
            return getTemperatureRampe();
            break;
        case 2:
            return getTemperatureSimulee();
            break;
        case 3:
            return getTemperatureMesuree();
            break;
        default:
            Serial.println("mode de calcul de la temperature inconnu");
    }
    return donneesGlobales.temperature;
}

