
#ifndef INCLUDE_HAL_RTC_H_
#define INCLUDE_HAL_RTC_H_

#include "HalCommon.h"
#include "esp_sleep.h"
namespace Hal
{

class Rtc
{
public:
	Rtc();
	~Rtc();
	bool Read(uint32_t offset, uint8_t *buffer, uint32_t length);
	bool Write(uint32_t offset, uint8_t *buffer, uint32_t length);

private:
	static constexpr uint16_t MemoryBaseOffset = 64;
	static constexpr uint32_t RtcMemorySize = 8 * 1024;
	static constexpr uint32_t RtcSlowMemoryAddress = 0x50000000;
	static constexpr uint32_t RtcFastMemoryAddress1 = 0x3FF80000;
	static constexpr uint32_t RtcFastMemoryAddress2 = 0x400C0000;

	struct RtcMemory
	{
		union {
			uint8_t Raw[RtcMemorySize];
		};
	};
	volatile RtcMemory *_slowMemory;
	volatile RtcMemory *_fastMemory1;
	volatile RtcMemory *_fastMemory2;
};
} // namespace Hal

#endif /* INCLUDE_HAL_RTC_H_ */
