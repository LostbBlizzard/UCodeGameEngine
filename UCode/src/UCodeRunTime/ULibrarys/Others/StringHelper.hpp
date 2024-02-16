#pragma once
#include "UCodeRunTime/BasicTypes.hpp"

CoreStart
class StringHelper
{
public:

	static Vector<StringView> Split(const StringView Base, const StringView spliter)
	{
		Vector<StringView> r;
		Split(Base, spliter, r);
		return r;
	}
	static void Split(const StringView Base, const StringView spliter, Vector<StringView>& out)
	{
		out.clear();

		size_t startindex = 0;
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
					StringView n = Base.substr(startindex, i - splitstart);

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

		if (out.size())
		{
			StringView n = Base.substr(splitstart, Base.size() - splitstart);

			out.push_back(n);

		}
	}
};
CoreEnd