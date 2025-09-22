
#ifndef ALUP_H
#define ALUP_H

#define CONNECTION_REQUEST_BYTE 255
#define CONNECTION_ACKNOWLEDGEMENT_BYTE 254
#define CONFIGURATION_START_BYTE 253
#define CONFIGURATION_ACKNOWLEDGEMENT_BYTE 252
#define CONFIGURATION_ERROR_BYTE 251
#define FRAME_ACKNOWLEDGEMENT_BYTE 250
#define FRAME_ERROR_BYTE 249

#define PROTOCOL_VERSION "0.2"
#define FRAME_BUFFER_SIZE 2

#include "Connection.h"
#include "Frame.h"
#include "CyclicBuffer.h"
#include <FastLED.h>

class Alup
{
    public:
        Alup(CRGB* leds, int ledCount, int dataPin, int clockPin);
        Connection* connection;
        bool connected = false;
        int Connect(Connection* _connection, String deviceName,  String extraValues);
        void Disconnect();
        void Run();


    protected:
        CRGB * leds;
        int ledCount;
        int dataPin;
        int clockPin;

        CyclicBuffer<Frame, FRAME_BUFFER_SIZE> frameBuffer;
        

        uint8_t ReadByte();
        void SendByte(uint8_t byte);
        void RequestAlupConnection();
        void Blink(int pin, int count, int blinkDelay);
        int SendConfiguration(String deviceName, int dataPin, int clockPin, int ledCount, String extraValues);
        int BuildConfiguration(byte*& buffer, String protocolVersion, String deviceName, int32_t dataPin, int32_t clockPin, int32_t ledCount, String extraValues);
        Frame* ReadFrame();
        int ApplyFrame(Frame &frame);
        int ApplyColors(Frame &frame);
        void ReplyToSender(int result);
        void SendAcknowledgement();
        int32_t ReadInt32();
        int32_t ReadUInt32();

        
    private:
    // incoming and outgoing timestamps of the current packet
    uint32_t t_in;
    uint32_t t_out;
        
};

#endif
