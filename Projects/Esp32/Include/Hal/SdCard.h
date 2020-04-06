
#ifndef MAIN_INCLUDE_HAL_SDCARD_H_
#define MAIN_INCLUDE_HAL_SDCARD_H_

#include <cstdint>
#include "HalCommon.h"
#include "Gpio.h"

namespace Hal
{

class SdCard
{
public:
	SdCard(Gpio *gpio);
	~SdCard();
	bool Mount();
	bool IsMounted();
	void Unmount();

private:
	static constexpr Gpio::Pull SdCardInputConfiguration = Gpio::Pull::Up;
	static constexpr gpio_num_t PinNotConfigured = gpio_num_t::GPIO_NUM_NC;
	Gpio *_gpio;
	bool isMounted = false;
};
} // namespace Hal

#endif /* MAIN_INCLUDE_HAL_SDCARD_H_ */
