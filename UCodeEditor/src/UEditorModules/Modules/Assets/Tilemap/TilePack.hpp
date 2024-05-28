#pragma once
#include "UEditorModules/Modules/CoreModule.hpp" 
#include <EditorWindows/DragAndDropNames.hpp>
#include "TilePaletteData.hpp"
EditorStart

class TilePackAssetFile :public UEditorAssetFileData
{
public:
	TilePackAssetFile();
	
	
	class Liveing :public UEditorAssetFile
	{
	public:
		Liveing();
		~Liveing();
		TilePackAsset _Data;
		Vector<Optional<TileDataAsset>> _Assets;
		void Init(const UEditorAssetFileInitContext& Context) override;

		bool DrawButtion(const UEditorAssetDrawButtionContext& Item);
		void DrawInspect(const UEditorAssetDrawInspectContext& Item) override;
		NullablePtr<UCode::Asset> LoadAsset(const LoadAssetContext& Item) override;
		void FileUpdated() override;
		void SaveFile(const UEditorAssetFileSaveFileContext& Context) override;

		void LoadAssetAt(size_t Index);
		void OnFileLoaded();
		void RenameTile(size_t Index, const String& newname);
		void RemoveTile(size_t Index);
		void DrawSubAssets(const UEditorDrawSubAssetContext& Item) override;
		void NewTile(TileDataPack::PackTile&& tile);
		bool ShouldBeUnloaded(const UEditorAssetShouldUnloadContext& Context) override;
	};
	Unique_ptr<UEditorAssetFile> GetMakeNewAssetFile() override
	{
		return Unique_ptr<UEditorAssetFile>(new Liveing());
	}

	ExportFileRet ExportFile(const Path& path, const ExportFileContext& Item) override;
	Optional<GetUIDInfo> GetFileUID(UEditorGetUIDContext& context) override;
};
EditorEnd
