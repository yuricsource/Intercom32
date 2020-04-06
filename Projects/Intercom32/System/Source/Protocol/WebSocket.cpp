#include "WebSocket.h"

namespace Protocol
{

void WebsocketPath::receivedData(const uint8_t *data, uint16_t length)
{
}

void WebsocketPath::connectionStateChanged(ConnectionState state, ConnectionChangeReason reason)
{
}

bool WebsocketPath::sendFrame(const uint8_t *data, uint16_t length)
{
    return false;
}

void setConnection()
{
}

bool isTerminated()
{
    return false;
}

bool start(BaseRouteHandler::ConnectionMode connectionMode, RemoteConnection *address, uint8_t processingIndex, uint8_t processingLogicalId)
{
    return false;
}

void terminate()
{
}

void process()
{
}

} // namespace Protocol