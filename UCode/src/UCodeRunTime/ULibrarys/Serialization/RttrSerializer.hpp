#pragma once
#include "UCodeRunTime/BasicTypes.hpp"
#include "rttr/registration.h"
#include "USerializer.hpp"
CoreStart

struct RttrSerializer
{
	static void Write(USerializer& serializer,const char* FieldName, rttr::variant& value);
	static void Read(UDeserializer& serializer,const char* FieldName, rttr::instance& value,const rttr::property& prp);
	static bool IsSame(rttr::type T,rttr::variant& A, rttr::variant& B);
};
CoreEnd
