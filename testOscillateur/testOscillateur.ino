//
//
//  testOscillateur.ino
//
//

#include <arduino.h>

#define MOY_NB_VALUE  1
#define LOOP_DELAY 100
#define LOOP_DISPLAY 10

int oscillateurInput = 10;
long compteur = 0;
long cptLoop = 0;
long moyenne = 0;
int cptMoyenne = 0;
long somme = 0;
long lastValue = 0;
long delta;
double deltaPercent;
char displayLine[100];

void setup() {        
  // initialize serial communication
  Serial.begin(38400);
  int timeoutInitSerial = 100;
  while (timeoutInitSerial-- > 0)
  {
      if (Serial)
          break;
      delay(10);
  }
  Serial.println("Serial initialized");

  pinMode(oscillateurInput, INPUT); 

  digitalWrite(oscillateurInput, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  attachInterrupt(oscillateurInput, clockInterrupt, RISING); 

}

void printDouble( double val, unsigned int precision){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)

   Serial.print (int(val));  //prints the int part
   Serial.print("."); // print the decimal point
   unsigned int frac;
   if(val >= 0)
       frac = (val - int(val)) * precision;
   else
       frac = (int(val)- val ) * precision;
   Serial.print(frac,DEC) ;
} 

long calcMoyenne(long value){
  if (cptMoyenne++ <= MOY_NB_VALUE){
    cptMoyenne=MOY_NB_VALUE;
    somme+=value;
    return value;
  }else{
    somme-=moyenne;
    somme+=value;
    moyenne = somme / MOY_NB_VALUE;
    return moyenne;
  }
}

void loop(){ 
  if (cptLoop++ >= LOOP_DISPLAY){
    //compteur = calcMoyenne(compteur);
    delta = (lastValue - compteur);
    deltaPercent = (double) delta / (double) compteur * 100.0;
    lastValue = compteur;
    Serial.print("loop : ");
    Serial.print(compteur);
    Serial.print("(");
    Serial.print(delta);
    Serial.print(") ");
    printDouble (deltaPercent,1000);
    Serial.print("%");
    Serial.println();
    //Serial.print("loop : ");
    //Serial.print(compteur);
    //Serial.println();
    compteur=0;
    cptLoop=0;
  }else{
    //Serial.print(".");
  }
  delay(LOOP_DELAY); //just here to slow down the output, and show it will work  even during a delay
}


void clockInterrupt(){
  compteur++;
}