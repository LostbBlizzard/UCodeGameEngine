#include "StringHelper.hpp"

EditorStart
bool StringHelper::Fllter(const String& filter, const String& ToCheck)
{
	if (filter.size())
	{
		String filterLoW = filter;
		String ToCheckLow = ToCheck;

		if (ToCheckLow._Starts_with(filterLoW)){return true;}


		return false;
	}
	return true;
}
bool StringHelper::Replace(String& str, const String& from, const String& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}
EditorEnd