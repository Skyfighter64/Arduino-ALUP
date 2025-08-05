#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include "ALUP/Connection.h"
#include <ESP8266WiFi.h>


class TcpConnection : public Connection
{
    public:
        //the port of this device's tcp socket where data is received
        int receivingPort = 5012;
        bool connected = false;

        //the wifi tcp socket
        WiFiClient tcp;

        TcpConnection(char* _wifiSSID, char* _wifiPassword, int _port);
        void Connect();
        void Disconnect();
        void Send(uint8_t* bytes, size_t size);
        int Read(uint8_t* buffer, size_t length);
        int Available();
        bool isConnected();
        
    private:
        //the credentials for the wifi-network
        char* wifiSSID;
        char* wifiPassword;
        void ConnectToWifi(char* _ssid, char* _password);

};



#endif