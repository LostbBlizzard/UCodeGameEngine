#pragma once
#include <EditorWindows/EditorWindow.hpp>
#include <Editor/EditorNamespace.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/Vector.hpp>
#include <filesystem>
#include "EditorWindows/OtherTypes/DirectoryViewer.hpp"
#include "Helper/FileHelper.hpp"

#include "UEditorModules/UEditorModule.hpp"
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

	
	static EditorWindowData GetEditorData();
	void OnSaveWindow(USerializer& JsonToSaveIn) override;
	void OnLoadWindow(UDeserializer& JsonToOutof) override;
private:
	static EditorWindow* MakeWin(const NewEditorWindowData& project);
	struct FileData
	{
		String FileName;
		Path FullFileName;
		FileHelper::FileType FileType = FileHelper::FileType::Null;
	};


	void UpdateDir();
	
	Path _LookingAtDir;
	Path _LookingAtDirReadable;
	String _FindFileName;
	Vector<FileData> _Files;
	DirectoryViewer Viewer;
	bool _ShowingAddNewItemWindow;
	optional<Path> RenameFile;
	static constexpr float AssetFileMaxLastUsed = 30;
	struct AssetFile
	{
		Unique_ptr<UEditorAssetFile> _File;
		ManagedPtr<UEditorAssetFile> _ManageFile;
		
		float LastUsed = AssetFileMaxLastUsed;
		
		void Set(Unique_ptr<UEditorAssetFile> File)
		{
			_ManageFile.Set_Value(File.get());
			_File = std::move(File);
		}
		AssetFile() = default;
		AssetFile(AssetFile&& ToCopyFrom) = default;
		AssetFile& operator=(AssetFile&& ToCopyFrom) = default;
	
		~AssetFile()
		{
			if (_File.get()) 
			{
				_ManageFile.Set_Value(nullptr);
			}
		}
	};
	Vector<AssetFile> _AssetFiles;
	
	optional<size_t> FindAssetFile(const Path& path);

	bool DrawFileItem(ProjectFilesWindow::FileData& Item, ImVec2& ButtionSize);
	static void DirectoryViewerOpenFile(DirectoryViewer& From, void* Ptr, const Path& Path);
};
EditorEnd
