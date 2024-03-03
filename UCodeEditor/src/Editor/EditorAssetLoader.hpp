#pragma once
#include "EditorNamespace.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetManager.hpp"

#include "UEditorModules/UEditorModule.hpp"
#include "ProjectManagement/ProjectFiles.hpp"
EditorStart
class EditorAssetLoader :public UCode::AssetLoader
{
public:
	RunTimeProjectData* RunTimeProject = nullptr;
	ProjectFiles* Files = nullptr;

	inline void Init(ProjectFiles* files,RunTimeProjectData* Project,UCode::AssetManager* Manager)
	{
		RunTimeProject = Project;
		_Manager = Manager;
		Files = files;
	}


	NullablePtr<UCode::Asset> LoadAssetPtr(const UID& Path) override;
	NullablePtr<UCode::Asset> LoadAssetPtr(const Path& Path) override;


};
EditorEnd