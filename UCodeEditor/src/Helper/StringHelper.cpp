#include "StringHelper.hpp"


EditorStart

void StringHelper::ToUpper(StringView Base, String& Out)
{
	Out.clear();
	for (auto& Item : Base)
	{
		Out.push_back(std::toupper(Item));
	}
}	

bool  StringHelper::StartsWith(StringView Val, StringView Match)
{
	if (Val.size() < Match.size())
	{
		return false;
	}
	for (size_t i = 0; i < Match.size(); i++)
	{
		if (Match[i] != Val[i])
		{
			return false;
		}
	}
	return true;
}
bool StringHelper::Fllter(const String& filter, const String& ToCheck)
{
	if (filter.size())
	{
		String filterLoW = ToUpper(filter);
		String ToCheckLow = ToUpper(ToCheck);

		if (Contains<char>(ToCheckLow,filterLoW)){return true;}


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