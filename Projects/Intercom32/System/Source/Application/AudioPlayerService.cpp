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

AudioPlayerService::AudioPlayerService() : cpp_freertos::Thread("AUDSVC", configAUDPLAYERSVC_STACK_DEPTH, 3)
{
}

AudioPlayerService::~AudioPlayerService()
{
}

void AudioPlayerService::Run()
{

	Logger::LogInfo(Utilities::Logger::LogSource::AudioPlayer, "Audio Player Task Initialized.");
	Hal::Hardware::Instance()->GetTimer0().AddCallback(this);
	Hal::Hardware::Instance()->GetTimer0().SetTimer(2);
	while (true)
	{
		vTaskDelay(1000);
	}
}

void AudioPlayerService::TimerCallback()
{
	Hal::Hardware::Instance()->GetLeds().Toggle(Hal::Leds::LedIndex::Blue);
}

} // namespace Applications
