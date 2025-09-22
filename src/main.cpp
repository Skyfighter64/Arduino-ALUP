#include <Arduino.h>
#include <FastLED.h>

// choose the connection type
//#include "ESP32/UdpConnection.h"
#include "ESP8266/TcpConnection.h"
#include "ALUP/SerialConnection.h"

#include "ALUP/ALUP.h"

#include "WiFi_Credentials.h"

#define NUM_LEDS 100
#define DATA_PIN 2
#define CLOCK_PIN 12

CRGB leds[NUM_LEDS];


Alup alup(leds, NUM_LEDS, DATA_PIN, CLOCK_PIN);
//UdpConnection connection = UdpConnection(SSID, PASSWORD, "192.168.178.35", 5012);
TcpConnection connection = TcpConnection(SSID, PASSWORD, 5012);
//SerialConnection connection = SerialConnection(115200);
//SerialConnection connection = SerialConnection(2000000);

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
    alup.Connect(&connection, "Arduino Nano", "timesync EXPERIMENTAL, LEDs: WS2812b");
    
  }
  //run the ALUP main loop
  alup.Run();
}
