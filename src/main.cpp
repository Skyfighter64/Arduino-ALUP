#include <Arduino.h>
#include <FastLED.h>

// include the debug macros?
#define DEBUG_ON 1
#define DEBUG_USE_TELNET 1

#include "ESPTelnet.h"


ESPTelnet telnet;
// choose the connection type
//#include "ESP32/UdpConnection.h"
//#include "ESP8266/TcpConnection.h"
#include "ALUP/SerialConnection.h"

#include "ALUP/ALUP.h"

#include "WiFi_Credentials.h"

#define NUM_LEDS 100
#define DATA_PIN 14
#define CLOCK_PIN 4

CRGB leds[NUM_LEDS];





Alup alup(leds, NUM_LEDS, DATA_PIN, CLOCK_PIN);
//UdpConnection connection = UdpConnection(SSID, PASSWORD, "192.168.178.35", 5012);
//TcpConnection connection = TcpConnection(SSID, PASSWORD, 5012);
SerialConnection connection = SerialConnection(115200);

void setup()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);
    
   //wait until the connection is established 
    while(WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
    }
    //initialize the LEDS
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    
    telnet.begin(23);
}
void loop()
{
    telnet.loop();
    //try to connect if not connected
    if(!alup.connected)
    {
        telnet.println("Waiting for ALUP Connection");
        //try to connect/reconnect
        delay(1000);
        alup.Connect(&connection, "D1 Mini", "LEDs: WS2812b");
      
    }
    //run the ALUP main loop
    alup.Run();
}
