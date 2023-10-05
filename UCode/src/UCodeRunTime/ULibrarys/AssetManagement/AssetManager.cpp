#include "AssetManager.hpp"
#include <UCodeRunTime/StaticComponentsIndex.hpp>
CoreStart
AssetManager::AssetManager(Gamelibrary* lib) : libraryBook(lib)
{
}

void AssetManager::GetDataFromGameFiles(GameFiles* gameFiles)
{
}


void AssetManager::AddAsset(Unique_ptr<Asset> Asset)
{

}

optional<Assetptr> AssetManager::FindAsset(const UID& Path)
{
	return {};
}

optional<Assetptr> AssetManager::FindAsset(const Path& Path)
{
	return {};
}

optional<Assetptr> AssetManager::LoadAsset(const UID& Path)
{
	return {};
}

optional<Assetptr> AssetManager::LoadAsset(const Path& Path)
{
	return {};
}

optional<Assetptr> AssetManager::FindOrLoad(const UID& Path)
{
	auto R = FindAsset(Path);
	if (!R.has_value())
	{
		R = LoadAsset(Path);
	}
	return R;
}

optional<Assetptr> AssetManager::FindOrLoad(const Path& Path)
{
	auto R = FindAsset(Path);
	if (!R.has_value())
	{
		R = LoadAsset(Path);
	}
	return R;
}

AssetManager::~AssetManager()
{
}

constexpr size_t MaxToGarbage = 1;//The AssetManager is has a ref to the items.

void AssetManager::GarbageCollect()
{
	auto Now = Time::clock::now();
	auto delta_timeT = Now - LastFixedUpdateTime;

	LastFixedUpdateTime = Now;
	auto delta_timefsec = std::chrono::duration_cast<Time::fsec>(delta_timeT);
	f32 delta_time = delta_timefsec.count();

	GarbageCollect(delta_time);
}
void AssetManager::GarbageCollect(const f32 delta_time)
{
	
}

void AssetManager::Update()
{

	if (UpdateTics == 0)
	{
		UpdateTics = 255;
		GarbageCollect();
	}
	UpdateTics--;
}
constexpr StaticBooksIndex_t KeyIdex = (StaticBooksIndex_t)StaticBooksIndex::AssetManager;
AssetManager* AssetManager::Get(Gamelibrary* lib)
{
	auto V = lib->Get_StaticComponent(KeyIdex);
	if (V) { return (AssetManager*)V; }

	AssetManager* r = new AssetManager(lib);;

	lib->MoveBook(r);
	lib->SetStaticComponent(KeyIdex, r);
	return r;
}
AssetManager* AssetManager::Find(const Gamelibrary* lib)
{
	auto V = lib->Get_StaticComponent(KeyIdex);
	return (AssetManager*)V;
}
CoreEnd