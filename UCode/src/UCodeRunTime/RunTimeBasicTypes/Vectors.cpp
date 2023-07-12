#include "Vectors.hpp"
#include <cmath>

CoreStart




Vec2 Vec2::GetRotated(float32 radians) const
{
	return { X * std::cos(radians) - Y * std::sin(radians),
			 X * std::sin(radians) + Y * std::cos(radians) };
}



float32 Vec2::Get_Magnitude()
{
	return sqrtf(X * X + Y * Y);
}

Vec2 Vec2::Get_Normalize()
{
	float32 m = Get_Magnitude();
	if (m == 0)
	{
		return { X,Y };
	}
	else
	{
		return { X / m,Y / m };
	}
}

float32 Vec3::Get_Magnitude()
{
	return sqrtf(X * X + Y * Y + Z + Z);
}
Vec3 Vec3::Normalize()
{
	float32 m = Get_Magnitude();
	if (m == 0)
	{
		return { X,Y };
	}
	else
	{
		return { X / m,Y / m };
	}
}
float32 Vec2i::Get_Magnitude()
{
	float32 _x = (float32)X;
	float32 _Y = (float32)Y;
	return sqrtf(_x * _x + _Y * _Y);
}
float32 Vec3i::Get_Magnitude()
{
	float32 _x = (float32)X;
	float32 _Y = (float32)Y;
	float32 _z = (float32)Z;

	return sqrtf(_x * _x + _Y * _Y + _z + _z);
}
CoreEnd


