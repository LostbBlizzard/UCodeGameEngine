#pragma once
#include "../BasicTypes.hpp"
#include "GameTime.hpp"
#include "UComponentsID.hpp"
#include <type_traits>

#include "UCodeRunTime/BasicTypes/ManagedPtr.hpp"


#include "UCodeLang/LangCore/LangDef.hpp"
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

UCodeLangExportSymbol("UCodeGameEngine") UCodeLangEmbed(
	R"(
    $Component trait:
	  uintptr _Handle = 0;
	  |entity[this&]  => ComponentAPI::entity(_Handle);
	  |ientity[this&] => ComponentAPI::ientity(_Handle);
    ")");

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

	Entity* NativeEntity() const { return _Entity; }


	void Set_CompoentActive(bool V) { _IsActive = V; }
	void Enable_Compoent() { _IsActive = true; }
	void Disable_Compoent() { _IsActive = false; }
	bool Get_IsActive() const { return _IsActive; }

	

	static void Destroy(Compoent* compoent) { compoent->_IsDestroyed = true; }
	bool Get_IsDestroyed() const { return _IsDestroyed; }
	UComponentData* Get_CompoentTypeData() const { return _TypeData; }

	bool Get_IsActive_InRunTime() const;
	GameRunTime* GetGameRunTime() const;
	RunTimeScene* Get_Scene() const;

	
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

UCodeLangExportSymbol("UCodeGameEngine") struct ComponentAPI
{
	static Compoent& Cast(uintptr_t _Handle)
	{
		return *(Compoent*)_Handle;
	}
	static const Compoent& iCast(uintptr_t _Handle)
	{
		return *(const Compoent*)_Handle;
	}

	UCodeLangExport static Entity& entity(uintptr_t _Handle)
	{
		return *Cast(_Handle).NativeEntity();
	}
	UCodeLangExport static const Entity& ientity(uintptr_t _Handle)
	{
		return *iCast(_Handle).NativeEntity();
	}
};

UCodeLangExportSymbol("UCodeGameEngine") class Entity
{
	friend GameRunTime;
	friend RunTimeScene;
public:
	Entity(RunTimeScene* runtime);
	~Entity();
	Entity(Entity&& source) = default;
	
	
	

	UCodeGameEngineForceinlne const auto& NativeCompoents() const
	{
		return _Compoents;
	}
	UCodeGameEngineForceinlne auto& NativeCompoents()
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
		if (t->NativeEntity() != this)
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

	UCodeGameEngineForceinlne RunTimeScene* NativeScene() const { return _Scene; }
	GameRunTime* NativeGameRunTime();

	UCodeGameEngineForceinlne void SetActive(bool V) { _IsActive = V; }

	UCodeGameEngineForceinlne bool GetActive() const { return _IsActive; }
	
	UCodeLangExport void Enable() { _IsActive = true; }
	UCodeLangExport void Disable() { _IsActive = false; }
	UCodeLangExport bool& active()
	{
		return _IsActive;
	}
	UCodeLangExport const bool& active() const
	{
		return _IsActive;
	}

	//

	UCodeLangExport void Destroy()
	{
		Destroy(this);
	}
	UCodeGameEngineForceinlne static void Destroy(Entity* compoent) { compoent->_IsDestroyed = true; }
	UCodeGameEngineForceinlne bool Get_IsDestroyed() const { return _IsDestroyed; }

	//Dont use me unless you know what you're doing
	UCodeGameEngineForceinlne void EditorAPI_Set_Scene(RunTimeScene* S) { _Scene = S; }
	
	//Dont use me unless you know what you're doing
	UCodeGameEngineForceinlne void EditorAPI_Set_ParentEntity(Entity* S) { _ParentEntity = S; }
	
	EntityPtr NativeManagedPtr()
	{
		return _Managed;
	}
	const EntityPtr NativeManagedPtr() const
	{
		return _Managed;
	}

	UCodeGameEngineForceinlne auto& NativeGetEntitys()
	{
		return _Entitys;
	}
	UCodeGameEngineForceinlne auto& NativeGetEntitys() const
	{
		return _Entitys;
	}

	Entity* NativeAddEntity();


	String& NativeName() { return _Name; }
	const String& NativeName() const { return _Name; }


	UCodeLangExport const StringView name() const {return _Name;}
	UCodeLangExport void name(StringView Value){_Name = Value;}


	UCodeLangExport Vec3& localposition()
	{
		return  _LocalPosition;
	}
	UCodeLangExport const Vec3& ilocalposition() const
	{
		return  _LocalPosition;
	}

	UCodeLangExport Vec2& localposition2d()
	{
		return  *(Vec2*)&_LocalPosition;
	}
	UCodeLangExport const Vec2& ilocalposition2d() const
	{
		return  *(Vec2*)&_LocalPosition;
	}

	UCodeLangExport Vec3& localrotation()
	{
		return  _LocalRotation;
	}
	UCodeLangExport const Vec3& ilocalrotation() const
	{
		return  _LocalRotation;
	}

	UCodeLangExport Vec2& localrotation2d()
	{
		return  *(Vec2*)&_LocalRotation;
	}
	UCodeLangExport const Vec2& ilocalrotation2d() const
	{
		return  *(Vec2*)&_LocalRotation;
	}

	UCodeLangExport Vec3& localscale()
	{
		return  _LocalScale;
	}
	UCodeLangExport const Vec3& ilocalscale() const
	{
		return  _LocalScale;
	}

	UCodeLangExport Vec2& localscale2d()
	{
		return  *(Vec2*)&_LocalScale;
	}
	UCodeLangExport const Vec2& ilocalscale2d() const
	{
		return  *(Vec2*)&_LocalScale;
	}



	UCodeLangExport Vec3 worldposition() const;
	UCodeLangExport Vec2 worldposition2d() const;

	UCodeLangExport Vec3 worldrotation() const;
	UCodeLangExport Vec2 worldrotation2d() const;

	UCodeLangExport Vec3 worldscale() const;
	UCodeLangExport Vec2 worldscale2d() const;

	UCodeLangExport void worldposition(const Vec3& Value);
	UCodeLangExport void worldposition(const Vec2& Value);

	UCodeLangExport void worldrotation(const Vec3& Value);
	UCodeLangExport void worldrotation(const Vec2& Value);
	
	UCodeLangExport void worldscale(const Vec3& Value);
	UCodeLangExport void worldscale(const Vec2& Value);

	UCodeGameEngineForceinlne const auto NativeParent() const
	{
		return   _ParentEntity;
	}

	UCodeGameEngineForceinlne auto NativeParent() 
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


template<class T, typename ...Pars> UCodeGameEngineForceinlne T* Compoent::AddCompoent(Pars... parameters)
{
		return _Entity->AddCompoent<T>(parameters...);
}
template<class T> UCodeGameEngineForceinlne T* Compoent::GetCompent()
{
	return _Entity->GetCompent<T>();
}
CoreEnd	
	

