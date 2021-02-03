// ESP32_horloge
// ntp.cpp
// (c) B. Froger 2020

#include <Arduino.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "globalDatas.hpp"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600 * 2, 60000);
char dayOfWeek[7][5] = {"dim", "lun", "mar", "mer", "jeu", "ven", "sam"};
boolean heureEte, tmpHeureEte;
int timeOffset;


//char heureFormatee[100];
//char dateFormatee[100];
unsigned long nbMillisecondUpdateHeure = 0;

extern bool refreshCadreValeurs;

//=========================================
//
//          initNtp
//
//=========================================
void initNtp(void){
    timeClient.begin();
    tmpHeureEte = heureEte;
}


#define DELAY_REFRESH_NTP   30000000
unsigned long ntpLastRefresh=-DELAY_REFRESH_NTP;
//=========================================
//
//          refreshNtpNow
//
//=========================================
void refreshNtpNow(void){
        //Serial.println("Refresh NTP");
        char tmp[50];
        if (donneesGlobales.annee > 2000){
            // en France le passage à l'heure d'été obéis à la règle suivante :
            // Du dernier Dimanche de Mars au dernier Dimanche d'octobre on ajoute 60 minutes
            // L'heure du changement est 3h00 
            if (donneesGlobales.mois > 10 || donneesGlobales.mois < 3 
                || (donneesGlobales.mois == 10 && (donneesGlobales.jour - donneesGlobales.jourSemaine) > 22) 
                || (donneesGlobales.mois == 3 && (donneesGlobales.jour - donneesGlobales.jourSemaine) < 24)){
                // heure d'hiver
                tmpHeureEte = false;
            } else {
                // heure d'été
                tmpHeureEte = true;
            }
        }
        if (tmpHeureEte == heureEte){
            if (heureEte){
                // 2 heure de décalage en été
                timeOffset = 3600 * 2;
            } else {
                //  heures de décalahge en hivers
                timeOffset = 3600;
            }
            timeClient.setTimeOffset(timeOffset);
            tmpHeureEte = heureEte;
        }
        timeClient.update();
        //unsigned long epochTime = timeClient.getEpochTime();
        String formatedDate = timeClient.getFormattedDate();
        //Serial.println(formatedDate);
        donneesGlobales.second = timeClient.getSeconds();
        donneesGlobales.minute = timeClient.getMinutes();
        donneesGlobales.heure = timeClient.getHours();
        donneesGlobales.jourSemaine = timeClient.getDay();
        //Serial.println(formatedDate.substring(8,10));
        donneesGlobales.jour = formatedDate.substring(8,10).toInt();
        //Serial.println(formatedDate.substring(5,7));
        donneesGlobales.mois = formatedDate.substring(5,7).toInt();
        //Serial.println(formatedDate.substring(0,4));
        donneesGlobales.annee = formatedDate.substring(0,4).toInt();
        sprintf(tmp,"Update NTP => %s %4d-%02d-%02d / %02d:%02d:%02d", 
                    dayOfWeek[donneesGlobales.jourSemaine], donneesGlobales.annee, 
                    donneesGlobales.mois, donneesGlobales.jour, donneesGlobales.heure, 
                    donneesGlobales.minute, donneesGlobales.second);
        Serial.println(tmp);
        ntpLastRefresh=millis();
}

//=========================================
//
//          refreshNtp
//
//=========================================
void refreshNtp(void){
    if ((millis() - ntpLastRefresh) > DELAY_REFRESH_NTP){
        refreshNtpNow();
    }
    if (donneesGlobales.annee < 2000) refreshNtpNow();
}
