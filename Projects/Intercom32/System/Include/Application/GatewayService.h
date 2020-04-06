#pragma once

#include "Hardware.h"
#include "WifiService.h"
#include "HttpServer.h"
#include "TimeLimit.h"
#include "ConfigurationCommon.h"
#include "BaseConnection.h"
#include "BaseRouteHandler.h"
#include "TcpConnection.h"
#include "Logger.h"
#include "WebSocket.h"

namespace Applications
{

using Hal::TimeLimit;
using Common::ProtocolType;
using Common::TransportLayerType;
using Protocol::BaseConnection;
using Protocol::BaseRouteHandler;
using Protocol::RemoteConnection;
using Protocol::TcpConnection;
using Utilities::Logger;

class GatewayService : public cpp_freertos::Thread
{
public:
    GatewayService();

private:
    struct RoutePathConnection
    {
        RoutePathConnection() : Connection(nullptr),
                                RouteHandler(nullptr),
                                Used(false)
        {
        }

        BaseConnection *Connection;
        BaseRouteHandler *RouteHandler;
        TransportLayerType TransportLayer;
        ProtocolType Protocol;
        bool Used;

        inline constexpr bool ConnectionValid() const
        {
            return Connection != nullptr;
        }
    };

    enum class ConnectionState : uint8_t
    {
        None,
        RestartConnection,
        TryToConnect,
        EstablishConnection,
        Done
    };

    RoutePathConnection _connectionPath;
    ConnectionState _connectionState;

    void changeState(ConnectionState connectionState)
    {
        _connectionState = connectionState;
    }

protected:
    void Run() override;

private:
    /// @brief	Hide Copy constructor.
    GatewayService(const GatewayService &) = delete;

    /// @brief	Hide Assignment operator.
    GatewayService &operator=(const GatewayService &) = delete;

    /// @brief	Hide Move constructor.
    GatewayService(GatewayService &&) = delete;

    /// @brief	Hide Move assignment operator.
    GatewayService &operator=(GatewayService &&) = delete;
};

} // namespace Applications