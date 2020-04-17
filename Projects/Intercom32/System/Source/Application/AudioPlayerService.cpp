#include "AudioPlayerService.h"
#include "ConfigurationAgent.h"
#include "Hardware.h"
#include "BankConfiguration.h"
#include "Md5Hash.h"
#include "HalCommon.h"
#include "Flash.h"
#include "Timer.h"

namespace Applications
{
using Hal::Gpio;

AudioPlayerService::AudioPlayerService() : cpp_freertos::Thread("AUDSVC", configAUDPLAYERSVC_STACK_DEPTH, 3),
											_audioBuffer(BufferSeconds * AudioSampling)
{
	_hardware = Hardware::Instance();
	_microphone = _hardware->GetAdc().GetAdcIndexFromGpio(Gpio::GpioIndex::Gpio35);
}

AudioPlayerService::~AudioPlayerService()
{
}

void AudioPlayerService::Run()
{
	_hardware->GetAdc().InitAdc(_microphone);
	_hardware->GetDac().Enable();
	Logger::LogInfo(Utilities::Logger::LogSource::AudioPlayer, "Audio Player Task Initialized.");
	Hal::Hardware::Instance()->GetTimer0().AddCallback(this);
	Hal::Hardware::Instance()->GetTimer0().SetTimer(AudioSampling);
	// _hardware->GetI2s().Start();
	// _hardware->GetI2s().UpdateChannelClock(Hal::I2sBitSample::Sample16Bits, Hal::I2sChannelMode::ChannelMono, (AudioSampling + AudioSampling / 25) * 2);
	for(;;)
	{
		vTaskDelay(1);
		// uint32_t temp = _hardware->GetAdc().GetAdcValue(_microphone);
		// // uint32_t temp = _hardware->GetAdc().GetAdcVoltage(_microphone);
		// uint8_t value = static_cast<uint8_t>(temp >> 4);
		// Logger::LogInfo(Utilities::Logger::LogSource::AudioPlayer, "Value 8 bits: %d, Value 32 bits: %d", value, temp);

	if (_audioBuffer.Used())
	{
		// _hardware->GetI2s().UpdateChannelClock(Hal::I2sBitSample::Sample16Bits, Hal::I2sChannelMode::ChannelMono, AudioSampling);
		uint32_t length = 0;
		do
		{
			// length = _audioBuffer.Used();
			uint8_t tempBuffer[TempBufferSize];
			length = _audioBuffer.Read(tempBuffer, TempBufferSize);
			// _hardware->GetI2s().Play(tempBuffer, length);
			// printf("len: %d \n", length);

			// for(int i = 0; i < length; i ++)
			// 	printf("0x%02X ", tempBuffer[i]);
		} while (length != 0);

		}
	}
	// _hardware->GetI2s().Stop();
}

void AudioPlayerService::resetAudioBudder()
{
	_audioBuffer.Skip(_audioBuffer.Used());
}

void AudioPlayerService::TimerCallback()
{
	Hal::Hardware::Instance()->GetLeds().Toggle(Hal::Leds::LedIndex::Blue);
	// _hardware->GetAdc().GetAdcValue(_microphone);
	if (_audioBuffer.Free() > 0)
	{
		// uint32_t temp = _hardware->GetAdc().GetAdcVoltage(_microphone);
		// convert 12 bit to 8 bits
		uint32_t temp = _hardware->GetAdc().GetAdcValue(_microphone);
		uint8_t value = static_cast<uint8_t>(temp >> 4);
		// Logger::LogInfo(Utilities::Logger::LogSource::AudioPlayer, "Value 8 bits: %d, Value 32 bits: %d", value, temp);
		_audioBuffer.Write(&value, 1);
		_hardware->GetDac().Set(value);
	}
	else
	{
		//Logger::LogInfo(Utilities::Logger::LogSource::AudioPlayer, "Buffer Full.");
	}
	
	
}

} // namespace Applications
