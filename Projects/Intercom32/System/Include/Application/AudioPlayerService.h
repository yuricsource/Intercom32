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

using Hal::Timer;
using Hal::Hardware;
using Utilities::Logger;

class AudioPlayerService : public cpp_freertos::Thread, Timer::Callback
{
public:
    AudioPlayerService();
    ~AudioPlayerService();

protected:
    void Run() override;

private:
	void TimerCallback();

private:
    /// @brief	Hide Copy constructor.
    AudioPlayerService(const AudioPlayerService &) = delete;

    /// @brief	Hide Assignment operator.
    AudioPlayerService &operator=(const AudioPlayerService &) = delete;

    /// @brief	Hide Move constructor.
    AudioPlayerService(AudioPlayerService &&) = delete;

    /// @brief	Hide Move assignment operator.
    AudioPlayerService &operator=(AudioPlayerService &&) = delete;
};

} // namespace Applications