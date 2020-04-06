
#include "Rtc.h"
#include <cstring>

namespace Hal
{

Rtc::Rtc() : _slowMemory(reinterpret_cast<volatile RtcMemory *>(RtcSlowMemoryAddress)),
			 _fastMemory1(reinterpret_cast<volatile RtcMemory *>(RtcFastMemoryAddress1)),
			 _fastMemory2(reinterpret_cast<volatile RtcMemory *>(RtcFastMemoryAddress2))
{
}

Rtc::~Rtc()
{
}

bool Rtc::Read(uint32_t offset, uint8_t *buffer, uint32_t length)
{
	if (length + offset > RtcMemorySize)
		return false;
#ifdef RTC_MEMORY_DEBUG
	printf("Reading %d from address %d\n", length, offset);
#endif
	memcpy(buffer, (void *)_slowMemory->Raw, length);
	return true;
}

bool Rtc::Write(uint32_t offset, uint8_t *buffer, uint32_t length)
{
	if (length + offset > RtcMemorySize)
		return false;
#ifdef RTC_MEMORY_DEBUG
	printf("Writing %d from address %d\n", length, offset);
#endif
	memcpy((void *)_slowMemory->Raw, buffer, length);
	return true;
}

} // namespace Hal
