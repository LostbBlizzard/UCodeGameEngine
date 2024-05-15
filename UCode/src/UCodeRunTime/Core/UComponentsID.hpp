#pragma once
#include "CoreNamespace.hpp"
#include "UCodeRunTime/BasicTypes.hpp"
#include "rttr/registration.h"
CoreStart

using UComponentsID = String;

class Compoent;
class Entity;

using NewComponentFunc = Compoent*(*)(Entity*);
struct UComponentData
{
	UComponentsID _Type;
	NewComponentFunc _NewComponent = nullptr;
	Optional<rttr::type> _RuntimeTypeReflection;
	UComponentData()
	{

	}
	UComponentData(const UComponentsID& Type, NewComponentFunc Component)
	{
		_Type = Type;
		_NewComponent = Component;
	}
	UComponentData(const UComponentsID& Type, NewComponentFunc Component,rttr::type _runtime)
		: _RuntimeTypeReflection(_runtime)
	{
		_Type = Type;
		_NewComponent = Component;
	}
	
};
CoreEnd