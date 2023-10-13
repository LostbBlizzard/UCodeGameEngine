#include "PhysicsRunTime2d.hpp"
PhysicsStart

UComponentData PhysicsRunTime::type_Data = { "PhysicsRunTime",nullptr};
PhysicsRunTime::PhysicsRunTime(Entity* p) : Compoent(p, &type_Data)
{
	
}
PhysicsRunTime::~PhysicsRunTime()
{
	for (size_t i = 0; i < physicsObjecs.size(); i++)
	{
		Compoent::Destroy(physicsObjecs[i]);
	}


	GetGameRunTime()->ReMoveStaticComponent((size_t)StaticRuntimeComponentsIndex::PhysicsRunTime);
	Entity::Destroy(GetMyEntity());
}
PhysicsRunTime* PhysicsRunTime::GetPhysics(GameRunTime* e)
{
	const auto index = (size_t)StaticRuntimeComponentsIndex::PhysicsRunTime;
	auto item = e->Get_StaticComponent(index);
	if (item == nullptr)
	{
		Entity* newe = e->NewEntityOnRunTimeScene();
		auto r = new PhysicsRunTime(newe);
		newe->MoveCompoent(r);
		e->SetStaticComponent(index, r);
		return r;
	}
	return (PhysicsRunTime*)item;
}
PhysicsRunTime* PhysicsRunTime::FindPhysics(const GameRunTime* e)
{
	const auto index = (size_t)StaticRuntimeComponentsIndex::PhysicsRunTime;
	auto item = e->Get_StaticComponent(index);
	return (PhysicsRunTime*)item;
}
void PhysicsRunTime::NewPhysicsObject(PhysicsObject2d* obj)
{
	physicsObjecs.push_back(obj);
}
void PhysicsRunTime::ReMovePhysicsObject(PhysicsObject2d* obj)
{
	for (auto it = physicsObjecs.begin(); it != physicsObjecs.end();)
	{
		auto Item = *it;
		if (Item == obj)
		{
			physicsObjecs.erase(it);
			break;
		}

	}
}
void PhysicsRunTime::FixedUpdate()
{
	_World2D.Step(GetGameRunTime()->Get_GameTime().FixedTimeDelta,6,2);
}
// PhysicsObject
PhysicsObject2d::PhysicsObject2d(Entity* p,UComponentData* typedata) : Compoent(p, typedata)
{
	GetPhysics()->NewPhysicsObject(this);
}

PhysicsObject2d::~PhysicsObject2d()
{
	GetPhysics()->ReMovePhysicsObject(this);
}

		
		PhysicsEnd

