//
//
//  testEncodeurRotatif.ino
//
//

#include <Arduino.h>

#define DUREE_TEST  1000

int encoderPinA = 14;
int encoderPinB = 32;
int buttonPin = 33;

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastEncoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;
int lastClock = HIGH;
boolean buttonPressed = false;
char buffer[100];
boolean modeInterruption = true;

long readEncoderValue(void){
    return encoderValue/4;
}
void updateEncodeur(){
    int newClock = digitalRead(encoderPinA);
    if ((lastClock == LOW) && (newClock == HIGH)){
        if (digitalRead(encoderPinB) == HIGH){
            encoderValue++;
        } else {
            encoderValue--;
        }
    }
    lastClock = newClock;
}

boolean isButtonPushDown(void){
  if(!digitalRead(buttonPin)){
    delay(5);
    if(!digitalRead(buttonPin))
      return true;
  }
  return false;
}

void updateEncoderInterrupt(){
  int MSB = digitalRead(encoderPinA); //MSB = most significant bit
  int LSB = digitalRead(encoderPinB); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderValue ++;
  }
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderValue --;
  }

  lastEncoded = encoded; //store this value for next time
}

void setup() {
  int dureeInitSerial=0;
  int deltaTemps=1;
  Serial.begin (115200);
  while (!Serial) {
    delay(deltaTemps);
    dureeInitSerial+=deltaTemps;
  }

  Serial.println();
  Serial.println("test encodeur rotatif => debut");
  sprintf(buffer,"Init Serial en %d ms ", dureeInitSerial); Serial.println(buffer);
  if (false){
    //detection broches actives
    Serial.print("faites tourner la roue codeuse pour chaque nouvelle valeur");
    for (int i = 0 ; i < 30 ; i++){
      Serial.print("test de la broche ");
      digitalWrite(i,HIGH);
      Serial.print(i);
      long timeDebut = millis();
      int lastValue = digitalRead(i);
      while ((millis() - timeDebut) < DUREE_TEST){
        if (digitalRead(i) != lastValue){
          break;
        }
      }
      if ((millis() - timeDebut) < DUREE_TEST){
        // on a detecte un changement avant la fin du test
        Serial.println(" => OK");
      } else {
        // pas de changement on passe au suivant
        Serial.println();
      }
    }
  }


  pinMode(encoderPinA, INPUT_PULLUP); 
  pinMode(encoderPinB, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);

  digitalWrite(encoderPinA, HIGH); //turn pullup resistor on
  digitalWrite(encoderPinB, HIGH); //turn pullup resistor on


  if (modeInterruption){
    //call updateEncoder() when any high/low changed seen
    //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
    attachInterrupt(encoderPinA, updateEncoderInterrupt, CHANGE); 
    attachInterrupt(encoderPinB, updateEncoderInterrupt, CHANGE);
    //attachInterrupt(buttonPin, updateEncoderInterrupt, CHANGE);
    Serial.println("mode avec interruption");
  } else {
    Serial.println("mode pooling");
  }

  Serial.println();
  Serial.println("test encodeur rotatif => fin setup");
}

void loop(){ 
  //Do stuff here
  if (!modeInterruption){
    // en mode pooling on scan la roue codeuse a chaque boucle
    updateEncodeur();
  }

  if (lastEncoderValue != encoderValue){
    Serial.print("EncoderValue = ");
    Serial.println(encoderValue);
    lastEncoderValue = encoderValue;
  }
  if(isButtonPushDown() && !buttonPressed){
    Serial.println("you push button down!!!");
    buttonPressed = true;
  } else if (!isButtonPushDown() && buttonPressed) {
    Serial.println("you release button !!!");
    buttonPressed = false;
  }
  delay(1); //just here to slow down the output, and show it will work  even during a delay
}

