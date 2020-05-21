#include "ApplicationAgent.h"

namespace Applications
{

ApplicationAgent *ApplicationAgent::_applications;

ApplicationAgent::ApplicationAgent()
{
    if (_applications == nullptr)
    {
        _applications = this;
    }
    else
    {
        printf("Error!\n");
    }
}

void ApplicationAgent::Initialize()
{
    _wifiService = new WifiService();
    // _httpServer = new HttpServer(80);
    _gatewayService = new GatewayService();
    _firmwareUpdateService = new FirmwareUpdateService();
    _audioPlayerService = new AudioPlayerService();
}

} // namespace Applications
