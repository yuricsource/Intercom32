
#ifndef INCLUDE_HAL_DEVICEINPUT_H_
#define INCLUDE_HAL_DEVICEINPUT_H_

#include "HalCommon.h"
#include "Gpio.h"

namespace Hal
{

class DeviceInput
{

public:
	static constexpr uint8_t MaxInputs = 1;

	enum class InputIndex : uint8_t
	{
		UserButton,
	};

	DeviceInput(Gpio *IoPins);
	~DeviceInput();
	bool GetInput(InputIndex input);

private:
	Gpio *_gpio;
	Gpio::GpioIndex inputIndex[MaxInputs] =
		{
			Gpio::GpioIndex::Gpio17 // Connected to User Button
	};
};
} // namespace Hal

#endif /* INCLUDE_HAL_DEVICEINPUT_H_ */
