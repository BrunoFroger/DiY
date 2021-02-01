//
//
//  mesure.cpp
//
//

#include <arduino.h>
#include "mesure.hpp"

//*********************************************
//*
//*       Constructor
//*
//*********************************************
MesureClass::MesureClass()
{

}

//*********************************************
//*
//*       Destructor
//*
//*********************************************
MesureClass::~MesureClass()
{
}

//*********************************************
//*
//*       init
//*
//*********************************************
void MesureClass::init(){
    for (int i = 0 ; i < NB_MESURES ; i++){
        tblMesures[i] = 0;
    }
    idxMesureWrite = 0;
}

//*********************************************
//*
//*       mesureInterrupt
//*
//*********************************************
void MesureClass::mesureInterrupt(){
    tblMesures[idxMesureWrite++] = analogRead(INPUT_MESURE);
    if (idxMesureWrite > NB_MESURES){
        idxMesureWrite = 0;
    }
    nbMesureDisponibles++;
}

//*********************************************
//*
//*       fakeMesure
//*
//*********************************************
void MesureClass::fakeMesure(){
    int valeur = 50 + (idxMesureWrite % 40);
    tblMesures[idxMesureWrite++] = valeur;
    if (idxMesureWrite > NB_MESURES){
        idxMesureWrite = 0;
    }
    nbMesureDisponibles++;
}


//*********************************************
//*
//*       getMesure
//*
//*********************************************
int MesureClass::getMesure(){
    if (idxMesureRead >= NB_MESURES){
        idxMesureRead=0;
    }
    int mesure = tblMesures[idxMesureRead];
    nbMesureDisponibles--;
    idxMesureRead++;
    return mesure;
}

//*********************************************
//*
//*       getCurrentMesure
//*
//*********************************************
int MesureClass::getCurrentMesure(){
    return tblMesures[idxMesureRead - 1];
}

//*********************************************
//*
//*       getMesure
//*
//*********************************************
int MesureClass::getLastMesure(){
    int mesure;
    if (idxMesureRead == 0){
        mesure = tblMesures[NB_MESURES - 1];
    }else{
        mesure = tblMesures[idxMesureRead - 1];
    }
    return mesure;

}



//*********************************************
//*
//*       isMesureDisponible
//*
//*********************************************
boolean MesureClass::isMesureDisponible(){
    if (nbMesureDisponibles > 0){
        return true;
    }
    return false;
}
