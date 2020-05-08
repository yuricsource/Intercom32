#include "Logger.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "esp_event_legacy.h"
#include <cstdio>
#include <cstdarg>

namespace Utilities
{

using Hal::Hardware;

static char const *seperator = " | ";
unsigned char const seperatorLen = 3;

static char const *newLine = "\r\n";
unsigned char const newLineLen = 2;

static char const *severityInfo = "I";
unsigned char const severityInfoLen = 1;

char const *severityError = "E";
unsigned char const severityErrorLen = 1;

Logger::LogInfos typeInfos[10] =
{
	{" HAL  ", 6},
	{"CONFIG", 6},
	{"CAMERA", 6},
	{" BLE  ", 6},
	{" WIFI ", 6},
	{" TCP  ", 6},
	{"WEBSRV", 6},
	{"GATEWY", 6},
	{"FWUPDT", 6},
	{"ADPLAY", 6}
};

void Logger::LogInfo(const char *format, ...)
{
	if (Hardware::Instance()->GetDebugPort().IsEnabled() == false)
		return;

	LockGuard logGuard(*_logLock);

	setColour(TerminalColour::Default, BackgroundColour::Default);

	do
	{
		if (!createPrefix(severityInfo, severityInfoLen, LogSource::Unknown))
			break;

		va_list argptr;
		va_start(argptr, format);
		vfprintf(stdout, format, argptr);
		va_end(argptr);
	} while (false);
	fwrite(newLine, 1, newLineLen, stdout);
}

void Logger::LogInfo(LogSource source, const char *format, ...)
{
	if (Hardware::Instance()->GetDebugPort().IsEnabled() == false)
		return;

	LockGuard logGuard(*_logLock);
	setColour(TerminalColour::Default, BackgroundColour::Default);

	do
	{
		if (!createPrefix(severityInfo, severityInfoLen, source))
			break;

		va_list argptr;
		va_start(argptr, format);
		vfprintf(stdout, format, argptr);
		va_end(argptr);
	} while (false);
	fwrite(newLine, 1, newLineLen, stdout);
}

void Logger::setColour(TerminalColour colour, BackgroundColour background, bool bold)
{
	char colourPrefix[10] = {};
	sprintf(colourPrefix, "\033[%d;%d;%dm", bold, static_cast<uint8_t>(background), static_cast<uint8_t>(colour));
	fwrite(colourPrefix, 1, strlen(colourPrefix), stdout);
}

void Logger::LogError(const char *format, ...)
{
	if (Hardware::Instance()->GetDebugPort().IsEnabled() == false)
		return;

	LockGuard logGuard(*_logLock);
	setColour(TerminalColour::Red);

	do
	{
		if (!createPrefix(severityError, severityErrorLen, LogSource::Unknown))
			break;

		va_list argptr;
		va_start(argptr, format);
		vfprintf(stdout, format, argptr);
		va_end(argptr);
	} while (false);
	fwrite(newLine, 1, newLineLen, stdout);
}

void Logger::LogError(LogSource source, const char *format, ...)
{
	if (Hardware::Instance()->GetDebugPort().IsEnabled() == false)
		return;

	LockGuard logGuard(*_logLock);
	setColour(TerminalColour::Red);

	do
	{
		if (!createPrefix(severityError, severityErrorLen, source))
			break;

		va_list argptr;
		va_start(argptr, format);
		vfprintf(stdout, format, argptr);
		va_end(argptr);
	} while (false);
	fwrite(newLine, 1, newLineLen, stdout);
}

bool Logger::createPrefix(const char *prefix, const char prefixLen, LogSource source)
{
	fwrite(prefix, 1, prefixLen, stdout);
	fwrite(seperator, 1, seperatorLen, stdout);
	uint32_t ms = Hardware::Instance()->Milliseconds();
	int milliseconds = (ms % 1000);
	int seconds = (ms / 1000) % 60;
	int minutes = ((ms / (1000 * 60)) % 60);
	int hours = ((ms / (1000 * 60 * 60)) % 24);
	int days = ((ms / (1000 * 60 * 60 * 24)) % 99);
	char timeText[20];
	uint8_t len = 0;
	if (ms >= (1000 * 60 * 60 * 24))
		len = std::snprintf(timeText, 20, "%d:%02d:%02d:%02d.%03d", days, hours, minutes, seconds, milliseconds);
	else
		len = std::snprintf(timeText, 20, "%02d:%02d:%02d.%03d", hours, minutes, seconds, milliseconds);
	fwrite(timeText, 1, len, stdout);
	fwrite(seperator, 1, seperatorLen, stdout);

#if LOGGER_PREFIX_TIME
	// This code is here in case the real time is required to be displayed.
	Hal::RtcTimeTypeDef time;
	Hal::System::Instance()->GetRtc().GetTime(time.Hours, time.Minutes, time.Seconds, time.IsPM);
	char timeText[9];
	if (std::snprintf(timeText, 9, "%02d:%02d:%02d", time.Hours, time.Minutes, time.Seconds) == 8)
	{
		fwrite(timeText, 1, 8, stdout);
		fwrite(seperator, 1, seperatorLen, stdout);
	}
#endif // LOGGER_PREFIX_TIME

	if (source != LogSource::Unknown)
	{
		Logger::LogInfos typeInfoItem = typeInfos[static_cast<uint8_t>(source)];
		fwrite(typeInfoItem.Type, 1, typeInfoItem.Length, stdout);
		fwrite(seperator, 1, seperatorLen, stdout);
	}
	return true;
}

MutexStandard *Logger::_logLock = new MutexStandard();

} // namespace Utilities