#pragma once

// number of LEDS at the connected strip
#define NUM_LEDS 100

/*
 digital IO pins for the data and clock signal.
 Clock signal depends on led chipset, set to -1 if not used.
*/
 #define DATA_PIN 14
#define CLOCK_PIN -1

/* chipset of the used LED strips
 one of the supported FastLED chipset names
 see https://github.com/FastLED/FastLED/wiki/Chipset-reference
*/
 #define LED_CHIPSET WS2812B
/*
 LED color order of the LED strip: RGB/GRB/BGR/...
*/
 #define COLOR_ORDER GRB


// Name of this ALUP Device shown to the sender
#define ALUP_NAME "Arduino"
// Additional text string which can be used for custom configuration data
#define ALUP_EXTRA_VALUES "LEDs: " STR(LED_CHIPSET)

/* 
Set the maximum number of frames to buffer.
Depends on the memory of the used microcontroller and NUM_LEDS.
For Arduino Uno/Nano: ~1-2
For ESP32/8266: ~5
Decrease if OutOfMemoryErrors occur.
*/
 #define FRAME_BUFFER_SIZE 1

/*
Select one ALUP connection type.
Note: This does not necessarily depend on the platform.
For example, SERIAL can used by any platform which supports
serial communication.
*/
#define SERIAL_CONNECTION
//#define TCP_ESP8266
//#define UDP_ESP32

/*
Define connection-specific settings
*/
#if defined(SERIAL_CONNECTION)
    #define BAUD_RATE 115200
#elif defined(TCP_ESP8266)
    // network name for the mDNS discovery. Should be unique on the local network
    #define NETWORK_NAME "Arduino"
    // network port to use for ALUP communication
    #define PORT 5012
#elif defined(UDP_ESP32)
    #define IP_ADDRESS "192.168.178.35"
    #define PORT 5012
#endif


