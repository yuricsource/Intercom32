#include "WebSocket.h"
#include "Hardware.h"
#include "Base64.h"
#include "TimeLimit.h"
#include "DebugAssert.h"
#include "Sha1.h"

namespace Protocol
{
using Hal::TimeLimit;
using Hal::Hardware;
using Utilities::Base64;

// CRLF characters to terminate lines/handshakes in headers.
#define CRLF "\r\n"

WebsocketPath::WebsocketPath()
{

}

WebsocketPath::~WebsocketPath()
{

}

bool WebsocketPath::DoHandshake(char * path, char * host)
{
    if (path == nullptr || host == nullptr)
        return false;

    // Check request and look for websocket handshake
#ifdef DEBUGGING
        Serial.println(F("Client connected"));
#endif
    if (analyzeRequest(path, host)) 
    {
#ifdef DEBUGGING
            Serial.println(F("Websocket established"));
#endif
            return true;
    } else {
        // Might just need to break until out of socket_client loop.
#ifdef DEBUGGING
        Serial.println(F("Invalid handshake"));
#endif
        disconnectStream();
        return false;
    }
    return true;
}


void WebsocketPath::disconnectStream()
{
#ifdef DEBUGGING
    Serial.println(F("Terminating socket"));
#endif
    // Should send 0x8700 to server to tell it I'm quitting here.
    array <uint8_t, 2> message = {0x00, 0x87}; // doble check the order

    sendFrame(message.data(), message.size());
    terminate();
}

bool WebsocketPath::analyzeRequest(char * path, char * host) 
{
    array<uint8_t, 64> temp = {};

    int bite = 0;
    bool foundupgrade = false;
    unsigned long intkey[2] = {};
    array<char, 17> keyStart = {};
    array<char, 25> b64Key = {};
    //String key = "------------------------";	              
	array<char, 25> key = {"dGhlIHNhbXBsZSBub25jZQ=="};

    for (int i=0; i<16; ++i) {
        keyStart.data()[i] = (char)Hal::Hardware::Instance()->GetRandomNumber();
    }

    Base64::Encode((uint8_t*)b64Key.data(), b64Key.size(), (uint8_t*)keyStart.data(), keyStart.size());
    // base64_encode(b64Key, keyStart, 16);

    for (int i=0; i<24; ++i)
        key.data()[i] = b64Key.data()[i];

#ifdef DEBUGGING
    Serial.println(F("Sending websocket upgrade headers"));
#endif    
    sendFrame((uint8_t*)"GET ", sizeof("GET "));
    sendFrame((uint8_t*)path, strlen(path));
    sendFrame((uint8_t*)" HTTP/1.1\r\n", strlen(" HTTP/1.1\r\n"));
    sendFrame((uint8_t*)"Upgrade: websocket\r\n", strlen("Upgrade: websocket\r\n"));
    sendFrame((uint8_t*)"Connection: Upgrade\r\n", strlen("Connection: Upgrade\r\n"));
    sendFrame((uint8_t*)"Host: ", strlen("Host: "));
    sendFrame((uint8_t*)host, strlen(host));
    sendFrame((uint8_t*)CRLF, strlen(CRLF));
    sendFrame((uint8_t*)"Sec-WebSocket-Key: ", strlen("Sec-WebSocket-Key: "));
    sendFrame((uint8_t*)key.data(), strlen(key.data()));
    sendFrame((uint8_t*)CRLF, strlen(CRLF));
    sendFrame((uint8_t*)"Sec-WebSocket-Version: 13\r\n", strlen("Sec-WebSocket-Version: 13\r\n"));
    sendFrame((uint8_t*)CRLF, strlen(CRLF));

#ifdef DEBUGGING
    Serial.println(F("Analyzing response headers"));
#endif    

    // TODO: More robust string extraction
    TimeLimit timeout;
    bool requestedReceived = false;
    char * messsage = nullptr;
    char * serverKey = nullptr;
    
    while(timeout.IsTimeUp(500))
    {
        vTaskDelay(20);
        
        if (_hayStackWorkingLength > 0)
        {
            messsage = strstr(&_hayStack[_hayStackWorkingLength], "Sec-WebSocket-Accept: ");
            if (messsage != nullptr)
            {
                serverKey = messsage + 22;
                requestedReceived = true;
                break;
            }
        }
    }

    if (requestedReceived == false)
        return false;
        
#ifdef DEBUGGING
            Serial.print("Got Header: " + serverKey);
#endif

    uint8_t *hash = nullptr;
    char result[21];
    char b64Result[30];

	Utilities::Sha1 sha1ctx;
	sha1ctx.AddBytes((unsigned char*) key.data(), strlen(key.data()));
	sha1ctx.AddBytes((unsigned char*) "258EAFA5-E914-47DA-95CA-C5AB0DC85B11", strlen("258EAFA5-E914-47DA-95CA-C5AB0DC85B11"));
	sha1ctx.GetDigest((unsigned char*) result);

    for (int i=0; i<20; ++i) {
        result[i] = (char)hash[i];
    }
    result[20] = '\0';
    Base64::Encode((uint8_t*)b64Result, 30, (uint8_t*)result, 20);
    
    return strstr(serverKey, b64Result) != nullptr;
}

void WebsocketPath::receivedData(const uint8_t *data, uint16_t length)
{
    if (_hayStackWorkingLength + length < MaxHayStackLength)
    {
        memcpy(&_hayStack[_hayStackWorkingLength], data, length);
        _hayStackWorkingLength += length;
    }
    else
        DebugAssertFail("No room for new incoming.");
}

void WebsocketPath::connectionStateChanged(ConnectionState state, ConnectionChangeReason reason)
{
}

bool WebsocketPath::sendFrame(const uint8_t *data, uint16_t length)
{
    return false;
}

void WebsocketPath::setConnection()
{
}

bool WebsocketPath::isTerminated() const
{
    return false;
}

bool WebsocketPath::start(BaseRouteHandler::ConnectionMode connectionMode, RemoteConnection *address, uint8_t processingIndex, uint8_t processingLogicalId)
{
    return false;
}

void WebsocketPath::terminate()
{
    // _hayStackWorkingLength = 0;
}

void WebsocketPath::process()
{
}

} // namespace Protocol