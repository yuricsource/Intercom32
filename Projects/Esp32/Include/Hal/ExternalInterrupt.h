
#ifndef INCLUDE_HAL_EXTERNALINTERRUPT_H_
#define INCLUDE_HAL_EXTERNALINTERRUPT_H_

#include "HalCommon.h"
#include "Gpio.h"
#include "driver/gpio.h"

namespace Hal
{

class ExternalInterrupt
{
public:
	typedef enum
	{
		None = 0,
		EdgePositive = 1,
		EdgeNegative = 2,
		EdgePositiveOrNegative = 3,
		LevelLow = 4,
		LevelHigh = 5
	} InterruptType;

	~ExternalInterrupt();
	ExternalInterrupt(Gpio::GpioIndex gpio, ExternalInterrupt::InterruptType interruptType);

	class Callback
	{
	public:
		virtual void InterruptCallback(uint8_t arg) = 0;
	};

	void ConfigureInterrupt(gpio_isr_t handler);
	void Enable();
	void Disable();

private:
	bool interruptionEnabled = false;
	gpio_isr_t _callbackPtr = nullptr;
	Gpio::GpioIndex _gpioInterrupt;
	Gpio &_gpio;
	ExternalInterrupt::InterruptType _interruptType;
};

} // namespace Hal

#endif /* INCLUDE_HAL_EXTERNALINTERRUPT_H_ */
