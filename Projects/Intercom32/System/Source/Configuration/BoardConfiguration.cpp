#include "BoardConfiguration.h"
#include "ArduinoJson.h"
#include "CommonTypes.h"
#include "IPParser.h"

using Common::IpAddress;
using Protocol::IPParser;

#define TEST_CLIENT

namespace Configuration
{

void BoardConfiguration::DefaultConfiguration()
{
    _configuration.GeneralConfig.Settings.Flags.WifiEnabled = true;
    _configuration.WifiConfig.Settings.Flags.DhcpEnabled = true;

#ifdef TEST_CLIENT
    /*Client Configuration*/
    _configuration.WifiConfig.WifiMode = WifiModeConfiguration::Client;
    sprintf(_configuration.WifiConfig.Ssid.data(), "%s", "Yuri_Duda");
    sprintf(_configuration.WifiConfig.Password.data(), "%s", "Australia2us");
    // memcpy(_configuration.ServerConfig.connection.Address.data(), "www.google.com", sizeof("www.google.com"));
    memcpy(_configuration.ServerConfig.Connection.Address.data(), "192.168.1.109", sizeof("192.168.1.109"));
    
    _configuration.ServerConfig.Connection.Port = 8080;
#else
    /*HotSpot Configuration*/
    _configuration.WifiConfig.Channel = 8;
    _configuration.WifiConfig.WifiMode = WifiModeConfiguration::HotSpot;
    sprintf(_configuration.WifiConfig.Ssid.data(), "%s", "WebCamera hotspot");
    sprintf(_configuration.WifiConfig.Password.data(), "%s", "12345678");
    _configuration.WifiConfig.AuthenticationMode = WifiAuthenticationMode::Wpa2Psk;
#endif
}

bool BoardConfiguration::Deserialize(const char *json)
{
    StaticJsonDocument<JsonConversionLength> doc;
    DeserializationError error = deserializeJson(doc, json);
    auto &generalChanges = _configuration.GeneralConfig.Changes.Flags;
    auto &generalFlags = _configuration.GeneralConfig.Settings.Flags;
    bool temp = false;

    if (doc["wifi"].isNull() == false)
    {
        if (doc["wifi"]["enabled"].isNull() == false)
        {
            temp = generalFlags.WifiEnabled;
            generalChanges.WifiEnabled = UpdateConfig(temp, doc["wifi"]["enabled"].as<bool>());
            generalFlags.WifiEnabled = temp;
        }
    }
    return true;
}
bool BoardConfiguration::Serialize(char *json, int length)
{
    if (json == nullptr)
        return false;

    auto &generalChanges = _configuration.GeneralConfig.Changes.Flags;
    auto &generalFlags = _configuration.GeneralConfig.Settings.Flags;
    auto &wificonfig = _configuration.WifiConfig;
    auto &wifiFlags = _configuration.WifiConfig.Settings.Flags;
    
    IpAddress ipTemp;
    StaticJsonDocument<JsonConversionLength> doc;

    doc["wifi"]["enabled"] = static_cast<bool>(generalFlags.WifiEnabled);

    doc["wifi"]["mode"] = static_cast<uint8_t>(wificonfig.WifiMode);

    IPParser::ToString(wificonfig.IPAddress, ipTemp.data(), ipTemp.size());
    doc["wifi"]["ipaddress"] = ipTemp.data();
 
    IPParser::ToString(wificonfig.Mask, ipTemp.data(), ipTemp.size());
    doc["wifi"]["mask"] = ipTemp.data();

    IPParser::ToString(wificonfig.DnsServer, ipTemp.data(), ipTemp.size());
    doc["wifi"]["dnsserver"] = ipTemp.data();

    doc["wifi"]["authmode"] = static_cast<uint8_t>(wificonfig.AuthenticationMode);
    
    doc["wifi"]["channel"] = wificonfig.Channel;

    doc["wifi"]["ssid"] = wificonfig.Ssid.data();
    doc["wifi"]["passwd"] =wificonfig.Password.data();
    doc["wifi"]["mac"] = wificonfig.Mac.data();
    
    uint16_t jsonLength = measureJson(doc) + 1;

    if (length < jsonLength)
        return false;

    char temp[JsonConversionLength];
    serializeJson(doc, temp);
    memcpy(json, temp, jsonLength + 1);

    return true;
}

BoardConfiguration::BoardConfiguration() : BaseConfiguration("Board Config")
{
    IPParser::Parse("0.0.0.0", _configuration.WifiConfig.IPAddress);
    IPParser::Parse("0.0.0.0", _configuration.WifiConfig.Mask);
    IPParser::Parse("0.0.0.0", _configuration.WifiConfig.GatewayAddress);
    IPParser::Parse("8.8.8.8", _configuration.WifiConfig.DnsServer);
}

BoardConfiguration::~BoardConfiguration()
{
}

} // namespace Configuration