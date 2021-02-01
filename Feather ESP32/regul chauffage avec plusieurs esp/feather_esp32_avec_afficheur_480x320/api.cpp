//
//
//  api.cpp
//
//

#include <Arduino.h>
#include <string.h>



int second = 0;
int minute = 25;
int heure = 12;
int jour = 17;
int jourSemaine;
int mois = 2;
int annee = 2021;
unsigned long nbMillisecondUpdateHeure = 0;
char dayOfWeek[7][5] = {"dim", "lun", "mar", "mer", "jeu", "ven", "sam"};
bool datasUpated = false;
chauffageDatas mesDonnees;

//=========================================
//
//          getHeureFormatee
//
//=========================================
chauffageDatas *getDatas(void){
    printf("heure formatee = %s\n", mesDonnees.heureFormatee);
    datasUpated = false;
    return (mesDonnees.heureFormatee);
}


//=========================================
//
//          updateHeure
//
//=========================================
bool isDatasAvailables(void){
    return datasUpated;
}

//=========================================
//
//          updateHeure
//
//=========================================
void updateHeure(void){
    int dernierJour = 31;

    if ((millis() - nbMillisecondUpdateHeure) >= 1000){
        nbMillisecondUpdateHeure = millis();

        Serial.println("Update Heure");

        second++;

        if (second >= 60){
            second = 0;
            minute++;
        }
        if (minute >= 60){
            minute = 0;
            heure++;
        }
        if (heure >= 24){
            heure = 0;
            jour++;
        }
        switch (mois){  
            case 4:
            case 6:
            case 9:
            case 11:    
                dernierJour = 30;
                break;      
            case 2:
                if ((annee % 4) != 0){
                    dernierJour = 29;
                } else {
                    dernierJour = 28;
                }
                break;
            default:
                dernierJour = 31;
        }
        if (jour > dernierJour){
            jour = 1;
            mois++;
        }
        if (mois > 12){
            mois = 0;
            annee++;
        }
        sprintf(mesDonnees.heureFormatee, "%2d:%02d:%02d", heure, minute, second);
        sprintf(mesDonnees.dateFormatee, "%s %2d/%02d/%2d", dayOfWeek[jourSemaine], jour, mois, annee);
        datasUpated = true;
    }
}
