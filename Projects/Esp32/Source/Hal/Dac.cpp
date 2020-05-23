
#include "Dac.h"
#include <stdio.h>
#include <stdlib.h>
// #include "DebugAssert.h"

namespace Hal
{

Dac::Dac(Gpio *IoPins, Gpio::GpioIndex dacIo) : _gpio(IoPins), _enabled(false)
{
	if (dacIo == Gpio::GpioIndex::Gpio25)
		_dac = DacIndex::DacChannel1;
	else if (dacIo == Gpio::GpioIndex::Gpio26)
		_dac = DacIndex::DacChannel2;
	// else
	// 	DebugAssertFail("Dac pin not supported.");
	
}

void Dac::Enable()
{
	if (_enabled == false)
	{
		_enabled = true;
		dac_output_enable(static_cast<dac_channel_t>(_dac));
	}
}

void Dac::Set(uint8_t dacValue)
{
	if (_enabled)
	{
		dac_output_voltage(static_cast<dac_channel_t>(_dac), dacValue);
	}
}

void Dac::Disable()
{
	if (_enabled)
	{
		_enabled = false;
		dac_output_disable(static_cast<dac_channel_t>(_dac));
	}
}

Dac::~Dac()
{
}

}; // namespace Hal
