#pragma once
#include "AssetManager.hpp"
#include "AssetPtr.hpp"
#include "SceneData.hpp"
CoreStart


struct ScencAsset : Asset
{
public:
	Scene2dData _Base;
	ScencAsset()
	{

	}
	ScencAsset(Scene2dData&& base)
		:_Base(std::move(base))
	{

	}

	ManagedPtr<ScencAsset> GetManaged()
	{
		auto V = this->Get_Managed();
		return *(ManagedPtr<ScencAsset>*)&V;
	}
};

using ScencAssetPtr = ManagedPtr<ScencAsset>;
using ScencPtr = AssetPtr<ScencAsset,Scene2dData>;

CoreEnd