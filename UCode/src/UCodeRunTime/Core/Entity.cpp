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

			const Time::Frame FramesToDestroy = NativeGameRunTime()->Get_GameTime().FramesToDestroy;
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

Vec3 Entity::worldposition() const
{
	Vec3 R;
	const Entity* Ptr = this;
	while (Ptr)
	{
		R += Ptr->ilocalposition();
		Ptr = Ptr->NativeParent();
	}
	return  R;
}
Vec2 Entity::worldposition2d() const
{
	Vec2 R;
	const Entity* Ptr = this;
	while (Ptr)
	{
		R += Ptr->ilocalposition2d();
		Ptr = Ptr->NativeParent();
	}
	return  R;
}
Vec3 Entity::worldrotation() const
{
	Vec3 R;
	const Entity* Ptr = this;
	while (Ptr)
	{
		R += Ptr->ilocalrotation();
		Ptr = Ptr->NativeParent();
	}
	return  R;
}

Vec2 Entity::worldrotation2d() const
{
	Vec2 R;
	const Entity* Ptr = this;
	while (Ptr)
	{
		R += Ptr->localrotation2d();
		Ptr = Ptr->NativeParent();
	}
	return  R;
}

Vec3 Entity::worldscale() const
{
	Vec3 R;
	const Entity* Ptr = this;
	while (Ptr)
	{
		R += Ptr->ilocalscale();
		Ptr = Ptr->NativeParent();
	}
	return  R;
}

Vec2 Entity::worldscale2d() const
{
	Vec2 R;
	const Entity* Ptr = this;
	while (Ptr)
	{
		R += Ptr->localscale2d();
		Ptr = Ptr->NativeParent();
	}
	return  R;
}
void Entity::worldposition(const Vec3& Value)
{
	auto Pos = worldposition();
	auto Offset = Value-Pos;
	_LocalPosition += Offset;
}
void Entity::worldposition(const Vec2& Value)
{
	auto Pos = worldposition2d();
	auto Offset = Value - Pos;
	*(Vec2*)&_LocalPosition += Offset;
}

void Entity::worldrotation(const Vec3& Value)
{
	auto Pos = worldrotation();
	auto Offset = Value - Pos;
	_LocalRotation += Offset;
}
void Entity::worldrotation(const Vec2& Value)
{
	auto Pos = worldrotation2d();
	auto Offset = Value - Pos;
	*(Vec2*)&_LocalRotation += Offset;
}

void Entity::worldscale(const Vec3& Value)
{
	auto Pos =worldposition();
	auto Offset = Value - Pos;
	_LocalScale += Offset;
}
void Entity::worldscale(const Vec2& Value)
{
	auto Pos =worldscale2d();
	auto Offset = Value - Pos;
	*(Vec2*)&_LocalScale += Offset;
}

CoreEnd

		
