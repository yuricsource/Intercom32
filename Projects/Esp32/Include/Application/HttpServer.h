// #pragma once

// #include <cstdint>
// #include <array>
// #include "Hardware.h"
// #include "freertos/FreeRTOS.h"
// #include "RTOSExtra.h"
// #include "freertos/task.h"
// #include "thread.hpp"
// #include "esp_err.h"
// #include "nvs_flash.h"
// #include "nvs.h"
// #include "esp_wifi.h"
// #include "Logger.h"
// #include "lwip/sockets.h"
// #include "esp_netif.h"

// namespace Applications
// {

// using Utilities::Logger;
// using std::array;

// class HttpServer : public cpp_freertos::Thread
// {
// public:
//     HttpServer(uint32_t port) : cpp_freertos::Thread("WEBSVC", configHTTPSVC_STACK_DEPTH, 3),
// 	_port(port)
//     {
//     }
// 	~HttpServer();
// protected:
//     void Run() override;

// private:
// 	uint16_t _httpResponse = 0;
// 	uint16_t _packetsCount = 0;
// 	static constexpr uint16_t _rxBufferSize = 1024;
// 	array<uint8_t, _rxBufferSize> _rxBuffer;

// 	static constexpr uint16_t _txBufferSize = 1024;
// 	array<uint8_t, _txBufferSize> _txBuffer;

// 	int _connectionTimeout;
// 	uint32_t _port = 0;

// 	sockaddr_in _localHost;
// private:
//     /// @brief	Hide Copy constructor.
//     HttpServer(const HttpServer &) = delete;

//     /// @brief	Hide Assignment operator.
//     HttpServer &operator=(const HttpServer &) = delete;

//     /// @brief	Hide Move constructor.
//     HttpServer(HttpServer &&) = delete;

//     /// @brief	Hide Move assignment operator.
//     HttpServer &operator=(HttpServer &&) = delete;
// };

// } // namespace Applications