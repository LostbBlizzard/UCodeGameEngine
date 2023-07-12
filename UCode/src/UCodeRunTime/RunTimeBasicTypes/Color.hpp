#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include "UCodeRunTime/UDefs.hpp"
CoreStart
struct Color32;

struct Color
{
	float32 R, G, B, A;
	constexpr Color() :R(1), G(1), B(1), A(1){}
	constexpr Color(float32 r,float32 g,float32 b) :R(r), G(g), B(b), A(1) {}
	constexpr Color(float32 r,float32 g, float32 b,float32 a) :R(r), G(g), B(b), A(a) {}
	constexpr static Color White() { return Color(1, 1, 1, 1); }
	constexpr bool operator==(const Color other)const
	{
		return (R == other.R && G == other.G && B == other.B && A == other.A);
	}
	constexpr bool operator!=(const Color other)const
	{
		return !(R == other.R && G == other.G && B == other.B && A == other.A);
	}
	constexpr Color operator*(const Color &other)const
	{
		return Color(R * other.R, G * other.G, B * other.B, A * other.A);
	}
	
	constexpr explicit operator Color32() const;
};

struct Color32
{
	typedef unsigned char Byte;
	Byte R, G, B, A;
	constexpr Color32() :R(255), G(255), B(255), A(255) {}
	constexpr Color32(Byte r, Byte g, Byte b) :R(r), G(g), B(b), A(255) {}
	constexpr Color32(Byte r, Byte g, Byte b, Byte a) :R(r), G(g), B(b), A(a) {}

	constexpr bool operator==(const Color32 other)const
	{
		return (R == other.R && G == other.G && B == other.B && A == other.A);
	}
	constexpr bool operator!=(const Color32 other)const
	{
		return !(R == other.R && G == other.G && B == other.B && A == other.A);
	}

	constexpr explicit operator Color() const
	{
		return	{ 
			((float32)R / (float32)255),
			((float32)G / (float32)255),
			((float32)B / (float32)255),
			((float32)A / (float32)255)};
	}
	
};

constexpr Color::operator Color32() const
{
	return	{
			(Color32::Byte)((SInt32)(R * 255)),
			(Color32::Byte)((SInt32)(G * 255)),
			(Color32::Byte)((SInt32)(B * 255)),
			(Color32::Byte)((SInt32)(A * 255))
	};
}

struct Color24;
struct ColorRGB
{
	float32 R, G, B;
	constexpr ColorRGB() :R(1), G(1), B(1)  {}
	constexpr ColorRGB(float32 r, float32 g, float32 b) : R(r), G(g), B(b)  {}
	constexpr static ColorRGB White() { return ColorRGB(1, 1, 1); }
	constexpr bool operator==(const ColorRGB other)const
	{
		return (R == other.R && G == other.G && B == other.B);
	}
	constexpr bool operator!=(const ColorRGB other)const
	{
		return !(R == other.R && G == other.G && B == other.B);
	}
	constexpr Color operator*(const ColorRGB& other)const
	{
		return Color(R * other.R, G * other.G, B * other.B);
	}

	constexpr explicit operator Color24() const;
};

struct Color24
{
	typedef unsigned char Byte;
	Byte R, G, B;
	constexpr Color24() :R(255), G(255), B(255) {}
	constexpr Color24(Byte r, Byte g, Byte b) : R(r), G(g), B(b){}

	constexpr bool operator==(const Color32 other)const
	{
		return (R == other.R && G == other.G && B == other.B);
	}
	constexpr bool operator!=(const Color32 other)const
	{
		return !(R == other.R && G == other.G && B == other.B);
	}

	constexpr explicit operator ColorRGB() const
	{
		return	{
			((float32)R / (float32)255),
			((float32)G / (float32)255),
			((float32)B / (float32)255),
		};
	}

};
constexpr ColorRGB::operator Color24() const
{
	return	{
			(Color24::Byte)((SInt32)(R * 255)),
			(Color24::Byte)((SInt32)(G * 255)),
			(Color24::Byte)((SInt32)(B * 255))
	};
}

typedef Color ColorRGBA;
CoreEnd
