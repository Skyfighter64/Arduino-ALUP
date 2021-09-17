#include "ALUP.h"
#include "Convert.h"


/**
 * default constructor
 * @param _leds: the led array used by FastLED
 * @param _ledCount: the size of _leds
 * @param _dataPin: the data pin used by FastLED
 * @param _clockPin: the clock pin used by FastLED
 */
Alup::Alup(CRGB* _leds, int _ledCount, int _dataPin, int _clockPin) : leds {_leds}, ledCount {_ledCount}, dataPin {_dataPin}, clockPin {_clockPin}
{

}


/**
 * function esablishing an ALUP connection
 * @param _connection: a connection object to use for the protocol
 * @param deviceName: a name for this device
 * @param dataPin: the data Pin of the LED strip; 0 if not used
 * @param clockPin: the clock pin of the LED strip; 0 if not used
 * @param ledCount: the number of LEDs on the led strip
 * @param extraValues: additional configuration values to send, "" if not used
 * @return: 1 if connected successfully, else 0
 */
int Alup::Connect(Connection* _connection, String deviceName,  String extraValues)
{
    pinMode(2, OUTPUT);
    connection = _connection;
    connection->Connect();

    Serial.println("UDP connection established");
    Serial.println("Requesting ALUP connection");

    RequestAlupConnection();

    Serial.println("ALUP connection established");
    Serial.println("Sending configuration");
    if(!SendConfiguration(deviceName, dataPin, clockPin, ledCount, extraValues))
    {
        connected = false;
        Serial.println("Configuration error");
        return 0;
    }

    connected = true;
    Serial.println("Finnished connecting");
    return 1;
}

/**
 * function sending a connection request and reading in a response if resent
 * Note: this function is blocking until an acknowledgement is received
 */
void Alup::RequestAlupConnection()
{
    while(true)
    {
        delay(5000);
        SendByte(CONNECTION_REQUEST_BYTE);
        //check if there is something to read
        if(connection->Available() <= 0)
        {
            //no byte available; skip the read in
            continue;
        }
        //read in the byte and check it for an acknowledgement
        if(ReadByte() == CONNECTION_ACKNOWLEDGEMENT_BYTE)
        {
            break;
        }
        
    }
}

/**
 * function building and sending the configuration to the slave device  
 * @param deviceName: a name for this device
 * @param dataPin: the data Pin of the LED strip; 0 if not used
 * @param clockPin: the clock pin of the LED strip; 0 if not used
 * @param ledCount: the number of LEDs on the led strip
 * @param extraValues: additional configuration values to send, "" if not used
 * @return: 1 if configuration was sent successfully, else 0
 * Note: Blocks until an answer to the configuration is received
 */
int Alup::SendConfiguration(String deviceName, int dataPin, int clockPin, int ledCount, String extraValues)
{
    Serial.println("Building Configuration...");

    byte* buff;
    int length = BuildConfiguration(buff, PROTOCOL_VERSION, deviceName, dataPin, clockPin, ledCount, extraValues);
    Serial.println("Sending Configuration...");
    connection->Send(buff, length);
    free(buff);

    //wait for the response
    while(true)
    {
        byte byte = ReadByte();
        if(byte == CONFIGURATION_ACKNOWLEDGEMENT_BYTE)
        {
            //configuration exchanged successfully
            Serial.println("Configuration exchanged successfully!");
            //continue normally
            return 1;
        }
        else if (byte == CONFIGURATION_ERROR_BYTE)
        {
            //configuration exchange failed
            Serial.println("Configuration exchange failed.");
            //abort the connection process 
            return 0;
        }
    }
}


/**
 * function generating a byte array containing the given values
 * @param buffer: the buffer in which the result will be stored. Has to be free()'d!!
 * @param protocolVersion: the protcol version of this implementation. Usually PROTOCOL_VERSION
 * @param deviceName: a name for this device
 * @param dataPin: the data Pin of the LED strip; 0 if not used
 * @param clockPin: the clock pin of the LED strip; 0 if not used
 * @param ledCount: the number of LEDs on the led strip
 * @param extraValues: additional configuration values to send, "" if not used
 * @return: the size of the buffer
 */
int Alup::BuildConfiguration(byte*& buffer, String protocolVersion, String deviceName, int32_t dataPin, int32_t clockPin, int32_t ledCount, String extraValues)
{
    //convert the protocol version into a null terminated byte array
    int versionLength = protocolVersion.length() + 1;
    byte* versionBytes = (byte*) malloc(versionLength);
    Convert::StringToBytes(protocolVersion, versionBytes, versionLength);

    //convert the device name into a null terminated byte array
    int nameLength = deviceName.length() + 1;
    byte* nameBytes = (byte*) malloc(nameLength);
    Convert::StringToBytes(deviceName, nameBytes, nameLength);

    //convert the extra values into a null terminated byte array
    int extraValuesLength = extraValues.length() + 1;
    byte* extraValuesBytes = (byte*) malloc(extraValuesLength);
    Convert::StringToBytes(extraValues, extraValuesBytes, extraValuesLength);


    // convert the integer values to bytes
    byte dataPinBytes[4];
    Convert::Int32ToBytes(dataPin, dataPinBytes);
    byte clockPinBytes[4];
    Convert::Int32ToBytes(clockPin, clockPinBytes);
    byte ledCountBytes[4];
    Convert::Int32ToBytes(ledCount, ledCountBytes);

    //calculate the buffer size
    //add one byte for the configuration start
    int bufferSize = 1 + versionLength + nameLength + extraValuesLength + sizeof(int32_t) * 3;
    //allocate the main buffer
    buffer = (byte*) malloc(bufferSize);
    buffer[0] = CONFIGURATION_START_BYTE;

    //concatenate all byte arrays  
    //using an offset so that the arrays get concatenated
    int offset = 1;
    //copy the protcol version
    memcpy( &buffer[offset], &versionBytes[0], versionLength);
    offset += versionLength;
    //copy the device name
    memcpy( &buffer[offset], &nameBytes[0], nameLength);
    offset += nameLength;
    //copy the data pin
    memcpy( &buffer[offset], &dataPinBytes[0], 4);
    offset += 4;
    //copy the clock pin
    memcpy( &buffer[offset], &clockPinBytes[0], 4);
    offset += 4;
    //copy the led count
    memcpy( &buffer[offset], &ledCountBytes[0], 4);
    offset += 4;
    //copy the extra values
    memcpy( &buffer[offset], &extraValuesBytes[0], extraValuesLength);


    //free the temporary buffers
    free(versionBytes);
    free(nameBytes);
    free(extraValuesBytes);

    return bufferSize;
}

/**
 * function reading a single byte from the connection
 * @return: the byte read from the connection
 * Note: This function is blocking until a byte was read
 */
byte Alup::ReadByte()
{
    byte buff[1];
    connection->Read(buff, 1);
    return buff[0];
}


/**
 * function sending a single byte from the connection
 * @param b: the byte to send
 */
void Alup::SendByte(byte b)
{
    //send the data
    connection->Send(new byte[1] {b}, 1);
}




void Alup::Disconnect()
{
    connection->Disconnect();
    connected = false;
}

void Alup::Run()
{
    //do nothing if not connected
    if(!connected)
    {
        return;
    }

    Serial.println("Waiting for frame");
    Frame frame = ReadFrame();
    Serial.println("Received a frame:");
    Serial.print("body size:");
    Serial.println(frame.body_size);
    Serial.print("body offset:");
    Serial.println(frame.offset);
    Serial.print("Command:");
    Serial.println(frame.command);
    //apply the frame to the leds
    int result = ApplyFrame(frame);
    if(result == 0)
    {
        Serial.println("Frame error occured");
        //frame could not be applied; answer with frame error
        SendByte(FRAME_ERROR_BYTE);
    }
    else if (result == 1)
    {
        //frame applied successfully
        //acknowledge frame
        Serial.println("Frame applied successfully");
        SendByte(FRAME_ACKNOWLEDGEMENT_BYTE);
        Serial.println("Sent Frame Acknowledgement");
    }
}

/**
 * function reading in a complete frame as defined in ALUP v.0.2
 * Note: this function blocks until a frame is received
 * @return frame: the received frame
 */
Frame Alup::ReadFrame()
{
    Frame frame = Frame();
    frame.body_size = ReadInt32();
    frame.offset = ReadInt32();
    frame.command = ReadByte();
    frame.unused = ReadByte();

    frame.body = (byte*) malloc(frame.body_size);
    Serial.print("Reading in body of size ");
    Serial.println(frame.body_size);
    connection->Read(frame.body, frame.body_size); 
    return frame;
}

/**
 * function applying the given frame by executing its command
 * @param frame: the frame to apply
 * @return: 1 if applied successfully, 0 if frame error occured, -1 if no acknowledgement should be sent
 */
int Alup::ApplyFrame(Frame frame)
{
    switch(frame.command)
    {
        case Command::NONE:
            return ApplyColors(frame);

        case Command::CLEAR: 
            FastLED.clear();
            return ApplyColors(frame);

        case Command::DISCONNECT: 
            Serial.println("Disconnecting...");
            //acknowledge the disconnect
            SendByte(FRAME_ACKNOWLEDGEMENT_BYTE);
            delay(100);
            //disconnect from the remote device
            Disconnect();
            Serial.println("Disconnected");
            return -1;
        case Command::LED_BUILTIN:
            //test command for power LED
            // initialite pin2 first!
            digitalWrite(2, !digitalRead(2));

        default:
            //invalid command received
            Serial.print("Invalid command received: ");
            Serial.println(frame.command);
            return 0;
    }
}


/**
 * function interpreting the body of the given frame as colors and applying them to the leds
 * @param frame: the frame of which the body will be applied
 * @return: 1 if applied successfully, else 0
 */
 int Alup::ApplyColors(Frame frame)
 {
    //check the frame body size if it is a multiple of 3 
    if(frame.body_size % 3 != 0)
    {
        //not a multiple of 3
        return 0;
    }

    //check if the body size including offest excceeds the actual LEDs: (choose the smaller one)
    int lastLED = ((frame.body_size / 3) + frame.offset) > ledCount ? ledCount - frame.offset : (frame.body_size / 3);
    //convert the body to color values and apply them to the leds 
    for(int i = 0; i < lastLED / 3; i++)
    {
        //apply the buffered data to the LEDs according to the ALUP v. 0.1 (internal)
        leds[i + frame.offset] = CRGB(frame.body[i*3], frame.body[i*3 + 1], frame.body[i*3 + 2]);
    }

    FastLED.show();
    return 1;
 }


/**
 * function reading in a 32bit integer from the connection
 * Note: blocks until the integer was read
 * @return: the read integer
 */
 int32_t Alup::ReadInt32()
 {
     byte buffer[4];
     connection->Read(buffer, 4);
     return Convert::BytesToInt32(buffer);
 }