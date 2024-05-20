#pragma once
#include "UEditorModules/Modules/CoreModule.hpp" 
#include "EditorWindows/OtherTypes/ColorData.hpp"
EditorStart

class UColorAssetFile :public UEditorAssetFileData
{
public:
	UColorAssetFile();

	class LiveingColor :public UEditorAssetFile
	{
	public:
		ColorData _ColorInfoFromFile;
		Unique_ptr<UCode::Texture> _Texture;
		bool WasUpdated = true;
		LiveingColor();

		void Init(const UEditorAssetFileInitContext& Context) override;
		void SaveFile(const UEditorAssetFileSaveFileContext& Context) override;
		bool DrawButtion(const UEditorAssetDrawButtionContext& Item) override;
		void DrawInspect(const UEditorAssetDrawInspectContext& Item) override;
	};

	virtual Unique_ptr<UEditorAssetFile> GetMakeNewAssetFile() override
	{
		return Unique_ptr< UEditorAssetFile>(new LiveingColor());
	}

};

EditorEnd
