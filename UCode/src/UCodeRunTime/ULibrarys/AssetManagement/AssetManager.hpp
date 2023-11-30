#pragma once

#include <UCodeRunTime/Core/System.hpp>
#include <UCodeRunTime/BasicTypes.hpp>
#include <UCodeRunTime/ULibrarys/AssetManagement/UID.hpp>
#include <UCodeRunTime/BasicTypes/ManagedPtr.hpp>
#include <UCodeRunTime/Core/GameTime.hpp>
#include <UCodeRunTime/CoreSystems/GameFiles.hpp>
#include <type_traits>
CoreStart


class AssetManager;
struct AssetManagerCleanUpData
{
	size_t _CurrentUsed=0;
	float _LastUsed = 0;
};
class Asset
{
public:
	friend AssetManager;
	//a hint to AssetManager for it to know when best to free this
	//how much this object allocated
	Optional<size_t> ObjectSize;

	//The path for AssetManager to get the object
	Optional<Path> ObjectPath;

	//The UID for AssetManager to get the object
	Optional<UID> Uid;

	//
	


	Asset()
	{
		_Managed.Set_Value(this);
	}

	virtual ~Asset()
	{
		_Managed.Set_Value(nullptr);
	}
	
	void UpdatePath(const Path& path)
	{
		_UpdatedPath = true;
		ObjectPath = path;
	}
	void UpdateUID(const UID& path)
	{
		_UpdatedUID = true;
		Uid = path;
	}
	void UpdateObjectSize(const size_t Size)
	{
		_UpdatedSize = true;
		ObjectSize = Size;
	}
	ManagedPtr<Asset> Get_Managed()
	{
		return _Managed;
	}	
	
	template<typename T> T* GetAssetAs()
	{
		constexpr bool IsCompoent = std::is_base_of<Asset, T>();
		static_assert(IsCompoent, " 'T' is not a Compoent");
		return dynamic_cast<T*>(this);
	}

	void UseAsset()
	{

	}
private:
	ManagedPtr<Asset> _Managed;
	bool _UpdatedPath = false;
	bool _UpdatedUID = false;
	bool _UpdatedSize = false;
	AssetManagerCleanUpData _CleanData;
};
using Assetptr = ManagedPtr<Asset>;


class AssetLoader
{
public:
	AssetManager* _Manager = nullptr;
	//In Bytes
	size_t MemoryTarget = GetMB(500);

	static size_t GetGB(size_t GB)
	{
		return  GetMB(GB) * 1000;
	}
	static size_t GetMB(size_t MB)
	{
		return 1000 * 1000 * MB;
	}

	virtual Optional<Unique_ptr<Asset>> LoadAsset(const UID& Path)
	{
		return {};
	}
	virtual Optional<Unique_ptr<Asset>> LoadAsset(const Path& Path)
	{
		return {};
	}
};

class AssetManager :private System
{
public:
	
	//
	static AssetManager* Get(Gamelibrary* lib);
	static AssetManager* Find(const Gamelibrary* lib);
	//

	void GetDataFromGameFiles(GameFiles* gameFiles);
	
	void AddAsset(Unique_ptr<Asset> Asset);

	Optional<Assetptr> FindAsset(const UID& Path);
	Optional<Assetptr> FindAsset(const Path& Path);
	
	Optional<Assetptr> LoadAsset(const UID& Path);
	Optional<Assetptr> LoadAsset(const Path& Path);

	Optional<Assetptr> FindOrLoad(const UID& Path);
	Optional<Assetptr> FindOrLoad(const Path& Path);

	void Set_AssetLoader(AssetLoader* Loader)
	{
		_Loader = Loader;
	}
	AssetLoader* Get_AssetLoader()
	{
	 	return _Loader;
	}
	const AssetLoader* Get_AssetLoader() const
	{
		return _Loader;
	}
	GameFiles* Get_GameFiles()
	{
		return GameFiles::Get(Getlibrary());
	}
	const GameFiles* Get_GameFiles() const
	{
		return GameFiles::Find(Getlibrary());
	}
private:
	AssetManager(Gamelibrary* lib);
	~AssetManager();
	void GarbageCollect();
	void GarbageCollect(const f32 delta_time);
	void Update() override;

	size_t UpdateTics = 0;
	AssetLoader* _Loader =nullptr;
	Time::clock::time_point LastFixedUpdateTime = {};
	Vector<Unique_ptr<Asset>> _Assets;
};
CoreEnd
