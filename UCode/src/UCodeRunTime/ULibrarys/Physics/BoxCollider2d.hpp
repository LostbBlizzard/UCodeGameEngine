#pragma once
#include <UCodeRunTime/includeForCompoent.hpp>
#include "PhysicsRunTime2d.hpp"

PhysicsStart

class BoxCollider2d final :public PhysicsObject2d
{
public:
	BoxCollider2d(Entity* p); 
	~BoxCollider2d();

private:
	static UComponentData type_Data;
	b2PolygonShape _Shape;


	b2BodyDef groundBodyDef;
	b2Body* groundBody = nullptr;
};

PhysicsEnd