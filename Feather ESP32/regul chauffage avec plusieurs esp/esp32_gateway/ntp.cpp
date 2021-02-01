// ESP32_horloge
// ntp.cpp
// (c) B. Froger 2020

#include <Arduino.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600 * 2, 60000);
char dayOfWeek[7][5] = {"dim", "lun", "mar", "mer", "jeu", "ven", "sam"};
boolean heureEte, tmpHeureEte;
int timeOffset;

// parametre temps
int second = 0;
int minute = 25;
int heure = 12;
int jour = 17;
int jourSemaine;
int mois = 2;
int annee = 2021;
char heureFormatee[100];
char dateFormatee[100];
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
        if (annee > 2000){
            // en France le passage à l'heure d'été obéis à la règle suivante :
            // Du dernier Dimanche de Mars au dernier Dimanche d'octobre on ajoute 60 minutes
            // L'heure du changement est 3h00 
            if (mois > 10 || mois < 3 
                || (mois == 10 && (jour - jourSemaine) > 22) 
                || (mois == 3 && (jour - jourSemaine) < 24)){
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
        second = timeClient.getSeconds();
        minute = timeClient.getMinutes();
        heure = timeClient.getHours();
        jourSemaine = timeClient.getDay();
        //Serial.println(formatedDate.substring(8,10));
        jour = formatedDate.substring(8,10).toInt();
        //Serial.println(formatedDate.substring(5,7));
        mois = formatedDate.substring(5,7).toInt();
        //Serial.println(formatedDate.substring(0,4));
        annee = formatedDate.substring(0,4).toInt();
        sprintf(tmp,"Update NTP => %s %4d-%02d-%02d / %02d:%02d:%02d", dayOfWeek[jourSemaine], annee, mois, jour, heure, minute, second);
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
}
