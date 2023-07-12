#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <inttypes.h>
CoreStart


typedef unsigned char UInt8;
typedef unsigned short UInt16;
typedef unsigned int UInt32;
typedef unsigned long long UInt64;


typedef signed char SInt8;
typedef signed short SInt16;
typedef signed int SInt32;
typedef signed long long SInt64;

typedef float float32;
typedef double float64;

//For Clarity reasons
typedef UInt8 Byte;


static_assert(sizeof(Byte) == 1, " 'Byte' is not 1 bytes");
static_assert(sizeof(UInt16) == 2, " 'UInt16' is not 2 bytes");
static_assert(sizeof(UInt32) == 4, " 'UInt32' is not 4 bytes");
static_assert(sizeof(UInt64) == 8, " 'UInt64' is not 8 bytes");

static_assert(sizeof(SInt8) == 1, " 'SInt8' is not 1 bytes");
static_assert(sizeof(SInt16) == 2, " 'SInt16' is not 2 bytes");
static_assert(sizeof(SInt32) == 4, " 'SInt32' is not 4 bytes");
static_assert(sizeof(SInt64) == 8, " 'SInt64' is not 8 bytes");

static_assert(sizeof(float32) == 4, " 'float32' is not 4 bytes");
static_assert(sizeof(float64) == 8, " 'float64' is not 8 bytes");

constexpr UInt8 UInt8MinValue = 0x00;
constexpr UInt8 UInt8MaxValue = 0xff;

constexpr UInt16 UInt16MinValue = 0x00;
constexpr UInt16 UInt16MaxValue = 0xffff;

constexpr UInt32 UInt32MinValue = 0x00;
constexpr UInt32 UInt32MaxValue = 0xffffffff;

constexpr UInt64 UInt64MinValue = 0x00;
constexpr UInt64 UInt64MaxValue = 0xffffffffffff;

constexpr SInt8 SInt8MinValue = -128;
constexpr SInt8 SInt8MaxValue = 127;

constexpr SInt16 SInt16MinValue = -32767;
constexpr SInt16 SInt16MaxValue = 32767;

constexpr SInt32 SInt32MinValue = -2147483647;
constexpr SInt32 SInt32MaxValue = 2147483647;

constexpr SInt64 SInt64MinValue = -9223372036854775806;
constexpr SInt64 SInt64MaxValue = 9223372036854775807;

CoreEnd