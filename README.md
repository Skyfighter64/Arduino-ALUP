# Arduino-ALUP
An ALUP implementation for Arduino and Arduino-Compatible Boards using the Serial Connection

## Table of Contents

This README includes the following points:

* Requirements
* Installation
* Configuration
* Usage
* Contributing
* License



## Requirements
Software:
* The [Arduino IDE](https://www.arduino.cc/en/Main/software "Download for the Arduino IDE") (preferably the latest version)
* The [FastLED library] (can be installed using the library manager, preferably the latest version)


Hardware:
* A supported Microcontroller
* A USB cable
* An indiviually addressable LED strip supported by the [FastLED library]

:warning: you may need additional hardware like an external power supply depending on your used hardware


The following Microcontrollers are supported by this implementation:


Microcontroller | Confirmed working | Specia instructions
--------------- | ----------------- | --------------------
Ardino Uno compatible boards      | No | 
Arduino Nano compatible boards    | No |
Arduino Micro compatible boards  | No | USB to Serial Converter needed

Although only some of the listed Microcontrollers are confirmed working, the other listed boards should work too.
In theory, all boards compatible with the [FastLED library] should work, but I am unable to test all of them. If you can confirm that one of the boards not listed above is working, we'll add it to the list. 


## Installation


### DISCLAIMER

[add disclaimer here]


1. Download the latest version of this sketch from [here] (...) (TODO: add link to latest release)
2. Configure it using the configuration guide below
3. Connect the microcontroller to the PC using an USB cable
3. Upload it to the microcontroller

The device is now fully set up and can be used with any serial ALUP master device implementation. (TODO: replace with "driver" ?)

## Configuration

Once you opened the skech in the Arduino IDE, you are ready to configure it for your specific use.
To do so, change the following values inside the sketch to fit your usecase:

Value | Default value | Description
--- | --- | ---
`NUM_LEDS` | 1 | The number of LEDs on the connected LED strip
 `DATA_PIN` | 1 | The data pin of the Microcontroller which is connected to the data terminal of the LED strip
 `CLOCK_PIN` | 1 | The clock pin of the Microcontroller which is connected to the clock terminal of the LED strip. :warning: Some LED strips don't need a clock signal. In this case, this value will be ignored, so set it to anything you like.
## Usage

[Guide on how to use]


## Contributing

[Guide on how to contribute]


## License

This project is licensed under [add license here]. For more information, see LICENSE


[FastLED Library]: https://github.com/FastLED/FastLED
