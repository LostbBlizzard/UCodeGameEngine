#include "Entity.hpp"
#include "GameRunTime.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/SceneData.hpp"
CoreStart
	

Compoent::Compoent(Entity* entity,UComponentData* TypeData)
	: _Entity(entity), _TypeData(TypeData),
	_IsDestroyed(false),_GameRunTimeHasCalledStart(false), _IsActive(true), _Managed(this)
{

}

Compoent::~Compoent()
{
	_Managed.Set_Value(nullptr);
}
GameRunTime* Entity::GetGameRunTime()
{
	return _Scene->Get_RunTime();
}

Entity::Entity(RunTimeScene* runtime)
	: _Scene(runtime),_IsDestroyed(false), _IsActive(true), _Managed(this)
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
		if (Item->Get_IsActive())
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
		if (Item->Get_IsActive())
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

			const Time::Frame FramesToDestroy = GetGameRunTime()->Get_GameTime().FramesToDestroy;
			Time::Frame& EntityFrame = (Time::Frame&)Item->_IsDestroyed;
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
Entity* Entity::Add_Entity()
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

Vec3 Entity::Get_WorldPosition() const
{
	Vec3 R;
	const Entity* Ptr = this;
	while (Ptr)
	{
		R += Ptr->Get_LocalPosition();
		Ptr = Ptr->Get_Parent();
	}
	return  R;
}
Vec2 Entity::Get_WorldPosition2D() const
{
	Vec2 R;
	const Entity* Ptr = this;
	while (Ptr)
	{
		R += Ptr->Get_LocalPosition2D();
		Ptr = Ptr->Get_Parent();
	}
	return  R;
}
Vec3 Entity::Get_WorldRotation() const
{
	Vec3 R;
	const Entity* Ptr = this;
	while (Ptr)
	{
		R += Ptr->Get_LocalRotation();
		Ptr = Ptr->Get_Parent();
	}
	return  R;
}

Vec2 Entity::Get_WorldRotation2D() const
{
	Vec2 R;
	const Entity* Ptr = this;
	while (Ptr)
	{
		R += Ptr->Get_LocalRotation2D();
		Ptr = Ptr->Get_Parent();
	}
	return  R;
}

Vec3 Entity::Get_WorldScale() const
{
	Vec3 R;
	const Entity* Ptr = this;
	while (Ptr)
	{
		R += Ptr->Get_LocalScale();
		Ptr = Ptr->Get_Parent();
	}
	return  R;
}

Vec2 Entity::Get_WorldScale2D() const
{
	Vec2 R;
	const Entity* Ptr = this;
	while (Ptr)
	{
		R += Ptr->Get_LocalScale2D();
		Ptr = Ptr->Get_Parent();
	}
	return  R;
}
void Entity::Set_WorldPosition(const Vec3& Value)
{
	auto Pos = Get_WorldPosition();
	auto Offset = Value-Pos;
	_LocalPosition += Offset;
}
void Entity::Set_WorldPosition(const Vec2& Value)
{
	auto Pos = Get_WorldPosition2D();
	auto Offset = Value - Pos;
	*(Vec2*)&_LocalPosition += Offset;
}

void Entity::Set_WorldRotation(const Vec3& Value)
{
	auto Pos = Get_WorldRotation();
	auto Offset = Value - Pos;
	_LocalRotation += Offset;
}
void Entity::Set_WorldRotation(const Vec2& Value)
{
	auto Pos = Get_WorldRotation2D();
	auto Offset = Value - Pos;
	*(Vec2*)&_LocalRotation += Offset;
}

void Entity::Set_WorldScale(const Vec3& Value)
{
	auto Pos =Get_WorldPosition();
	auto Offset = Value - Pos;
	_LocalScale += Offset;
}
void Entity::Set_WorldScale(const Vec2& Value)
{
	auto Pos =Get_WorldScale2D();
	auto Offset = Value - Pos;
	*(Vec2*)&_LocalScale += Offset;
}

CoreEnd

		
