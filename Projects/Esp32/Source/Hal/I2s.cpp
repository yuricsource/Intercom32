
#include "Hardware.h"
#include "I2s.h"
#include "driver/adc.h"

namespace Hal
{

I2s::I2s(Gpio *gpio, I2sBus i2sBus, I2sBitSample bitSample, I2sChannelMode channelMode) :
		_gpio(gpio), _i2s(i2sBus), _bitSample(bitSample), _channelMode(channelMode),
		_channelFormat(I2sChannelFormat::ChannelFormatRightAndLeft), _dacMode(I2sDacMode::DacBothChannelsEnable),
		_communicationStandardFormat(I2sCommunicationStandardFormat::CommunicationFormatI2s_PCM)
{
}

I2s::~I2s()
{
}

bool I2s::UpdateChannelClock(I2sBitSample sampleBit, I2sChannelMode channelMode, uint32_t sampleRate)
{
	if (_enabled == false)
		return false;

	_bitSample = sampleBit;
	_channelMode = channelMode;
	_sampleRate = sampleRate;

	if (_channelMode == I2sChannelMode::ChannelStereo)
		_sampleRate = _sampleRate / 2;
	printf("Sample Rate: %d\n", _sampleRate);

	i2s_set_clk(static_cast<i2s_port_t>(_i2s), _sampleRate, static_cast<i2s_bits_per_sample_t>(_bitSample), static_cast<i2s_channel_t>(_channelMode));

	return true;
}

void I2s::Start()
{
	_sampleRate = static_cast<uint32_t>(I2sSampleRate::SampleRate16K);

	if (_channelMode == I2sChannelMode::ChannelStereo)
		_sampleRate = _sampleRate / 2;

	i2s_config_t i2s_config = { };
	i2s_config.mode = static_cast<i2s_mode_t>(DefaultConfigurationMode);
	i2s_config.sample_rate = static_cast<int>(_sampleRate);
	i2s_config.bits_per_sample = static_cast<i2s_bits_per_sample_t>(_bitSample);
	i2s_config.channel_format = (i2s_channel_fmt_t) _channelFormat;
	i2s_config.communication_format = static_cast<i2s_comm_format_t>(_communicationStandardFormat);
	i2s_config.intr_alloc_flags = 0;
	i2s_config.dma_buf_count = 2;
	i2s_config.dma_buf_len = 1024;
	i2s_config.use_apll = 1;
	i2s_config.tx_desc_auto_clear = false;
	i2s_config.fixed_mclk = 0;

	//install and start i2s driver
	i2s_driver_install(static_cast<i2s_port_t>(_i2s), &i2s_config, 0, nullptr);
	//init DAC pad
	i2s_set_dac_mode(static_cast<i2s_dac_mode_t>(_dacMode));

	i2s_set_clk(static_cast<i2s_port_t>(_i2s), _sampleRate, static_cast<i2s_bits_per_sample_t>(_bitSample), static_cast<i2s_channel_t>(_channelMode));
	_enabled = true;
}

void I2s::Stop()
{
	if (_enabled)
	{
		i2s_driver_uninstall(static_cast<i2s_port_t>(_i2s));
		_enabled = false;
	}
}

bool I2s::Play(const uint8_t *buffer, uint32_t size)
{

	if (buffer == nullptr || size == 0)
		return false;

	size_t bytes_written = 0;
	int offset = 0;
	int play_len = 0;
	int i2s_wr_len = 0;
	int tot_size = size; //sizeof(audio_table);

	while (offset < tot_size)
	{
		if (_bitSample == I2sBitSample::Sample16Bits)
			play_len = ((tot_size - offset) > (AudioBufferingSize / 2)) ? (AudioBufferingSize / 2) : (tot_size - offset);
		else
			play_len = ((tot_size - offset) > (AudioBufferingSize / 4)) ? (AudioBufferingSize / 4) : (tot_size - offset);
		i2s_wr_len = dacDataScale((uint8_t *)_audioBuffer, (uint8_t *)(buffer + offset), play_len);
		i2s_write(static_cast<i2s_port_t>(_i2s), _audioBuffer, (size_t)i2s_wr_len, &bytes_written, portMAX_DELAY); // @suppress("Invalid arguments")
		offset += play_len;
	}

	return true;
}

/**
 * @brief Scale data to 16bit/32bit for I2S DMA output.
 *        DAC can only output 8bit data value.
 *        I2S DMA will still send 16 bit or 32bit data, the highest 8bit contains DAC data.
 */
int I2s::dacDataScale(uint8_t *d_buff, uint8_t *s_buff, uint32_t len)
{
	uint32_t j = 0;

	if (_bitSample == I2sBitSample::Sample8Bits)
	{
		for (int i = 0; i < len; i++)
		{
			d_buff[j++] = s_buff[i];
		}
		return (len);
	}

	else if (_bitSample == I2sBitSample::Sample16Bits)
	{
		for (int i = 0; i < len; i++)
		{
			d_buff[j++] = 0;
			d_buff[j++] = s_buff[i];
		}
		return (len * 2);
	}

	else if (_bitSample == I2sBitSample::Sample24Bits)
	{
		for (int i = 0; i < len; i++)
		{
			d_buff[j++] = 0;
			d_buff[j++] = 0;
			d_buff[j++] = s_buff[i];
		}

		return (len * 3);
	}
	else
	{
		// for 32 bits
		for (int i = 0; i < len; i++)
		{
			d_buff[j++] = 0;
			d_buff[j++] = 0;
			d_buff[j++] = 0;
			d_buff[j++] = s_buff[i];
		}

		return (len * 4);
	}
	return 0;
}

} // namespace Hal
