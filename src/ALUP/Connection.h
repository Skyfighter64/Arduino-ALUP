#ifndef CONNECTION_H
#define CONNECTION_H
#include<Arduino.h>

class Connection
{
    public:
        //function establishing the connection
        virtual void Connect() = 0;
        //function terminating the connection
        virtual void Disconnect() = 0;
        //function sending the given bytes
        virtual void Send(uint8_t* bytes, size_t length) = 0;
        //function receiving the given amount of bytes; has to be blocking
        virtual int Read(uint8_t* buffer, size_t length) = 0;
        //function returning the nunber of bytes in the read buffer
        virtual int Available() = 0;
        //function returning if the connection is established
        virtual bool isConnected() = 0;
};


#endif