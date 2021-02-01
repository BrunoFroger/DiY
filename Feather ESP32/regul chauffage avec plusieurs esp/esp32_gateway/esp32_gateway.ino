//
//
//  ESP 32 Wemos gateway accessPoint Wifi
//
//




#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


#define LOOP_DELAY 10

// Replace with your network credentials
const char* ssid     = "gateway-chauffage";
const char* password = "0296911369";


// Set web server port number to 80
WiFiServer server(80);


// Variable to store the HTTP request
String header;

WiFiClient client;


//=================================================
//
//      setup
//
//=================================================
void setup() { 
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
    Serial.println("Serial initialized");

    Serial.print("Setting soft-AP ... ");
    boolean result = WiFi.softAP("Gateway_chauffage", "sdfhijsl5!qsd");
    if(result == true)
    {
        Serial.println("Ready");
    }
    else
    {
        Serial.println("Failed!");
    }
}

//=================================================
//
//      loop
//
//=================================================
void loop(){

    Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
    delay(3000);
}