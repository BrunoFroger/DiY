/*
Orange Labs Lannion
ORANGE/IMT/OLPS/SOFT/LANNION

projet 3DHandZ
module interruptions

pour arduino nano

*/

#include <arduino.h>

#define DELAY_LOOP 10000
int cpt=0;
int debutInterruption=0;
int inInterruption=0;
/*************************************************
 *
 *    fonction Interruption
 *
 *************************************************/
void interruption(void){
    // avec systeme anti rebond
    if (inInterruption == 0){
        inInterruption=1; 
        debutInterruption=millis();
        //Serial.println("appui bouton");
    }else{
        int tmp = millis() - debutInterruption;
        if (tmp > 10){
            //Serial.println("relache bouton");
            cpt++;
            inInterruption=0;
        }
    }
}

/*************************************************
 *
 *    fonction S E T U P
 *
 *************************************************/
void setup() {
    // put your setup code here, to run once:

    // initialize serial communication at 9600 bits per second:
    int timeoutInitSerial=100;
    Serial.begin(38400);
    while(timeoutInitSerial-- > 0){
    if (Serial) break;
        delay(1);
    }

    // init afficheur  
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
    delay(10); // on laisse le temps a l'afficheur de s'initialiser

    // init de l'interruption
    attachInterrupt(0,interruption,CHANGE);

    Serial.println("fin d'init");
}

/*************************************************
 *
 *    fonction L O O P
 *
 *************************************************/
void loop() {
  // put your main code here, to run repeatedly
  
  Serial.print("nb appui : ");
  Serial.println(cpt);
  delay(1000);
}
