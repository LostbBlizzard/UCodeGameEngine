#include "MP3AssetFile.hpp"

#include "Editor/EditorAppCompoent.hpp"
EditorStart
MP3AssetFile::MP3AssetFile()
{
	FileExtWithDot = ".mp3";
	FileMetaExtWithDot = UEditorAssetFileData::DefaultMetaFileExtWithDot;
	CanHaveLiveingAssets = true;
}
void MP3AssetFile::LiveingAsset::Init(const UEditorAssetFileInitContext& Context)
{
	file.Load(this->FileFullPath);
}

bool MP3AssetFile::LiveingAsset::DrawButtion(const UEditorAssetDrawButtionContext& Item)
{
	return ImGuIHelper::ImageButton(Item.ObjectPtr, AppFiles::sprite::AudioIcon, *(ImVec2*)&Item.ButtionSize);
}

void MP3AssetFile::LiveingAsset::DrawInspect(const UEditorAssetDrawInspectContext& Item)
{
	if (ImGui::Button("Play"))
	{

		auto audio = UCode::AudioSystem::Get(EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_EditorLibrary());
		audio->Play(file, playsettings);
	}
}



ExportFileRet MP3AssetFile::ExportFile(const Path& path, const ExportFileContext& Item)
{
	return ExportFileRet();
}
EditorEnd
