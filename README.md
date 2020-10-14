# Arduino-ALUP
An ALUP implementation for Arduino and Arduino-Compatible Boards using the Serial Connection. It is used to control an individually addressable LED strip from a Computer or any other device, using a microcontroller as a proxy (middle-man) over a USB / Serial connection.

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
  * ALUP v.0.1 (internal)
  
### Compatible Connection Types
  * USB (using Serial)

### Supported Microcontrollers:


Microcontroller | Confirmed working | Special instructions
--------------- |:-----------------:| --------------------
Ardino Uno       | :x: | 
Arduino Nano   | :heavy_check_mark: |
Arduino Micro   | :x: | USB to Serial Converter needed

Although only some of the listed microcontrollers are confirmed working, the other listed boards should work too.

:information_source: In theory, all boards compatible with the [FastLED library] should work, but I am unable to test all of them. If you can confirm that one of the boards not listed as confirmed above is working, we'll add it to the list. 

### Supported LED strips

 * All individually addressable LED strips supported by the [FastLED library]


### Protocol specific

#### Timeouts

Description| timeout in ms
--- | ---
Requesting connection | 250 per try, infinite tries
Waiting for configuration acknowledgement | 5000
Waiting for configuration error | 5000


#### Causes of Frame Errors

 * The Frame body size of a received frame is not a multiple of 3
 * The Frame `bodySize * 3` is bigger than `NUM_LEDS`
 * The Frame body size of a received frame is smaller than 0
 
 * The Frame body offset of a received frame is smaller than 0
 * The Frame body offset of a received frame is out of range (smaller or equal to `NUM_LEDS - (bodySize / 3)` )

## Requirements
Software:
* [Arduino IDE](https://www.arduino.cc/en/Main/software "Download for the Arduino IDE") (preferably the latest version)
* [FastLED library] (can be installed using the library manager, preferably the latest version)


Hardware:
* A supported Microcontroller
* A USB cable
* An individually addressable LED strip supported by the [FastLED library]

:warning: You may need additional components like an external power supply depending on your used hardware.




## Installation


### DISCLAIMER

[add disclaimer here]


1. Download the latest version of this sketch from [here] (...) (TODO: add link to latest release)
2. Configure it using the configuration guide below
3. Upload it to the microcontroller

## Configuration

### Set up the LEDs

To configure the sketch for your specific LED strip, scroll down  to `SetupLEDs()` in the sketch and comment everything except the line corresponding to the type of LEDs you use.

Example for WS2812B:

```cpp

/**
 * function setting up the leds for use with the FastLED protocol
 */
void SetupLEDs()
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

:information_source: If you notice that your LED strip displays the wrong colors (e.g. red instead of blue) while using, you can change the `RGB` in the uncommented line to one of `RGB`, `RBG`, `BRG`, `BGR`, `GRB` or `GBR`.




### Configuration values:

This implementation has some values which have to be configured. The following tables list all of those values and explain how to set them for your hardware and use case.

Name | Default value | Valid values | Description
--- | --- | --- | ---
`NUM_LEDS` | 1 | 1 - 715827882| The number of LEDs on the connected LED strip

To set the number of LEDs, simply count the LEDs on your addressable LED strip.

:information_source: If you want to just use part of this strip, simply decrease it.

:information_source: This value is limited by the amount of RAM on the microcontroller. 

<br/>

Name | Default value | Valid values | Description
--- | --- | --- | ---
 `DATA_PIN` | 3 | Any valid GPIO pin| The pin on the microcontroller to which the data line of the LED strip is connected 
 
 Set this value to the GPIO pin to which the data line of your LED strip is connected to.
 
 
 
<br/>
 
Name | Default value | Valid values | Description
--- | --- | --- | ---
 `CLOCK_PIN` | 4 | Any valid GPIO pin | The pin on the microcontroller to which the clock line of the LED strip is connected 
 
 Set this value to the GPIO pin to which the clock line of your LED strip is connected to. 

:information_source: Some LED strips don't need a clock signal. In this case, this value will be ignored, so set it to any positive number you like.
 

 
<br/>
 
 
 Name | Default value | Valid values | Description
--- | --- | --- | ---
 `FRAME_DELAY` | 0 | Any positive value or 0 | The delay between the frames in ms.
 
This value limits the maximum refresh rate of the LED strip.

:information_source: Some types of LEDs may start glitching if the rate at which the microcontroller tries to change them is too high. This value is used to prevent such glitches.

There are 2 ways to set it for your type of LED strip:

* Calculate the delay:

  Research the maximum refresh rate of your LED strip. For WS2812Bs it's 400Hz. To get the delay needed, divide 1000 by this refresh rate, and round it to the next bigger value if needed.
  
  For the WS2812B this would be `1000/400Hz = 2.5ms -> rounded: 3ms`.
  
* Try and error:

  Just try any value: If the LEDs glitch out, increase the `FRAME_DELAY` until you find a value which works

 
 
<br/>
 
 
  Name | Default value | Valid values | Description
--- | --- | --- | ---
`BAUD` | 115200 | Depending on the microcontroller | The baud rate used for serial communication

The baud rate represents the amount of bits per second transferred over the serial connection.

Its maximum value depends on the used hardware. When it is too high, you may experience `Frame Errors`, but the default value should cause no problems.

:warning: The baud rate set here has to be the same as the one used for the master device.

:information_source: If you have many LEDs connected to the microcontroller (500 or more), you may experience a lower frame rate when using the default baud rate. In this case you can try to increase the `BAUD` value.



<br/>

  Name | Default value | Valid values | Description
--- | --- | --- | ---
`DEVICE_NAME` | "ArduinoALUP" | Any String value | The name of this device. Set it to anything you like (multiple devices can have the same name)

The `DEVICE_NAME` can be set to anything you'd like.

Its use is to distinguish between different devices if multiple microcontrollers using this sketch are connected to the same computer, so try to use different names for each device you configure. 

:warning: Note that multiple devices are allowed to have the same name, which means that this sketch won't give you an error or warning if it happens.


<br/>

 Name | Default value | Valid values | Description
--- | --- | --- | ---
`EXTRA_VALUES` | "" | Any String value | Additional configuration values

This value is not required for normal use, unless the program you want to use with this sketch on your master device states otherwise.


<br/>

### Subprograms

#### Specifications

* Subprogram execution: Linear, before applying the LED data
* The Protocol command offset is already applied to the ID used in `ExecuteSubCommand()`


#### Implementation specific subprograms

A list of subprograms which are already implemented.

Function | ID | Description
--- | --- | ---
`ExampleSubProgram()` | 1 | An example subprogram which lets the built in LED blink 10 times


#### Adding your own subprograms

The following guide will show you how to add your own subprograms to this sketch. If you want to learn more about subprograms and subcommands, visit the [ALUP Documentation] (...) (TODO: ad link here)

1. Add your own subprogram by first creating a function containing the code you want to execute.

Example:
```cpp

/**
 * an example subprogram 
 */
void ExampleSubProgram()
{
  Blink(LED_BUILTIN, 10, 10);
}
```

:warning: Subprograms are executed inside the main thread. Your subprogram can therefore have a significant impact on the LED frame rate if it is resource heavy.

2. Find the function `ExecuteSubCommand()` and add your subprogram to the switch-case as described in the code:

```cpp
/**
 * function executing the given subcommand
 * @param id: the id of the command with the subcommand offset already applied according to the ALUP v. 0.1
 */
void ExecuteSubCommand(byte id)
{
    //Execute a subprogram depending on the given ID
    switch(id)
    {
      case 0:
        ExampleSubProgram();
        break;

      /*
       * add your own subprograms here by adding a new 'case' with an unused ID ranging from 0-247
       *Example subprogram with an ID of 1:
       *
       *case 1:
       *  MySubProgram();
       *  break;
       */
          
    }
}

```

The value entered after the 'case' represents the ID of your Subprogram. You will need this ID to execute it later.

:warning: The ID of your subprogram has to be unique and within the range of 0 - 247.

3. To execute your subprogram, send a frame using the ID to the microcontroller. For more information on how to execute subprograms, see the documentation of the used master device implementation.


## Usage

Connect the microcontroller to the PC using a USB cable.
Use a program which implements the ALUP, or write your own by using one of the master device implementations (TODO: add link) to write your own program controlling the LEDs.


## Contributing

If you want to contribute to this project, please see CONTRIBUTING.md (TODO: add link)

## Credits

Libraries used:

* [FastLED Library]


## License

This project is licensed under the MIT License. For more information, see [LICENSE](https://github.com/Skyfighter64/Arduino-ALUP/blob/master/LICENSE)


[FastLED Library]: https://github.com/FastLED/FastLED
