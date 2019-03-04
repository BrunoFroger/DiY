//
//  test Capteur de temperature
//
//  Created by    : Bruno FROGER
//  creation date : 2019-01-01
//
// Pour Arduino Nano
// processeur ATMega328P (old Bootloader)
//

#include <arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


Adafruit_SSD1306 display = Adafruit_SSD1306();

//*********************************************
//*
//*       S E T U P
//*
//*********************************************
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

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.setTextColor(WHITE);

}

int mesure; 
long temperature;
long tmp;

//*********************************************
//*
//*       L O O P
//*
//*********************************************
void loop() {

  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);

  Serial.println("==============================");
  mesure = analogRead(A0);
  Serial.print("mesure      = ");
  Serial.println(mesure);
  display.print("mes  ");
  display.println(mesure);
  temperature = map(mesure,0,1023,-3272,10035);
  Serial.print("temperature = ");
  Serial.print(temperature/100);
  Serial.print(",");
  Serial.println(temperature-(temperature/100));
  display.print("temp ");
  display.print(temperature/100);
  display.print(",");
  display.println(temperature-(temperature/100));

  display.display();

  delay(1000);
}


