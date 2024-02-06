#pragma once



#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/BasicTypes.hpp>

#include "System.hpp"
#include "Entity.hpp"
#include "GameTime.hpp"
CoreStart

class Gamelibrary
{
public:
	Gamelibrary();
	~Gamelibrary();

	Gamelibrary(Gamelibrary&& Other) = default;
	Gamelibrary& operator=(Gamelibrary&& Other) = default;


	UCodeGEForceinlne void MoveSystem(System* book)
	{
#if UCodeGEDebug
		if (book->Getlibrary() != this)
		{
			throw std::runtime_error("Cant Move Compoent ,Compoent was made with Entity a different Entity");
		}

		for (auto& Item : _Systems)
		{
			if (Item.get() == book)
			{
				throw std::runtime_error("Cant Move Compoent ,this Entity has this Compoent");
			}
		}
#endif // DEBUG

		_Systems.push_back(Unique_ptr<System>(book));
	}
	UCodeGEForceinlne void* Get_LibraryOwner() { return LibraryOwner; }
	UCodeGEForceinlne void Set_LibraryOwner(void* NewOwner) { LibraryOwner = NewOwner; }
	UCodeGEForceinlne void Set_LibraryOwnerToNull() { LibraryOwner = nullptr; }
	UCodeGEForceinlne bool HasNoLibraryOwner() { return LibraryOwner == nullptr; }
	void libraryUpdate();//LibraryOwner is responsible for call this.

	UCodeGEForceinlne void SetStaticComponent(size_t key, System* Value) { _StaticSystems[key] = Value; }
	UCodeGEForceinlne void ReMoveStaticComponent(size_t key) { _StaticSystems[key] = nullptr; }
	UCodeGEForceinlne System* Get_StaticComponent(size_t key) const { return _StaticSystems[key]; }

	UCodeGEForceinlne auto& Get_Books() { return _Systems; }
	UCodeGEForceinlne const auto& Get_Books() const { return _Systems; }

	UCodeGEForceinlne auto& Get_StaticBooks() { return _StaticSystems; }
	UCodeGEForceinlne const auto& Get_StaticBooks() const { return _StaticSystems; }


	static Gamelibrary* Current();
private:
	Vector<Unique_ptr<System>> _Systems;
	Vector<System*> _StaticSystems;
	void* LibraryOwner = nullptr;
	void DestroyNullBook();
};

class GameRunTime;


class RunTimeScene;

using RunTimeScenePtr = ManagedPtr<RunTimeScene>;

class RunTimeScene
{
	friend GameRunTime;
public:
	RunTimeScene(GameRunTime* RunTime);
	~RunTimeScene();
	RunTimeScene(const RunTimeScene& other) = delete;
	RunTimeScene& operator=(const RunTimeScene& other) = delete;


	UCodeGEForceinlne Entity* NewEntity()
	{
		auto v = std::make_unique<Entity>(this);
		auto r = v.get();
		_Entitys.push_back(std::move(v));
		return r;
	}
	UCodeGEForceinlne void MoveEntity(Entity* e)
	{
		_Entitys.push_back(Unique_ptr<Entity>(e));
	}


	UCodeGEForceinlne auto& Get_Entitys()
	{
		return _Entitys;
	}
	UCodeGEForceinlne const auto& Get_Entitys() const
	{
		return _Entitys;
	}

	UCodeGEForceinlne void SetStaticComponent(size_t key, Compoent* Value) { _StaticComponents[key] = Value; }
	UCodeGEForceinlne void ReMoveStaticComponent(size_t key) { _StaticComponents[key] = nullptr; }
	UCodeGEForceinlne Compoent* Get_StaticComponent(size_t key) const { return _StaticComponents[key]; }

	UCodeGEForceinlne  GameRunTime* Get_RunTime() { return  _RunTime; }

	UCodeGEForceinlne bool Get_IsDestroyed() { return _IsSceneDestroyed; }

	UCodeGEForceinlne static void Destroy(RunTimeScene* Scene) { Scene->_IsSceneDestroyed = true; }


	static void MoveEntity(Entity* EntityToMove, RunTimeScene* MoveNextTo);
	static void MoveEntity(Entity* EntityToMove, const Entity* MoveNextTo, bool MoveIntoFolder);


	RunTimeScenePtr Get_ManagedPtr()
	{
		return _Managed;
	}
	const RunTimeScenePtr Get_ManagedPtr() const
	{
		return _Managed;
	}


	inline auto& Get_Name()
	{
		return _Name;
	}
	inline auto& Get_Name() const
	{
		return _Name;
	}

	inline auto& Get_UID()
	{
		return  _UID;
	}
	inline auto& Get_UID() const
	{
		return _UID;
	}

private:
	bool _IsSceneDestroyed;
	GameRunTime* _RunTime;
	Vector<Unique_ptr<Entity>> _Entitys;
	Vector<Compoent*> _StaticComponents;
	String _Name;
	UID _UID;
	RunTimeScenePtr _Managed;

	void UpdateEntitys();
	void FixedUpdateEntitys();
	void DestroyNullEntityAndCompoents();
};


class GameRunTime
{
public:
	GameRunTime();
	GameRunTime(Ref<Gamelibrary> Library);
	~GameRunTime();
	void Init();
	void GameUpdate();
	void EndRunTime();

	UCodeGEForceinlne void StopRunTime() { _IsGameRuning = false; }
	UCodeGEForceinlne bool Get_IsGameRuning() const { return _IsGameRuning; };


	GameRunTime(const GameRunTime& other) = delete;
	GameRunTime& operator=(const GameRunTime& other) = delete;

	UCodeGEForceinlne const Gamelibrary* Get_Library() const { return _Library.get(); }
	UCodeGEForceinlne Gamelibrary* Get_Library_Edit() { return _Library.get(); }//thread safety

	UCodeGEForceinlne Ref<Gamelibrary> Get_LibraryRef() { return _Library; }

	UCodeGEForceinlne auto& Get_Scenes()
	{
		return  _Scenes;
	}
	UCodeGEForceinlne const auto& Get_Scenes() const
	{
		return  _Scenes;
	}
	UCodeGEForceinlne RunTimeScene* Add_NewScene()
	{
		auto V = std::make_unique<RunTimeScene>(this);
		auto R = V.get();
		_Scenes.push_back(std::move(V));
		return R;
	}

	UCodeGEForceinlne Entity* NewEntityOnRunTimeScene()
	{
		return _RunTimeScene.NewEntity();
	}

	UCodeGEForceinlne void SetStaticComponent(size_t key, Compoent* Value) { _StaticComponents[key] = Value; }
	UCodeGEForceinlne void ReMoveStaticComponent(size_t key) { _StaticComponents[key] = nullptr; }
	UCodeGEForceinlne Compoent* Get_StaticComponent(size_t key) const { return _StaticComponents[key]; }
	UCodeGEForceinlne const GameTime& Get_GameTime() const
	{
		return _GameTime;
	}
	UCodeGEForceinlne void Set_GameTime_FramesToDestroy(Frame FramesToDestroy)
	{
		_GameTime.FramesToDestroy = FramesToDestroy;
	}

	static GameRunTime* Current();
private:
	bool _IsGameRuning;
	Ref<Gamelibrary> _Library;
	Vector<Compoent*> _StaticComponents;
	RunTimeScene _RunTimeScene;
	Vector<Unique_ptr<RunTimeScene>> _Scenes;


	GameTime _GameTime;
	Time_point LastFixedUpdateTime;
	f32 _FixedUpdateTimer;


	void DoUpdate();
	void DoFixedUpdate();
public:
	void DestroyNullScenes();
	void DestroyAllScenes();
};
CoreEnd


