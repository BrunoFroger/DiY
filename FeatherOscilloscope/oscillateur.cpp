//
//
//  oscillateur.cpp
//
//

#include <arduino.h>
#include "oscillateur.hpp"
#include "mesure.hpp"

extern MesureClass mesure;

//*********************************************
//*
//*       Constructor
//*
//*********************************************
OscillateurClass::OscillateurClass()
{

}

//*********************************************
//*
//*       Destructor
//*
//*********************************************
OscillateurClass::~OscillateurClass()
{
}

//*********************************************
//*
//*       clockInterrupt
//*
//*********************************************
void clockInterrupt(){
    mesure.mesureInterrupt();
}

//*********************************************
//*
//*       init
//*
//*********************************************
void OscillateurClass::init(int oscillateurInput){
  pinMode(oscillateurInput, INPUT); 
  digitalWrite(oscillateurInput, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  attachInterrupt(oscillateurInput, clockInterrupt, RISING); 
}

