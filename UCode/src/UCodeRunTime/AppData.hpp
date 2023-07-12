#pragma once
#include "UCodeRunTime/Core/CoreNamespace.hpp"
CoreStart 

struct AppData
{
	enum class BuildPlatform
	{
		Null_OS,
		Windows_OS
	};
	
	static constexpr BuildPlatform _BuildPlatform =
#ifdef UCode_Build_Windows_OS
		BuildPlatform::Windows_OS;
#else
		BuildPlatform::Null_OS;
#endif // UCode_Build_Windows_OS
};	
CoreEnd
