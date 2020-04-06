
#ifndef INCLUDE_HAL_TIMER_H_
#define INCLUDE_HAL_TIMER_H_

#include <TimerInterruptHandler.h>
#include "HalCommon.h"

namespace Hal
{

class Timer : TimerInterruptHandler::Callback
{
public:
	static constexpr uint8_t MaxTimerCallBack = 10;
	~Timer();
	Timer(TimerInterruptHandler *interruptHandler, TimerSelect timerSelected);

	void SetTimer(uint32_t frequency);
	void Start();
	void Stop();
	void Initlialize();

	class Callback
	{
	public:
		Callback(){};
		~Callback(){};
		virtual void TimerCallback() = 0;
	};

	bool AddCallback(Timer::Callback *handler);

protected:
	static void InterruptHandler(void *arg);
	void InterruptCallback();

private:
	TimerInterruptHandler *interruptHandler;
	Timer::Callback *callbackList[MaxTimerCallBack];

private:
	/// @brief	Hide Copy constructor.
	Timer(const Timer &) = delete;

	/// @brief	Hide Assignment operator.
	Timer &operator=(const Timer &) = delete;

	/// @brief	Hide Move constructor.
	Timer(Timer &&) = delete;

	/// @brief	Hide Move assignment operator.
	Timer &operator=(Timer &&) = delete;
};

} // namespace Hal

#endif /* INCLUDE_HAL_TIMER_H_ */
