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
        Serial.begin(250000);
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
    server.setNoDelay(true);
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
        tcp.setNoDelay(true);
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

    Serial.print("\nConnecting to wifi: ");
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
    size_t remaining_bytes_to_read = length;

    if(!WiFi.isConnected() || !isConnected())
    {
        Serial.println("Could not receive data: WiFi or TCP not connected!");
        return 0;
    }

    while(remaining_bytes_to_read > 0)
    {
        if(tcp.available() <= 0)
            {
                // no data available to read
                yield();
                continue;
            }
            
            // read in as much data as possible but not more than needed
            size_t read_bytes = tcp.read(&buffer[length - remaining_bytes_to_read], remaining_bytes_to_read);
            remaining_bytes_to_read -= read_bytes;
    }

    // print out what was read for debugging
    Serial.print("Received: ");
    for(unsigned int i = 0; i < length; i ++)
    {
        Serial.print(buffer[i]);
        Serial.print(" ");
    }
    
    return length;
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

