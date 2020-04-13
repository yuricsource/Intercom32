
#include "HalCommon.h"
#include "Leds.h"

namespace Hal
{

Leds::Leds(Gpio *IoPins) : _gpio(IoPins)
{
	// Initializing all leds as output
	for (auto &led : ledsIndex)
	{
		_gpio->SetMode(led, Gpio::Mode::Output);
		_gpio->Reset(led);
	}
}

Leds::~Leds()
{
}

void Leds::SetLed(LedIndex led)
{
	_gpio->Set(ledsIndex[static_cast<uint8_t>(led)]);
}

void Leds::SetAll()
{
	for (auto &led : ledsIndex)
	{
		_gpio->Set(led);
	}
}

void Leds::ResetAll()
{
	for (auto &led : ledsIndex)
	{
		_gpio->Reset(led);
	}
}

void Leds::ResetLed(LedIndex led)
{
	_gpio->Reset(ledsIndex[static_cast<uint8_t>(led)]);
}

void Leds::Toggle(LedIndex led)
{
	_gpio->Toggle(ledsIndex[static_cast<uint8_t>(led)]);
}

} // namespace Hal
