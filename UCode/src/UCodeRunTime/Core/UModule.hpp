#pragma once
#include "CoreNamespace.hpp"
#include "../BasicTypes.hpp"
#include "Entity.hpp"

CoreStart

class Asset;

struct UModuleComponentData
{
	UComponentData* TypeData=nullptr;
	UModuleComponentData(){}
	UModuleComponentData(UComponentData* V)
	{
		TypeData = V;
	}
};

struct UModuleAssetData
{
	using LoadAsset_t = Optional<Unique_ptr<Asset>>(*)(UDeserializer& serializer);
	UModuleAssetData() {}
	UModuleAssetData(const Path& ExtWithDot,LoadAsset_t LoadAsset)
		:_LoadAsset(LoadAsset), _AssetExtWithDot(ExtWithDot)
	{
	}

	Optional<Unique_ptr<Asset>> LoadAsset(UDeserializer& serializer)
	{
		return _LoadAsset(serializer);
	}

	
	Path _AssetExtWithDot;
	LoadAsset_t _LoadAsset =nullptr;
};

class UModule
{
public:
	UModule()
	{

	}
	virtual ~UModule()
	{

	}
	virtual const Span<UModuleComponentData> GetCompoents() { return {}; };
	virtual const Span<UModuleAssetData> GetAssets() { return {}; };

	UModuleComponentData* GetComponet(const UComponentsID& ID);
	UModuleAssetData* GetAssetData(const Path& ExtWithDot);
private:
};

using UModuleInit = UModule* (*)();


class UModules
{
public:

	static void Init();
	static const Span<UModule*> GetModules();

	static UModuleComponentData* GetComponet(const UComponentsID& ID);
	static UModuleAssetData* GetAsset(const Path& ExtWithDot);
private:
	inline static Vector<Unique_ptr<UModule>> _Modules;
};
CoreEnd