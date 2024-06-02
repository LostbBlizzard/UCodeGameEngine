#pragma once
#include "UEditorModules/Modules/CoreModule.hpp" 
#include "UCodeRunTime/ULibrarys/Audio/AudioSystem.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AudioAssets.hpp"
EditorStart
class MP3AssetFile :public UEditorAssetFileData
{
public:
	MP3AssetFile();

	class LiveingAsset :public UEditorAssetFile
	{
	public:
		UCode::AudioAsset file;

		inline static UCode::AudioPlaySettings playsettings;
		void Init(const UEditorAssetFileInitContext& Context) override;
		bool DrawButtion(const UEditorAssetDrawButtionContext& Item) override;
		void DrawInspect(const UEditorAssetDrawInspectContext& Item) override;
	};

	ExportFileRet ExportFile(const Path& path, const ExportFileContext& Item) override;

	virtual Unique_ptr<UEditorAssetFile> GetMakeNewAssetFile() override
	{
		return Unique_ptr< UEditorAssetFile>(new LiveingAsset());
	}

};
EditorEnd
