//
//
//  temperature.cpp
//
//

#include <Arduino.h>

#include "globalDatas.hpp"

int deltaTempExt;
int deltaConsigne;
int deltaPuissance;
int variationTemperatureParChauffage;
int variationTemperatureParExterieur;
int variationTemperature;

//=========================================
//
//          displayInfosSimulation
//
//=========================================
void displayInfosSimulation(void){
    char tmp[100];
    Serial.println("+-----------------------------------------+");
    Serial.println("|     Simulation variation temperature    |");
    Serial.println("+-------------------+---------------------+");
    Serial.println("|   Variable        |    Valeur           |");
    Serial.println("+-------------------+---------------------+");
    sprintf(tmp,   "| Temp int          |    %6d           |", donneesGlobales.temperature);
    Serial.println(tmp);
    sprintf(tmp,   "| Temp ext          |    %6d           |", donneesGlobales.temperatureExterieure);
    Serial.println(tmp);
    sprintf(tmp,   "| delta temp ext    |    %6d           |", deltaTempExt);
    Serial.println(tmp);
    sprintf(tmp,   "| Consigne          |    %6d           |", donneesGlobales.consigne);
    Serial.println(tmp);
    sprintf(tmp,   "| delta consigne    |    %6d           |", deltaConsigne);
    Serial.println(tmp);
    Serial.println("+-------------------+---------------------+");
    sprintf(tmp,   "| Chauffage on/off  |    %6d           |", donneesGlobales.chauffageOnOff);
    Serial.println(tmp);
    sprintf(tmp,   "| Puiss chauffage   |    %6d           |", donneesGlobales.puissanceChauffage);
    Serial.println(tmp);
    sprintf(tmp,   "| delta puissance   |    %6d           |", deltaPuissance);
    Serial.println(tmp);
    Serial.println("+-------------------+---------------------+");
    sprintf(tmp,   "| var / chauffage   |    %6d           |", variationTemperatureParChauffage);
    Serial.println(tmp);
    sprintf(tmp,   "| var / exterieur   |    %6d           |", variationTemperatureParExterieur);
    Serial.println(tmp);
    sprintf(tmp,   "| var globale       |    %6d           |", variationTemperature);
    Serial.println(tmp);
    Serial.println("+-------------------+---------------------+");

}

//=========================================
//
//          getTemperatureSimulee
//
//=========================================
void calculRegulation(void){

    //if (donneesGlobales.modeVerbose) Serial.println("------------------------------------");
    //if (donneesGlobales.modeVerbose) Serial.println("getTemperature => debut");
    deltaTempExt = donneesGlobales.temperature - donneesGlobales.temperatureExterieure;
    deltaConsigne = donneesGlobales.consigne - donneesGlobales.temperature;
    //if (donneesGlobales.modeVerbose) Serial.print("deltaTemperature = ");
    //if (donneesGlobales.modeVerbose) Serial.println(deltaTempExt);
    // calcul de la variation de puissance 
    // variation = 10% du delta consigne
    //deltaPuissance = deltaConsigne  * 10 / 100;
    //deltaPuissance = deltaConsigne;
    if (deltaConsigne > 10){
        deltaPuissance = deltaConsigne  * 10 / 100;
    } else {
        deltaPuissance = deltaConsigne;
    }
    if (deltaPuissance == 0){
        deltaPuissance = 1;
    }
    // on calcule le changement de puissance du chauffage
    if (deltaConsigne >= 0){
        //if (donneesGlobales.modeVerbose) Serial.println("on augmente le chauffage");
        // trop froid on allume la chaudiere
        donneesGlobales.chauffageOnOff = true;
        // on augmente la puissance de la chaudiere
        //if (donneesGlobales.modeVerbose) Serial.print("deltaPuissance = ");
        //if (donneesGlobales.modeVerbose) Serial.println(deltaPuissance);
        donneesGlobales.puissanceChauffage += deltaPuissance;
        if (donneesGlobales.puissanceChauffage >= 100){
            donneesGlobales.puissanceChauffage = 100;
        }
    } else { // il fait trop chaud, on reduit le chauffage
        //if (donneesGlobales.modeVerbose) ("on diminue le chauffage");
        donneesGlobales.puissanceChauffage += deltaPuissance;
        if (donneesGlobales.puissanceChauffage <= 0){
            donneesGlobales.chauffageOnOff = false;
            donneesGlobales.puissanceChauffage = 0;
        }
    }
}

//=========================================
//
//          getTemperatureSimulee
//
//=========================================
int getTemperatureSimulee(void){
    char tmp[200];

    calculRegulation();

    //  Simulation
    // on calcule la variation de temperature
    // on augente la temperature de 1/10 de degre par 10% de chauffage
    variationTemperatureParChauffage = donneesGlobales.puissanceChauffage / 10;

    // on applique la baisse de temperature vis a vis de l'exterieur
    if (donneesGlobales.temperature <= donneesGlobales.temperatureExterieure){
        donneesGlobales.temperature = donneesGlobales.temperatureExterieure;
    }

    // calcul de la variation du a l'ecart de temperature ext
    // on diminue la temperature interieur en fonction de la temperature exterieure
    // la variation est de 1/10 de degre par degre de difference
    variationTemperatureParExterieur = -(deltaTempExt / 10);
    //if (donneesGlobales.modeVerbose) Serial.print("variation temperature chauffage : ");
    //if (donneesGlobales.modeVerbose) Serial.println(variationTemperatureParChauffage);
    //if (donneesGlobales.modeVerbose) Serial.print("variation temperature exterieur : ");
    //if (donneesGlobales.modeVerbose) Serial.println(variationTemperatureParExterieur);
    //if (donneesGlobales.modeVerbose) Serial.print("variation temperature globale : ");
    //if (donneesGlobales.modeVerbose) Serial.println(variationTemperatureParChauffage);
    variationTemperature = variationTemperatureParChauffage + variationTemperatureParExterieur;
    //if (donneesGlobales.modeVerbose) Serial.println(variationTemperature);

    // on calcule la nouvelle temperature
    donneesGlobales.temperature += variationTemperature;

    //sprintf(tmp,"getTemperature => on/off=%d, puissance=%d, temperature=%d", donneesGlobales.chauffageOnOff, donneesGlobales.puissanceChauffage, donneesGlobales.temperature);
    //if (donneesGlobales.modeVerbose) Serial.println(tmp);
    displayInfosSimulation();
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
    if (donneesGlobales.modeVerbose) Serial.print("getTemperature => ");
    if (donneesGlobales.modeVerbose) Serial.println(donneesGlobales.temperature);
    return donneesGlobales.temperature;
}


//=========================================
//
//          getTemperature
//
//=========================================
int getTemperature(void){
    int resultat;
    switch (donneesGlobales.modeCalculTemperature){
        case 1:
            resultat = getTemperatureRampe();
            break;
        case 2:
            resultat = getTemperatureSimulee();
            break;
        case 3:
            resultat = getTemperatureMesuree();
            break;
        default:
            Serial.println("mode de calcul de la temperature inconnu");
            resultat = 0;
    }
    donneesGlobales.temperature = resultat;
    return donneesGlobales.temperature;
}

