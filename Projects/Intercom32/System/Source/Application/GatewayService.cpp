#include "GatewayService.h"
#include "ConfigurationAgent.h"
#include "WebSocket.h"

namespace Applications
{

using Configuration::ConfigurationAgent;
using Protocol::WebsocketPath;
// using ProtocolType::Websocket;
// using Applications::GatewayService;

GatewayService::GatewayService() : cpp_freertos::Thread("GWSVC", configGATEWAYSVC_STACK_DEPTH, 3),
    _connectionPath(), _connectionState(ConnectionState::None)
{
    _connectionPath.Protocol = ProtocolType::Websocket;
    _connectionPath.TransportLayer = TransportLayerType::Wifi;
    _connectionPath.Connection = new TcpConnection();
    _connectionPath.RouteHandler = new WebsocketPath();
}

void GatewayService::Run()
{
    vTaskDelay(6000);

    RemoteConnection *connection = &ConfigurationAgent::Instance()->GetBoardConfiguration()->GetConfiguration()->ServerConfig.Connection;
    _connectionPath.RouteHandler->SetConnection(_connectionPath.Connection);
    _connectionPath.RouteHandler->Start();

    for (;;)
    {
        switch (_connectionState)
        {
        case ConnectionState::None:
            changeState(ConnectionState::RestartConnection);
            break;

        case ConnectionState::RestartConnection:
            _connectionPath.Connection->Close();
            changeState(ConnectionState::TryToConnect);
            break;

        case ConnectionState::TryToConnect:
            Logger::LogInfo(Logger::LogSource::Gateway, "Connecting to %s:%d",connection->Address.data(), connection->Port);
            if (BaseConnection::ConnectStatus::SuccessfullyConnected == _connectionPath.Connection->Connect(*connection)
                && _connectionPath.RouteHandler->Start())
            {
                changeState(ConnectionState::EstablishConnection);
            }
            else
            {
                Logger::LogError(Logger::LogSource::Gateway, "Failed to Connect.");
                changeState(ConnectionState::RestartConnection);
            }
            break;

        case ConnectionState::EstablishConnection:
            // _connectionPath.RouteHandler->Process
            Logger::LogInfo(Logger::LogSource::Gateway, "Connection Established.");
            changeState(ConnectionState::Done);
            break;

        case ConnectionState::Done:
            break;

        default:
            break;
        }
        Delay(1000);
    }
}

} // namespace Applications
