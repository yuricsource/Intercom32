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

#define DEBUGGING

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
        printf("Client connected\n");
#endif
    if (analyzeRequest(path, host)) 
    {
#ifdef DEBUGGING
        printf("Websocket established\n");
#endif
        return true;
    } else
    {
        // Might just need to break until out of socket_client loop.
#ifdef DEBUGGING
        printf("Invalid handshake\n");
#endif
        disconnectStream();
        return false;
    }
    return true;
}


void WebsocketPath::disconnectStream()
{
#ifdef DEBUGGING
    printf("Terminating socket\n");
#endif
    array <uint8_t, 2> message = {0x00, 0x87}; // doble check the order

    SendFrame(message.data(), message.size());
    terminate();
}

bool WebsocketPath::analyzeRequest(char * path, char * host) 
{
    char sendingBuffer[500] = {};
    uint16_t offset = 0;
    array<uint8_t, 64> temp = {};
    int bite = 0;
    bool foundupgrade = false;
    unsigned long intkey[2] = {};
    array<char, 16> keyStart = {};
    array<char, 24> b64Key = {};
	array<char, 25> key = {"dGhlIHNhbXBsZSBub25jZQ=="};

    for (int i=0; i<16; ++i) {
        keyStart.data()[i] = (char)Hal::Hardware::Instance()->GetRandomNumber();
    }

    Base64::Encode((uint8_t*)keyStart.data(), keyStart.size(), (uint8_t*)b64Key.data(), b64Key.size());

    for (int i=0; i<24; ++i)
        key.data()[i] = b64Key.data()[i];

#ifdef DEBUGGING
    printf("Sending websocket upgrade headers\n");
#endif
    memcpy(sendingBuffer + offset,"GET ", strlen("GET "));
    offset += strlen("GET ");
    memcpy(sendingBuffer + offset,path, strlen(path));
    offset +=  strlen(path);
    memcpy(sendingBuffer + offset, " HTTP/1.1\r\n", strlen(" HTTP/1.1\r\n"));
    offset +=  strlen(" HTTP/1.1\r\n");
    memcpy(sendingBuffer + offset, "Upgrade: websocket\r\n", strlen("Upgrade: websocket\r\n"));
    offset +=  strlen("Upgrade: websocket\r\n");
    memcpy(sendingBuffer + offset, "Connection: Upgrade\r\n", strlen("Connection: Upgrade\r\n"));
    offset +=  strlen("Connection: Upgrade\r\n");
    memcpy(sendingBuffer + offset, "Host: ", strlen("Host: "));
    offset +=  strlen("Host: ");
    // memcpy(sendingBuffer + offset, host, strlen(host));
    // offset +=  strlen(host);
    memcpy(sendingBuffer + offset, "127.0.0.1", strlen("127.0.0.1"));
    offset +=  strlen("127.0.0.1");
    memcpy(sendingBuffer + offset, ":4567", strlen(":4567"));
    offset +=  strlen(":4567");
    memcpy(sendingBuffer + offset, CRLF, strlen(CRLF));
    offset +=  strlen(CRLF);
    memcpy(sendingBuffer + offset, "Sec-WebSocket-Key: ", strlen("Sec-WebSocket-Key: "));
    offset +=  strlen("Sec-WebSocket-Key: ");
    memcpy(sendingBuffer + offset, key.data(), strlen(key.data()));
    offset +=  strlen(key.data());
    memcpy(sendingBuffer + offset, CRLF, strlen(CRLF));
    offset +=  strlen(CRLF);
    memcpy(sendingBuffer + offset, "Sec-WebSocket-Version: 13\r\n", strlen("Sec-WebSocket-Version: 13\r\n"));
    offset +=  strlen("Sec-WebSocket-Version: 13\r\n");
    memcpy(sendingBuffer + offset, CRLF, strlen(CRLF));
    offset +=  strlen(CRLF);
#ifdef DEBUGGING
    printf("Header sent:\n");
    printf("%s", sendingBuffer);
#endif    
    _connection->Send((const unsigned char*)sendingBuffer, offset);

#ifdef DEBUGGING
    printf("Analyzing response headers\n");
#endif    

    TimeLimit timeout = {};
    bool requestedReceived = false;
    char * messsage = nullptr;
    char * serverKey = nullptr;
    
    while(timeout.IsTimeUp(10000) == false)
    {
        vTaskDelay(20);
        
        if (_hayStackWorkingLength > 0)
        {
            printf("Message Received, analysing the respond.(%d)\n", _hayStackWorkingLength);
            messsage = strstr(_hayStack, "Sec-WebSocket-Accept: ");
            if (messsage != nullptr)
            {
                serverKey = messsage + 22;
                requestedReceived = true;
                break;
            }
            _hayStackWorkingLength = 0;
        }
    }

    if (requestedReceived == false)
    {
#ifdef DEBUGGING
        printf("Socket Timeout.\n");
#endif
        return false;
    }
        
#ifdef DEBUGGING
    printf("Got Header: %s\n", serverKey);
#endif

    uint8_t *hash = nullptr;
    char result[21];
    char b64Result[30];

	Utilities::Sha1 sha1ctx;
	sha1ctx.AddBytes((unsigned char*) key.data(), strlen(key.data()));
	sha1ctx.AddBytes((unsigned char*) "258EAFA5-E914-47DA-95CA-C5AB0DC85B11", strlen("258EAFA5-E914-47DA-95CA-C5AB0DC85B11"));
	sha1ctx.GetDigest((unsigned char*) result);

    result[20] = '\0';
    Base64::Encode((uint8_t*)result, 20, (uint8_t*)b64Result, 30);
    
    return strstr(serverKey, b64Result) != nullptr;
}

void WebsocketPath::ReceivedData(const uint8_t *data, uint16_t length)
{
    if (_hayStackWorkingLength + length < MaxHayStackLength)
    {
        memcpy(&_hayStack[_hayStackWorkingLength], data, length);
        _hayStackWorkingLength += length;
        printf("_hayStackWorkingLength: %d, _hayStack: %s\n", _hayStackWorkingLength, _hayStack);
    }
    else
        DebugAssertFail("No room for new incoming.");
}

void WebsocketPath::ConnectionStateChanged(ConnectionState state, ConnectionChangeReason reason)
{
}

bool WebsocketPath::SendFrame(const uint8_t *data, uint16_t length)
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

bool WebsocketPath::start()
{
    _connection->SetDataReceived(fastdelegate::MakeDelegate(this, &WebsocketPath::ReceivedData));
    if (DoHandshake("/", (char *)_connection->GetRemoteConnection()->Address.data()))
    {
        _websocketConnected = true;
        pingTimeout.Reset();
    }
    return _websocketConnected;
}

void WebsocketPath::terminate()
{
    // _hayStackWorkingLength = 0;
}

void WebsocketPath::process()
{
    if (_websocketConnected && pingTimeout.IsTimeUp(10000))
    {
        
    }
}

} // namespace Protocol