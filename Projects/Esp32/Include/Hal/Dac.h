
#ifndef INCLUDE_HAL_DAC_H_
#define INCLUDE_HAL_DAC_H_

#include "HalCommon.h"
#include "Gpio.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "driver/dac.h"

namespace Hal
{
class Dac
{

public:

	Dac(Gpio *IoPins, Gpio::GpioIndex dacIo);
	~Dac();

	enum class DacIndex : uint8_t
	{
		DacChannel1,
		DacChannel2
	};
	void Set(uint8_t dacValue);
	void Enable();
	void Disable();
private:
	Gpio *_gpio;
	DacIndex _dac;
	bool _enabled;
};
} // namespace Hal

#endif /* INCLUDE_HAL_DAC_H_ */
