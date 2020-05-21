
#ifndef HAL_WIFIDRIVER_H_
#define HAL_WIFIDRIVER_H_

#include "Gpio.h"
#include "HalCommon.h"
#include <array>
#include "esp_wifi.h"

namespace Hal
{
using std::array;

class WifiDriver
{
public:
	WifiDriver();
	~WifiDriver();
	void ResetDriver();
	bool Enable();
	bool Disable();
	inline bool IsEnabled(){return _isEnabled;};
	bool SetSsid(const char* ssid, uint8_t len);
	bool SetPassword(const char* passwd, uint8_t len);
	bool SetMode(WifiModeConfiguration wifiConfiguration);
	bool SetAuthentication(WifiAuthenticationMode authentication);
	bool SetChannel(uint8_t channel);
//	esp_netif_t *GetWifiClientNetif() {return _clientNetif;}
//	esp_netif_t *GetWifiHostNetif() {return _hotstopNetif;}

private:
	bool _isEnabled = false;
	bool _isConnected = false;
//	esp_netif_t *_hotstopNetif = nullptr;
//	esp_netif_t *_clientNetif = nullptr;
	WifiAuthenticationMode _authentication = WifiAuthenticationMode::Open;
	WifiModeConfiguration _wifiConfiguration = WifiModeConfiguration::HotSpot;
	uint8_t _channel = 8;

	WifiSsid _ssid = {};
	WifiPassword _password = {};
};

} // namespace Hal

// wifi_config_t wifi_config = {
// .ap = {
// 	.ssid = EXAMPLE_ESP_WIFI_SSID,
// 	.ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
// 	.password = EXAMPLE_ESP_WIFI_PASS,
// 	.max_connection = EXAMPLE_MAX_STA_CONN,
// 	.authmode = WIFI_AUTH_WPA_WPA2_PSK
// },
//                  |          (A) USER CODE                 |
//                  |                                        |
//     .............| init          settings      events     |
//     .            +----------------------------------------+
//     .               .                |           *
//     .               .                |           *
// --------+        +===========================+   *     +-----------------------+
//         |        | new/config     get/set    |   *     |                       |
//         |        |                           |...*.....| init                  |
//         |        |---------------------------|   *     |                       |
//   init  |        |                           |****     |                       |
//   start |********|  event handler            |*********|  DHCP                 |
//   stop  |        |                           |         |                       |
//         |        |---------------------------|         |                       |
//         |        |                           |         |    NETIF              |
//   +-----|        |                           |         +-----------------+     |
//   | glue|----<---|  esp_netif_transmit       |--<------| netif_output    |     |
//   |     |        |                           |         |                 |     |
//   |     |---->---|  esp_netif_receive        |-->------| netif_input     |     |
//   |     |        |                           |         + ----------------+     |
//   |     |....<...|  esp_netif_free_rx_buffer |...<.....| packet buffer         |
//   +-----|        |                           |         |                       |
//         |        |                           |         |         (D)           |
//   (B)   |        |          (C)              |         +-----------------------+
// --------+        +===========================+
// communication                                                NETWORK STACK
// DRIVER                   ESP-NETIF
//
// ........ Initialization line from user code to ESP-NETIF and communication driver
// --<--->-- Data packets going from communication media to TCP/IP stack and back
// ******** Events aggregated in ESP-NETIF propagates to driver, user code and network stack
// | User settings and runtime configuration


#endif /* HAL_WIFIDRIVER_H_ */
