#ifndef SERIAL_CONNECTION_H
#define SERIAL_CONNECTION_H

#include "Connection.h"
#include "Arduino.h"

#include "ESPTelnet.h"

#define SERIAL_TIMEOUT_MS 10000

/**
 * class implementing serial connectivity for this library
 */

class SerialConnection : public Connection
{
public:
    /**
     * default constructor
     * @param _baud: the communication speed of the devices
     */
    SerialConnection(long _baud) : baud{_baud}
    {
      
    }

    long baud = 115200;

    /**
     * function establishing the connection
     */
    void Connect()
    {
        //set the serial timeout to 10s
        //this value may need adjustment
        Serial.setTimeout(SERIAL_TIMEOUT_MS);
        Serial.begin(baud);    
        delay(100);   
    }
    /**
     * function terminating the connection
     */
    void Disconnect()
    {
        Serial.flush();
        Serial.end();
    }
    /**
     * function sending the given bytes
     * @param bytes: a byte array containing the bytes to send
     * @param length: the length of the given array
     */
    void Send(uint8_t* bytes, size_t length)
    {
        Serial.write(bytes, length);
        String log = "Sending: ";
        for(size_t i=0; i < length; i++)
        {
            log += bytes[i];
            log += " ";
        }
        //telnet.println(log);
        DEBUG_MSG(log);
    }
    /**
     * function receiving the given amount of bytes
     * Note: blocks until the given amount of bytes was read
     * @param buffer: a pre-initialized buffer of the given size
     * @param size: the size of the buffer
     * @return: the number of bytes read; >= 0
     */
    int Read(uint8_t* buffer, size_t length) 
    {
        size_t remaining_bytes_to_read = length;

        // Read in the data from the serial connection
        // NOTE: we read data in chunks because the Hardware serial buffer
        // can only store 64 Bytes but our data might be more than that
        while(remaining_bytes_to_read > 0)
        {
            if(Available() <= 0)
            {
                // no data available to read
                yield();
                continue;
            }
            // read in a chunk of data, depending on the currently needed and available data
            //NOTE: Chunk size always needs to be >=0 
            size_t chunk_size = min((int) remaining_bytes_to_read, Available());

            // read in the next chunk of data
            size_t read_bytes = Serial.readBytes(&buffer[length - remaining_bytes_to_read], chunk_size);
            // NOTE: This should always end up at exactly 0 in the end
            remaining_bytes_to_read -= read_bytes;
        }
        // print out what was read for debugging
        String log = "Received: ";
        for(size_t i=0; i < length; i++)
        {
            log += buffer[i];
            log += " ";
        }
        DEBUG_MSG(log);
        return length;
    }
    /**
     * function returning the number of bytes in the read buffer
     * @return: the number of bytes ready to read from the buffer
     */
    int Available()
    {
        return Serial.available();
    }
    /**
     * function returning if the connection is established
     * @return: true if connected, else false
     */
    virtual bool isConnected()
    {
        return Serial;
    }
};

#endif
