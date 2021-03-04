// chauffage
// encoder.cpp
// (c) B. Froger 2020


#include <Arduino.h>

int val;
int encoderPinDT = 14;          // (DT)  encodeur B
int encoderPinCLK = 32;         // (CLK) encodeur A
int encoderPinButton = 33 ;             // (SW)  bouton
volatile int lastEncoded = 0;
volatile long encoderValue = 0;
int encoderPinCLKLast = LOW;
int newClock = LOW;
boolean buttonPressed = false;
boolean modeInterruption = true;

//=================================================
//
//      interruptionEncoder
//
//=================================================
void interruptionEncoder(){
  int MSB = digitalRead(encoderPinCLK); //MSB = most significant bit
  int LSB = digitalRead(encoderPinDT); //LSB = least significant bit

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

//=================================================
//
//      initEncoder
//
//=================================================
void initEncoder(void){
    pinMode (encoderPinCLK, INPUT_PULLUP);
    digitalWrite(encoderPinCLK,HIGH);
    pinMode (encoderPinDT, INPUT_PULLUP);
    digitalWrite(encoderPinDT,HIGH);
    pinMode (encoderPinButton, INPUT_PULLUP);
    digitalWrite(encoderPinButton,HIGH);
    buttonPressed = false;

    if (modeInterruption){
        //call updateEncoder() when any high/low changed seen
        //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
        attachInterrupt(encoderPinCLK, interruptionEncoder, CHANGE); 
        attachInterrupt(encoderPinDT, interruptionEncoder, CHANGE);
        Serial.println("initEncoder => mode avec interruption");
    } else {
        Serial.println("initEncoder => mode pooling");
    }

}

//=================================================
//
//      checkEncoder
//
//=================================================
void checkEncoder(void){
    if (!modeInterruption){
        // a ne faire que si on est pas en mode interruption !!
        newClock = digitalRead(encoderPinCLK);
        if ((encoderPinCLKLast == LOW) && (newClock == HIGH)) {
            if (digitalRead(encoderPinDT) == LOW) {
                encoderValue--;
            } else {
                encoderValue++;
            }
            //Serial.print("checkEncoder : ");
            //Serial.print (encoder0Pos);
            //Serial.println();
        }
        encoderPinCLKLast = newClock;
        if (digitalRead(encoderPinButton) == LOW){
            //Serial.println("bouton appuyé");
            buttonPressed=true;
        }
    }
}

//=================================================
//
//      getEncoder
//
//=================================================
int getEncoder(void){
    return encoderValue;
}

//=================================================
//
//      isButtonPressed
//
//=================================================
boolean isButtonPressed(void){
    return buttonPressed;
}

//=================================================
//
//      setEncoder
//
//=================================================
void setEncoder(int value){
    encoderValue = value;
}

//=================================================
//
//      resetEncoder
//
//=================================================
void resetEncoder(int value){
    setEncoder(0);
}
