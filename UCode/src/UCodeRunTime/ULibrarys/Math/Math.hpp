#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/IntTypes.hpp>
#include <cmath>
CoreStart
class Math
{
public:
	static const f32 PI;
	static const f32 Rad2Deg;
	static const f32 Deg2Rad;

	inline static f32 Lerp(f32 a, f32 b, f32 f)
	{
		return a + f * (b - a);
	}
	inline static f32 Sin(f32 Radians)
	{
		return sinf(Radians);
	}
	inline static f32 Cos(f32 Radians)
	{
		return cosf(Radians);
	}
	inline static f32 Sqrt(f32 value)
	{
		return sqrtf(value);
	}
	inline static f32 Atan2(f32 y, f32 x)
	{
		return atan2f(y, x);
	}
};
CoreEnd