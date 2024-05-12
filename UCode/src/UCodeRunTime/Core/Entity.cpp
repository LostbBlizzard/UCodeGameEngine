#include "Entity.hpp"
#include "GameRunTime.hpp"
CoreStart
	

Compoent::Compoent(Entity* entity,UComponentData* TypeData)
	: _Entity(entity), _TypeData(TypeData),
  _Managed(this)
{

}


bool Compoent::Get_IsActive_InRunTime() const
{
	return _IsActive && _Entity->GetActive();
}
GameRunTime* Compoent::GetGameRunTime() const { return _Entity->NativeGameRunTime(); }
RunTimeScene* Compoent::Get_Scene() const { return _Entity->NativeScene(); }

Compoent::~Compoent()
{
	_Managed.Set_Value(nullptr);
}
GameRunTime* Entity::NativeGameRunTime()
{
	return _Scene->Get_RunTime();
}

Entity::Entity(RunTimeScene* runtime)
	: _Scene(runtime), _Managed(this)
{

}
Entity::~Entity()
{
	_Managed.Set_Value(nullptr);
}
void Entity::RunTimeUpdate()
{
	for (size_t i = 0; i < _Compoents.size(); i++)
	{
		auto& Item = _Compoents[i];
		if (Item->Get_IsActive())
		{

			if (Item->_GameRunTimeHasCalledStart)
			{
				Item->Update();
			}
			else
			{
				Item->_GameRunTimeHasCalledStart = true;
				Item->Start();
			}
		}
	}

	for (size_t i = 0; i < _Entitys.size(); i++)
	{
		auto& Item = _Entitys[i];
		if (Item->GetActive())
		{
			Item->RunTimeUpdate();
		}
	}
}
void Entity::FixedUpdate()
{
	for (size_t i = 0; i < _Compoents.size(); i++)
	{
		auto& Item = _Compoents[i];
		if (Item->Get_IsActive()) { Item->FixedUpdate(); }
	}

	for (size_t i = 0; i < _Entitys.size(); i++)
	{
		auto& Item = _Entitys[i];
		if (Item->GetActive())
		{
			Item->FixedUpdate();
		}
	}
}
void Entity::DestroyNullCompoents()
{
	for (auto it = _Compoents.begin(); it != _Compoents.end();)
	{
		auto& Item = *it;

		if (Item->Get_IsDestroyed())
		{

			const Frame FramesToDestroy = NativeGameRunTime()->Get_GameTime().FramesToDestroy;
			Frame& EntityFrame = (Frame&)Item->_IsDestroyed;
			if (EntityFrame >= FramesToDestroy)
			{
				it = _Compoents.erase(it);
				continue;
		
			}
			EntityFrame++;
		}
		++it;
	}
}
Entity* Entity::NativeAddEntity()
{
	auto Ptr = new Entity(_Scene);
	Ptr->_ParentEntity = this;
	_Entitys.push_back(Unique_ptr<Entity>(Ptr));
	return Ptr;
}
void Entity::DestroyCompoents()
{
	_Compoents.clear();
}

Vec3 Entity::WorldPosition() const
{
	Vec3 R;
	const Entity* Ptr = this;
	while (Ptr)
	{
		R += Ptr->NativeLocalPosition();
		Ptr = Ptr->NativeParent();
	}
	return  R;
}
Vec2 Entity::WorldPosition2D() const
{
	Vec2 R;
	const Entity* Ptr = this;
	while (Ptr)
	{
		R += Ptr->LocalPosition2D();
		Ptr = Ptr->NativeParent();
	}
	return  R;
}
Vec3 Entity::WorldRotation() const
{
	Vec3 R;
	const Entity* Ptr = this;
	while (Ptr)
	{
		R += Ptr->LocalRotation();
		Ptr = Ptr->NativeParent();
	}
	return  R;
}

Vec2 Entity::WorldRotation2D() const
{
	Vec2 R;
	const Entity* Ptr = this;
	while (Ptr)
	{
		R += Ptr->LocalRotation2D();
		Ptr = Ptr->NativeParent();
	}
	return  R;
}

Vec3 Entity::WorldScale() const
{
	Vec3 R;
	const Entity* Ptr = this;
	while (Ptr)
	{
		R += Ptr->LocalScale();
		Ptr = Ptr->NativeParent();
	}
	return  R;
}

Vec2 Entity::WorldScale2D() const
{
	Vec2 R;
	const Entity* Ptr = this;
	while (Ptr)
	{
		R += Ptr->LocalScale2D();
		Ptr = Ptr->NativeParent();
	}
	return  R;
}
void Entity::WorldPosition(const Vec3& Value)
{
	auto Pos = WorldPosition();
	auto Offset = Value-Pos;
	_LocalPosition += Offset;
}
void Entity::WorldPosition2D(const Vec2& Value)
{
	auto Pos = WorldPosition2D();
	auto Offset = Value - Pos;
	*(Vec2*)&_LocalPosition += Offset;
}

void Entity::WorldRotation(const Vec3& Value)
{
	auto Pos = WorldRotation();
	auto Offset = Value - Pos;
	_LocalRotation += Offset;
}
void Entity::WorldRotation2D(const Vec2& Value)
{
	auto Pos = WorldRotation2D();
	auto Offset = Value - Pos;
	*(Vec2*)&_LocalRotation += Offset;
}

void Entity::WorldScale(const Vec3& Value)
{
	auto Pos = WorldScale();
	auto Offset = Value - Pos;
	_LocalScale += Offset;
}
void Entity::WorldScale2D(const Vec2& Value)
{
	auto Pos = WorldScale2D();
	auto Offset = Value - Pos;
	*(Vec2*)&_LocalScale += Offset;
}

CoreEnd

		
