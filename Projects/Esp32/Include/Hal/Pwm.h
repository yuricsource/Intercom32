
#ifndef HAL_PWM_H_
#define HAL_PWM_H_
#include <TimerInterruptHandler.h>
#include "HalCommon.h"
#include "Gpio.h"

namespace Hal
{

class Pwm : TimerInterruptHandler::Callback
{
public:
	enum class PwmIndex : uint8_t
	{
		PWM0,
		PWM1
	};

	Pwm(TimerInterruptHandler *itrpHandler, TimerSelect timerSelected, Gpio *gpio);
	~Pwm();
	void SetPin(PwmIndex pwm, Gpio::GpioIndex gpio);
	void SetPwmFrequence(uint32_t freq);
	void SetDutyCycle(PwmIndex pwm, uint32_t dutyCycle);
	void StartPwm();
	void StopPwm();
	void Start(PwmIndex pwm);
	void Stop(PwmIndex pwm);
	void Restart();
	void InterruptCallback();

private:
	static constexpr uint32_t DutyCycleResolution = 100;
	static constexpr uint8_t MaxPwmPins = 2;
	TimerInterruptHandler *_itrpHandler;
	TimerSelect _timerSelected;
	Gpio *_gpio;
	enum class PwmStep : uint8_t
	{
		PeriodOn = 0,
		PeriodOff = 1
	};

	struct PwmConfig
	{
		uint32_t DutyCycle = 0;
		uint32_t DutyCycleCount = 0;
		Gpio::GpioIndex GpioPin;
		bool Enabled = false;
	};

	PwmConfig pwmPins[MaxPwmPins];
};

} // namespace Hal

#endif /* HAL_PWM_H_ */
