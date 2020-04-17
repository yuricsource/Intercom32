
#ifndef INCLUDE_HAL_ADC_H_
#define INCLUDE_HAL_ADC_H_

#include "HalCommon.h"
#include "Gpio.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

namespace Hal
{

class Adc
{

public:
	static constexpr uint8_t MaxAdcConverters = 18;

	enum class AdcIndex : uint8_t
	{
		Adc1Channel0,
		Adc1Channel1,
		Adc1Channel2,
		Adc1Channel3,
		Adc1Channel4,
		Adc1Channel5,
		Adc1Channel6,
		Adc1Channel7,
		Adc2Channel0,
		Adc2Channel1,
		Adc2Channel2,
		Adc2Channel3,
		Adc2Channel4,
		Adc2Channel5,
		Adc2Channel6,
		Adc2Channel7,
		Adc2Channel8,
		Adc2Channel9,
		InvalidAdc
	};

	Adc(Gpio *IoPins);
	~Adc();
	bool InitAdc(AdcIndex adcIndex);
	uint32_t GetAdcValue(AdcIndex adcIndex, uint16_t averageSamples = 1);
	uint32_t GetAdcVoltage(AdcIndex adcIndex, uint16_t averageSamples = 1);
	Adc::AdcIndex GetAdcIndexFromGpio(Gpio::GpioIndex gpio);


private:
	Gpio *_gpio;

	static constexpr uint16_t DefaultVref = 1100; // ESP comoment: Use adc2_vref_to_gpio() to obtain a better estimate
	// static constexpr uint8_t AverageSamples = 64;

	enum class AdcSelection : uint8_t
	{
		Adc1 = 1,
		Adc2 = 2
	};

	Gpio::GpioIndex AdcPinSelect[MaxAdcConverters] =
	{
		Gpio::GpioIndex::Gpio36, //Adc1Channel0
		Gpio::GpioIndex::Gpio37, //Adc1Channel1
		Gpio::GpioIndex::Gpio38, //Adc1Channel2
		Gpio::GpioIndex::Gpio39, //Adc1Channel3
		Gpio::GpioIndex::Gpio32, //Adc1Channel4
		Gpio::GpioIndex::Gpio33, //Adc1Channel5
		Gpio::GpioIndex::Gpio34, //Adc1Channel6
		Gpio::GpioIndex::Gpio35, //Adc1Channel7
		Gpio::GpioIndex::Gpio4,  //Adc2Channel0
		Gpio::GpioIndex::Gpio0,  //Adc2Channel1
		Gpio::GpioIndex::Gpio2,  //Adc2Channel2
		Gpio::GpioIndex::Gpio15, //Adc2Channel3
		Gpio::GpioIndex::Gpio13, //Adc2Channel4
		Gpio::GpioIndex::Gpio12, //Adc2Channel5
		Gpio::GpioIndex::Gpio14, //Adc2Channel6
		Gpio::GpioIndex::Gpio27, //Adc2Channel7
		Gpio::GpioIndex::Gpio25, //Adc2Channel8
		Gpio::GpioIndex::Gpio26, //Adc2Channel9
	};

	esp_adc_cal_characteristics_t _adcCharacteristics[MaxAdcConverters];
};

} // namespace Hal

#endif /* INCLUDE_HAL_ADC_H_ */
