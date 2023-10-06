#pragma once
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetManager.hpp"
CoreStart
class StandardAssetLoader :public AssetLoader
{
public:
	GameFiles* gamefiles = nullptr;
	inline void Init(GameFiles* files, UCode::AssetManager* Manager)
	{
		gamefiles = files;
		_Manager = Manager;
	}

	inline const GameFilesData& Get_GameFile()
	{
		return gamefiles->Get_FilesData();
	}

	Optional<Assetptr> LoadAsset(const UID& Path);
	Optional<Assetptr> LoadAsset(const Path& Path);
};
CoreEnd

