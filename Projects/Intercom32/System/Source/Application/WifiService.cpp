#include "WifiService.h"
#include "Logger.h"
#include "RTOSExtra.h"
#include "DebugAssert.h"

namespace Applications
{

using Hal::Hardware;
using Utilities::Logger;
using Configuration::BoardConfiguration;
using Configuration::ConfigurationAgent;

void WifiService::ConfigurationUpdated()
{
    BoardConfiguration &configuration = *ConfigurationAgent::Instance()->GetBoardConfiguration();
    _authentication = configuration.GetConfiguration()->WifiConfig.AuthenticationMode;
    _wifiMode = configuration.GetConfiguration()->WifiConfig.WifiMode;
    _channel = configuration.GetConfiguration()->WifiConfig.Channel;
    _useDhcp = configuration.GetConfiguration()->WifiConfig.Settings.Flags.DhcpEnabled;
    memcpy(_ssid.data(), configuration.GetConfiguration()->WifiConfig.Ssid.data(),configuration.GetConfiguration()->WifiConfig.Ssid.size());
    memcpy(_password.data(), configuration.GetConfiguration()->WifiConfig.Password.data(),configuration.GetConfiguration()->WifiConfig.Password.size());
    ip_addr_copy(_ipAddress, configuration.GetConfiguration()->WifiConfig.IPAddress);
    ip_addr_copy(_maskAddress, configuration.GetConfiguration()->WifiConfig.Mask);
    ip_addr_copy(_gatewayAddress, configuration.GetConfiguration()->WifiConfig.GatewayAddress);
    ip_addr_copy(_dnsServer, configuration.GetConfiguration()->WifiConfig.DnsServer);
    changeState(WifiState::ResetAdapter);
}

void WifiService::changeState(WifiState wifiState)
{
    _wifiState = wifiState;
    Logger::LogInfo(Logger::LogSource::Wifi, "wifiState: %d",static_cast<uint8_t>(wifiState));
}


void WifiService::Run()
{
    tcpip_adapter_init();
    DebugAssert(esp_netif_init(), ESP_OK);

    WifiDriver &_wifiDriver = Hardware::Instance()->GetWifi();

    Logger::LogInfo(Logger::LogSource::Wifi, "Running Wifi.");
    for (;;)
    {
        vTaskDelay(1000);
        switch (_wifiState)
        {
        case WifiState::Idle:
        {
            _connected = false;
            vTaskDelay(10000);
        }
        break;
        case WifiState::ResetAdapter:
        {
            _wifiDriver.Disable();
            changeState(WifiState::ConfigureTransmitter);
        }
        break;
        case WifiState::ConfigureTransmitter:
        {
            if (_wifiMode == WifiModeConfiguration::Client)
            {
                Logger::LogInfo(Logger::LogSource::Wifi, "SSID: %s.",_ssid.data());
                _wifiDriver.SetMode(WifiModeConfiguration::Client);
                _wifiDriver.SetSsid(_ssid.data(),_ssid.size());
                _wifiDriver.SetPassword(_password.data(),_password.size());
                _wifiDriver.SetAuthentication(_authentication);
            }
            else
            {
               Logger::LogInfo(Logger::LogSource::Wifi, "SSID: %s.",_ssid.data());
                _wifiDriver.SetMode(WifiModeConfiguration::HotSpot);
                _wifiDriver.SetSsid(_ssid.data(),_ssid.size());
                _wifiDriver.SetPassword(_password.data(),_password.size());
                _wifiDriver.SetAuthentication(_authentication);
                _wifiDriver.SetChannel(_channel);
            }
            _wifiDriver.Enable();
            changeState(WifiState::WaitingTransmitter);
        }
        case WifiState::WaitingTransmitter:
        {
            changeState(WifiState::PrepareWifiConnection);
        }
        break;
        case WifiState::PrepareWifiConnection:
        {
            if (_wifiMode == WifiModeConfiguration::Client)
            {
                if (_useDhcp)
                    changeState(WifiState::DhcpRequest);
                else
                    changeState(WifiState::StaticIpRequest);
            }
            else
            {
                changeState(WifiState::StartHotspot);
            }
        }
        break;
        case WifiState::StaticIpRequest:
        {
            changeState(WifiState::StaticIpDone);
        }
        break;
        case WifiState::StaticIpDone:
        {

        }
        break;
        case WifiState::DhcpRequest:
        {
            changeState(WifiState::DhcpWaiting);
        }
        break;
        case WifiState::DhcpWaiting:
        {
            esp_netif_ip_info_t ip_info = {};
            esp_netif_t *gnetif = _wifiDriver.GetWifiClientNetif();
            
            assert(gnetif);
            DebugAssert(esp_netif_get_ip_info(gnetif, &ip_info), ESP_OK);
    
            Logger::LogInfo(Logger::LogSource::Wifi, "Ip: %s",ip4addr_ntoa((const ip4_addr_t*)&ip_info.ip));
            Hardware::Instance()->GetCamera().Init();
            changeState(WifiState::DhcpDone);
            
        }
        break;
        case WifiState::DhcpDone:
        {
            
        }
        break;
        case WifiState::StartHotspot:
        {
            if (_useDhcp)
                    changeState(WifiState::StartDhcpServer);
                else
                    changeState(WifiState::StartDhcpServer);
        }
        break;
        case WifiState::SetStaticIp:
        {
            changeState(WifiState::StaticIpDone);
        }
        break;
        case WifiState::StartDhcpServer:
        {

            esp_netif_ip_info_t ip_info = {};
            esp_netif_t *gnetif = _wifiDriver.GetWifiHostNetif();
            
            assert(gnetif);
            DebugAssert(esp_netif_get_ip_info(gnetif, &ip_info), ESP_OK);
    
            Logger::LogInfo(Logger::LogSource::Wifi, "Ip: %s",ip4addr_ntoa((const ip4_addr_t*)&ip_info.ip));
            Hardware::Instance()->GetCamera().Init();
            changeState(WifiState::DhcpDone);
        }
        break;

        default:
            break;
        }
    }
}

} // namespace Applications