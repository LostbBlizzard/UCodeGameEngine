#pragma once

#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/UDefs.hpp>
CoreStart
struct Vec2
{
public:
	float32 X, Y;
	constexpr Vec2() :X(0), Y(0) {};
	constexpr Vec2(float32 x, float32 y) : X(x), Y(y) { };

	constexpr Vec2 operator+(const Vec2 other)const
	{
		return Vec2(X + other.X, Y + other.Y);
	}
	constexpr Vec2 operator-(const Vec2 other)const
	{
		return Vec2(X - other.X, Y - other.Y);
	}
	constexpr Vec2 operator/(const float32 other)const
	{
		return Vec2(X / other, Y / other);
	}
	constexpr Vec2 operator*(const float32 other)const
	{
		return Vec2(X * other, Y * other);
	}
	constexpr void operator+=(Vec2 other)
	{
		X += other.X;
		Y += other.Y;
	}
	constexpr void operator-=(Vec2 other)
	{
		X -= other.X;
		Y -= other.Y;
	}
	constexpr bool operator==(const Vec2 other)const
	{
		return (X == other.X && Y == other.Y);
	}
	constexpr bool operator!=(const Vec2 other)const
	{
		return !(X == other.X && Y == other.Y);
	}

	Vec2 GetRotated(float32 radians) const;
	
	float32 Get_Magnitude();
	static float32 Distance(const Vec2& a, const Vec2& b)
	{
		return (a - b).Get_Magnitude();
	}
	constexpr static Vec2 Lerp(const Vec2& a, const Vec2& b, float32 t)
	{
		return a + (b - a) * t;
	}

	Vec2 Get_Normalize();
	constexpr static Vec2 One() { return Vec2(1, 1); }
	constexpr static Vec2 Zero() { return Vec2(0, 0); }

	

	/*
	constexpr inline explicit operator Vec2Int() const
	{
		return Vec2Int((int)X, (int)Y);
	}
	*/
};
struct Vec3
{
	float32 X, Y, Z;
	constexpr Vec3():X(0), Y(0), Z(0){}
	constexpr Vec3(float32 x, float32 y, float32 z) :X(x), Y(y), Z(z){}
	constexpr Vec3(float32 x, float32 y): X(x), Y(y), Z(0){}
	constexpr Vec3(Vec2 vector) : X(vector.X), Y(vector.Y), Z(0) {}

	constexpr static Vec3 One() { return Vec3(1, 1, 1); }
	constexpr static Vec3 Zero() { return Vec3(0, 0, 0); }

	constexpr Vec3 operator+(const Vec3 other)const
	{
		return Vec3(X + other.X, Y + other.Y, Z + other.Z);
	}
	constexpr Vec3 operator-(const Vec3 other)const
	{
		return Vec3(X - other.X, Y - other.Y, Z - other.Z);
	}
	constexpr Vec3 operator/(const float32 other)const
	{
		return Vec3(X / other, Y / other, Z / other);
	}
	constexpr Vec3 operator*(const float32 other)const
	{
		return Vec3(X * other, Y * other, Z * other);
	}
	constexpr void operator+=(Vec3 other)
	{
		X += other.X;
		Y += other.Y;
		Z += other.Z;
	}
	constexpr void operator-=(Vec3 other)
	{
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
	}
	constexpr bool operator==(const Vec3 other)const
	{
		return (X == other.X && Y == other.Y && Z == other.Z);
	}
	constexpr bool operator!=(const Vec3 other)const
	{
		return !(X == other.X && Y == other.Y && Z == other.Z);
	}

	float32 Get_Magnitude();
	Vec3 Normalize();
	static float32 Distance(const Vec3& a, const Vec3& b)
	{
		return (a - b).Get_Magnitude();
	}
	constexpr static Vec3 Lerp(const Vec3& a, const Vec3& b, float32 t)
	{
		return a + (b - a) * t;
	}
};
struct Vec2i
{
	SInt32 X, Y;
	constexpr Vec2i() :X(0), Y(0){}
	constexpr Vec2i(SInt32 x, SInt32 y) :X(x), Y(y){}

	constexpr Vec2i operator+(const Vec2i other) const
	{
		return Vec2i(X + other.X, Y + other.Y);
	}
	constexpr Vec2i operator-(const Vec2i other) const
	{
		return Vec2i(X - other.X, Y - other.Y);
	}
	constexpr Vec2i operator/(const SInt32 other) const
	{
		return Vec2i(X / other, Y / other);
	}
	constexpr Vec2i operator*(const SInt32 other) const
	{
		return Vec2i(X * other, Y * other);
	}


	constexpr void operator+=(Vec2i other)
	{
		X += other.X;
		Y += other.Y;
	}
	constexpr void operator-=(Vec2i other)
	{
		X -= other.X;
		Y -= other.Y;
	}
	constexpr bool operator==(const Vec2i other)const
	{
		return (X == other.X && Y == other.Y);
	}
	constexpr bool operator!=(const Vec2i other)const
	{
		return !(X == other.X && Y == other.Y);
	}

	float32 Get_Magnitude();
	static float32 Distance(const Vec2i& a, const Vec2i& b)
	{
		return (a - b).Get_Magnitude();
	}



	constexpr explicit operator Vec2() const
	{
		return Vec2((float32)X, (float32)Y);
	}
	




	constexpr static Vec2i One() { return Vec2i(1, 1); }
	constexpr static Vec2i Zero() { return Vec2i(0, 0); }
};
struct Vec3i
{
	SInt32 X, Y, Z;
	constexpr Vec3i() : X(0), Y(0), Z(0) {}
	constexpr Vec3i(SInt32 x, SInt32 y, SInt32 z) :X(x), Y(y), Z(z) {}
	constexpr Vec3i operator+(const Vec3i& other) const
	{
		return Vec3i(X + other.X, Y + other.Y, Z + other.Z);
	}
	constexpr Vec3i operator-(const Vec3i& other) const
	{
		return Vec3i(X - other.X, Y - other.Y, Z - other.Z);
	}
	constexpr Vec3i operator/(const SInt32& other) const
	{
		return Vec3i(X / other, Y / other, Z / other);
	}
	constexpr Vec3i operator*(const SInt32& other) const
	{
		return Vec3i(X * other, Y * other, Z * other);
	}
	constexpr void operator+=(Vec3i other)
	{
		X += other.X;
		Y += other.Y;
		Z += other.Z;
	}
	constexpr void operator-=(Vec3i other)
	{
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
	}

	constexpr bool operator==(const Vec3i other)const
	{
		return (X == other.X && Y == other.Y && Z == other.Z);
	}
	constexpr bool operator!=(const Vec3i other)const
	{
		return !(X == other.X && Y == other.Y && Z == other.Z);
	}

	constexpr explicit operator Vec3() const
	{
		return Vec3((float32)X, (float32)Y,(float32)Z);
	}

	constexpr static Vec3i One() { return Vec3i(1, 1, 1); }
	constexpr static Vec3i Zero() { return Vec3i(0, 0, 0); }

	float32 Get_Magnitude();
	static float32 Distance(const Vec3i& a, const Vec3i& b)
	{
		return (a - b).Get_Magnitude();
	}

};
CoreEnd
