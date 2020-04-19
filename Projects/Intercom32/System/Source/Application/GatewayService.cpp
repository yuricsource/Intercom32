#include "GatewayService.h"
#include "ConfigurationAgent.h"
#include "WebSocket.h"

namespace Applications
{

using Configuration::ConfigurationAgent;
// using Applications::GatewayService;

GatewayService::GatewayService() : cpp_freertos::Thread("GWSVC", configGATEWAYSVC_STACK_DEPTH, 3),
    _connectionPath(), _connectionState(ConnectionState::None)
{
    _connectionPath.Protocol = ProtocolType::Websocket;
    _connectionPath.TransportLayer = TransportLayerType::Wifi;
    _connectionPath.Connection = new TcpConnection();
    _connectionPath.RouteHandler = new Protocol::WebsocketPath();
}

void GatewayService::Run()
{
    vTaskDelay(6000);

    RemoteConnection *connection = &ConfigurationAgent::Instance()->GetBoardConfiguration()->GetConfiguration()->ServerConfig.Connection;
    _connectionPath.RouteHandler->SetConnection(_connectionPath.Connection);
    
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
            if (BaseConnection::ConnectStatus::SuccessfullyConnected == _connectionPath.Connection->Connect(*connection))
            {
                changeState(ConnectionState::EstablishConnection);
            }
            else
            {
                changeState(ConnectionState::RestartConnection);
            }
            break;

        case ConnectionState::EstablishConnection:
            changeState(ConnectionState::Done);
            break;

        case ConnectionState::Done:
            break;

        default:
            break;
        }
        vTaskDelay(100);
    }
}

} // namespace Applications
