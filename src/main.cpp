#include <Arduino.h>
#include <FastLED.h>
#include "ALUP/UdpConnection.h"
#include "ALUP/SerialConnection.h"
#include "ALUP/ALUP.h"
#include "WiFi_Credentials.h"

#define NUM_LEDS 10
#define DATA_PIN 12
#define CLOCK_PIN 13

CRGB leds[NUM_LEDS];


Alup alup(leds, DATA_PIN, CLOCK_PIN, NUM_LEDS);
//UdpConnection connection = UdpConnection(SSID, PASSWORD, "192.168.178.35", 5012);
SerialConnection connection = SerialConnection(115200);

void setup()
{
    //Serial.begin(115200);
    //Serial.println("Starting connection attempt");
    alup.Connect(&connection, "Test", "Extra values");
}
void loop()
{
    alup.Run();
}

