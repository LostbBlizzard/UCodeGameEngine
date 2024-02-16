#include "Version.hpp"
#include "UCodeRunTime/ULibrarys/Others/StringHelper.hpp"

CoreStart


Optional<Version> Version::Parse(const StringView str)
{

	auto list = StringHelper::Split(str,".");

	if (list.size() == 3)
	{
		auto majora = String(list[0]);
		auto minora = String(list[1]);
		auto patcha = String(list[2]);

		return Version(std::stoi(majora), std::stoi(minora), std::stoi(patcha));
	}

	return Optional<Version>();
}

CoreEnd

