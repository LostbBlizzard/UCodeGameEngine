#pragma once
#include "PhysicsNamespace.hpp"
#include "UCodeRunTime/BasicTypes.hpp"
#include <box2d/box2d.h>
PhysicsStart

UCodeGEForceinlne b2Vec2 To(const UCode::Vec2& Item)
{
	return *(b2Vec2*)&Item;
}
UCodeGEForceinlne UCode::Vec2 To(const b2Vec2& Item)
{
	return *(UCode::Vec2*)&Item;
}

PhysicsEnd