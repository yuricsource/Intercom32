
#include "HalCommon.h"
#include "DeviceInput.h"

namespace Hal
{

DeviceInput::DeviceInput(Gpio *IoPins) : _gpio(IoPins)
{
	// Initializing all Inputs
	for (auto &input : inputIndex)
	{
		_gpio->ConfigInput(input, Gpio::Pull::Up);
	}
}

DeviceInput::~DeviceInput()
{
}

bool DeviceInput::GetInput(InputIndex input)
{
	return _gpio->Get(inputIndex[static_cast<uint8_t>(input)]);
}

} // namespace Hal
