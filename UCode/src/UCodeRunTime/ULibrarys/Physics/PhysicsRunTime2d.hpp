#pragma once
#include <UCodeRunTime/includeULibrayCompoent.hpp>
#include <UCodeRunTime/BasicTypes.hpp>
#include "PhysicsNamespace.hpp"
#include "PhysicsWord2D.hpp"
PhysicsStart

class PhysicsObject2d;
class PhysicsRunTime final :private Compoent
{
	friend PhysicsObject2d;
public:
	static PhysicsRunTime* GetPhysics(GameRunTime* e);
	static PhysicsRunTime* FindPhysics(const GameRunTime* e);

	//inline const auto& Get_Word3D() const { return  _Word3D; }
	inline const auto& Get_Word2D() const { return  _World2D; }

	//inline auto& Get_Word3D() { return  _Word3D; }
	inline auto& Get_Word2D() { return  _World2D; }
private:
	PhysicsRunTime(Entity* p);
	~PhysicsRunTime();
	void NewPhysicsObject(PhysicsObject2d* obj);
	void ReMovePhysicsObject(PhysicsObject2d* obj);

	
	static UComponentData type_Data;
	void FixedUpdate() override;
	

	b2World _World2D = To(Vec2(0,-10));

	Vector<PhysicsObject2d*> physicsObjecs;
	
};
class PhysicsObject2d :public Compoent
{
public:
	PhysicsObject2d(Entity* p, UComponentData* typedata);
	~PhysicsObject2d();
	//virtual void Physics_Step(){}
	inline const PhysicsRunTime* GetPhysics() const
	{
		return PhysicsRunTime::GetPhysics(GetGameRunTime());
	}
	inline PhysicsRunTime* GetPhysics() 
	{
		return PhysicsRunTime::GetPhysics(GetGameRunTime());
	}
	inline const auto* GetPhysicsWorld() const
	{
		return &GetPhysics()->Get_Word2D();
	}
	inline auto* GetPhysicsWorld()
	{
		return &GetPhysics()->Get_Word2D();
	}
};

PhysicsEnd