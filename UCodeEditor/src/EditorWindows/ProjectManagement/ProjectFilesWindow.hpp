#pragma once
#include <EditorWindows/EditorWindow.hpp>
#include <Editor/EditorNamespace.hpp>
#include <UCodeRunTime/BasicTypes.hpp>
#include <filesystem>
#include "EditorWindows/OtherTypes/DirectoryViewer.hpp"
#include "Helper/FileHelper.hpp"

#include "UEditorModules/UEditorModule.hpp"
#include "../ProjectManagement/ProjectFiles.hpp"
EditorStart

class ProjectFilesWindow :public EditorWindow
{
public:
	
	ProjectFilesWindow(const NewEditorWindowData& project);
	~ProjectFilesWindow() override;
	
	void UpdateWindow() override;

	void ShowFileCells();

	void ShowExlorer();


	void ShowDirButtions();

	void OnFileUpdate(const Path& path);
	
	static EditorWindowData GetEditorData();
	void OnSaveWindow(USerializer& JsonToSaveIn) override;
	void OnLoadWindow(UDeserializer& JsonToOutof) override;

	void OpenAndFocusOnAsset(UID id);
	void OpenAndFocusOnAsset(const Path& assetfullpath);
private:
	static EditorWindow* MakeWin(const NewEditorWindowData& project);
	struct FileData
	{
		String FileName;
		Path FullFileName;
		FileHelper::FileType FileType = FileHelper::FileType::Null;
	};

	NullablePtr<UEditorAssetFileData> GetAssetData(Path fullpath);
	NullablePtr<ProjectFiles::AssetFile> TryLoadAsset(Path fullpath);
	void ShowDirButtionsPaste();
	void UpdateDir();
	
	Optional<Path> _LookingAtDir;
	Path _LookingAtDirReadable;
	String _FindFileName;
	Vector<FileData> _Files;
	DirectoryViewer Viewer;
	bool _ShowingAddNewItemWindow =false;
	Optional<Path> RenameFile;
	Optional<Path> _LookingAtAssetForSubAssets;
	Optional<Path> _OpenAndFocus;
	ProjectFiles& Get_ProjectFiles();

	struct ButtionSizeData
	{
		float cellSize;
		Vec2 thumbnail;
	};
	ButtionSizeData GetButtionSize();
	bool DrawFileItem(ProjectFilesWindow::FileData& Item, ImVec2& ButtionSize);
	static void DirectoryViewerOpenFile(DirectoryViewer& From, void* Ptr, const Path& Path);
};
EditorEnd
