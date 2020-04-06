#pragma once
#include "RTOSExtra.h"
#include "Hardware.h"
#include "freertos/FreeRTOS.h"
#include "RTOSExtra.h"
#include "freertos/task.h"
#include "thread.hpp"
#include "esp_err.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_wifi.h"
#include "WifiDriver.h"
#include "ConfigurationAgent.h"
#include "Hardware.h"

namespace Applications
{

using Hal::WifiAuthenticationMode;
using Hal::WifiModeConfiguration;
using Hal::WifiSsid;
using Hal::WifiPassword;
using Hal::WifiDriver;

class WifiService : public cpp_freertos::Thread
{
public:
    WifiService() : cpp_freertos::Thread("WIFISVC", configWIFISVC_STACK_DEPTH, 3)
    {
    }

    inline bool IsConnected()
    {
        return _connected;
    }

    void ConfigurationUpdated();

protected:
    void Run() override;

private:
    enum class WifiState : uint8_t
    {
        Idle,
        ResetAdapter,
        ConfigureTransmitter,
        WaitingTransmitter,
        PrepareWifiConnection,
        StaticIpRequest,
        StaticIpDone,
        DhcpStart,
        DhcpRequest,
        DhcpWaiting,
        DhcpDone,
        StartHotspot,
        SetStaticIp,
        StartDhcpServer
    };

    WifiState _wifiState = WifiState::Idle;
    bool _connected = false;
    bool _useDhcp = true;

    ip_addr_t _ipAddress = {};
    ip_addr_t _maskAddress = {};
    ip_addr_t _gatewayAddress = {};
    ip_addr_t _dnsServer = {};
	WifiAuthenticationMode _authentication = WifiAuthenticationMode::Open;
	WifiModeConfiguration _wifiMode = WifiModeConfiguration::HotSpot;    
	uint8_t _channel = 8;
	WifiSsid _ssid = {};
	WifiPassword _password = {};

    void changeState(WifiState wifiState);

private:
    /// @brief	Hide Copy constructor.
    WifiService(const WifiService &) = delete;

    /// @brief	Hide Assignment operator.
    WifiService &operator=(const WifiService &) = delete;

    /// @brief	Hide Move constructor.
    WifiService(WifiService &&) = delete;

    /// @brief	Hide Move assignment operator.
    WifiService &operator=(WifiService &&) = delete;
};

} // namespace Applications