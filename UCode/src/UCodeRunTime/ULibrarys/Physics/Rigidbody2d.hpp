#pragma once
#include <UCodeRunTime/includeForCompoent.hpp>
#include "PhysicsRunTime2d.hpp"

PhysicsStart
class Rigidbody2d final :public PhysicsObject2d
{
private:
	void FixedUpdate() override;
public:
	Rigidbody2d(Entity* p);
	~Rigidbody2d();

	Vec2 Velocity;//Move To Compoent
private:
	static UComponentData type_Data;

	b2BodyDef _bodyDef;
	b2Body* _body=nullptr;

	//
	Vec2 LastPhysicPos;
};
PhysicsEnd
