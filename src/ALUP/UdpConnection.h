#ifndef UDP_CONNECTION_H
#define UDP_CONNECTION_H

#include "Connection.h"
#include <WiFi.h>
#include <WiFiUdp.h>


class UdpConnection : public Connection
{
    public:
        //ip and port of the remote socket
        char* ip;
        int port;
        //the port of this device's udp socket where data is received
        int receivingPort = 5012;
        bool connected = false;

        //the wifi udp socket
        WiFiUDP udp;

        UdpConnection(char* _wifiSSID, char* _wifiPassword, char* _ip, int _port);
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