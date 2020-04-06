
#ifndef INCLUDE_HAL_INTERRUPTHANDLER_H_
#define INCLUDE_HAL_INTERRUPTHANDLER_H_

#include "HalCommon.h"

namespace Hal
{

class TimerInterruptHandler
{
public:
	static constexpr uint8_t MaxTimerInterruptCount = 2;

	~TimerInterruptHandler();
	TimerInterruptHandler();
	class Callback
	{
	public:
		Hal::Preemption Preemption = Hal::Preemption::Tick;
		TimerInterruptHandler *InterruptHandlerProcessing;
		uint32_t Frequency;
		bool AutoReload;
		virtual void InterruptCallback() = 0;
	};
	bool SetFrequency(TimerInterruptHandler::Callback *handler);
	void SetCallback(Callback *callback);
	static void IRAM_ATTR CallbackHandler(void *arg);
	void Enable(Hal::Preemption preemption);
	void Disable(Hal::Preemption preemption);
	void InterruptProcessor(Hal::Preemption preemption);
	uint32_t GetLoad(Preemption preemption);
	void SetLoad(Preemption preemption, uint32_t value);
	static constexpr uint8_t TimerDivider = 16;								  //  Hardware timer clock divider
	static constexpr uint32_t CountsPerSecond = TimeBaseClock / TimerDivider; // convert counter value to seconds

private:
	TimerInterruptHandler::Callback *callbackList[MaxTimerInterruptCount];
};
} // namespace Hal
#endif /* INCLUDE_HAL_INTERRUPTHANDLER_H_ */
