#pragma once
#include "UCodeRunTime/BasicTypes.hpp"
#include "rttr/registration.h"
#include "USerializer.hpp"
CoreStart

struct RttrSerializer
{
	
	static void Write(USerializer& serializer,const char* FieldName, rttr::variant& value,rttr::type FieldType);
	static void Write(USerializer& serializer,const char* FieldName, rttr::variant& value)
	{
		return Write(serializer, FieldName, value, value.get_type());
	}
	static void Read(UDeserializer& serializer,const char* FieldName, rttr::instance& value,const rttr::property& prp);
	static bool IsSame(rttr::type T,rttr::variant& A, rttr::variant& B);
};
CoreEnd
