
#ifndef INCLUDE_HAL_WDT_H_
#define INCLUDE_HAL_WDT_H_

#include "HalCommon.h"
#include "esp_task_wdt.h"

namespace Hal
{

class Wdt
{
public:
	Wdt();
	~Wdt();
	void Feed();
	void Start(uint8_t timeoutInSeconds, bool panic);
	void Stop();

private:
	bool enabled = false;
};

/*******************************************************************************/
// In order to get the WDT working totally handled by FreeRTOS,
// the line 359 at sdk_pp_soft_wdt_init function, must be commented out.
// Then once the program starts, a FreeRTOS task must start and feed the WDT.
/*******************************************************************************/

} // namespace Hal

#endif /* INCLUDE_HAL_WDT_H_ */
