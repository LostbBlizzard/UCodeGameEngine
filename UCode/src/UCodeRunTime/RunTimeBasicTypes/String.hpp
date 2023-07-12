#pragma once

#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <string>
#include <string_view>
#include <filesystem>

#include <xhash>
CoreStart
typedef std::string String;
typedef std::string_view StringView;
typedef std::filesystem::path Path;

#ifdef UCode_Build_Windows_OS
#define ToPathChar(X) L##X
#define ToPathChar2(X) L#X

#else
#define ToPathChar(X)
#define ToPathChar2(X) L#X
#endif // DEBUG


CoreEnd

namespace std 
{
	template <>
	struct hash<UCode::Path>
	{
		std::size_t operator()(const UCode::Path& k) const
		{
			std::hash<UCode::Path::string_type> V;
			return V(k.native());
		}
	};

}
