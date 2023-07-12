#include "BoxCollider2d.hpp"
PhysicsStart

UComponentData BoxCollider2d::type_Data = UComponentData("BoxCollider2d",[](Entity* E) {return (Compoent*)new BoxCollider2d(E); });
BoxCollider2d::BoxCollider2d(Entity* p) : PhysicsObject2d(p,&type_Data)
{
	auto world = GetPhysicsWorld();

	
	groundBodyDef.position.Set(0.0f, -10.0f);

	groundBody = world->CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);

	groundBody->CreateFixture(&groundBox, 0.0f);
}
BoxCollider2d::~BoxCollider2d()
{
	auto world = GetPhysicsWorld();
	world->DestroyBody(groundBody);

	groundBody = nullptr;
}

PhysicsEnd