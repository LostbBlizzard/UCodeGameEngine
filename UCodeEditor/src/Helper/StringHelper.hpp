#pragma once
#include "Editor/EditorNamespace.hpp"
#include "Typedefs.hpp"
#include "UCodeRunTime/ULibrarys/Others/StringHelper.hpp"
EditorStart
class StringHelper
{
public:
	static void ToUpper(StringView Base, String& Out);
	static String ToUpper(StringView Base)
	{
		String r;
		ToUpper(Base,r);
		return r;
	}
	static bool StartsWith(StringView Val,StringView Match);
	static bool StartsWith(const String& Val,StringView Match)
	{
		return StartsWith((StringView)Val,Match);
	}



	//Update Flliter to use Fuzzy find
	static bool Fllter(const String& filter, const String& ToCheck);

	//Update Flliter to use Fuzzy find
	template<typename T> 
	static void Fllter(const String& filter, Span<T>& list, std::function<String(const T&)> onitem)
	{

	}


	static bool Replace(String& str, const String& from, const String& to);

	static Vector<StringView> Split(const StringView Base, const StringView spliter)
	{
		return UCode::StringHelper::Split(Base, spliter);
	}
	static void Split(const StringView Base, const StringView spliter,Vector<StringView>& out)
	{	
		return UCode::StringHelper::Split(Base, spliter,out);
	}

	template<typename T>
	static bool Contains(const UCode::StringBase<T>& string, const UCode::StringViewBase<T> MatchString)
	{
		return  UCode::StringHelper::Contains<T>(string, MatchString);
	}
	
	template<typename T>
	static bool Contains(const UCode::StringViewBase<T> string, const UCode::StringViewBase<T> MatchString)
	{
		return  UCode::StringHelper::Contains<T>(string, MatchString);
	}
};
EditorEnd
