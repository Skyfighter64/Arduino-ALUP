# Arduino-ALUP
An ALUP implementation for Arduino and Arduino-Compatible Boards using the Serial Connection. It is used to control an individually adressable LED strip from a Computer or any other device, using a microcontroller as a proxy (middle-man) over a USB / Serial connection.

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
* [Arduino IDE](https://www.arduino.cc/en/Main/software "Download for the Arduino IDE") (preferably the latest version)
* [FastLED library] (can be installed using the library manager, preferably the latest version)


Hardware:
* A supported Microcontroller
* A USB cable
* An indiviually addressable LED strip supported by the [FastLED library]

:warning: you may need additional components like an external power supply depending on your used hardware


#### Supported Microcontrollers:


Microcontroller | Confirmed working | Special instructions
--------------- | ----------------- | --------------------
Ardino Uno compatible boards      | No | 
Arduino Nano compatible boards    | No |
Arduino Micro compatible boards  | No | USB to Serial Converter needed

Although only some of the listed microcontrollers are confirmed working, the other listed boards should work too.
In theory, all boards compatible with the [FastLED library] should work, but I am unable to test all of them. If you can confirm that one of the boards not listed above is working, we'll add it to the list. 


## Installation


### DISCLAIMER

[add disclaimer here]


1. Download the latest version of this sketch from [here] (...) (TODO: add link to latest release)
2. Configure it using the configuration guide below
3. Upload it to the microcontroller

The device is now fully set up and can be used with any serial ALUP master device implementation. (TODO: replace with "driver" ?)

## Configuration

Once you opened the skech in the Arduino IDE, you are ready to configure it for your specific use.
To do so, change the following values inside the sketch to fit your usecase:

Configuration values:


Name | Default value | Valid values | Description
--- | --- | --- | ---
`NUM_LEDS` | 1 | 1 - 715827882| The number of LEDs on the connected LED strip

To set the number of LEDs, simply count the LEDs on your adressable LED strip.

If you want to just use part of this strip, simply decrease it. This value has to be  at least 1 and can be a maximum of 715 827 882 (limited by the maximum frame body size), even though the practical maximum number is lower depending on the amount of RAM of the microcontroller. 

<br/>

Name | Default value | Valid values | Description
--- | --- | --- | ---
 `DATA_PIN` | 3 | Any valid GPIO pin| The pin on the microcontroller to which the data line of the LED strip is connected 
 
 Set this value to the GPIO pin to which the data line of your LED strip is connected to
 
 
 
<br/>
 
Name | Default value | Valid values | Description
--- | --- | --- | ---
 `CLOCK_PIN` | 4 | Any valid GPIO pin | The pin on the microcontroller to which the clock line of the LED strip is connected 
 Set this value to the GPIO pin to which the clock line of your LED strip is connected to. 

:information_source: Some LED strips don't need a clock signal. In this case, this value will be ignored, so set it to any psoitive number you like.
 

 
<br/>
 
 
 Name | Default value | Valid values | Description
--- | --- | --- | ---
 `FRAME_DELAY` | 0 | Any positive value or 0 | The delay between the frames in ms.
 
:information_source: Some types of LEDs may start glitching if the rate at which the microcontroller tries to change them is too high. This value is used to prevent such glitches.

This value limits the maximum refresh rate of the LED strip. There are 2 options to set it for your type of LED strip:

* Caclulate the delay:
  Research the maximum refresh rate of your LED strip. For WS2812Bs it's 400Hz. To get the delay needed, divide 1000 by this refresh rate, and round it to the next bigger value if needed. For the WS2812B this would be `1000/400Hz = 2.5ms -> rounded: 3ms`.
  
* Try and error:
  Just try any value: If the LEDs glitch out, increase the `FRAME_DELAY` until you find a value which works

 
 
<br/>
 
 
  Name | Default value | Valid values | Description
--- | --- | --- | ---
`BAUD` | 115200 | Depending on th microcontroller | The baud rate used for serial communication

:warning: The baud rate set here has to be the same as the one used for the master device

The baud rate represents the amount of bits per second transfered over the serial connection.

The maximum value depends on the used hardware used. When it is too high, you may experience `Frame Errors`, but its default value should cause no problems.

:information_source: If you have many LEDs connected to the microcontroller (500 or more), you may have a lower frame rate when using the default baud rate (Try to increase it).



<br/>

  Name | Default value | Valid values | Description
--- | --- | --- | ---
`DEVICE_NAME` | "ArduinoALUP" | Any String value | The name of this device. Set it to anything you like (multiple devices can have the same name)

The `DEVICE_NAME` can be set to aything you'd like.

Its use is to distinguish between different devices if multiple microcontrollers using this sketch are connected to the same computer, so try to use differen names for each device you configure. 
:warning: Note that multiple devices are allowed to have the same name, which means that this sketch won't give you an error or warning if it happens.


<br/>

 Name | Default value | Valid values | Description
--- | --- | --- | ---
`EXTRA_VALUES` | "" | Any String value | Additional configuration values
<br/>
This value is not required for normal use, unless the program you want to use with this sketch on your master device states otherwise.


<br/>

### Subcommands

[Guide on how to add custom subcommands]

## Usage

Connect the microcontroller to the PC using a USB cable.
Use a program which imlements the ALUP, or write your own by using one of the master device implementations (TODO: add link) to write your own program controlling the LEDs.


## Contributing

[Guide on how to contribute]


## License

This project is licensed under [add license here]. For more information, see LICENSE


[FastLED Library]: https://github.com/FastLED/FastLED
