
#ifndef INCLUDE_HAL_DEVICEOUTPUT_H_
#define INCLUDE_HAL_DEVICEOUTPUT_H_

#include "HalCommon.h"
#include "Gpio.h"

namespace Hal
{

class DeviceOutput
{

public:
	static constexpr uint8_t MaxOutputs = 1;

	enum class OutputIndex : uint8_t
	{
		Output1,
	};

	DeviceOutput(Gpio *IoPins);
	~DeviceOutput();
	bool GetOutput(OutputIndex input);
	void SetOutput(OutputIndex input, Gpio::State state = Gpio::State::High);
	void ResetOutput(OutputIndex);
	void Toggle(OutputIndex output);

private:
	Gpio *_gpio;
	Gpio::GpioIndex outputIndex[MaxOutputs] =
	{
		Gpio::GpioIndex::Gpio2 // Connected to User Button
	};
};
} // namespace Hal

#endif /* INCLUDE_HAL_DEVICEOUTPUT_H_ */
