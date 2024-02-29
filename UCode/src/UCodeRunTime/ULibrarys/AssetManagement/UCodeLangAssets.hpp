#pragma once
#include "AssetManager.hpp"
#include "AssetPtr.hpp"
#include "../UCodeLang/ScirptableObject.hpp"
CoreStart


struct ScirptableObjectAsset : Asset
{
public:
	ScirptableObject _Base;
	ScirptableObjectAsset()
	{

	}
	ScirptableObjectAsset(ScirptableObject&& base)
		:_Base(std::move(base))
	{

	}

	ManagedPtr<ScirptableObjectAsset> GetManaged()
	{
		auto V = this->Get_Managed();
		return *(ManagedPtr<ScirptableObjectAsset>*)&V;
	}
};

using ScirptableObjectAssetPtr = ManagedPtr<ScirptableObjectAsset>;
using ScirptableObjectPtr = AssetPtr<ScirptableObjectAsset,ScirptableObject>;

CoreEnd
