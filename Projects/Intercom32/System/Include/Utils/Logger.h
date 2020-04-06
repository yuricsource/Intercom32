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
		HttpServer,
		Gateway,
		FirmwareUpdate,
		Unknown = 255
	};

	struct LogInfos
	{
		char const *Type;
		unsigned char const Length;
	};

private:
	static MutexStandard *_logLock;
	static bool createPrefix(const char *prefix, const char prefixLen, LogSource source);

public:
	static void LogError(const char *format, ...);
	static void LogError(LogSource source, const char *format, ...);

	static void LogInfo(const char *format, ...);
	static void LogInfo(LogSource source, const char *format, ...);
};

} // namespace Utilities