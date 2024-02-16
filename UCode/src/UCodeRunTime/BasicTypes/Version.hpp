#pragma once
#include "UCodeRunTime/Core/CoreNamespace.hpp"
#include "../BasicTypes.hpp"
CoreStart

struct Version
{
	int Major = 0;
	int Minor = 0;
	int Patch = 0;

	constexpr Version(int major,int minor,int patch)
		:Major(major),Minor(minor),Patch(patch)
	{
	}

	static constexpr Version CurrentUCodeVersion()
	{
		return Version(UCodeGEMajor, UCodeGEMinor, UCodeGEPatch);
	}


	bool operator==(const Version& other) const
	{
		return Major == other.Major && Minor == other.Minor && Patch == other.Patch;
	}
	bool operator!=(const Version& other) const
	{
		return !this->operator==(other);
	}
	
	bool operator>(const Version& other) const
	{
		if (Major > other.Major)
		{
			return true;
		}
		if (Minor > other.Minor)
		{
			return true;
		}	
		if (Patch > other.Patch)
		{
			return true;
		}
	
		return false;
	}

	bool operator<(const Version& other) const
	{
		if (Major < other.Major)
		{
			return true;
		}
		if (Minor < other.Minor)
		{
			return true;
		}	
		if (Patch < other.Patch)
		{
			return true;
		}
	
		return false;
	}

	bool operator<=(const Version& other) const
	{
		return this->operator==(other) || this->operator<(other);
	}
	bool operator>=(const Version& other) const
	{
		return this->operator==(other) || this->operator>(other);
	}

	String ToString() const
	{
		return std::to_string(Major) + "." + std::to_string(Minor) + "." + std::to_string(Patch);
	}

	static Optional<Version> Parse(const StringView str);
};

CoreEnd
