#include <Arduino.h>
#include <FastLED.h>

#include "ALUP/ALUP.h"
#include "config.h"

// import the platform dependent connection type
#if defined(SERIAL_CONNECTION)
    #include "ALUP/SerialConnection.h"
#elif defined(TCP_ESP8266)
    #include "ESP8266/TcpConnection.h"
    #include "WiFi_Credentials.h"
#elif defined(UDP_ESP32)
    #include "ESP32/UdpConnection.h"
    #include "WiFi_Credentials.h"
#else
    #error No valid connection selected
#endif


CRGB leds[NUM_LEDS];
Alup alup(leds, NUM_LEDS, DATA_PIN, CLOCK_PIN);


//select the connection type according to the config file
#if defined(SERIAL_CONNECTION)
auto connection = SerialConnection(BAUD_RATE);
#elif defined(TCP_ESP8266)
auto connection = TcpConnection(SSID, PASSWORD, PORT);
#elif defined(ESP32)
auto connection = UdpConnection(SSID, PASSWORD, IP_ADDRESS, PORT);
#endif


void setup()
{
    //initialize the LEDS
    #if CLOCK_PIN < 0
    FastLED.addLeds<LED_CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    #else
    FastLED.addLeds<LED_CHIPSET, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    #endif
}
void loop()
{
    //try to connect if not connected
    if(!alup.connected)
    {
      //try to connect/reconnect
      delay(1000);
      alup.Connect(&connection, ALUP_NAME, ALUP_EXTRA_VALUES);
      
    }
    //run the ALUP main loop
    alup.Run();
}
