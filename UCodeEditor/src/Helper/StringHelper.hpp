#pragma once
#include "Editor/EditorNamespace.hpp"
#include "Typedefs.hpp"
EditorStart
class StringHelper
{
public:
	
	static bool StartsWith(StringView Val,StringView Match);
	static bool StartsWith(const String& Val,StringView Match)
	{
		return StartsWith((StringView)Val,Match);
	}


	static bool Fllter(const String& filter, const String& ToCheck);

	static bool Replace(String& str, const String& from, const String& to);
};
EditorEnd
