#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <chrono>
#include <UCodeRunTime/RunTimeBasicTypes/IntTypes.hpp>
#define TimeStart CoreStart namespace Time{
#define TimeEnd CoreEnd }

TimeStart
		
using clock = std::chrono::high_resolution_clock;
typedef std::chrono::duration<float> fsec;
typedef std::chrono::milliseconds ms;
typedef clock::time_point Time_point;
typedef unsigned char Frame;
class GameTime
{
private:
	static const SInt32 MaxFixedUpdatePerSec = 20;
public:
	static constexpr float32 FixedTimeDelta = float(1.0f / MaxFixedUpdatePerSec);
	float32 UpateDelta;
	Frame FramesToDestroy;//The Time it takes for a libraryBook,Entity,Scene and Compoent to be freed in memory
	constexpr GameTime() :UpateDelta(0), FramesToDestroy(1)
	{

	}
};

	
TimeEnd