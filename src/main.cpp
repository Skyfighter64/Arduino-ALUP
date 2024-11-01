#include <Arduino.h>
#include <FastLED.h>
//#include "UdpConnection.h"
#include "ALUP/SerialConnection.h"
#include "ALUP/ALUP.h"

//#include "WiFi_Credentials.h"

#define NUM_LEDS 10
#define DATA_PIN 13
#define CLOCK_PIN 12

CRGB leds[NUM_LEDS];


Alup alup(leds, NUM_LEDS, DATA_PIN, CLOCK_PIN);
//UdpConnection connection = UdpConnection(SSID, PASSWORD, "192.168.178.35", 5012);
SerialConnection connection = SerialConnection(115200);

void setup()
{
    //initialize the LEDS
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    
}
void loop()
{
    //try to connect if not connected
    if(!alup.connected)
    {
      //try to connect/reconnect
      delay(1000);
      alup.Connect(&connection, "Test", "Extra values");
      
    }
    //run the ALUP main loop
    alup.Run();
}
