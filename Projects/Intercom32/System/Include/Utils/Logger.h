#pragma once

#include <cstdint>
#include <cstdarg>
#include "Hardware.h"
#include "mutex.hpp"

using cpp_freertos::LockGuard;
using cpp_freertos::MutexStandard;

namespace Utilities
{
class Logger
{
public:
	enum class LogSource : uint8_t
	{
		Hal,
		Configuration,
		Camera,
		Ble,
		Wifi,
		Tcp,
		HttpServer,
		Gateway,
		FirmwareUpdate,
		AudioPlayer,
		Unknown = 255
	};

	struct LogInfos
	{
		char const *Type;
		unsigned char const Length;
	};

private:

	enum class TerminalColour : uint8_t
	{
		Black	= 30,
		Red		= 31,
		Green	= 32,
		Brown	= 33,
		Blue	= 34,
		Purple	= 35,
		Cyan	= 36,
		Default = 39
	};

	enum class BackgroundColour : uint8_t
	{
		Red		= 41,
		Green	= 42,
		Blue	= 44,
		Default = 49
	};

	static MutexStandard *_logLock;
	static void setColour(TerminalColour colour, BackgroundColour background = BackgroundColour::Default, bool bold = false);
	static bool createPrefix(const char *prefix, const char prefixLen, LogSource source);

public:
	static void LogError(const char *format, ...);
	static void LogError(LogSource source, const char *format, ...);

	static void LogInfo(const char *format, ...);
	static void LogInfo(LogSource source, const char *format, ...);
};

} // namespace Utilities