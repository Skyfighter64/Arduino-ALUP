#include "UdpConnection.h"



/** 
 * default constructors
 * @param _wifiSSID: the ssid of the wifi network to connect to
 * @param _wifiPassword: the password of the wifi network to connect to
 * @param _ip: the ip address of the remote device
 * @param _port: the port of the remote device
 */
UdpConnection::UdpConnection(char* _wifiSSID, char* _wifiPassword, char* _ip, int _port) : ip {_ip}, port {_port}, wifiSSID {_wifiSSID}, wifiPassword {_wifiPassword}
{
    if(!Serial)
    {
        Serial.begin(115200);
    }
}

/**
 * function establishing a wifi and duplex udp connection using the given parameters

 */
void UdpConnection::Connect()
{
    //establish a wifi connection
    ConnectToWifi(wifiSSID, wifiPassword);
    //start the UDP listener
    udp.begin(receivingPort);
    connected = true;
}

/**
 * function establishing a wifi connection using the given credentials
 * @param _ssid: the ssid of the wifi network to connect to
 * @param _password: the password of the wifi network to connect to
 * @note: this function blocks until the connection is established successfully 
 */
void UdpConnection::ConnectToWifi(char* _ssid, char* _password)
{
    //initialize the network connection
    WiFi.begin(_ssid, _password);

    Serial.print("Connecting to wifi: ");
    Serial.println(_ssid);
    
    //wait until the connection is established 
    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("Connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

/**
 * function terminating the udp listener and wifi connection
 */
void UdpConnection::Disconnect()
{
    udp.stop();
    WiFi.disconnect();
    connected = false;
    Serial.println("Disconnected from WiFi.");
}

/**
 * function sending the given bytes over the udp connection
 * @param bytes: the bytes to send
 * @param lenght: the length of the bytes array
 */
void UdpConnection::Send(uint8_t* bytes, size_t lenght)
{
    if(!WiFi.isConnected() || !isConnected())
    {
        Serial.println("Could not send data: Not connected!");
        return;
    }
    Serial.println("Sending: ");
    for(int i = 0; i < lenght; i ++)
    {
        Serial.print(bytes[i]);
        Serial.print(" ");
    }
    Serial.println();
    udp.beginPacket(ip, port);
    udp.write(bytes, lenght);
    udp.endPacket();
}

/**
 * function reading the given amount of bytes into the given buffer
 * @param buffer: a buffer for the incoming bytes
 * @param length: the number of bytes to read
 * @return: the number of bytes filled into the buffer 
 */
int UdpConnection::Read(uint8_t* buffer, size_t length)
{
    //check if nothing needs to be read
    if(length == 0)
    {
        return 0;
    }

    if(!WiFi.isConnected() || !isConnected())
    {
        Serial.println("Could not receive data: Not connected!");
        return 0;
    }

    while(udp.available() <= 0)
    {
        udp.parsePacket();
    }
    return udp.read(buffer, length);
}

/**
 * function returning the number of bytes available in the read buffer
 * @return: the number of bytes available for read
 */
int UdpConnection::Available()
{
    udp.parsePacket();
    return udp.available();
}
/**
 * function returning if the connection is established
 * @return: true if connected, else false
 */
bool UdpConnection::isConnected()
{
    return connected;
}