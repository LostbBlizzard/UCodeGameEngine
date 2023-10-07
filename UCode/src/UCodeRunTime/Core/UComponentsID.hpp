#pragma once
#include "CoreNamespace.hpp"
#include "UCodeRunTime/BasicTypes.hpp"
CoreStart

using UComponentsID = String;

class Compoent;
class Entity;

using NewComponentFunc = Compoent*(*)(Entity*);
struct UComponentData
{
	UComponentsID _Type;
	NewComponentFunc _NewComponent = nullptr;
	UComponentData()
	{

	}
	UComponentData(const UComponentsID& Type, NewComponentFunc Component)
	{
		_Type = Type;
		_NewComponent = Component;
	}
};
CoreEnd