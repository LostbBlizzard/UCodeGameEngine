#pragma once
#include "../BasicTypes.hpp"
#include "GameTime.hpp"
#include "UComponentsID.hpp"
#include <type_traits>

#include "UCodeRunTime/BasicTypes/ManagedPtr.hpp"

#include "../Core/../ULibrarys/Loger.hpp"
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
	static constexpr bool IsCompoent = std::is_base_of<Compoent, T>();
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
	UComponentData* _TypeData = nullptr;
	bool _IsDestroyed = false, _GameRunTimeHasCalledStart = false, _IsActive = true;
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
	virtual rttr::instance Get_Rttr_Instance() { return nullptr; }

	bool Get_IsActive_InRunTime() const;
	GameRunTime* GetGameRunTime() const;
	RunTimeScene* Get_Scene() const;


	template<class T, typename... Pars> UCodeGEForceinlne T* AddCompoent(Pars... parameters);
	template<class T> UCodeGEForceinlne NullablePtr<T> GetCompent();

	template<class T> CompoentPtr<T> Get_ManagedPtr()
	{
		constexpr bool IsCompoent = std::is_base_of<Compoent, T>();
		static_assert(IsCompoent, " 'T' is not a Compoent");
#if UCodeGEDebug

		T* cast = dynamic_cast<T*>(_Managed.Get_Value());
		if (cast == nullptr)
		{
			UCodeGEError("bad type");
		}

#endif // DEBUG
		return *(CompoentPtr<T>*) & _Managed;
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




	UCodeGEForceinlne const auto& NativeCompoents() const
	{
		return _Compoents;
	}
	UCodeGEForceinlne auto& NativeCompoents()
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

#if UCodeGEDebug
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
	template<class T> NullablePtr<T> GetCompent()
	{
		constexpr bool IsCompoent = std::is_base_of< Compoent, T>();
		static_assert(IsCompoent, " 'T' is not a Compoent");

		for (size_t i = 0; i < _Compoents.size(); i++)
		{
			auto& c = _Compoents[i];
			T* cast = dynamic_cast<T*>(c.get());
			if (cast)
			{
				return Nullableptr(cast);
			}
		}

		return  nullptr;
	}
	template<class T> const NullablePtr<T> GetCompent() const
	{
		constexpr bool IsCompoent = std::is_base_of< Compoent, T>();
		static_assert(IsCompoent, " 'T' is not a Compoent");

		for (size_t i = 0; i < _Compoents.size(); i++)
		{
			auto& c = _Compoents[i];
			T* cast = dynamic_cast<T*>(c.get());
			if (cast)
			{
				return Nullableptr(cast);
			}
		}

		return nullptr;
	}

	template<class T>
	void GetCompents(Vector<NeverNullPtr<T>> out)
	{
		out.clear();
		constexpr bool IsCompoent = std::is_base_of< Compoent, T>();
		static_assert(IsCompoent, " 'T' is not a Compoent");

		for (size_t i = 0; i < _Compoents.size(); i++)
		{
			auto& c = _Compoents[i];
			T* cast = dynamic_cast<T*>(c.get());
			if (cast)
			{
				out.push_back(NeverNullptr(cast));
			}
		}
	}
	template<class T>
	void GetCompents(Vector<const NeverNullPtr<T>> out) const
	{
		out.clear();
		constexpr bool IsCompoent = std::is_base_of< Compoent, T>();
		static_assert(IsCompoent, " 'T' is not a Compoent");

		for (size_t i = 0; i < _Compoents.size(); i++)
		{
			auto& c = _Compoents[i];
			T* cast = dynamic_cast<T*>(c.get());
			if (cast)
			{
				out.push_back(NeverNullptr(cast));
			}
		}
	}
	template<class T>
	Vector<NeverNullPtr<T>> GetCompents()
	{
		Vector<NeverNullPtr<T>> r;
		GetCompents(r);
		return r;
	}
	template<class T>
	Vector<NeverNullPtr<T>> GetCompents() const
	{
		Vector<NeverNullPtr<T>> r;
		GetCompents(r);
		return r;
	}


	UCodeGEForceinlne RunTimeScene* NativeScene() const { return _Scene; }
	GameRunTime* NativeGameRunTime();

	UCodeGEForceinlne void SetActive(bool V) { _IsActive = V; }

	UCodeGEForceinlne bool GetActive() const { return _IsActive; }

	UCodeLangExport void Enable() { _IsActive = true; }
	UCodeLangExport void Disable() { _IsActive = false; }
	UCodeLangExport bool& active()
	{
		return _IsActive;
	}
	UCodeLangExport const bool& iactive() const
	{
		return _IsActive;
	}

	//

	UCodeLangExport void Destroy()
	{
		Destroy(this);
	}
	UCodeGEForceinlne static void Destroy(Entity* compoent) { compoent->_IsDestroyed = true; }
	UCodeGEForceinlne bool Get_IsDestroyed() const { return _IsDestroyed; }

	//Dont use me unless you know what you're doing
	UCodeGEForceinlne void EditorAPI_Set_Scene(RunTimeScene* S) { _Scene = S; }

	//Dont use me unless you know what you're doing
	UCodeGEForceinlne void EditorAPI_Set_ParentEntity(Entity* S) { _ParentEntity = S; }

	EntityPtr NativeManagedPtr()
	{
		return _Managed;
	}
	const EntityPtr NativeManagedPtr() const
	{
		return _Managed;
	}

	UCodeGEForceinlne auto& NativeGetEntitys()
	{
		return _Entitys;
	}
	UCodeGEForceinlne auto& NativeGetEntitys() const
	{
		return _Entitys;
	}

	Entity* NativeAddEntity();


	String& NativeName() { return _Name; }
	const String& NativeName() const { return _Name; }


	UCodeLangExport const StringView Name() const { return _Name; }
	UCodeLangExport void Name(StringView Value) { _Name = Value; }


	UCodeLangExport Vec3& LocalPosition()
	{
		return  _LocalPosition;
	}
	UCodeLangExport void LocalPosition(const Vec3& value)
	{
		_LocalPosition = value;
	}

	UCodeLangExport Vec2 LocalPosition2D() const
	{
		return  *(Vec2*)&_LocalPosition;
	}
	UCodeLangExport void LocalPosition2D(const Vec2& value)
	{
		*(Vec2*)&_LocalPosition = value;
	}

	UCodeLangExport Vec3 LocalRotation() const
	{
		return  _LocalRotation;
	}
	UCodeLangExport void LocalRotation(const Vec3& value)
	{
		_LocalRotation = value;
	}

	UCodeLangExport Vec2 LocalRotation2D() const
	{
		return  *(Vec2*)&_LocalRotation;
	}
	UCodeLangExport void LocalRotation2D(const Vec2& value) 
	{
		*(Vec2*)&_LocalRotation = value;
	}	

	UCodeLangExport void LocalScale(const Vec3& value)
	{
		_LocalScale = value;
	}
	UCodeLangExport Vec3 LocalScale() const
	{
		return  _LocalScale;
	}

	UCodeLangExport void LocalScale2D(const Vec2 value)
	{
		*(Vec2*)&_LocalScale = value;
	}
	UCodeLangExport Vec2 LocalScale2D() const
	{
		return  *(Vec2*)&_LocalScale;
	}



	UCodeLangExport Vec3 WorldPosition() const;
	UCodeLangExport Vec2 WorldPosition2D() const;

	UCodeLangExport Vec3 WorldRotation() const;
	UCodeLangExport Vec2 WorldRotation2D() const;

	UCodeLangExport Vec3 WorldScale() const;
	UCodeLangExport Vec2 WorldScale2D() const;

	UCodeLangExport void WorldPosition(const Vec3& Value);
	UCodeLangExport void WorldPosition2D(const Vec2& Value);

	UCodeLangExport void WorldRotation(const Vec3& Value);
	UCodeLangExport void WorldRotation2D(const Vec2& Value);

	UCodeLangExport void WorldScale(const Vec3& Value);
	UCodeLangExport void WorldScale2D(const Vec2& Value);

	UCodeGEForceinlne const auto NativeParent() const
	{
		return   _ParentEntity;
	}

	UCodeGEForceinlne auto NativeParent()
	{
		return   _ParentEntity;
	}

	UCodeGEForceinlne auto& NativeLocalPosition()
	{
		return   _LocalPosition;
	}
	UCodeGEForceinlne auto& NativeLocalPosition() const
	{
		return   _LocalPosition;
	}
	UCodeGEForceinlne auto& NativeLocalRotation()
	{
		return   _LocalRotation;
	}
	UCodeGEForceinlne auto& NativeLocalRotation() const
	{
		return  _LocalRotation;
	}
	UCodeGEForceinlne auto& NativeLocalScale()
	{
		return  _LocalScale;
	}
	UCodeGEForceinlne auto& NativeLocalScale() const
	{
		return  _LocalScale;
	}
private:
	RunTimeScene* _Scene = nullptr;
	bool _IsDestroyed = false, _IsActive = true;
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


template<class T, typename ...Pars> UCodeGEForceinlne T* Compoent::AddCompoent(Pars... parameters)
{
	return _Entity->AddCompoent<T>(parameters...);
}
template<class T> UCodeGEForceinlne NullablePtr<T> Compoent::GetCompent()
{
	return _Entity->GetCompent<T>();
}
CoreEnd


