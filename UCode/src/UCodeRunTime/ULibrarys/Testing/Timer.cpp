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

float32 Timer::GetTimeInSec()
{
	auto diff = _TimerEnd - _STime;
	auto r = std::chrono::duration_cast<Time::fsec>(diff);
	return r.count();
}

SInt32 Timer::GetTimeInMS()
{
	return (SInt32)(GetTimeInSec() * 1000.0f);
}

std::string Timer::GetTimeAsString()
{
	float32 time = GetTimeInSec();

	if (time > 0.20f)
	{
		return "Sec:" + std::to_string(time);
	}
	else
	{
		SInt32 ms = (SInt32)(time * 1000.0f);

		if (ms > 5)
		{
			return  "Ms:" + std::to_string(ms);
		}
		else
		{
			SInt32 mms = (SInt32)(time * 1000.0f * 1000.0f);


		

			if (mms == 0) {
				SInt32 ns = (SInt32)(time * 1000.0f * 1000.0f * 1000.0f);
				return  "Ns:" + std::to_string(ns);
			}	
			
			return  "Mico Sec:" + std::to_string(mms);
		}
	
	}

	return std::string();
}
CoreEnd