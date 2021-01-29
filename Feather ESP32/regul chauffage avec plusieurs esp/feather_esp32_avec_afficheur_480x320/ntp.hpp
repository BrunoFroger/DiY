// ESP32_horloge
// ntp.hpp
// (c) B. Froger 2020


#ifndef __NTP
#define __NTP

//extern char dayOfWeek[7][5];

extern void refreshNtp();
extern void initNtp();
extern void refreshNtpNow();
extern void updateHeure(void);
extern char *getHeureFormatee(void);
extern char *getDateFormatee(void);

#endif
