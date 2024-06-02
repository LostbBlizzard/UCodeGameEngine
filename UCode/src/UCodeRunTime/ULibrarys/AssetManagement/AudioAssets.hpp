#pragma once
#include "../Audio/AudioSystem.hpp"
#include "AssetManager.hpp"
#include "AssetPtr.hpp"
CoreStart

struct AudioAsset : Asset
{
public:
	AudioFile _Base;
	AudioAsset()
	{

	}
	AudioAsset(AudioFile&& base)
		:_Base(std::move(base))
	{

	}

	ManagedPtr<AudioFile> GetManaged()
	{
		auto V = this->Get_Managed();
		return *(ManagedPtr<AudioFile>*)&V;
	}
};

using AudioAssetPtr = ManagedPtr<AudioFile>;
using AudioPtr = AssetPtr<AudioAsset,AudioFile>;

CoreEnd
