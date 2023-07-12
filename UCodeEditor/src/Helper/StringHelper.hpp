#pragma once
#include "Editor/EditorNamespace.hpp"
#include "Typedefs.hpp"
EditorStart
class StringHelper
{
public:

	static bool Fllter(const String& filter, const String& ToCheck);

	static bool Replace(String& str, const String& from, const String& to);
};
EditorEnd
