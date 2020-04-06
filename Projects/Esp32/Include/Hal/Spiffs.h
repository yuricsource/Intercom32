
#ifndef INCLUDE_HAL_EXTERNAL_FLASHT_H_
#define INCLUDE_HAL_EXTERNAL_FLASHT_H_

#include "HalCommon.h"
#include "esp_spiffs.h"
#include "sys/stat.h"
#include "sys/unistd.h"
#include "string.h"

namespace Hal
{

class Spiffs
{
public:
	Spiffs();
	~Spiffs();
	uint32_t FreeMemory();
	uint32_t MemoryUsed();
	bool Mount();
	void Unmount();
	bool IsMounted();

private:
	uint32_t _memoryUsed = 0;
	uint32_t _memoryFree = 0;
	bool _isMounted = false;
};

} // namespace Hal

#endif /* INCLUDE_HAL_WDT_H_ */
