//
//
//  testEncodeurRotatif.ino
//
//



int encoderPinA = 14;
int encoderPinB = 12;
int buttonPin = 2;

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;
int lastClock = HIGH;

long readEncoderValue(void){
    return encoderValue/4;
}
void updateEncodeur(){
    int newClock = digitalRead(encoderPinA);
    if ((lastClock == LOW) && (newClock == HIGH)){
        if (digitalRead(encoderPinB) == HIGH){
            Serial.print("encodeur.cpp : readEncodeur-- => ");
            encoderValue--;
        } else {
            Serial.print("encodeur.cpp : readEncodeur++ => ");
            encoderValue++;
        }
        Serial.println(encoderValue);
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

void setup() {
  Serial.begin (115200);

  pinMode(encoderPinA, INPUT_PULLUP); 
  pinMode(encoderPinB, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);

  digitalWrite(encoderPinA, HIGH); //turn pullup resistor on
  digitalWrite(encoderPinB, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
  //attachInterrupt(encoderPinA, updateEncoder, CHANGE); 
  //attachInterrupt(encoderPinB, updateEncoder, CHANGE);

}

void loop(){ 
  //Do stuff here
  updateEncodeur();
  if(isButtonPushDown()){
    Serial.println("you push button down!!!");
  }
  delay(1); //just here to slow down the output, and show it will work  even during a delay
}


void updateEncoder(){
  int MSB = digitalRead(encoderPinA); //MSB = most significant bit
  int LSB = digitalRead(encoderPinB); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

  lastEncoded = encoded; //store this value for next time
}