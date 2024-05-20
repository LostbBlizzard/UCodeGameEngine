#pragma once

#include "UEditorModules/Modules/CoreModule.hpp" 
EditorStart
class UCodeAssetFile :public UEditorAssetFileData
{
public:
	UCodeAssetFile();
	virtual bool Draw(UEditorAssetDataConext& Data, const Path& path) override;
};
EditorEnd
