
#ifndef INCLUDE_HAL_GPIO_H_
#define INCLUDE_HAL_GPIO_H_

#include "HalCommon.h"

// Arduino Port
#define OUTPUT 0
#define INPUT 1
#define INPUT_PULLUP 2

typedef unsigned int uint32;

namespace Hal
{
class Gpio
{

public:
	static constexpr uint8_t MaxIoPins = 40;
	//    @note There are more enumerations like that
	//   up to GPIO39, excluding GPIO20, GPIO24 and GPIO28..31.
	//    They are not shown here to reduce redundant information.
	//    @note GPIO34..39 are input mode only. */
	enum class GpioIndex : uint8_t
	{
		Gpio0 = 0,
		Gpio1,
		Gpio2,
		Gpio3,
		Gpio4,
		Gpio5,
		Gpio6,
		Gpio7,
		Gpio8,
		Gpio9,
		Gpio10,
		Gpio11,
		Gpio12,
		Gpio13,
		Gpio14,
		Gpio15,
		Gpio16,
		Gpio17,
		Gpio18,
		Gpio19,
		RESERVED0,
		Gpio21,
		Gpio22,
		Gpio23,
		RESERVED1,
		Gpio25,
		Gpio26,
		Gpio27,
		RESERVED2,
		RESERVED3,
		RESERVED4,
		RESERVED5,
		Gpio32,
		Gpio33,
		Gpio34,
		Gpio35,
		Gpio36,
		Gpio37,
		Gpio38,
		Gpio39
	};

	enum class Mode : uint8_t
	{
		Disable = 0,
		Input = 1,
		Output = 2,
		OutputOpenDrain = 6
	};

	enum class OutputType : uint8_t
	{
		PullUp = 0,
		OpenDrain = 1
	};

	enum class Pull : uint8_t
	{
		Up = 0,
		Down = 1,
		UpDown = 2,
		Float = 3
	};

	enum class AltFunc : uint8_t
	{
		Gpio,
		Pwm,
		Timer,
		Spi,
		I2c,
		Can,
		SdCard,
		Analog,
		Uart
	};

	enum class State : uint8_t
	{
		Low,
		High
	};

	Gpio();
	~Gpio();

	bool Get(GpioIndex index);
	void Set(GpioIndex index);
	void Set(GpioIndex index, bool state);
	void Reset(GpioIndex index);
	void Toggle(GpioIndex index);

	void SetMode(GpioIndex index, Mode mode);
	Mode GetMode(GpioIndex index);

	void SetPull(GpioIndex index, Pull pull, bool KeepInSleepMode = true);
	void SetAlternate(GpioIndex index, AltFunc altFunc);

	void ConfigInput(GpioIndex index, Pull pull = Pull::Down);
	void ConfigOutput(GpioIndex index, OutputType outputType);
	
	/// @brief Artduino Port
	void ArduinoPinMode(int pin, int mode);

private:
	struct Pinout
	{
		Mode mode;
		OutputType ouputType;
		Pull pull;
		AltFunc altFunc;
		State state;
	} pins[MaxIoPins];
};

} // namespace Hal

#endif /* INCLUDE_HAL_GPIO_H_ */
