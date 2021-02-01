// ESP32_regul_chauffage
// chauffage.hpp
// (c) B. Froger 2021



#ifndef __CHAUFFAGE
#define __CHAUFFAGE

//extern void analyseRequest(String);
extern int getConsigne();
extern bool setConsigne(int consigne);
extern int getChaufffagePuissance(void);
extern bool getChauffageOnOff(void);

#endif