
#include <ExternalInterrupt.h>
#include "Hardware.h"

namespace Hal
{

/*--- InterruptHandler Class ---------------------------------------------------------------------------------*/

ExternalInterrupt::ExternalInterrupt(Gpio::GpioIndex gpio, ExternalInterrupt::InterruptType interruptType) : _gpioInterrupt(gpio), _gpio(Hardware::Instance()->GetGpio()), _interruptType(interruptType)
{
	//install gpio isr service
	gpio_install_isr_service(0);
}

ExternalInterrupt::~ExternalInterrupt()
{
}

void ExternalInterrupt::ConfigureInterrupt(gpio_isr_t handler)
{
	if (handler == nullptr)
	{
		printf("!!! Error Impossible to set an Invalid Callback !!!\n");
		return;
	}
	//gpio_isr_handler_add();
	_callbackPtr = handler;
}

void ExternalInterrupt::Enable()
{
	if (interruptionEnabled == false)
	{
		interruptionEnabled = true;
		gpio_set_intr_type(static_cast<gpio_num_t>(_gpioInterrupt), static_cast<gpio_int_type_t>(_interruptType));
		gpio_isr_handler_add(static_cast<gpio_num_t>(_gpioInterrupt), _callbackPtr, nullptr);
	}
}

void ExternalInterrupt::Disable()
{
	if (interruptionEnabled)
	{
		interruptionEnabled = false;
		//gpio_set_interrupt(static_cast<uint8_t>(_gpioInterrupt), static_cast<gpio_inttype_t>(InterruptType::None), nullptr);
		//gpio_isr_handler_add(static_cast<gpio_num_t>(_gpioInterrupt), static_cast<gpio_isr_t>(InterruptType::None), nullptr);
		gpio_isr_handler_remove((static_cast<gpio_num_t>(_gpioInterrupt)));
	}
}

} // namespace Hal
