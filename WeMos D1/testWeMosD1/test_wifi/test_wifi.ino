// ESP32_horloge
// test_wifi.ino.cpp
// (c) B. Froger 2020

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include "BFR_PA_Wifi.hpp"
#include "BFR_serveurWeb.hpp"
#include "BFR_Web_Pages.hpp"
 
LiquidCrystal_I2C lcd(0x27,16,2);
//LiquidCrystal_I2C lcd(0x23,16,2);
//LiquidCrystal_I2C lcd(0x3F,16,2); // set the LCD address to 0x3F for a 16 chars and 2 line display
 
char ipAdress[20];
char wifiSsid[30];
char wifiPassword[50];
BFR_PA_Wifi myWifiAccessPoint;
BFR_serveurWeb myServerWeb;

void setup()
{    delay(1000);
    // initialize serial communication
    Serial.begin(115200);
    int timeoutInitSerial = 100;
    while (timeoutInitSerial-- > 0)
    {
        if (Serial)
        break;
        delay(10);
    }
    delay(1000);
    if (timeoutInitSerial != 0)
    {
        Serial.println();
        Serial.println("Serial initialized");
    } else
    {
        Serial.println("Serial not initialized");
        exit(0);
    }

    Serial.println("+----------------------------------+");
    Serial.println("+                                  +");
    Serial.println("+          debut de SETUP          +");
    Serial.println("+          test_wifi.ino           +");
    Serial.println("+                                  +");
    Serial.println("+----------------------------------+");
    //------------
    // init LCD
    //------------
    lcd.init(); // initialize the lcd
    // Print a message to the LCD.
    lcd.setBacklight(HIGH); 
    lcd.clear();
    Serial.println("lcd initialise");
    lcd.setCursor(0,0);
    lcd.print("lcd OK");
    delay(1000);

    //------------
    // init du point d'acces wifi
    //------------
    myWifiAccessPoint.init("BFR_AP","");
    Serial.print ("nouveau ssid => ");
    Serial.println(myWifiAccessPoint.getSsid());
    lcd.setCursor(0,0);
    lcd.print("Wifi AP OK");
    lcd.setCursor(0,1);
    lcd.print(myWifiAccessPoint.getSsid());


    //------------
    // init du serveur web
    //------------
    myServerWeb.init();
    web_Pages_Init();


    Serial.println("+----------------------------------+");
    Serial.println("+          fin de SETUP            +");
    Serial.println("+----------------------------------+");
    return;


  //----------------------------------------
  //----------------------------------------
  //----------------------------------------
  //----------------------------------------
  //----------------------------------------


  //------------
  // init Wifi
  //------------
  //strcpy(wifiSsid,"Livebox-webcom");
  //strcpy(wifiPassword,"webcom2014");
  //strcpy(wifiSsid,"NETGEAR17");
  //strcpy(wifiPassword,"largesea818");
  strcpy(wifiSsid,"TP-LINK_88E2");
  strcpy(wifiPassword,"75546007");
  Serial.print("Connecting to ");
  Serial.print(wifiSsid);
  //Serial.print("/");
  //Serial.print(wifiPassword);
  Serial.println();
  delay(1000);
  WiFi.begin(wifiSsid, wifiPassword);  
  int cpt=0;
  while (WiFi.status() != WL_CONNECTED) {  //Attente de la connexion
      delay(500);
      //char tmp[10];
      //sprintf(tmp,"%d,",cpt);
      //Serial.print(tmp);
      Serial.print(".");   //Typiquement 5 à 10 points avant la connexion
      if (cpt++ >= 5){
          Serial.println();
          cpt=0;
          WiFi.begin(wifiSsid, wifiPassword);
      }
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  IPAddress tmpIp = WiFi.localIP();
  sprintf(ipAdress,"%d.%d.%d.%d",tmpIp[0],tmpIp[1],tmpIp[2],tmpIp[3]);
  //Serial.print(WiFi.localIP());
  Serial.print(ipAdress);
  Serial.println("/");  //Utiliser cette URL sous Firefox de preference à Chrome


}
 
void loop()
{
    // check request from clients
    myServerWeb->handleClient();
}
