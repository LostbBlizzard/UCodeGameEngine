#pragma once



#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/BasicTypes.hpp>

#include "libraryBook.hpp"
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

	
	UCodeGameEngineForceinlne void MoveBook(libraryBook* book)
	{
#if UCodeGameEngineDEBUG
		if (book->Getlibrary() != this)
		{
			throw std::runtime_error("Cant Move Compoent ,Compoent was made with Entity a different Entity");
		}

		for (auto& Item : _Books)
		{
			if (Item.get() == book)
			{
				throw std::runtime_error("Cant Move Compoent ,this Entity has this Compoent");
			}
		}
#endif // DEBUG

		_Books.push_back(Unique_ptr<libraryBook>(book));
	}
	UCodeGameEngineForceinlne void* Get_LibraryOwner() { return LibraryOwner; }
	UCodeGameEngineForceinlne void Set_LibraryOwner(void* NewOwner) { LibraryOwner = NewOwner; }
	UCodeGameEngineForceinlne void Set_LibraryOwnerToNull() { LibraryOwner = nullptr; }
	UCodeGameEngineForceinlne bool HasNoLibraryOwner() { return LibraryOwner==nullptr; }
	void libraryUpdate();//LibraryOwner is responsible for call this.
	
	UCodeGameEngineForceinlne void SetStaticComponent(size_t key, libraryBook* Value) { _StaticBooks[key] = Value; }
	UCodeGameEngineForceinlne void ReMoveStaticComponent(size_t key) { _StaticBooks[key] = nullptr; }
	UCodeGameEngineForceinlne libraryBook* Get_StaticComponent(size_t key) const { return _StaticBooks[key]; }

	UCodeGameEngineForceinlne auto& Get_Books() { return _Books; }
	UCodeGameEngineForceinlne const auto& Get_Books() const { return _Books; }

	UCodeGameEngineForceinlne auto& Get_StaticBooks() { return _StaticBooks; }
	UCodeGameEngineForceinlne const auto& Get_StaticBooks() const { return _StaticBooks; }

private:
	Vector<Unique_ptr<libraryBook>> _Books;
	Vector<libraryBook*> _StaticBooks;
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
	

	UCodeGameEngineForceinlne Entity* NewEntity()
	{
		auto v =std::make_unique<Entity>(this);
		auto r = v.get();
		_Entitys.push_back(std::move(v));
		return r;
	}
	UCodeGameEngineForceinlne void MoveEntity(Entity* e)
	{
		_Entitys.push_back(Unique_ptr<Entity>(e));
	}
	

	UCodeGameEngineForceinlne auto& Get_Entitys()
	{
		return _Entitys;
	}
	UCodeGameEngineForceinlne const auto& Get_Entitys() const
	{
		return _Entitys;
	}

	UCodeGameEngineForceinlne void SetStaticComponent(size_t key, Compoent* Value) { _StaticComponents[key] = Value; }
	UCodeGameEngineForceinlne void ReMoveStaticComponent(size_t key) { _StaticComponents[key] = nullptr; }
	UCodeGameEngineForceinlne Compoent* Get_StaticComponent(size_t key) const { return _StaticComponents[key]; }

	UCodeGameEngineForceinlne  GameRunTime* Get_RunTime() { return  _RunTime; }

	UCodeGameEngineForceinlne bool Get_IsDestroyed() { return _IsSceneDestroyed; }

	UCodeGameEngineForceinlne static void Destroy(RunTimeScene* Scene) { Scene->_IsSceneDestroyed = true; }
	

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
	
	UCodeGameEngineForceinlne void StopRunTime(){ _IsGameRuning = false; }
	UCodeGameEngineForceinlne bool Get_IsGameRuning() const { return _IsGameRuning; };


	GameRunTime(const GameRunTime& other) = delete;
	GameRunTime& operator=(const GameRunTime& other) = delete;
	
	UCodeGameEngineForceinlne const Gamelibrary* Get_Library() const { return _Library.get(); }
	UCodeGameEngineForceinlne Gamelibrary* Get_Library_Edit() { return _Library.get(); }//thread safety
	
	UCodeGameEngineForceinlne Ref<Gamelibrary> Get_LibraryRef() { return _Library; }

	UCodeGameEngineForceinlne auto& Get_Scenes()
	{
		return  _Scenes;
	}
	UCodeGameEngineForceinlne const auto& Get_Scenes() const
	{
		return  _Scenes;
	}
	UCodeGameEngineForceinlne RunTimeScene* Add_NewScene()
	{
		auto V = std::make_unique<RunTimeScene>(this);
		auto R = V.get();
		_Scenes.push_back(std::move(V));
		return R;
	}

	UCodeGameEngineForceinlne Entity* NewEntityOnRunTimeScene()
	{
		return _RunTimeScene.NewEntity();
	}

	UCodeGameEngineForceinlne void SetStaticComponent(size_t key, Compoent* Value) { _StaticComponents[key] = Value; }
	UCodeGameEngineForceinlne void ReMoveStaticComponent(size_t key) { _StaticComponents[key] = nullptr; }
	UCodeGameEngineForceinlne Compoent* Get_StaticComponent(size_t key) const { return _StaticComponents[key]; }
	UCodeGameEngineForceinlne const Time::GameTime& Get_GameTime() const
	{
		return GameTime;
	}
	UCodeGameEngineForceinlne void Set_GameTime_FramesToDestroy(Time::Frame FramesToDestroy)
	{
		GameTime.FramesToDestroy = FramesToDestroy;
	}

	
private:	
	bool _IsGameRuning;
	Vector<Compoent*> _StaticComponents;
	RunTimeScene _RunTimeScene;
	Vector<Unique_ptr<RunTimeScene>> _Scenes;
	

	Ref<Gamelibrary> _Library;
	Time::GameTime GameTime;
	Time::Time_point LastFixedUpdateTime;
	f32 _FixedUpdateTimer;

	
	void DoUpdate();
	void DoFixedUpdate();
public:
	void DestroyNullScenes();
	void DestroyAllScenes();
};
CoreEnd


