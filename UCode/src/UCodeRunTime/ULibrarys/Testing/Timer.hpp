#pragma once
#include "UCodeRunTime/Core/GameTime.hpp"
#include <string>
#include <UCodeRunTime/BasicTypes.hpp>
CoreStart
class Timer
{
public:
	Timer();

	void EndTime();

	f32 GetTimeInSec();
	i32 GetTimeInMS();
	std::string GetTimeAsString();
private:
	Time::clock::time_point _STime,_TimerEnd;

};
CoreEnd

