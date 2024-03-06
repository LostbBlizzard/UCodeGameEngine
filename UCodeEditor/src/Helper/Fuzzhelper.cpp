#include "Fuzzhelper.hpp"

#include "OtherLibrarys/rapidfuzz/fuzz.hpp"
#include "Helper/StringHelper.hpp"
EditorStart

float FuzzHelper::GetFuzzRotio(StringView Base, StringView Other)
{
	static String upbase;
	static String upOther;
	StringHelper::ToUpper(upbase);
	StringHelper::ToUpper(upOther);

	return rapidfuzz::fuzz::ratio(upbase, upOther);
}
EditorEnd
