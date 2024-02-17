#pragma once
#include "UCodeRunTime/BasicTypes.hpp"

CoreStart
class StringHelper
{
public:

	template<typename T>
	static bool StartWith(const StringViewBase<T> Base,const StringViewBase<T> ToMatch)
	{
		if (ToMatch.size() > Base.size()) { return false; }

		for (size_t i = 0; i < ToMatch.size(); i++)
		{
			if (Base[i] != ToMatch[i]) { return false; }
		}
		return true;
	}
	
	template<typename T>
	static bool EndWith(const StringViewBase<T> string, const StringViewBase<T>  MatchString)
	{
		if (string.size() < MatchString.size()) { return false; }
		
		for (size_t i = 0; i < MatchString.size(); i++)
		{
			if (string[string.size() -1 - i] != MatchString[MatchString.size()-1 - i])
			{
				return false;
				break;
			}
		}
		return true;
	}

	template<typename T>
	static Vector< StringViewBase<T> > Split_t(const StringViewBase<T> Base, const StringViewBase<T> spliter)
	{
		Vector< StringViewBase<T> > r;
		Split_t<T>(Base, spliter, r);
		return r;
	}
	template<typename T>
	static void Split_t(const StringViewBase<T> Base, const StringViewBase<T> spliter, Vector< StringViewBase<T> >& out)
	{
		out.clear();

		size_t splitstart = 0;
		size_t splitercount = 0;
		for (size_t i = 0; i < Base.size(); i++)
		{
			char basechar = Base[i];

			if (spliter[splitercount] == basechar)
			{
				splitercount++;

				if (splitercount == spliter.size())
				{
					StringViewBase<T> n = Base.substr(splitstart, i - splitstart);

					out.push_back(n);
					splitercount = 0;
					splitstart = i + 1;
				}
			}
			else
			{
				splitercount = 0;
			}
		}

		if (out.size() && !EndWith(Base,spliter))
		{
			StringViewBase<T> n = Base.substr(splitstart, Base.size() - splitstart);

			out.push_back(n);

		}
	}
	static void Split(const StringViewBase<char> Base, const StringViewBase<char> spliter, Vector< StringViewBase<char> >& out)
	{
		return Split_t<char>(Base, spliter, out);
	}
	static Vector< StringViewBase<char> > Split(const StringViewBase<char> Base, const StringViewBase<char> spliter)
	{
		return Split_t<char>(Base, spliter);
	}
	static void Split(const StringViewBase<utf8> Base, const StringViewBase<utf8> spliter, Vector< StringViewBase<utf8> >& out)
	{
		return Split_t<utf8>(Base, spliter, out);
	}
	static Vector< StringViewBase<utf8> > Split(const StringViewBase<utf8> Base, const StringViewBase<utf8> spliter)
	{
		return Split_t<utf8>(Base, spliter);
	}
	static void Split(const StringViewBase<utf16> Base, const StringViewBase<utf16> spliter, Vector< StringViewBase<utf16> >& out)
	{
		return Split_t<utf16>(Base, spliter, out);
	}
	static Vector< StringViewBase<utf16> > Split(const StringViewBase<utf16> Base, const StringViewBase<utf16> spliter)
	{
		return Split_t<utf16>(Base, spliter);
	}
};
CoreEnd