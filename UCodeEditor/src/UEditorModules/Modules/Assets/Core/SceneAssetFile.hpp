#pragma once
#include "UEditorModules/Modules/CoreModule.hpp" 
#include "UCodeRunTime/ULibrarys/AssetManagement/SceneData.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/RawEntityData.hpp"
#include <EditorWindows/DragAndDropNames.hpp>
EditorStart
class ScencAssetFile :public UEditorAssetFileData
{
public:
	ScencAssetFile();

	class LiveingAsset :public UEditorAssetFile
	{
	public:
		UCode::ScencAsset file;

		void Init(const UEditorAssetFileInitContext& Context) override;
		void DrawInspect(const UEditorAssetDrawInspectContext& Item) override;
		NullablePtr<UCode::Asset> LoadAsset(const LoadAssetContext& Item) override;
		void TryLoadAsset();
		bool DrawButtion(const UEditorAssetDrawButtionContext& Item);

	};


	ExportFileRet ExportFile(const Path& path, const ExportFileContext& Item) override;
	Optional<GetUIDInfo> GetFileUID(UEditorGetUIDContext& context) override;
	Unique_ptr<UEditorAssetFile> GetMakeNewAssetFile();
};
EditorEnd
