
#include "Pwm.h"

namespace Hal
{

Pwm::Pwm(TimerInterruptHandler *itrpHandler, TimerSelect timerSelected, Gpio *gpio) : _itrpHandler(itrpHandler), _timerSelected(timerSelected), _gpio(gpio)
{
	switch (timerSelected)
	{
	case TimerSelect::Timer0:
		this->Preemption = Hal::Preemption::TIMER0;
		break;
	case TimerSelect::Timer1:
		this->Preemption = Hal::Preemption::TIMER1;
		break;
	}

	this->AutoReload = false;
	this->Frequency = 1;

	_itrpHandler->SetCallback(this);
	_itrpHandler->SetFrequency(this);

	for (auto &pin : pwmPins)
	{
		pin.DutyCycle = 0;
		pin.DutyCycleCount = 0;
		pin.Enabled = false;
	}
}

Pwm::~Pwm()
{
}

// TO-DO implement remove pin
void Pwm::SetPin(PwmIndex pwm, Gpio::GpioIndex gpio)
{
	_gpio->SetMode(gpio, Gpio::Mode::Output);
	_gpio->Reset(gpio);
	pwmPins[static_cast<uint8_t>(pwm)].GpioPin = gpio;
	_gpio->SetAlternate(gpio, Gpio::AltFunc::Pwm);
}

void Pwm::SetPwmFrequence(uint32_t freq)
{
	this->Frequency = freq * DutyCycleResolution;
	printf("Frequency: %u\n", this->Frequency);
	_itrpHandler->SetFrequency(this);
	Restart();
}

void Pwm::SetDutyCycle(PwmIndex pwm, uint32_t dutyCycle)
{
	pwmPins[static_cast<uint8_t>(pwm)].DutyCycle = dutyCycle;
	pwmPins[static_cast<uint8_t>(pwm)].DutyCycleCount = 0;
	//printf("Duty set at %u\n", pwmPins[static_cast<uint8_t>(pwm)].DutyCycle);
}

void Pwm::StartPwm()
{
	_itrpHandler->Enable(this->Preemption);
	for (auto &pin : pwmPins)
	{
		if (pin.Enabled)
			_gpio->Reset(pin.GpioPin);
	}
}

void Pwm::StopPwm()
{
	_itrpHandler->Disable(this->Preemption);
	for (auto &pin : pwmPins)
	{
		if (pin.Enabled)
			_gpio->Reset(pin.GpioPin);
	}
}

void Pwm::Start(PwmIndex pwm)
{
	pwmPins[static_cast<uint8_t>(pwm)].Enabled = true;
	pwmPins[static_cast<uint8_t>(pwm)].DutyCycleCount = 0;
	_gpio->Reset(pwmPins[static_cast<uint8_t>(pwm)].GpioPin);
}

void Pwm::Stop(PwmIndex pwm)
{
	pwmPins[static_cast<uint8_t>(pwm)].Enabled = false;
	pwmPins[static_cast<uint8_t>(pwm)].DutyCycleCount = 0;
	_gpio->Reset(pwmPins[static_cast<uint8_t>(pwm)].GpioPin);
}

void Pwm::Restart()
{
	StartPwm();
	StopPwm();
}

void Pwm::InterruptCallback()
{
	for (auto &pin : pwmPins)
	{
		if (pin.Enabled)
		{
			if (pin.DutyCycleCount > pin.DutyCycle)
				_gpio->Reset(pin.GpioPin);
			else
				_gpio->Set(pin.GpioPin);
			pin.DutyCycleCount = (pin.DutyCycleCount > DutyCycleResolution) ? 0 : pin.DutyCycleCount + 1;
		}
	}
}
} // namespace Hal
