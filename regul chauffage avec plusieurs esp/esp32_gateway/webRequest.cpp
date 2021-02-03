//
//
//  webRequest.cpp
//
//

#include <Arduino.h>
#include <String.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "analyseHeader.hpp"

String header = "";

//=================================================
//
//      analyseRequete
//
//=================================================
void analyseRequete(WiFiClient client){
    if (client){
        String currentLine = "";           // read a byte, then
        header = "";
        while (client.connected()){
            if (client.available()){
                char car = client.read();
                //Serial.write(car);          
                header += car;
                if (car == '\n') { 
                    if (currentLine.length() == 0) {
                        analyseHeader(client, header);
                        break;
                    }  else { // if you got a newline, then clear currentLine
                        currentLine = "";
                    }

                } else if (car != '\r') {  // if you got anything else but a carriage return character,
                    currentLine += car;      // add it to the end of the currentLine
                }            
            }
        }
        // Clear the header variable
        header = "";
        // Close the connection
        client.stop();
    }
}