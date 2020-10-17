#include <FastLED.h>


/*
 *  protocol constants 
 *  
 *  do not change any of them
 *
 */

#define PROTOCOL_VERSION "0.1 (internal)"

#define CONNECTION_REQUEST_BYTE 255
#define CONNECTION_ACKNOWLEDGEMENT_BYTE 254
#define CONFIGURATION_START_BYTE 253
#define CONFIGURATION_ACKNOWLEDGEMENT_BYTE 252
#define CONFIGURATION_ERROR_BYTE 251
#define FRAME_ACKNOWLEDGEMENT_BYTE 250
#define FRAME_ERROR_BYTE 249

#define SUBCOMMAND_OFFSET 8



/*
 * Toggle for debug mode
 * 
 * Comment to deactivate debug LEDs, uncomment to activate
 * 
 * The debug mode is using 5 separate standard (non-addressable) LEDs and 3 buttons 
 * to help for following the protocol flow of this device and debug the program
 */
 
#define debug

/*
  * Debug LED Signals:
  * 
  * Blue 0 (led_0)
  *   State: flashing every 250ms for 100ms 
  *   Description: Waiting for connection
  *   
  *   State: On
  *   Description: Connected
  *   
  *   
  * Blue 1 (led_1)
  *   State: On
  *   Description: Try to connect
  *   
  *   State: flashing 10 times, delay: 10ms, totoal: 0.1s
  *   Description: Test Subprogram
  *   
  *   
  * Green: (led_2)   
  *   State: On
  *   Description: Connected 
  *   
  *   State: Off
  *   Description: Not connected
  *   
  *Red 0: (led_3) 
  *   State: flashing 5 times, delay: 250ms, total: 2.5s
  *   Description: Faulty Frame Body received (Frame Error)
  *   
  *   State: flashing 100 times, delay: 100ms, total: 20s
  *   Description: Memmory allocation error (Probably out of RAM)
  *   
  *   
  *Red 1: (led_4)   
  *   State: On for 200ms
  *   Description: Discarded faulty frame (after Frame Error)
  *   
  *   
  */
 


//define the pins of the buttons
#define button_0 2
#define button_1 7
#define button_2 8


//define the pins of the leds
#define led_0 3
#define led_1 5
#define led_2 6
#define led_3 10
#define led_4 11


/*
 *    Configuration
 * 
 *  The folowing values are for configuring this sketch:
 * 
 * 
 */



/*
 * the values for the connected LED Strip
 * Change them depending on your configuration
 */
#define NUM_LEDS 10
#define DATA_PIN 12
#define CLOCK_PIN 13


/*
 * the delay for each frame in ms
 * This delay depends on the type of LEDs and its maximum refresh rate
 * 
 * WS28121b:
 *  This type of LEDs have a maximum refresh rate of 400hz which translates to at least 2.5ms needed delay between each frame
 *  Therefore, the recommended value for this type is 3
 *  
 *  
 *  TOD: improvement idea: measure execution time for each frame and subtract it from the delay
 */
#define FRAME_DELAY 3

/*
 * the baud rate for serial communication
 * make sure to set the same value to both devices
 */
#define BAUD 115200

/*
 * the device name 
 * set it to anything you like
 */
#define DEVICE_NAME "ArduinoALUP"

/*
 * extra configuration values
 * Not required for normal use, unless stated otherwise in the configuration guide
 */
#define EXTRA_VALUES ""





/*
 * enumerator with all connection states
 */
enum ConnectionState
{
  Connecting,
  Connected,
  Disconnected
};

//the current connection state
ConnectionState state;

// Define the array of leds
CRGB leds[NUM_LEDS];



void setup() {


  //setup for debugging

#ifdef debug
  pinMode(button_0, INPUT);
  pinMode(button_1, INPUT);
  pinMode(button_2, INPUT);


  //initialize leds
  pinMode(led_0, OUTPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_4, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);


  //turn all leds off
  digitalWrite(led_0, LOW);
  digitalWrite(led_1, LOW);
  digitalWrite(led_2, LOW);
  digitalWrite(led_3, LOW);
  digitalWrite(led_4, LOW);
#endif

  SetupLEDs();
  //open the serial connection
  Serial.begin(BAUD);
  

}

void loop() 
{
  state = Disconnected;
  
  RequestConnection();
  state = Connecting;
   
  //try to start a ALUP connection over the serial connection
  int result = Connect();
  if(result == 0)
  {
    
    
    //turn on the green led to notify the user that the process is finished
    #ifdef debug        
    digitalWrite(led_2, HIGH);
    #endif    

    //connected successfully
    state = Connected;

    //read in data frames while connected
    while(state == Connected)
    {
      byte commandByte;

      //read in the data frame
      if(ReceiveFrame(&commandByte) == -1)
      {
        //an error occured while receiving the frame body
        //send Frame error back and abort receiving a frame
        SendFrameErrorByte();

        
        //let the red led blink 5 times
        #ifdef debug
        Blink(led_3, 5, 250);
        #endif

        
        continue;
      }
      
      //execute the command received in the frame header if needed
      ExecuteCommand(commandByte);
      //show the frame on the LEDs
      ShowFrame();

      //send a frame Acknowlegement
      SendFrameAcknowledgementByte();

     
    }
  }
  else if(result == -1)
  {
    //the connection could not be established
    #ifdef debug
    digitalWrite(led_3, HIGH);
    delay(1000);
    digitalWrite(led_3, LOW);
    #endif   
  }

  //disconnected from the device 
  //disable the green led
  #ifdef debug  
  digitalWrite(led_2, LOW);
  #endif





//TODO: move to a test function
  
 /* SendLong(5L);
 // SendLong(-5L);
 // SendLong(2147483647L);
 // SendLong(-2147483648L);

   /* byte buff[1] = {0xff};
    Serial.write(buff, 1);

    delay(250);
  byte buff[1];
  Serial.readBytes(buff, 1);
  analogWrite(led_2, buff[0]);
*/
}



/**
 * function executing the given command
 * @param id: the id of the command to execute according to the ALUP v. 0.1
 */
void ExecuteCommand(byte id)
{
  //check if the given id corresponds to a protocol command or a subcommand
  if(id < SUBCOMMAND_OFFSET)
  {
    ExecuteProtocolCommand(id);
  }
  else
  {
    ExecuteSubCommand(id - SUBCOMMAND_OFFSET);
  }
}



/**
 * function executing the protocol command with the given ID according to the ALUP v. 0.1
 */
void ExecuteProtocolCommand(byte id)
{
  switch(id)
  {
    case 1:
      //clear command received
      //do nothing as the clear command is handled within ReceiveBody()
      break;
    case 2:
      //Disconnect command received
      state = Disconnected;
      break;
  }
}



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




/**
 * an example subprogram 
 */
void ExampleSubProgram()
{
  Blink(LED_BUILTIN, 10, 10);
}


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




/**
 * function starting a connection to the master device so LED data can be transmitted 
 * Note: this function blocks until the connection is established successfully
 * @return a number indicating the success of this function
 *          One of:
 *             0: The connection could be established 
 *            -1: An error occured while trying to connect
 */
int Connect()
{ 
  //send the configuration to the master device
  SendConfiguration(EXTRA_VALUES);

  //wait for a configuration acknowldgement byte indicating that the configuration was received and applied 
  //successfully by the master device


  //check if a configuration error byte was received or the function timed out while waiting for a configuration acknowledgement
  if(WaitForOneOf(new byte[2] {CONFIGURATION_ACKNOWLEDGEMENT_BYTE, CONFIGURATION_ERROR_BYTE}, 2, 5000) != 0)
  {
    //the configuration could not be received/applied successfully by the master device (Timeout of 5s reached)
    //or a configuration error happened on the master device
    
    #ifdef debug
    //turn on the first red led to notify the user
    digitalWrite(led_3, HIGH);
    digitalWrite(led_0, LOW);
    #endif
    
    return -1;
  }

  //the configuration was applied successfully by the master device and a configuration acknowledgement was received
  
  #ifdef debug
  //turn on the first blue led to notify the user
  digitalWrite(led_3, LOW);
  digitalWrite(led_0, HIGH);
  #endif
  
  //send a configuration acknowledgement to tell the master device that this device is now ready to receive data 
  SendConfigurationAcknowledgementByte();   
  return 0;
}


/**
 * wrapper function for reading in bytes over the hardware connection
 * @param buff: an array of bytes with a size of numOfBytes which should get sent over the hardware connection
 * @param numOfBytes: the number of bytes to write
 */
void writeBytes(byte bytes[], int numOfBytes)
{
  Serial.write(bytes, numOfBytes);
}

/**
 * wrapper function for writing bytes over the hardware connection
 * Note: This function blocks until the specified number of bytes is read
 * @param buff: an array of bytes with a size of numOfBytes in which the read bytes will get stored
 * @param numOfBytes: the number of bytes to read 
 */
void readBytes(byte buff[], int numOfBytes)
{
  Serial.readBytes(buff, numOfBytes);
}

/**
 * wrapper function for getting the number of available bytes from the hardware connection
 * @return: the number of available bytes
 */
int availableBytes()
{
    return Serial.available();
}




/**
 * function waiting to receive a frame over the serial connection
 * Note: this function blocks until a frame is received
 * @param *commandByte: a pointer used to store the command byte when Receiving the 
 * @return: -1 if the frame body size or the body offset is invalid, else 0
 */
int ReceiveFrame(byte *commandByte)
{
   long bodyOffset;
  
   //get the frame header 
   long bodySize = ReceiveHeader(commandByte, &bodyOffset);

   //validate the body size and body offset to receive and apply the frame body
   if(!(ValidateBodySize(bodySize) && ValidateBodyOffset(bodyOffset, bodySize)))
   {
      //the bodySize is invalid or the body offset is invalid or out of range
      DiscardBytes(bodySize);
      return -1;
   }

   //receive the frame body and apply it to the led array
   ReceiveBody(bodySize, bodyOffset, *commandByte);
   return 0;
}



/**
 * function discarding the given amount of bytes from the incoming serial connection
 * @param numOfBytes: the number of bytes to discard; has to be 0 or higher
 */
void DiscardBytes(long numOfBytes)
{
  //check if there are bytes to remove
  if(numOfBytes > 0)
  {
    //discard the faulty frame body
    byte *buff = malloc(numOfBytes);
    
    //check if memory could be allocated
    if(buff == NULL)
    {
      //memory could not be allocated; 
      
      #ifdef debug
      //notify the user by blinking and return without reading in the data
      Blink(led_3, 100, 100);
      #endif
      
      return;
    }

    //read the bytes in from the serial buffer
    readBytes(buff, numOfBytes);
    free(buff);

    #ifdef debug
    Blink(led_4, 1, 200);
    #endif
  }
}




/**
 * function showing the frame by applying it to the actual leds using the FastLED library
 */
void ShowFrame()
{
    FastLED.show();

    /*
     * delay the frame acknowledgement
     * This has to be done for some types of LEDs like WS2812b as they have a maximum data rate 
     * To change the delay, edit the constant FRAME_DELAY at the top of the script
     */
    delay(FRAME_DELAY);
}



/**
 * function validating the given bodySize according to the ALUP v. 0.1
 * @param bodySize: the BodySize to validate
 * @return true if the given bodySize is valid, else false
 */
bool ValidateBodySize(long bodySize)
{
  //check if the given bodySize is a multiple of 3, less or equal than NUM_LEDS * 3 and greater or equal 0
  return (bodySize <= NUM_LEDS * 3) && (bodySize >= 0) && (bodySize % 3 == 0);
}

/**
 * function validating the given bodyOffset according to the ALUP v. 0.1
 * @param offset: the offset value to validate
 * @param bodySize: the size of the frame body; has to be a multiple of 3 and >= 0
 * @return true if the given bodyOffset is valid, else false
 */
bool ValidateBodyOffset(long offset, long bodySize)
{
  //check if the given bodyOffset is >= 0, less or equal than NUM_LEDS - (bodySize / 3)
  return (offset >= 0) && (offset <= NUM_LEDS - (bodySize / 3));
} 


/**
 * function waiting to receive a frame header over the serial connection
 * Note: this function blocks until a frame header is received
 * @param *commandByte: a pointer used to store the command byte when receiving the header
 * @param *offset: a pointer used to store the led offset when receiving the header
 * @return the bodyLength received with the header
 */
long ReceiveHeader(byte *commandByte, long *offset)
{
   while(true)
   {
      if(availableBytes() >= 9)
      {
        //receive the header bytes over the serial connection
        byte headerBytes[9];
        readBytes(headerBytes, 9);

        //set the command byte according to the ALUP v. 0.1 (internal)
        *commandByte = headerBytes[8];

        //build the bodyLength from 4 received bytes according to the ALUP v. 0.1 (internal)
        byte bodySizeBytes[] = {headerBytes[0],headerBytes[1], headerBytes[2], headerBytes[3]};

        //build the offset from 4 received bytes according to the ALUP v. 0.1 (internal)
        byte offsetBytes[] = {headerBytes[4],headerBytes[5], headerBytes[6], headerBytes[7]};
        *offset = BytesToLong(offsetBytes);
        
        return BytesToLong(bodySizeBytes);       
      }
   }
}



/**
 * function receiving the frame body with the given size over the serial connection and applying it to the "leds" array
 * @param bodySize: the size of the frame body in bytes; has to be >= 0 and <= NUM_LEDS * 3
 * @param bodyOffset: the offset for the LEDs used when applying the frame body to the LEDs; has to be  >= 0 and <= NUM_LEDS - (bodySize/3)
 * @param commandByte: the command byte received with the frame header; this is needed for the "clear" command
 */
void ReceiveBody(long bodySize, long bodyOffset, byte commandByte)
{
   //check if a clear command was received
  if(commandByte == 1)
  {
   //clear command received
   //clear the current LED array
   FastLED.clear();
  } 
  
  //check if the body size is not 0
  if(bodySize == 0)
  {
    
    //the body size is 0; skip reading it in
    return;
  }
  
  //allocate a buffer to read in the data
  byte *buff = malloc(sizeof(byte) * bodySize);

  //check if memory could be allocated
  if(buff == NULL)
  {
    //memory could not be allocated; 

    #ifdef debug
    //notify the user by blinking and return without reading in the data
    Blink(led_3, 100, 100);
    #endif
    
    return;
  }

 

  //read the bytes from the serial port into the buffer
  readBytes(buff, bodySize);
  
  //loop through all received bytes and apply them to the "leds" array
  for(int i = 0; i < bodySize / 3; i++)
  {
    //apply the buffered data to the LEDs according to the ALUP v. 0.1 (internal)
    leds[i + bodyOffset] = CRGB(buff[i*3], buff[i*3 + 1], buff[i*3 + 2]);
  }

  //free the memory used by the buffer
  free(buff);
}





/**
 * function sending connection requests over the serial connection until a connection acknowledgement was received
 * Note: this funciton blocks until a response was received
 */
void RequestConnection()
{
  while(true)
  {
    
    //Send a connection request
    writeBytes(new byte[1] {CONNECTION_REQUEST_BYTE}, 1);

    //wait for a connection acknowledgement
    if(WaitForByte(CONNECTION_ACKNOWLEDGEMENT_BYTE, 250))
    {
      //connection acknowledgement received
      return;
    }

    #ifdef debug
    //notify the user by letting the first blue led blink
    digitalWrite(led_0, HIGH);
    delay(100);
    digitalWrite(led_0, LOW);
    #endif
    
  }
}



/**
 * function sending the configuration over the serial connection
 * @param extraValues: A string of additional configuration values
 */
void SendConfiguration(String extraValues)
{
  SendConfigurationStartByte();
  //check if extraValues is null
  if(extraValues == NULL)
  {
    //extraValues is null
    //make it an empty string
    extraValues = "";
  }
  
  //send the configuration values
  SendString(PROTOCOL_VERSION);
  SendString(DEVICE_NAME);
  SendLong(NUM_LEDS);
  SendLong(DATA_PIN);
  SendLong(CLOCK_PIN);
  SendString(extraValues);
  
}



/**
 * function letting the given led blink the given times with the given delay between each blink
 *@para: a valid pin number which has to be initialized as OUTPUT
 *@param times: how many times the LED should blink
 *@param msDelay: the delay between each blink in ms
 */
void Blink(int led, int times, int msDelay)
{
   for(int i = 0; i < times; i++)
   {
      digitalWrite(led, HIGH);
      delay(msDelay);
      digitalWrite(led, LOW);
      delay(msDelay);
   }
}



/**
 * function waiting for the specified byte to be received
 * Note: this function blocks until the byte is received or the timeout was exceeded
 * @param b: the byte to listen for
 * @param timeOut: the timeout in ms; has to be > 0
 * @return: true if the byte was received within the timeout, else false
 */
bool WaitForByte(byte b, int timeOut)
{
  return WaitForOneOf(new byte[1] {b}, 1, timeOut) == 0;
}



/**
 * function waiting for one of the given bytes to be received over the serial connection
 * Note: this function blocks until one of the given bytes was received or the timeout was exceeded
 * @param bytes: an array of bytes
 * @param bytesLength:  the length of the bytes array
 * @param timeOut: the timeout in ms; has to be > 0
 * @retrun: the index of the received byte, or -1 if the timeout was exceeded
 */ 
int WaitForOneOf(byte* bytes, int bytesLength, int timeOut)
{
   int passedTime = 0;

   while(passedTime <= timeOut)
   {
    //check if there are bytes to read in the serial buffer
      if(availableBytes() > 0)
      {
          //read one byte from the serial buffer
          byte buff[1];
          readBytes(buff, 1);
    
          //check if the received byte was one of the given bytes
          for(int i = 0; i < bytesLength; i++)
          {
            if(buff[0] == bytes[i])
            {
              return i;
            }
          }
      }
      
      delay(1);
      passedTime ++;
   }

   //the specified byte was not received within the time limit
   return -1;
}



/**
 * function sending a configuration start byte over the serial connection
 */
void SendConfigurationStartByte()
{
  byte txBytes[] = {CONFIGURATION_START_BYTE}; 
  writeBytes(txBytes, 1);
}



/**
 * function sending a configuration acknowledgement byte over the serial connection
 */
void SendConfigurationAcknowledgementByte()
{
  byte txBytes[] = {CONFIGURATION_ACKNOWLEDGEMENT_BYTE}; 
  writeBytes(txBytes, 1);
}



/**
 * function sending a frame acknowledgement byte over the serial connection
 */
void SendFrameAcknowledgementByte()
{
  byte txBytes[] = {FRAME_ACKNOWLEDGEMENT_BYTE}; 
  writeBytes(txBytes, 1);
}



/**
 * function sending a frame error byte over the serial connection
 */
void SendFrameErrorByte()
{
  byte txBytes[] = {FRAME_ERROR_BYTE}; 
  writeBytes(txBytes, 1);
}



/**
 * function sendin text over the serial connection together with a null byte as terminator
 */
void SendString(String text)
{ 
  //TODO: replace serial.print() with writeBytes and string to bytes conversion
    Serial.print(text);
    writeBytes('\0');
}



/**
 * function sending the given unsigned 32-bit number over the serial connection
 * @param number: the number to send over the serial connection
 */
void SendLong(long number)
{
  //create a 32bit buffer
  byte outBytes[4];

  //fill the byte representation of the number into the buffer
  LongToBytes(number, outBytes);

  //send the bytes over the serial connection
  writeBytes(outBytes, 4);
}



/**
 *function converting 4 bytes to an unsigned long (32 bit integer)
 *@param bytes: an array of 4 bytes which should be converted
 *@return: the unsigned long converted from the given bytes
 */
unsigned long BytesToUnsignedLong(byte bytes[])
{
  unsigned long number = 0;

  //shift each byte to its correspondig position and add it to the number
  number += (unsigned long) bytes[0] << 24;
  number += (unsigned long) bytes[1] << 16;
  number += (unsigned long) bytes[2] << 8;
  number += (unsigned long) bytes[3];
 
  return number;
}



/**
 *function converting 4 bytes to a long (32 bit integer)
 *@param bytes: an array of 4 bytes which should be converted
 *@return: the long converted from the given bytes
 */
long BytesToLong(byte bytes[])
{
  unsigned long number = 0;

  //shift each byte to its correspondig position and add it to the number
  number += (long) bytes[0] << 24;
  number += (long) bytes[1] << 16;
  number += (long) bytes[2] << 8;
  number += (long) bytes[3];
 
  return number;
}




/*
 *function converting 2 bytes to an unsigned int (16 bit number)
 *@param bytes: an array of 2 bytes which should be converted
 *@return: the unsigned int converted from the given bytes 
 */
unsigned int BytesToInt(byte bytes[])
{
  unsigned int number = 0;

  //shift each byte to its correspondig position and add it to the number
  number +=  bytes[0] << 8;
  number +=  bytes[1];
  
  return number;
}



/**
 * function converting a long (32bit integer) to an array of 4 bytes
 * @param number: the number which should be converted
 * @param outBytes: a pointer to the charArray where the result will be stored; has to have a size of 4
 * @return; an integer representing the length of the outBytes array
 */
int LongToBytes(long number, byte * outBytes)
{
  
  outBytes[0] = (number >> 24) & 0xFF;
  outBytes[1] = (number >> 16) & 0xFF;
  outBytes[2] = (number >> 8) & 0xFF;
  outBytes[3] = number & 0xFF;
  
  return 4;
}



/**
 * function converting an unsigned int (16bit integer) to an array of 2 chars
 * @param number: the number which should be converted
 * @param outBytes: a pointer to the charArray where the result will be stored; has to have a size of 2
 * @return; an integer representing the length of the outBytes array
 */
int IntToBytes(unsigned int number, unsigned char * outBytes)
{
  
  outBytes[0] = (number >> 8) & 0xFF;
  outBytes[1] = number & 0xFF;
  
  return 2;
}
