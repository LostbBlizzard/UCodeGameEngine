#pragma once
#include "IntTypes.hpp"

CoreStart
struct Version
{
	UInt64 Major;
	UInt64 Minor;
	UInt64 Revision;
	Version()
	{
		Major = 0;
		Minor = 0;
		Revision = 0;
	}
	Version(UInt64 major, UInt64 minor, UInt64 revision)
	{
		Major = major;
		Minor = minor;
		Revision = revision;
	}
};
CoreEnd