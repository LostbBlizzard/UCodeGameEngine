#include "AssetManager.hpp"
#include <UCodeRunTime/StaticComponentsIndex.hpp>
CoreStart
AssetManager::AssetManager(Gamelibrary* lib) : System(lib)
{
}

void AssetManager::GetDataFromGameFiles(GameFiles* gameFiles)
{
}


void AssetManager::AddAsset(Unique_ptr<Asset> Asset)
{
	_Assets.push_back(std::move(Asset));
}

Optional<Assetptr> AssetManager::FindAsset(const UID& Path)
{
	for (auto& Item : _Assets)
	{
		if (Item->Uid.has_value() && Item->Uid.value() == Path)
		{
			return Item->Get_Managed();
		}
	}
	return {};
}

Optional<Assetptr> AssetManager::FindAsset(const Path& Path)
{
	for (auto& Item : _Assets)
	{
		if (Item->ObjectPath.has_value() && Item->ObjectPath.value() == Path)
		{
			return Item->Get_Managed();
		}
	}


	return {};
}

Optional<Assetptr> AssetManager::LoadAsset(const UID& Path)
{
	auto r = _Loader->LoadAsset(Path);
	if (r.has_value())
	{
		auto m = r.value()->Get_Managed();
		AddAsset(std::move(r.value()));

		return m;
	}
	else
	{
		auto p = _Loader->LoadAssetPtr(Path);

		if (p.has_value())
		{
			return p.value().value()->Get_Managed();
		}
	}
	return {};
}

Optional<Assetptr> AssetManager::LoadAsset(const Path& Path)
{
	auto r = _Loader->LoadAsset(Path);
	if (r.has_value())
	{
		auto m = r.value()->Get_Managed();
		AddAsset(std::move(r.value()));

		return m;
	}
	else
	{
		auto p = _Loader->LoadAssetPtr(Path);

		if (p.has_value())
		{
			return p.value().value()->Get_Managed();
		}
	}
	return {};
}

Optional<Assetptr> AssetManager::FindOrLoad(const UID& Path)
{
	auto R = FindAsset(Path);
	if (!R.has_value())
	{
		R = LoadAsset(Path);
	}
	return R;
}

Optional<Assetptr> AssetManager::FindOrLoad(const Path& Path)
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
	auto Now = clock::now();
	auto delta_timeT = Now - LastFixedUpdateTime;

	LastFixedUpdateTime = Now;
	auto delta_timefsec = std::chrono::duration_cast<fsec>(delta_timeT);
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

	lib->MoveSystem(r);
	lib->SetStaticComponent(KeyIdex, r);
	return r;
}
AssetManager* AssetManager::Find(const Gamelibrary* lib)
{
	auto V = lib->Get_StaticComponent(KeyIdex);
	return (AssetManager*)V;
}
CoreEnd