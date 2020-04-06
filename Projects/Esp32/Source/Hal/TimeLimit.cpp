
#include "TimeLimit.h"
#include "Hardware.h"

namespace Hal
{

TimeLimit::TimeLimit()
	: startTicks(Hardware::Instance()->Milliseconds())
{
}

void TimeLimit::Reset()
{
	startTicks = Hardware::Instance()->Milliseconds();
}

bool TimeLimit::IsTimeUp(uint32_t time) const
{
	return (Hardware::Instance()->Milliseconds() - startTicks) >= time;
}

uint32_t TimeLimit::ElapsedTime() const
{
	return (Hardware::Instance()->Milliseconds() - startTicks);
}

} // namespace Hal
