#pragma once
#include "PhysicsNamespace.hpp"
#include "UCodeRunTime/RunTimeBasicTypes.hpp"
#include <box2d/box2d.h>
PhysicsStart

UCODE_ENGINE_FORCE_INLINE b2Vec2 To(const UCode::Vec2& Item)
{
	return *(b2Vec2*)&Item;
}
UCODE_ENGINE_FORCE_INLINE UCode::Vec2 To(const b2Vec2& Item)
{
	return *(UCode::Vec2*)&Item;
}

PhysicsEnd