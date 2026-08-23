# Arduino-ALUP
Reference Implementation for ALUP Receivers using Arduino and Arduino-Compatible Boards. 


## What it is:
ALUP is a protocol for communicating RGB data from a sender (eg. python program) to a receiver (Arduino/ESP) to control individually addressable LED strips.
Stream RGB data in real-time from your PC to addressable LED strips over USB or the network.

This code implements the receiver-side (Arduino/ESP/...) of the ALUP protocol which directly controls the LED strips.

### Features:
- _Real-Time_: Designed for real-time applications such as light shows, audio-visualizers, animations and more.
- _Time-Synchronization_: Automatic synchronization of the receivers internal time on the sender side.  
- _Time Stamps_ (optional): Specify the time at which the received colors are applied to the LEDs.
- _Buffering_ (optional): Receive multiple frames in advance to balance out connection jitter. 
- _Drift Correction_ (optional): Manually correct constant drift of the internal clock. 

## Table of Contents

This README includes the following points:

* Specifications
* Requirements
* Installation
* Configuration
* Usage
* Contributing
* Credits
* License


## Specifications

### Compatible Protocol Versions
  * ALUP v.0.3 
  
### Implemented connections
  + TCP over WiFi 
  * UDP over WiFi (untested with v.0.3)
  * Serial over USB

### Supported Microcontrollers:

Microcontroller | Tested | Special instructions
--------------- |:-----------------:| --------------------
Ardino (Uno/Nano/...)       | :heavy_check_mark: | Limited memory, use of Buffering not recommended. Inaccurate timekeeping, manual drift correction may be needed.  
ESP8266                     | :heavy_check_mark: | Multiple ways of digital pin selection. See [here](https://github.com/FastLED/FastLED/wiki/ESP8266-notes "FastLED notes on ESP8266") for details
ESP32                       | ? | 


:information_source: All boards compatible with the Arduino Framework and the [FastLED library] should work in practice

### Supported LED strips

 * All individually addressable RGB-LED strips supported by the [FastLED library].


## Requirements
Software:
* Visual Studio Code with PlatformIO
* [FastLED library] (installed via PlatformIO)


Hardware:
* A  microcontroller (Arduino/ESP32/D1 Mini/...)
* A USB cable (for Serial Connection only)
* Individually addressable LEDs

:warning: Additional hardware like an external power supply or controller board might be needed for more than 10 LEDs.




## Installation

1. Clone this repository
2. Configure `main.cpp` (see below)
3. Compile and upload to the microcontroller

## Configuration

### Set up the LEDs

To configure the sketch for your specific LED strip, set up FastLED by going to `src/main.cpp` and editing the LED type in `setup()` corresponding to your LEDs.
Possible types from the FastLED examples:

```cpp

/**
 * function setting up the leds for use with the FastLED protocol
 */
void setup()
{
  /*
   * The setup for the connected LED strip
   * Uncomment/edit one of the following lines for your type of LEDs
   * 
   * You may also change the frame delay, data and clock pin, and number of LEDs at the top of this file depending on your LEDs
   * 
   * If your LEDs show the wrong colors (e.g. Blue instead of Red, etc), change the "RGB" in your uncommented line 
   * to either RGB, RBG, BRG, BGR, GRB or GBR
   */
      // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
       FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
      //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
      // FastLED.addLeds<APA104, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903B, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<GW6205, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<GW6205_400, DATA_PIN, RGB>(leds, NUM_LEDS);
      
      // FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<P9813, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<APA102, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<DOTSTAR, RGB>(leds, NUM_LEDS);

      // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
}
```

:information_source: If the LED strip displays the wrong colors (e.g. red instead of blue) while using, try changing the `RGB` in the uncommented line to one of `RGB`, `RBG`, `BRG`, `BGR`, `GRB` or `GBR`.




### Configuring ALUP:

This implementation has some values which have to be configured. The following tables list all of those values and explain how to set them for your hardware and use case.


#### Configuring FastLED (`main.cpp`):
1. Set `NUM_LEDS` to the number of LEDs on the connected LED strip. 
   Note: The maximum number of LEDs might be limited by the microcontrollers memory. Decrease if you receive memory-related errors. 

2. Set the GPIO-Pin at which the LEDs are connected:
  - `DATA_PIN`: the pin at which the Data-Wire is connected
  - `CLOCK_PIN`: the pin ath which the Clock-Wire is connected (optional, ignored if not needed)

#### Configuring the connection (`main.cpp`): 
  1. Uncomment the `#import `- Statement for your chosen connection type. If using TCP/UDP over WiFi, create a `WiFi_Credentials.h`-file inside the `src`-folder with the following contents and replace the default values with your WiFi SSID and password:
    ```cpp
    #ifndef WIFI_CREDENTIALS_H
    #define WIFI_CREDENTIALS_H

    #define SSID (char*) "my_wifi_name"
    #define PASSWORD (char*) "my_wifi_password"

    #endif
    ```
    :warning: Please watch out to NOT push this file to any git-repository! 
  2. Uncomment the statement for the used connection type.
      eg. For Serial:
      ```cpp
      //UdpConnection connection = UdpConnection(SSID, PASSWORD, "192.168.178.35", 5012);
      //TcpConnection connection = TcpConnection(SSID, PASSWORD, 5012);
      SerialConnection connection = SerialConnection(115200);
      ```

    - *Serial:* Set the baud-rate used for communication. Has to be set to the same value on the Sender. Default: 115200
    - *TCP:* 
        - SSID and PASSWORD: Already configured from `WiFiCredentials.h`.
        - Set the TCP Port. Has to be the same value on the Sender. Default 5012
    - *UDP:* 
        - SSID and PASSWORD are already configured from `WiFiCredentials.h`.
        - Set the IP-Address of the Sender.
        - Set the TCP Port. Has to be the same value on the Sender. Default 5012

#### Configuring ALUP (`include/config.h, include/configs/`):
  1. Create a new config file in `include/configs/` or use/copy an existing config file.
  2. Set the configuration values according to your liking. Each value is explained in the default configuration file `include/configs/default.h`
  3. Select your config file in `platformio.ini` by adding 
  ```ini
  build_flags = -D CONFIG_FILE=\"configs/d1mini_tcp.h\"
  ```
  to your build environment.
  - NOTE: If no valid config file is specified, `include/configs/default.h` will be used as a default. 

#### Time Drift Correction

To correct the constant drift of the internal clock, measure the drift correction factor over multiple hours (eg. using the test script from my ALUP-Controller repository) and enter it in `ALUP/Time.h` for `TIME_CORRECTION_FACTOR`

#### Buffering

For devices with a lot of memory (eg. ESP32) or few LEDs, frame buffering can be used to even out some fluctuations in connection latency. To do so, set `FRAME_BUFFER_SIZE` in `ALUP/ALUP.h` to the desired number of frames which should be buffered or to `1` to disable buffering.

When buffering is used, newly received Frames will immediately return an acknowledgement to the Sender, but wait in the buffer until their time stamp is reached. As soon as the time stamp is reached, they will be applied to the LED strip.

Note that the frames in the buffer are not sorted by time stamps but by arrival time. If a newer Frame has an earlier timestamp than a Frame from the buffer, it will NOT be prioritized.


## Commands

This implementation supports the following ALUP Comamnds:

Command (Value) | Description
-----------------------------
NONE (0) | No command. Apply color values to the LEDs normally.
CLEAR (1) | Set all LEDs to black. If Color values are given, they will be applied after clearing the LEDs.
DISCONNECT (2) | Close the ALUP Connection and free resources. Makes it possible for another Sender to connect to this device
TOGGLE_INTERNAL_LED (4) | Toggle the builtin LED of the microcontroller if present. Can be used for testing purposes.

#### Custom Commands

To create custom commands, add your custom functionalities to `src/ALUP.cpp: Alup::ApplyFrame(...)` with its corresponding ID Value (can be from 8 to 255).

If the chosen command is received from a Sender, the custom code will be executed. 

Custom commands can decide on their own how the frame body is used and interpreted, there will be no other action taken. Furthermore, they should return `-1` on success and a Frame Error Code otherwise (see `ALUP.h`).


## Usage
Use an ALUP Sender implementation (such asl Python-ALUP) to send RGB Color data, commands and more to the receiver.


## Credits

Libraries used:

* [FastLED Library]


## License

This project is licensed under the MIT License. For more information, see [LICENSE](https://github.com/Skyfighter64/Arduino-ALUP/blob/master/LICENSE)


[FastLED Library]: https://github.com/FastLED/FastLED
