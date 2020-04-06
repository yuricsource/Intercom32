
#include "Gpio.h"
#include "string.h"
#include "driver/gpio.h"

//#define DEBUG

namespace Hal
{

Gpio::Gpio()
{
	memset(pins, 0, sizeof(pins));
}

Gpio::~Gpio()
{
}

bool Gpio::Get(GpioIndex index)
{
	uint8_t i = static_cast<uint8_t>(index);
	if (pins[i].mode == Mode::Input)
	{
		if (gpio_get_level(static_cast<gpio_num_t>(i)))
			pins[i].state = State::High;
		else
			pins[i].state = State::Low;
	}
#ifdef DEBUG
	printf("\nRead Gpio %d: %d\n", static_cast<uint8_t>(index), static_cast<uint8_t>(pins[i].state));
#endif
	return pins[i].state == State::High;
}

void Gpio::Set(Gpio::GpioIndex index)
{
	uint8_t i = static_cast<uint8_t>(index);
	if (pins[i].mode == Mode::Output)
	{
		pins[i].state = State::High;
		gpio_set_level(static_cast<gpio_num_t>(i), static_cast<uint8_t>(State::High));
	}
}

void Gpio::Reset(Gpio::GpioIndex index)
{
	uint8_t i = static_cast<uint8_t>(index);
	if (pins[i].mode == Mode::Output)
	{
		pins[i].state = State::Low;
		gpio_set_level(static_cast<gpio_num_t>(i), static_cast<uint8_t>(State::Low));
	}
}

void Gpio::Toggle(Gpio::GpioIndex index)
{
	uint8_t i = static_cast<uint8_t>(index);

	if (pins[i].mode == Mode::Output)
	{
		pins[i].state = (pins[i].state == State::Low) ? State::High : State::Low;
		gpio_set_level(static_cast<gpio_num_t>(i), static_cast<uint8_t>(pins[i].state));
	}
}

void Gpio::SetMode(Gpio::GpioIndex index, Gpio::Mode mode)
{
	uint8_t pin = static_cast<uint8_t>(index);
	pins[pin].mode = mode;
#ifdef DEBUG
	printf("Setting Gpio mode: %d\n", static_cast<uint8_t>(mode));
#endif
	gpio_set_direction(static_cast<gpio_num_t>(index), static_cast<gpio_mode_t>(mode));
}

Gpio::Mode Gpio::GetMode(GpioIndex index)
{
	return pins[static_cast<uint8_t>(index)].mode;
}
void Gpio::SetPull(Gpio::GpioIndex index, Gpio::Pull pull, bool KeepInSleepMode)
{
	uint8_t i = static_cast<uint8_t>(index);
	pins[i].pull = pull;
	gpio_set_pull_mode(static_cast<gpio_num_t>(index), static_cast<gpio_pull_mode_t>(pull));
}

void Gpio::SetAlternate(Gpio::GpioIndex index, Gpio::AltFunc altFunc)
{
	pins[static_cast<uint8_t>(index)].altFunc = altFunc;
}

void Gpio::ConfigInput(Gpio::GpioIndex index, Gpio::Pull pull)
{
	SetMode(index, Mode::Input);
	SetPull(index, pull);
}

void Gpio::ConfigOutput(Gpio::GpioIndex index, Gpio::OutputType outputType)
{
	if (outputType == OutputType::PullUp)
	{
		SetMode(index, Mode::Output);
		SetPull(index, Gpio::Pull::Up);
	}
	else
	{
		SetPull(index, Gpio::Pull::Up);
		SetMode(index, Mode::OutputOpenDrain);
	}
}

} // namespace Hal
