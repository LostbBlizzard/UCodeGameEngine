#pragma once
#include "UCodeRunTime/Core/CoreNamespace.hpp"
CoreStart

class RunTimeInfo
{
	enum class Version
	{
		Null,
		_0_0_1,
	};

	static constexpr Version CurrentVersion = Version::_0_0_1;
	static constexpr const char* VersionString = "0.0.1";
};

CoreEnd