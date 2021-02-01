// ESP32_regul_chauffage
// eeprom.cpp
// (c) B. Froger 2021


#include <Arduino.h>
#include <String.h>


#define CONSIGNE_MAX    50
#define CONSIGNE_MIN    0


int consigne = 20;
int chauffagePuissance;
bool chauffageOnOff = false;


int getConsigne(void){
    return consigne;
}

bool setConsigne(int valeur){
    if (valeur < CONSIGNE_MIN) return false;
    if (valeur > CONSIGNE_MAX) return false;
    consigne = valeur;
    return true;
}

int getChaufffagePuissance(void){
    return chauffagePuissance;
}

bool getChauffageOnOff(void){
    return chauffageOnOff;
}
