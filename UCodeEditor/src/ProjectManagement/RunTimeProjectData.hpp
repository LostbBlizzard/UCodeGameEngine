#pragma once
#include "ProjectData.hpp"
#include <EditorWindows/OtherTypes/ImageData.hpp>

#include <Typedefs.hpp>
#include <UCodeRunTime/UDefs.hpp>
#include <filesystem>
#include "EditorIndex.hpp"
#include <UCodeRunTime/ULibrarys/Others/Random.hpp>
EditorStart

class RunTimeProjectData
{
public:
	RunTimeProjectData();

	void SetProject(const ProjectData& Data, const Path& ProjDir);
	void SetProjectToNull();

	UCODE_EDITOR_NODISCARD Path GetAssetsDir();
	UCODE_EDITOR_NODISCARD Path GetSrcDir();
	UCODE_EDITOR_NODISCARD Path GetSrcLibsDir();
	UCODE_EDITOR_NODISCARD Path GetCachedDir();
	UCODE_EDITOR_NODISCARD Path GetOutDir();
	UCODE_EDITOR_NODISCARD Path GetGameLibDir();

	UCODE_EDITOR_NODISCARD Path GetULangIntDir();
	UCODE_EDITOR_NODISCARD Path GetULangOutDir();

	UCODE_EDITOR_NODISCARD Path Get_ProjectPrefsDir();
	UCODE_EDITOR_NODISCARD UCODE_EDITOR_FORCEINLINE Path Get_ProjectDir() { return _ProjDir; }
	UCODE_EDITOR_NODISCARD UCode::UID GetNewUID();
	
	UCODE_EDITOR_FORCEINLINE bool Is_ProjLoaded() { return _ProjDir != ""; }

	UCODE_EDITOR_FORCEINLINE ProjectData& Get_ProjData() { return _Data; }

	UCODE_EDITOR_FORCEINLINE auto& Get_AssetIndex() { return _Index; }
	UCODE_EDITOR_FORCEINLINE auto& Get_FileWatcher() { return _Watcher; }

	UCODE_EDITOR_NODISCARD Path GetProjectDataPath();
	UCODE_EDITOR_NODISCARD Path GetAssetIndexPath();

	void UpdateFileWatcher()
	{
		_Watcher.Step();
	}
private:
	ProjectData _Data; 
	Path _ProjDir;
	UCode::Random _UIDGenerator;
	EditorIndex _Index;
	FileWatcher _Watcher;
};
EditorEnd
