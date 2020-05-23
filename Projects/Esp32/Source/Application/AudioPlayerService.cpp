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
	for(;;)
	{
		vTaskDelay(1);
		if (_audioBuffer.Used())
		{
			uint32_t length = 0;
			do
			{
				uint8_t tempBuffer[TempBufferSize];
				length = _audioBuffer.Read(tempBuffer, TempBufferSize);
				// 	printf("0x%02X ", tempBuffer[i]);
			} while (length != 0);

		}
	}
}

void AudioPlayerService::resetAudioBudder()
{
	_audioBuffer.Skip(_audioBuffer.Used());
}

void AudioPlayerService::TimerCallback()
{
	Hal::Hardware::Instance()->GetLeds().Toggle(Hal::Leds::LedIndex::Blue);
	if (_audioBuffer.Free() > 0)
	{
		uint32_t temp = _hardware->GetAdc().GetAdcValue(_microphone);
		uint8_t value = static_cast<uint8_t>(temp >> 4);
		_audioBuffer.Write(&value, 1);
		_hardware->GetDac().Set(value);
	}
	else
	{
		//Logger::LogInfo(Utilities::Logger::LogSource::AudioPlayer, "Buffer Full.");
	}
	
	
}

} // namespace Applications
