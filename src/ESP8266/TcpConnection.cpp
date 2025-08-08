#include "TcpConnection.h"


/** 
 * default constructors
 * @param _wifiSSID: the ssid of the wifi network to connect to
 * @param _wifiPassword: the password of the wifi network to connect to
 * @param _port: the port to listen for incoming connections
 */
TcpConnection::TcpConnection(char* _wifiSSID, char* _wifiPassword, int _port) : receivingPort {_port}, wifiSSID {_wifiSSID}, wifiPassword {_wifiPassword}
{
    if(!Serial)
    {
        Serial.begin(115200);
    }
    pinMode(LED_BUILTIN, OUTPUT);
}

/**
 * function establishing a wifi and duplex udp connection using the given parameters
 */
void TcpConnection::Connect()
{
    //establish a wifi connection
    ConnectToWifi(wifiSSID, wifiPassword);
    //start the TCP listener
    WiFiServer server(receivingPort);
    server.begin();

    Serial.println("Waiting for TCP connection");
    // wait until someone connects to the tcp listener
    while(!tcp && !tcp.connected())
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);

        tcp = server.accept();
    }
    connected = true;
    Serial.println("TCP Connected");
}   

/**
 * function establishing a wifi connection using the given credentials
 * @param _ssid: the ssid of the wifi network to connect to
 * @param _password: the password of the wifi network to connect to
 * @note: this function blocks until the connection is established successfully 
 */
void TcpConnection::ConnectToWifi(char* _ssid, char* _password)
{
    //initialize the network connection
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);

    Serial.print("Connecting to wifi: ");
    Serial.println(_ssid);
    
    //wait until the connection is established 
    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
    }
    Serial.println("Connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

/**
 * function terminating the udp listener and wifi connection
 */
void TcpConnection::Disconnect()
{
    tcp.stop();
    //WiFi.disconnect();
    connected = false;
    Serial.println("Disconnected from WiFi.");
}

/**
 * function sending the given bytes over the udp connection
 * @param bytes: the bytes to send
 * @param lenght: the length of the bytes array
 */
void TcpConnection::Send(uint8_t* bytes, size_t length)
{
    if(!WiFi.isConnected() || !isConnected())
    {
        Serial.println("Could not send data: Not connected!");
        return;
    }
    Serial.println("Sending: ");
    for(unsigned int i = 0; i < length; i ++)
    {
        Serial.print(bytes[i]);
        Serial.print(" ");
    }
    Serial.println();
    tcp.write(bytes, length);
}

/**
 * function reading the given amount of bytes into the given buffer
 * @param buffer: a buffer for the incoming bytes
 * @param length: the number of bytes to read
 * @return: the number of bytes filled into the buffer 
 */
int TcpConnection::Read(uint8_t* buffer, size_t length)
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

    while(tcp.available() <= 0)
    {
        //wait until data is available
    }
    return tcp.read(buffer, length);
}

/**
 * function returning the number of bytes available in the read buffer
 * @return: the number of bytes available for read
 */
int TcpConnection::Available()
{
    return tcp.available();
}
/**
 * function returning if the connection is established
 * @return: true if connected, else false
 */
bool TcpConnection::isConnected()
{
    return connected;
}

