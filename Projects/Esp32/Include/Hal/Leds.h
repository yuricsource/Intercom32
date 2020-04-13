
#ifndef INCLUDE_HAL_LEDS_H_
#define INCLUDE_HAL_LEDS_H_

#include "HalCommon.h"
#include "Gpio.h"

namespace Hal
{

class Leds
{

public:
	static constexpr uint8_t MaxLeds = 1;

	enum class LedIndex : uint8_t
	{
		Blue
	};

	Leds(Gpio *IoPins);
	~Leds();
	void SetLed(LedIndex led);
	void ResetLed(LedIndex led);
	void Toggle(LedIndex led);
	void SetAll();
	void ResetAll();

private:
	Gpio *_gpio;
	Gpio::GpioIndex ledsIndex[MaxLeds] =
	{
		Gpio::GpioIndex::Gpio2, // Blue Led
	};
};
} // namespace Hal

#endif /* INCLUDE_HAL_LEDS_H_ */
