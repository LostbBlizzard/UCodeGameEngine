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
	bool Draw(UEditorAssetDataConext& Data, const Path& path) override;

	ExportFileRet ExportFile(const Path& path, const ExportFileContext& Item) override;
	Optional<GetUIDInfo> GetFileUID(UEditorGetUIDContext& context) override;
};
EditorEnd
