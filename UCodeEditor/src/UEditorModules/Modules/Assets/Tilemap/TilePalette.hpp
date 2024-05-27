#pragma once
#include "UEditorModules/Modules/CoreModule.hpp" 
#include <EditorWindows/DragAndDropNames.hpp>
#include "TilePaletteData.hpp"
EditorStart


class TilePaletteAssetFile :public UEditorAssetFileData
{
public:
	TilePaletteAssetFile();
	bool Draw(UEditorAssetDataConext& Data, const Path& path) override;
	
	
	class Liveing :public UEditorAssetFile
	{
	public:
		Liveing();
		~Liveing();
		TilePaletteAsset _Asset;
		
		void Init(const UEditorAssetFileInitContext& Context) override;

		bool DrawButtion(const UEditorAssetDrawButtionContext& Item);
		void DrawInspect(const UEditorAssetDrawInspectContext& Item) override;
		NullablePtr<UCode::Asset> LoadAsset(const LoadAssetContext& Item) override;
		void FileUpdated() override;
		void SaveFile(const UEditorAssetFileSaveFileContext& Context) override;
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
