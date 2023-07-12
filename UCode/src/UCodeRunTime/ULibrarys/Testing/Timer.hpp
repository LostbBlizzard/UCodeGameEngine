#pragma once
#include "UCodeRunTime/Core/GameTime.hpp"
#include <string>
#include <UCodeRunTime/RunTimeBasicTypes/IntTypes.hpp>
CoreStart
class Timer
{
public:
	Timer();

	void EndTime();

	float32 GetTimeInSec();
	SInt32 GetTimeInMS();
	std::string GetTimeAsString();
private:
	Time::clock::time_point _STime,_TimerEnd;

};
CoreEnd

