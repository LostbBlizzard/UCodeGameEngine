#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/IntTypes.hpp>
#include <cmath>
CoreStart
class Math
{
public:
	static const float32 PI;
	static const float32 Rad2Deg;
	static const float32 Deg2Rad;

	inline static float32 Lerp(float32 a, float32 b, float32 f)
	{
		return a + f * (b - a);
	}
	inline static float32 Sin(float32 Radians)
	{
		return sinf(Radians);
	}
	inline static float32 Cos(float32 Radians)
	{
		return cosf(Radians);
	}
	inline static float32 Sqrt(float32 value)
	{
		return sqrtf(value);
	}
	inline static float32 Atan2(float32 y, float32 x)
	{
		return atan2f(y, x);
	}
};
CoreEnd