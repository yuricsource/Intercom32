#pragma once

#include "Hardware.h"
#include "WifiService.h"
#include "HttpServer.h"
#include "TimeLimit.h"
#include "ConfigurationCommon.h"
#include "BaseConnection.h"
#include "BaseRouteHandler.h"
#include "TcpConnection.h"
#include "Logger.h"
#include "WebSocket.h"

namespace Applications
{

class FirmwareUpdateService : public cpp_freertos::Thread
{
public:
    FirmwareUpdateService();
    ~FirmwareUpdateService();

private:
    bool eraseOtherBank();


protected:
    void Run() override;

private:
    /// @brief	Hide Copy constructor.
    FirmwareUpdateService(const FirmwareUpdateService &) = delete;

    /// @brief	Hide Assignment operator.
    FirmwareUpdateService &operator=(const FirmwareUpdateService &) = delete;

    /// @brief	Hide Move constructor.
    FirmwareUpdateService(FirmwareUpdateService &&) = delete;

    /// @brief	Hide Move assignment operator.
    FirmwareUpdateService &operator=(FirmwareUpdateService &&) = delete;
};

} // namespace Applications