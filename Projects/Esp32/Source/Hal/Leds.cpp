
#include "HalCommon.h"
#include "Leds.h"

namespace Hal
{

Leds::Leds(Gpio *IoPins) : _gpio(IoPins)
{
	// Initializing all leds as output
	for (auto &led : ledsIndex)
	{
		if (led == ledsIndex[static_cast<uint8_t>(LedIndex::FlashLed)])
		{
			_gpio->SetMode(led, Gpio::Mode::Input);
			_gpio->SetPull(led, Gpio::Pull::Down);
		}
		else
		{
			_gpio->SetMode(led, Gpio::Mode::Output);
			_gpio->Reset(led);
		}
	}
}

Leds::~Leds()
{
}

void Leds::SetLed(LedIndex led)
{
	if (led == LedIndex::FlashLed)
		_gpio->SetPull(ledsIndex[static_cast<uint8_t>(led)], Gpio::Pull::Up);
	else
		_gpio->Set(ledsIndex[static_cast<uint8_t>(led)]);
}

void Leds::SetAll()
{
	for (auto &led : ledsIndex)
	{
		if (led == ledsIndex[static_cast<uint8_t>(LedIndex::FlashLed)])
			_gpio->SetPull(ledsIndex[static_cast<uint8_t>(led)], Gpio::Pull::Up);
		else
			_gpio->Set(led);
	}
}

void Leds::ResetAll()
{
	for (auto &led : ledsIndex)
	{
		if (led == ledsIndex[static_cast<uint8_t>(LedIndex::FlashLed)])
			_gpio->SetPull(ledsIndex[static_cast<uint8_t>(led)], Gpio::Pull::Down);
		else
			_gpio->Reset(led);
	}
}

void Leds::ResetLed(LedIndex led)
{
	if (led == LedIndex::FlashLed)
		_gpio->SetPull(ledsIndex[static_cast<uint8_t>(led)], Gpio::Pull::Down);
	else
		_gpio->Reset(ledsIndex[static_cast<uint8_t>(led)]);
}

void Leds::Toggle(LedIndex led)
{
	if (led != LedIndex::FlashLed)
		_gpio->Toggle(ledsIndex[static_cast<uint8_t>(led)]);
}

} // namespace Hal
