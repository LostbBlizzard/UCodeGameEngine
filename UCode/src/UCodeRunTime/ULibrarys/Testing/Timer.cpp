#include "Timer.hpp"
CoreStart
typedef Time::clock clock_this;
Timer::Timer()
{
	_STime = clock_this::now();
}

void Timer::EndTime()
{
	_TimerEnd = clock_this::now();
}

f32 Timer::GetTimeInSec()
{
	auto diff = _TimerEnd - _STime;
	auto r = std::chrono::duration_cast<Time::fsec>(diff);
	return r.count();
}

i32 Timer::GetTimeInMS()
{
	return (i32)(GetTimeInSec() * 1000.0f);
}

std::string Timer::GetTimeAsString()
{
	f32 time = GetTimeInSec();

	if (time > 0.20f)
	{
		return "Sec:" + std::to_string(time);
	}
	else
	{
		i32 ms = (i32)(time * 1000.0f);

		if (ms > 5)
		{
			return  "Ms:" + std::to_string(ms);
		}
		else
		{
			i32 mms = (i32)(time * 1000.0f * 1000.0f);


		

			if (mms == 0) {
				i32 ns = (i32)(time * 1000.0f * 1000.0f * 1000.0f);
				return  "Ns:" + std::to_string(ns);
			}	
			
			return  "Mico Sec:" + std::to_string(mms);
		}
	
	}

	return std::string();
}
CoreEnd