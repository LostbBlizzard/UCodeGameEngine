#include "Rigidbody2d.hpp"
PhysicsStart
UComponentData Rigidbody2d::type_Data = { "Rigidbody2d",[](Entity* E) {return (Compoent*)new Rigidbody2d(E); } };
Rigidbody2d::Rigidbody2d(Entity* p) : PhysicsObject2d(p, &type_Data), Velocity(Vec2::Zero())
{
	auto World = GetPhysicsWorld();

	_bodyDef.type = b2BodyType::b2_dynamicBody;
	_body = World->CreateBody(&_bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	_body->CreateFixture(&fixtureDef);
}
Rigidbody2d::~Rigidbody2d()
{
	auto World = GetPhysicsWorld();
	World->DestroyBody(_body);

	_body = nullptr;
}
void Rigidbody2d::FixedUpdate()
{

	if (LastPhysicPos != NativeEntity()->WorldPosition2D())
	{
		_body->SetTransform(To(NativeEntity()->WorldPosition2D()), _body->GetAngle());
	}

	NativeEntity()->WorldPosition(To(_body->GetPosition()));
	LastPhysicPos = To(_body->GetPosition());
}
PhysicsEnd