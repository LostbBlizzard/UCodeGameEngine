#include "GameRunTime.hpp"
#include <UCodeRunTime/StaticComponentsIndex.hpp>
CoreStart

constexpr StaticBooksIndex_t StaticBookSize = (StaticBooksIndex_t)StaticBooksIndex::MaxValue;
constexpr StaticRunTimeComponentsIndex_t StaticRunTimeComponentsSize = (StaticRunTimeComponentsIndex_t)StaticRuntimeComponentsIndex::MaxValue;
constexpr StaticSceneComponentsIndex_t StaticSceneComponentsSize = (StaticSceneComponentsIndex_t)StaticSceneComponentsIndex::MaxValue;

Gamelibrary::Gamelibrary() : _StaticSystems()
{

	_StaticSystems.resize(StaticBookSize);
	for (size_t i = 0; i < StaticBookSize; i++){_StaticSystems[i] = nullptr;}
}
Gamelibrary::~Gamelibrary()
{
}
void Gamelibrary::libraryUpdate()
{
	for (size_t i = 0; i < _Systems.size(); i++)
	{
		auto& Item = _Systems[i];
		Item->Update();
	}


	DestroyNullBook();
}
void Gamelibrary::DestroyNullBook()
{
	for (auto it = _Systems.begin(); it != _Systems.end();)
	{
		auto& Item = *it;

		if (Item->Get_IsDestroyed())
		{
			it = _Systems.erase(it);
			continue;
		}
		++it;
	}
}
GameRunTime::GameRunTime() :_IsGameRuning(false), _StaticComponents()
, _FixedUpdateTimer(0), _RunTimeScene(this), _Library(Ref<Gamelibrary>::Create())
{
	_FixedUpdateTimer = 0;

	_Library->Set_LibraryOwner(this);
	
	_StaticComponents.resize(StaticRunTimeComponentsSize);
	for (size_t i = 0; i < StaticRunTimeComponentsSize; i++){_StaticComponents[i] = nullptr;}
}
GameRunTime::GameRunTime(Ref<Gamelibrary> Library) :_IsGameRuning(false), _StaticComponents()
,_FixedUpdateTimer(0), _RunTimeScene(this), _Library(Library)
{
	_StaticComponents.resize(StaticRunTimeComponentsSize);
	for (size_t i = 0; i < StaticRunTimeComponentsSize; i++){_StaticComponents[i] = nullptr;}


	if (_Library->HasNoLibraryOwner())
	{
		_Library->Set_LibraryOwner(this);
	}
}
GameRunTime::~GameRunTime()
{
	if (_IsGameRuning) { EndRunTime(); }

	if (_Library->Get_LibraryOwner() == this)
	{
		_Library->Set_LibraryOwnerToNull();
	}
}
void GameRunTime::Init()
{
	LastFixedUpdateTime = clock::now();
	_IsGameRuning = true;
}
void GameRunTime::GameUpdate()
{
	auto Now = clock::now();
	auto delta_timeT = Now - LastFixedUpdateTime;

	LastFixedUpdateTime = Now;
	auto delta_timefsec = std::chrono::duration_cast<fsec>(delta_timeT);
	f32 delta_time = delta_timefsec.count();

	_FixedUpdateTimer += delta_time;
	GameTime.UpateDelta = delta_time;
	if (_FixedUpdateTimer > GameTime::FixedTimeDelta)
	{
		_FixedUpdateTimer = 0;
		DoFixedUpdate();
	}


	DoUpdate();

	DestroyNullScenes();

	if (_Library->HasNoLibraryOwner())
	{
		_Library->Set_LibraryOwner(this);
	}
	if (_Library->Get_LibraryOwner() == this)
	{
		_Library->libraryUpdate();
	}
}
void GameRunTime::EndRunTime()
{
	_IsGameRuning = false;
	DestroyAllScenes();
}	

void GameRunTime::DoUpdate()
{
	_RunTimeScene.UpdateEntitys();
	for (size_t i = 0; i < _Scenes.size(); i++)
	{
		auto& Item = _Scenes[i];
		Item->UpdateEntitys();
	}
}
void GameRunTime::DoFixedUpdate()
{
	_RunTimeScene.FixedUpdateEntitys();
	for (size_t i = 0; i < _Scenes.size(); i++)
	{
		auto& Item = _Scenes[i];
		Item->FixedUpdateEntitys();
	}
}
void GameRunTime::DestroyNullScenes()
{
	_RunTimeScene.DestroyNullEntityAndCompoents();
	for (auto it = _Scenes.begin(); it != _Scenes.end();)
	{
		auto& Item = *it;

		Item->DestroyNullEntityAndCompoents();
		if (Item->Get_IsDestroyed())
		{
			const Frame FramesToDestroy = GameTime.FramesToDestroy;
			Frame& SceneFrame = (Frame&)Item->_IsSceneDestroyed;
			if (SceneFrame >= FramesToDestroy)
			{
				it = _Scenes.erase(it);
				continue;

			}
			SceneFrame++;
		}

		++it;
	}
}
void GameRunTime::DestroyAllScenes()
{
	_Scenes.clear();
}
RunTimeScene::RunTimeScene(GameRunTime* RunTime) :_IsSceneDestroyed(false),_RunTime(RunTime), _StaticComponents()
,_Managed(this)
{

	_StaticComponents.resize(StaticSceneComponentsSize);
	for (size_t i = 0; i < StaticSceneComponentsSize; i++) { _StaticComponents[i] = nullptr; }
}
RunTimeScene::~RunTimeScene()
{
	_Managed.Set_Value(nullptr);
	_IsSceneDestroyed = true;
	for (size_t i = 0; i < _Entitys.size(); i++)
	{
		auto& Item = _Entitys[i]; 
		Entity::Destroy(Item.get());
	}
	DestroyNullEntityAndCompoents();
}
void RunTimeScene::UpdateEntitys()
{
	for (size_t i = 0; i < _Entitys.size(); i++)
	{
		auto& Item = _Entitys[i];
		if (Item->GetActive()) { Item->RunTimeUpdate(); }
	}
}
void RunTimeScene::FixedUpdateEntitys()
{
	
	for (size_t i = 0; i < _Entitys.size(); i++)
	{
		auto& Item = _Entitys[i];
		if (Item) { Item->FixedUpdate(); }
	}

}
void RunTimeScene::DestroyNullEntityAndCompoents()
{
	const Frame FramesToDestroy = Get_RunTime()->Get_GameTime().FramesToDestroy;
	for (auto it = _Entitys.begin(); it != _Entitys.end();)
	{
		auto& Item = *it;



		if (Item->Get_IsDestroyed())
		{

			
			Frame& EntityFrame = (Frame&)Item->_IsDestroyed;
			if (EntityFrame >= FramesToDestroy)
			{

				Item->DestroyCompoents();
				it = _Entitys.erase(it);
				continue;

			}
			EntityFrame++;
		}
		else
		{
			Item->DestroyNullCompoents();
		}



		++it;

	}

}
void RunTimeScene::MoveEntity(UCode::Entity* EntityToMove, UCode::RunTimeScene* MoveNextTo)
{
	UCode::RunTimeScene* Scene = EntityToMove->NativeScene();

	auto& SceneEntitys = Scene->Get_Entitys();
	auto& ThisEntitylist = EntityToMove->NativeParent() ? EntityToMove->NativeParent()->NativeGetEntitys() : SceneEntitys;
	
	
	Unique_ptr<Entity> EntityAsPtr;
	for (auto it = ThisEntitylist.begin(); it != ThisEntitylist.end();)
	{
		auto& Item = *it;
		if (Item.get() == EntityToMove)
		{
			EntityAsPtr = std::move(Item);
			ThisEntitylist.erase(it);
			if (EntityToMove->NativeParent())
			{
				EntityToMove->EditorAPI_Set_ParentEntity(nullptr);
			}
			break;
		}

		it++;
	}

	EntityToMove->EditorAPI_Set_Scene(MoveNextTo);

	if (EntityAsPtr) 
	{
		SceneEntitys.push_back(std::move(EntityAsPtr));
	}

}
void RunTimeScene::MoveEntity(UCode::Entity* EntityToMove, const UCode::Entity* MoveNextTo, bool MoveIntoFolder)
{
	UCodeGEToDo();
}

CoreEnd

