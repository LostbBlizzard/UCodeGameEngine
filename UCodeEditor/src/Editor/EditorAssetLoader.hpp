#pragma once
#include "EditorNamespace.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetManager.hpp"

#include "UEditorModules/UEditorModule.hpp"
EditorStart
class EditorAssetLoader :public UCode::AssetLoader
{
public:
	RunTimeProjectData* RunTimeProject = nullptr;

	inline void Init(RunTimeProjectData* Project,UCode::AssetManager* Manager)
	{
		RunTimeProject = Project;
		_Manager = Manager;
	}


	Optional<UCode::Assetptr> LoadAsset(const UID& Path) override
	{
		return {};
	}
	Optional<UCode::Assetptr> LoadAsset(const Path& Path) override
	{
		return {};
	}


};
EditorEnd