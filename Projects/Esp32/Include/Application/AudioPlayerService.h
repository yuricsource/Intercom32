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
#include "CircularBuffer.h"

namespace Applications
{

using Hal::Timer;
using Hal::Adc;
using Hal::Hardware;
using Utilities::Logger;
using Utilities::CircularBuffer;

class AudioPlayerService : public cpp_freertos::Thread, Timer::Callback
{
public:
    AudioPlayerService();
    ~AudioPlayerService();
    static constexpr uint8_t BufferSeconds = 3;
    static constexpr uint32_t AudioSampling = 8000;
protected:
    void Run() override;

private:
	void IRAM_ATTR TimerCallback();
    void resetAudioBudder();
    CircularBuffer <uint8_t> _audioBuffer;
    Hardware *_hardware;
    Adc::AdcIndex _microphone;
    static constexpr uint16_t TempBufferSize = 512;

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