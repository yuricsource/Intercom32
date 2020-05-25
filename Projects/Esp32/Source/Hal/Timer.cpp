
#include "Timer.h"
//#define DEBUG_TIMER
namespace Hal
{

Timer::Timer(Hal::TimerInterruptHandler *itrpHandler, TimerSelect timerSelected) : interruptHandler(itrpHandler)
{
	switch (timerSelected)
	{
	case TimerSelect::Timer0:
		this->Preemption = Hal::Preemption::TIMER0;
		break;
	case TimerSelect::Timer1:
		this->Preemption = Hal::Preemption::TIMER1;
		break;
	}

}

Timer::~Timer()
{
}

void Timer::Initlialize()
{
#ifdef DEBUG_TIMER
	printf("Setting timer callback\n");
#endif
	// Initialize timer with 1 Hz
	this->AutoReload = false;
	this->Frequency = 1;

	for (uint8_t i = 0; i < MaxTimerCallBack; i++)
	{
		callbackList[i] = nullptr;
	}

	interruptHandler->SetCallback(this);

}

void Timer::InterruptCallback()
{
	// Call all interruptions registered in this timer
#ifdef DEBUG_TIMER
	printf("Timer::InterruptCallback()\n");
#endif

	for (uint8_t i = 0; i < MaxTimerCallBack; i++)
	{
		if (callbackList[i] != nullptr)
		{
			callbackList[i]->TimerCallback();
#ifdef DEBUG_TIMER
			printf("CallbackList %d: OKAY\n", i);
		}
		else
			printf("CallbackList %d: NULL\n", i);
#else
		}
#endif
	}
}

void Timer::Start()
{
	if (Frequency != 0)
		interruptHandler->Enable(this->Preemption);
}

void Timer::Stop()
{
	interruptHandler->Disable(this->Preemption);
}

void Timer::SetTimer(uint32_t frequency)
{
	if (frequency == 0)
		return;

	this->Frequency = (frequency + frequency / 25) * 2;
	interruptHandler->SetFrequency(this);
}

bool Timer::AddCallback(Timer::Callback *timerCallback)
{

#ifdef DEBUG_TIMER
	printf("Timer::AddCallback()\n");
#endif

	if (timerCallback == nullptr)
		return false;

	for (uint8_t i = 0; i < MaxTimerCallBack; i++)
	{
		if (callbackList[i] == nullptr)
		{
			callbackList[i] = timerCallback;
			return true;
		}
	}
	return false;
}

} // namespace Hal
