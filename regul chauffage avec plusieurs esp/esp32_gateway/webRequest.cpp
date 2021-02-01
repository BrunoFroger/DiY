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
                Serial.write(car);          
                header += car;
                if (car == '\n') { 
                    if (currentLine.length() == 0) {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();
                        Serial.println("200 OK envoye");

                        // Display the HTML web page
                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        client.println("<link rel=\"icon\" href=\"data:,\">");
                        // CSS to style the on/off buttons 
                        // Feel free to change the background-color and font-size attributes to fit your preferences
                        client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                        client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
                        client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                        client.println(".button2 {background-color: #555555;}</style></head>");
                        
                        Serial.println("header = " + header);
                        analyseHeader(client, header);
                        /*
                        // Web Page Heading
                        client.println("<body><h1>ESP32 Web Server</h1>");
                        // TODO 
                        client.println("<p>ca marche</p>");
                        client.println("</body></html>");
                        client.println();
                        // Break out of the while loop
                        */
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
        Serial.println("Client disconnected.");
        Serial.println("");
    }
}