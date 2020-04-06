#include "GatewayService.h"
#include "ConfigurationAgent.h"

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
    _connectionPath.RouteHandler = nullptr;
}

void GatewayService::Run()
{
    vTaskDelay(6000);

    RemoteConnection *connection = &ConfigurationAgent::Instance()->GetBoardConfiguration()->GetConfiguration()->ServerConfig.connection;

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
            _connectionPath.Connection->Connect(*connection);
            changeState(ConnectionState::EstablishConnection);
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
