
#include "Dwt.h"
#include "Hardware.h"
#include <sys/time.h>
#include "esp32/rom/ets_sys.h"

namespace Hal
{

Dwt::Dwt()
{
}

void Dwt::DelayMicrosecond(uint32_t us)
{
	ets_delay_us(us);
}

void Dwt::DelayMilliseconds(uint32_t ms)
{
	for (uint16_t i = 0; i < 1000; i++)
		ets_delay_us(ms);
}

} // namespace Hal
