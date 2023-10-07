#pragma once
#include "..\BasicTypes.hpp"
#include "GameTime.hpp"
#include "UComponentsID.hpp"
#include <type_traits>

#include "UCodeRunTime/BasicTypes/ManagedPtr.hpp"
CoreStart
class RunTimeScene;
class Entity;
class GameRunTime;
class GameTime;
class Compoent;
class USerializer;
class UDeserializer;

using EntityPtr = ManagedPtr<Entity>;
using Compoent_Ptr = ManagedPtr<Compoent>;

template<typename T>
struct CompoentPtr
{
private:
	static constexpr bool IsCompoent = std::is_base_of<Compoent,T>();
	static_assert(IsCompoent, " 'T' is not a Compoent");
public:
	ManagedPtr<T> Ptr;

	
};

class Compoent
{
	friend Entity;
private:
	Entity* _Entity = nullptr;
	UComponentData* _TypeData =nullptr;
	bool _IsDestroyed= false, _GameRunTimeHasCalledStart = false, _IsActive = true;
	Compoent_Ptr _Managed;
public:
	Compoent(Entity* entity, UComponentData* TypeData);
	virtual ~Compoent();

	virtual void Start() {}
	virtual void Update() {}
	virtual void FixedUpdate() {}

	virtual void Serialize(USerializer& Serializer) const {}
	virtual void Deserialize(UDeserializer& Serializer) {}

	UCodeGameEngineForceinlne Entity* GetMyEntity() const { return _Entity; }


	UCodeGameEngineForceinlne void Set_CompoentActive(bool V) { _IsActive = V; }
	UCodeGameEngineForceinlne void Enable_Compoent() { _IsActive = true; }
	UCodeGameEngineForceinlne void Disable_Compoent() { _IsActive = false; }
	UCodeGameEngineForceinlne bool Get_IsActive() const { return _IsActive; }

	
	UCodeGameEngineForceinlne static void Destroy(Compoent* compoent) { compoent->_IsDestroyed = true; }
	UCodeGameEngineForceinlne bool Get_IsDestroyed() const { return _IsDestroyed; }
	UCodeGameEngineForceinlne UComponentData* Get_CompoentTypeData() const { return _TypeData; }

	inline bool Get_IsActive_InRunTime() const;
	inline GameRunTime* GetGameRunTime() const;
	inline RunTimeScene* Get_Scene() const;

	
	template<class T, typename... Pars> UCodeGameEngineForceinlne T* AddCompoent(Pars... parameters);
	template<class T> UCodeGameEngineForceinlne T* GetCompent();

	template<class T> CompoentPtr<T> Get_ManagedPtr()
	{
		constexpr bool IsCompoent = std::is_base_of<Compoent, T>();
		static_assert(IsCompoent, " 'T' is not a Compoent");
		#if UCodeGameEngineDEBUG

		T* cast = dynamic_cast<T*>(_Managed.Get_Value());
		if (cast == nullptr)
		{
			UCODE_ENGINE_ERROR("bad type");
		}

		#endif // DEBUG
		return *(CompoentPtr<T>*)&_Managed;
	}

	Compoent_Ptr Get_ManagedPtr()
	{
		return _Managed;
	}
};

class Entity
{
	friend GameRunTime;
	friend RunTimeScene;
public:
	Entity(RunTimeScene* runtime);
	~Entity();
	Entity(Entity&& source) = default;
	
	
	

	UCodeGameEngineForceinlne const auto& Get_Compoents() const
	{
		return _Compoents;
	}
	UCodeGameEngineForceinlne auto& Get_Compoents()
	{
		return _Compoents;
	}
	template<class T, typename... Pars> T* AddCompoent(Pars... parameters)
	{
		constexpr bool IsCompoent = std::is_base_of< Compoent, T>();
		static_assert(IsCompoent, " 'T' is not a Compoent");


		auto t = std::make_unique<T>(this, parameters...);
		auto r = t.get();
		_Compoents.push_back(std::move(t));
		return r;
	}
	inline void MoveCompoent(Compoent* t)
	{

		#if UCodeGameEngineDEBUG
		if (t->GetMyEntity() != this)
		{
			throw std::runtime_error("Cant Move Compoent ,Compoent was made with Entity a different Entity");
		}

		for (auto& Item : _Compoents)
		{
			if (Item.get() == t)
			{
				throw std::runtime_error("Cant Move Compoent ,this Entity has this Compoent");
			}
		}
		#endif // DEBUG



		_Compoents.push_back(Unique_ptr<Compoent>(t));
	}
	template<class T> T* GetCompent()
	{
		constexpr bool IsCompoent = std::is_base_of< Compoent, T>();
		static_assert(IsCompoent, " 'T' is not a Compoent");

		for (size_t i = 0; i < _Compoents.size(); i++)
		{
			auto& c = _Compoents[i];
			T* cast = dynamic_cast<T*>(c.get());
			if (cast)
			{
				return cast;
			}
		}

		return nullptr;
	}
	template<class T> const  T* GetCompent() const
	{
		constexpr bool IsCompoent = std::is_base_of< Compoent, T>();
		static_assert(IsCompoent, " 'T' is not a Compoent");

		for (size_t i = 0; i < _Compoents.size(); i++)
		{
			auto& c = _Compoents[i];
			T* cast = dynamic_cast<T*>(c.get());
			if (cast)
			{
				return cast;
			}
		}

		return nullptr;
	}

	UCodeGameEngineForceinlne RunTimeScene* Get_Scene() const { return _Scene; }
	GameRunTime* GetGameRunTime();

	UCodeGameEngineForceinlne void Set_Active(bool V) { _IsActive = V; }
	UCodeGameEngineForceinlne void Enable_Entity() { _IsActive = true; }
	UCodeGameEngineForceinlne void Disable_Entity() { _IsActive = false; }
	UCodeGameEngineForceinlne bool Get_IsActive() const { return _IsActive; }

	//
	UCodeGameEngineForceinlne static void Destroy(Entity* compoent) { compoent->_IsDestroyed = true; }
	UCodeGameEngineForceinlne bool Get_IsDestroyed() const { return _IsDestroyed; }

	//Dont use me unless you know what you're doing
	UCodeGameEngineForceinlne void EditorAPI_Set_Scene(RunTimeScene* S) { _Scene = S; }
	
	//Dont use me unless you know what you're doing
	UCodeGameEngineForceinlne void EditorAPI_Set_ParentEntity(Entity* S) { _ParentEntity = S; }
	
	EntityPtr Get_ManagedPtr()
	{
		return _Managed;
	}
	const EntityPtr Get_ManagedPtr() const
	{
		return _Managed;
	}

	UCodeGameEngineForceinlne auto& Get_Entitys()
	{
		return _Entitys;
	}
	UCodeGameEngineForceinlne auto& Get_Entitys() const
	{
		return _Entitys;
	}

	Entity* Add_Entity();



	UCodeGameEngineForceinlne auto& Get_Name()
	{
		return _Name;
	}
	UCodeGameEngineForceinlne auto& Get_Name() const
	{
		return _Name;
	}


	UCodeGameEngineForceinlne auto& Get_LocalPosition()
	{
		return  _LocalPosition;
	}
	UCodeGameEngineForceinlne auto& Get_LocalPosition() const
	{
		return  _LocalPosition;
	}

	UCodeGameEngineForceinlne auto& Get_LocalPosition2D()
	{
		return  *(Vec2*)&_LocalPosition;
	}
	UCodeGameEngineForceinlne auto& Get_LocalPosition2D() const
	{
		return  *(Vec2*)&_LocalPosition;
	}

	UCodeGameEngineForceinlne auto& Get_LocalRotation()
	{
		return  _LocalRotation;
	}
	UCodeGameEngineForceinlne auto& Get_LocalRotation() const
	{
		return  _LocalRotation;
	}

	UCodeGameEngineForceinlne auto& Get_LocalRotation2D()
	{
		return  *(Vec2*)&_LocalRotation;
	}
	UCodeGameEngineForceinlne auto& Get_LocalRotation2D() const
	{
		return  *(Vec2*)&_LocalRotation;
	}

	UCodeGameEngineForceinlne auto& Get_LocalScale()
	{
		return  _LocalScale;
	}
	UCodeGameEngineForceinlne auto& Get_LocalScale() const
	{
		return  _LocalScale;
	}

	UCodeGameEngineForceinlne auto& Get_LocalScale2D()
	{
		return  *(Vec2*)&_LocalScale;
	}
	UCodeGameEngineForceinlne auto& Get_LocalScale2D() const
	{
		return  *(Vec2*)&_LocalScale;
	}



	Vec3 Get_WorldPosition() const;
	Vec2 Get_WorldPosition2D() const;
	Vec3 Get_WorldRotation() const;

	Vec2 Get_WorldRotation2D() const;

	Vec3 Get_WorldScale() const;

	Vec2 Get_WorldScale2D() const;

	void Set_WorldPosition(const Vec3& Value);
	void Set_WorldPosition(const Vec2& Value);
	void Set_WorldRotation(const Vec3& Value);
	void Set_WorldRotation(const Vec2& Value);
	void Set_WorldScale(const Vec3& Value);
	void Set_WorldScale(const Vec2& Value);

	UCodeGameEngineForceinlne const auto Get_Parent() const
	{
		return   _ParentEntity;
	}

	UCodeGameEngineForceinlne auto Get_Parent() 
	{
		return   _ParentEntity;
	}
private:
	RunTimeScene* _Scene =nullptr;
	bool _IsDestroyed =false, _IsActive =true;
	Vector<Unique_ptr<Compoent>> _Compoents;
	Vector<Unique_ptr<Entity>> _Entitys;
	String _Name;

	Vec3 _LocalPosition;
	Vec3 _LocalRotation;
	Vec3 _LocalScale = { 1,1,1 };

	EntityPtr _Managed;
	Entity* _ParentEntity = nullptr;

	void RunTimeUpdate();
	void FixedUpdate();
	void DestroyNullCompoents();
	void DestroyCompoents();

};

	 

 

//Helpers
UCodeGameEngineForceinlne bool Compoent::Get_IsActive_InRunTime() const
{
	return _IsActive && _Entity->Get_IsActive();
}
UCodeGameEngineForceinlne GameRunTime* Compoent::GetGameRunTime() const { return _Entity->GetGameRunTime(); }
UCodeGameEngineForceinlne RunTimeScene* Compoent::Get_Scene() const{ return _Entity->Get_Scene();}
	





template<class T, typename ...Pars> UCodeGameEngineForceinlne T* Compoent::AddCompoent(Pars... parameters)
{
		return _Entity->AddCompoent<T>(parameters...);
}
template<class T> UCodeGameEngineForceinlne T* Compoent::GetCompent()
{
	return _Entity->GetCompent<T>();
}
CoreEnd	
	

