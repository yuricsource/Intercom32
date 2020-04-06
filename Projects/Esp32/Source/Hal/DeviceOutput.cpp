
#include "HalCommon.h"
#include "DeviceOutput.h"

namespace Hal
{

DeviceOutput::DeviceOutput(Gpio *IoPins) : _gpio(IoPins)
{
	// Initializing all Inputs
	for (auto &output : outputIndex)
	{
		_gpio->ConfigOutput(output, Gpio::OutputType::PullUp);
		_gpio->Reset(outputIndex[static_cast<uint8_t>(output)]);
	}
}

DeviceOutput::~DeviceOutput()
{
}

bool DeviceOutput::GetOutput(OutputIndex output)
{
	return _gpio->Get(outputIndex[static_cast<uint8_t>(output)]);
}

void DeviceOutput::SetOutput(OutputIndex output, Gpio::State state)
{
	if (state == Gpio::State::High)
		_gpio->Set(outputIndex[static_cast<uint8_t>(output)]);
	else
		_gpio->Reset(outputIndex[static_cast<uint8_t>(output)]);
}

void DeviceOutput::ResetOutput(OutputIndex output)
{
	_gpio->Reset(outputIndex[static_cast<uint8_t>(output)]);
}

void DeviceOutput::Toggle(OutputIndex output)
{
	_gpio->Toggle(outputIndex[static_cast<uint8_t>(output)]);
}

} // namespace Hal
