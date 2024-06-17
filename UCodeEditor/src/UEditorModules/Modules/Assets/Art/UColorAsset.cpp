#include "UColorAsset.hpp"
#include "Editor/EditorAppCompoent.hpp"
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
	auto runinfo = UCodeEditor::EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
	ColorData::ReadFromFile(FileFullPath, _ColorInfoFromFile,runinfo->Get_ProjData()._SerializeType);

	UCode::Color32 Color32 = (UCode::Color32)_ColorInfoFromFile._Color;
	_Texture = Unique_ptr<UCode::Texture>(new UCode::Texture(1, 1, &Color32));
}

void UColorAssetFile::LiveingColor::SaveFile(const UEditorAssetFileSaveFileContext& Context)
{
	auto runinfo = UCodeEditor::EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
	ColorData::WriteToFile(FileFullPath, _ColorInfoFromFile,runinfo->Get_ProjData()._SerializeType);

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
	ImGuIHelper::InputText("Type", tep);

	String tep2 = FileFullPath.filename().replace_extension("").generic_string();
	ImGuIHelper::InputText("Name", tep2);


	ImGui::EndDisabled();

	ImGui::Separator();

	if (ImGuIHelper::ColorField("Color", _ColorInfoFromFile._Color))
	{
		WasUpdated = true;
	}
}
EditorEnd
