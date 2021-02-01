//
//
//  mesure.hpp
//
//

#include <arduino.h>

#ifndef __MESURE_HPP
#define __MESURE_HPP

#define NB_MESURES  1000
#define INPUT_MESURE 3

class MesureClass
{
    public:
    MesureClass(void);
    ~MesureClass();
    void init();
    void mesureInterrupt();
    void fakeMesure();
    int getMesure();
    int getLastMesure();
    int getCurrentMesure();
    boolean isMesureDisponible();

    private:
    int tblMesures[NB_MESURES];
    int idxMesureWrite;
    int idxMesureRead;
    int nbMesureDisponibles = 0;
};

#endif