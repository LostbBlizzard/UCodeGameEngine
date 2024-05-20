#include "UColorAsset.hpp"

EditorStart
UColorAssetFile::UColorAssetFile()
{
	FileExtWithDot = ColorData::FileExtDot;
	CanHaveLiveingAssets = true;
}

UColorAssetFile::LiveingColor::LiveingColor()
{

}

void UColorAssetFile::LiveingColor::Init(const UEditorAssetFileInitContext& Context)
{
	ColorData::ReadFromFile(FileFullPath, _ColorInfoFromFile);

	UCode::Color32 Color32 = (UCode::Color32)_ColorInfoFromFile._Color;
	_Texture = Unique_ptr<UCode::Texture>(new UCode::Texture(1, 1, &Color32));
}

void UColorAssetFile::LiveingColor::SaveFile(const UEditorAssetFileSaveFileContext& Context)
{
	ColorData::WriteToFile(FileFullPath, _ColorInfoFromFile, UCode::USerializerType::Readable);

}

bool UColorAssetFile::LiveingColor::DrawButtion(const UEditorAssetDrawButtionContext& Item)
{
	if (WasUpdated)
	{
		UCode::Color32 Color32 = (UCode::Color32)_ColorInfoFromFile._Color;
		_Texture = Unique_ptr<UCode::Texture>(new UCode::Texture(1, 1, &Color32));
	}

	return ImGuIHelper::ImageButton(Item.ObjectPtr, _Texture.get(), *(ImVec2*)&Item.ButtionSize);
}

void UColorAssetFile::LiveingColor::DrawInspect(const UEditorAssetDrawInspectContext& Item)
{
	ImGui::BeginDisabled(true);


	ImGuIHelper::Image(AppFiles::sprite::Uility_image, { 20 ,20 });
	ImGui::SameLine();

	String tep = "Color";
	Item.Drawer->StringField("Type", tep);

	String tep2 = FileFullPath.filename().replace_extension("").generic_string();
	Item.Drawer->StringField("Name", tep2);


	ImGui::EndDisabled();

	ImGui::Separator();

	if (Item.Drawer->ColorField("Color", _ColorInfoFromFile._Color))
	{
		WasUpdated = true;
	}
}
EditorEnd
