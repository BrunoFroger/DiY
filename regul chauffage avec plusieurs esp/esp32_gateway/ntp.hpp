// ESP32_horloge
// ntp.hpp
// (c) B. Froger 2020


#ifndef __NTP
#define __NTP

//extern char dayOfWeek[7][5];
extern int second;
extern int minute;
extern int heure;
extern int jour;
extern int jourSemaine;
extern int mois;
extern int annee;

extern void refreshNtp();
extern void initNtp();
extern void refreshNtpNow();

#endif
