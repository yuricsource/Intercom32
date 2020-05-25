
#include "TimerInterruptHandler.h"
#include "driver/timer.h"
// #include "hal/timer_types.h"

#include <stdio.h>
#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"

namespace Hal
{

	TimerInterruptHandler::TimerInterruptHandler()
	{

		for (uint8_t i = 0; i < MaxTimerInterruptCount; i++)
		{
			callbackList[i] = nullptr;
		}
	}

	TimerInterruptHandler::~TimerInterruptHandler()
	{
	}

	uint32_t TimerInterruptHandler::GetLoad(Preemption preemption)
	{
		uint64_t timerValue = 0;
		if (preemption == Preemption::TIMER0)
			timerValue = timer_get_counter_value(TIMER_GROUP_0, TIMER_0, &timerValue);
		else if (preemption == Preemption::TIMER1)
			timerValue = timer_get_counter_value(TIMER_GROUP_1, TIMER_1, &timerValue);

		return timerValue;
	}

	void TimerInterruptHandler::SetLoad(Preemption preemption, uint32_t value)
	{
		if (preemption == Preemption::TIMER0)
			timer_set_counter_value(TIMER_GROUP_0, TIMER_0, value);
		else if (preemption == Preemption::TIMER1)
			timer_set_counter_value(TIMER_GROUP_1, TIMER_1, value);
	}

	bool TimerInterruptHandler::SetFrequency(TimerInterruptHandler::Callback *handler)
	{
		if (handler->Preemption == Hal::Preemption::TIMER0 ||
			handler->Preemption == Hal::Preemption::TIMER1)
		{
			TimerSelect timer = (handler->Preemption == Preemption::TIMER0) ? TimerSelect::Timer0 : TimerSelect::Timer1;

			callbackList[static_cast<uint8_t>(timer)]->Frequency = handler->Frequency;
		}
		else
			return false;

		return true;
	}

	void IRAM_ATTR TimerInterruptHandler::CallbackHandler(void *arg)
	{
		TimerInterruptHandler::Callback *handler = (TimerInterruptHandler::Callback *)arg;

		if (handler->Frequency == 0)
			return;

		timer_idx_t timer;

		if (handler->Preemption == Preemption::TIMER0)
			timer = TIMER_0;
		else if (handler->Preemption == Preemption::TIMER1)
			timer = TIMER_1;
		else
			return;

		if (handler->AutoReload == false)
		{
			// uint64_t timer_counter_value = timer_group_get_counter_value_in_isr(TIMER_GROUP_0, timer);
			// timer_group_intr_clr_in_isr(TIMER_GROUP_0, timer);
			// timer_counter_value += (uint64_t)(CountsPerSecond / handler->Frequency);
			// timer_group_set_alarm_value_in_isr(TIMER_GROUP_0, timer, timer_counter_value);
			uint32_t intr_status = TIMERG0.int_st_timers.val;
			TIMERG0.hw_timer[timer].update = 1;
			if (intr_status & BIT(timer))
			{
				uint64_t timer_counter_value = ((uint64_t)TIMERG0.hw_timer[timer].cnt_high) << 32 | TIMERG0.hw_timer[timer].cnt_low;
				TIMERG0.int_clr_timers.t0 = 1;
				timer_counter_value += (uint64_t)(CountsPerSecond / handler->Frequency);
				TIMERG0.hw_timer[timer].alarm_high = (uint32_t)(timer_counter_value >> 32);
				TIMERG0.hw_timer[timer].alarm_low = (uint32_t)timer_counter_value;
			}
		}
		else
			TIMERG0.int_clr_timers.t1 = 1;
		// timer_group_intr_clr_in_isr(TIMER_GROUP_0, timer);

		// timer_group_enable_alarm_in_isr(TIMER_GROUP_0, timer);

		// TIMERG0.hw_timer[timer_idx].config.alarm_en = TIMER_ALARM_EN;

		TIMERG0.hw_timer[timer].config.alarm_en = TIMER_ALARM_EN;

		if (handler->InterruptHandlerProcessing != nullptr)
			handler->InterruptHandlerProcessing->InterruptProcessor(handler->Preemption);
	}

	void TimerInterruptHandler::SetCallback(TimerInterruptHandler::Callback *handler)
	{
		if (handler == nullptr &&
			(handler->Preemption != Preemption::TIMER0 || handler->Preemption != Preemption::TIMER1))
		{
			return;
		}

		uint8_t interruptIndex = (handler->Preemption == Preemption::TIMER0) ? 0 : 1;

		if (callbackList[interruptIndex] != nullptr)
		{
			return;
		}
		timer_idx_t timer;
		timer_config_t config;
		handler->InterruptHandlerProcessing = this;

		if (handler->Frequency == 0)
			return;

		if (handler->Preemption == Preemption::TIMER0)
			timer = TIMER_0;
		else if (handler->Preemption == Preemption::TIMER1)
			timer = TIMER_1;
		else
			return;

		// Configuring the TIMER
		config.divider = TimerDivider;
		config.counter_dir = TIMER_COUNT_UP;
		config.counter_en = TIMER_PAUSE;
		config.alarm_en = TIMER_ALARM_EN;
		config.intr_type = TIMER_INTR_LEVEL;
		config.auto_reload = (timer_autoreload_t)handler->AutoReload;
		timer_init(TIMER_GROUP_0, timer, &config);

		timer_set_counter_value(TIMER_GROUP_0, timer, 0);

		timer_set_alarm_value(TIMER_GROUP_0, timer, CountsPerSecond / handler->Frequency);

		timer_isr_register(TIMER_GROUP_0, timer, CallbackHandler,
						   (void *)handler, ESP_INTR_FLAG_IRAM, NULL);

		callbackList[interruptIndex] = handler;
	}

	void TimerInterruptHandler::InterruptProcessor(Hal::Preemption preemption)
	{
		uint8_t interruptIndex = (preemption == Preemption::TIMER0) ? 0 : 1;

		if (callbackList[interruptIndex] != nullptr)
		{
			callbackList[interruptIndex]->InterruptCallback();
		}
	}

	void TimerInterruptHandler::Enable(Hal::Preemption preemption)
	{
		switch (preemption)
		{
		case Hal::Preemption::TIMER0:
			timer_enable_intr(TIMER_GROUP_0, TIMER_0);
			timer_start(TIMER_GROUP_0, TIMER_0);
			break;
		case Hal::Preemption::TIMER1:
			timer_enable_intr(TIMER_GROUP_0, TIMER_1);
			timer_start(TIMER_GROUP_0, TIMER_1);
			break;
		default:
			break;
		}
	}

	void TimerInterruptHandler::Disable(Hal::Preemption preemption)
	{
		switch (preemption)
		{
		case Hal::Preemption::TIMER0:
			timer_disable_intr(TIMER_GROUP_0, TIMER_0);
			timer_pause(TIMER_GROUP_0, TIMER_0);
			break;
		case Hal::Preemption::TIMER1:
			timer_disable_intr(TIMER_GROUP_0, TIMER_1);
			timer_pause(TIMER_GROUP_0, TIMER_1);
			break;
		default:
			break;
		}
	}

} // namespace Hal
