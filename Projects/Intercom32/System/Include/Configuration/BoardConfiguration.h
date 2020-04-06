#pragma once

#include "Crc32xZlib.h"
#include "lwip/ip_addr.h"
#include "BaseConfiguration.h"
#include "ConfigurationCommon.h"
#include "ConnectionTypes.h"
#include "HalCommon.h"

namespace Configuration
{

using Hal::MacAddress;
using Hal::WifiAuthenticationMode;
using Hal::WifiModeConfiguration;
using Hal::WifiPassword;
using Hal::WifiSsid;
using Protocol::RemoteConnection;
using Utilities::Crc32xZlib;

struct BoardConfigurationData
{
    /// @brief	General Board configuration.
    struct GeneralConfiguration
    {
        /// @brief	Configuration flags
        union _Settings {
            struct
            {
                bool WifiEnabled : 1;
                bool BluetoothEnabled : 1;
                bool WebServerEnebled : 1;
                uint32_t _NotUsed : 29;
            } Flags;
            uint32_t AllFlags;
        } Settings;
        /// @brief	Configuration flags
        union _Changes {
            struct
            {
                bool WifiEnabled : 1;
                bool BluetoothEnabled : 1;
                bool WebServerEnebled : 1;
                uint64_t _NotUsed : 61;
            } Flags;
            uint64_t AllChanges;
        } Changes;
    };

    /// @brief	Wifi configuration.
    struct WifiConfiguration
    {
        ip_addr_t IPAddress = {};
        ip_addr_t Mask = {};
        ip_addr_t GatewayAddress = {};
        ip_addr_t DnsServer = {};
        WifiModeConfiguration WifiMode = WifiModeConfiguration::Client;
        WifiAuthenticationMode AuthenticationMode = WifiAuthenticationMode::Open;
        uint8_t Channel = 0;
        WifiSsid Ssid = {};
        WifiPassword Password = {};
        MacAddress Mac = {};

        union _Settings {
            struct
            {
                bool DhcpEnabled : 1;
                bool Encrypted : 1;
                uint8_t _NotUsed : 6;
            } Flags;
            uint8_t AllFlags;
        } Settings;
        /// @brief	Configuration flags
        union _Changes {
            struct
            {
                bool WifiEnabled : 1;
                uint32_t _NotUsed : 31;
            } Flags;
            uint32_t AllChanges;
        } Changes;
        
        uint32_t GetCRC() const
        {
            return Crc32xZlib::GetCrc((unsigned char *)this, sizeof(WifiConfiguration), Crc32xZlib::Polynomial);
        }
    };

    /// @brief	Server configuration.
    struct ServerConfiguration
    {
        RemoteConnection connection = {};

        union _Settings {
            struct
            {
                uint8_t _NotUsed : 8;
            } Flags;
            uint8_t AllFlags;
        } Settings;
        /// @brief	Configuration flags
        union _Changes {
            struct
            {
                uint32_t _NotUsed : 32;
            } Flags;
            uint32_t AllChanges;
        } Changes;

        uint32_t GetCRC() const
        {
            return Crc32xZlib::GetCrc((unsigned char *)this, sizeof(ServerConfiguration), Crc32xZlib::Polynomial);
        }
    };

    GeneralConfiguration GeneralConfig;
    WifiConfiguration WifiConfig;
    ServerConfiguration ServerConfig;

    BoardConfigurationData() : GeneralConfig(),
                               WifiConfig(),
                               ServerConfig()

    {
    }

    uint32_t GetCRC() const
    {
        return Crc32xZlib::GetCrc((unsigned char *)this, sizeof(BoardConfigurationData), Crc32xZlib::Polynomial);
    }
};

class BoardConfiguration : public BaseConfiguration
{
public:
    BoardConfiguration();
    ~BoardConfiguration();

    bool Deserialize(const char *json);
    bool Serialize(char *json, int length);
    BoardConfigurationData *GetConfiguration() { return &_configuration; }
    void DefaultConfiguration();

private:
    BoardConfigurationData _configuration = {};
    static constexpr uint16_t JsonConversionLength = 512;

private:
    /// @brief	Hide Copy constructor.
    BoardConfiguration(const BoardConfiguration &) = delete;

    /// @brief	Hide Assignment operator.
    BoardConfiguration &operator=(const BoardConfiguration &) = delete;

    /// @brief	Hide Move constructor.
    BoardConfiguration(BoardConfiguration &&) = delete;

    /// @brief	Hide Move assignment operator.
    BoardConfiguration &operator=(BoardConfiguration &&) = delete;
};

} // namespace Configuration