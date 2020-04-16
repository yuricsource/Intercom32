
#ifndef INCLUDE_HAL_I2S_H_
#define INCLUDE_HAL_I2S_H_

#include "HalCommon.h"
#include "driver/i2s.h"
#include "Gpio.h"

namespace Hal
{

class I2s
{
public:
	I2s(Gpio *gpio, I2sBus i2sBus, I2sBitSample bitSample, I2sChannelMode channelMode);
	~I2s();
	void Start();
	void Stop();
	bool Play(const uint8_t *buffer, uint32_t size);
	bool UpdateChannelClock(I2sBitSample sampleBit, I2sChannelMode channelMode, uint32_t sampleRate);

private:
	enum class I2sSampleRate : uint32_t
	{
		SampleRate8K = 8000,
		SampleRate16K = 16000,
		SampleRate24K = 24000,
		SampleRate32K = 32000
	};
	Gpio *_gpio;
	I2sBus _i2s;
	I2sBitSample _bitSample;
	uint32_t _sampleRate = static_cast<uint32_t>(I2sSampleRate::SampleRate16K);
	I2sChannelMode _channelMode;
	I2sChannelFormat _channelFormat;
	I2sDacMode _dacMode;
	I2sCommunicationStandardFormat _communicationStandardFormat;
	static constexpr uint16_t AudioBufferingSize = 1024;
	uint8_t *_audioBuffer[AudioBufferingSize];
	static constexpr uint32_t DefaultConfigurationMode = (I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN | I2S_MODE_ADC_BUILT_IN);
	bool _enabled = false;
	int dacDataScale(uint8_t *d_buff, uint8_t *s_buff, uint32_t len);
};
} // namespace Hal

#endif /* INCLUDE_HAL_I2S_H_ */
