#pragma once
#include "AssetManager.hpp"
#include "AssetPtr.hpp"
#include "SceneData.hpp"
CoreStart


struct ScencAsset : Asset
{
public:
	Scene2dData _Base;

	ManagedPtr<ScencAsset> GetManaged()
	{
		return *(ManagedPtr<ScencAsset>*) & this->Get_Managed();
	}
};

using ScencAssetPtr = ManagedPtr<ScencAsset>;
using ScencPtr = AssetPtr<ScencAsset,Scene2dData>;

CoreEnd