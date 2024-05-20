#include "UCodeAssetFile.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include "Helper/UserSettings.hpp"
#include "UCodeLang/LangCore/LangTypes.hpp"
#include "EditorWindows/DragAndDropNames.hpp"
EditorStart
UCodeAssetFile::UCodeAssetFile()
{
	FileExtWithDot = UCodeLang::FileExt::SourceFileWithDot;
}

bool UCodeAssetFile::Draw(UEditorAssetDataConext& Data, const Path& path)
{
	bool Value = ImGuIHelper::ImageButton(Data.ObjectPtr, AppFiles::sprite::ULangScript, *(ImVec2*)&Data.ButtionSize);

	ImGuiDragDropFlags src_flags = 0;
	//src_flags |= ImGuiDragDropFlags_::ImGuiDragDropFlags_AcceptBeforeDelivery;
	if (ImGui::BeginDragDropSource(src_flags))
	{
		ImGuIHelper::Text(StringView("Moving .uc"));
		auto Tep = &path;
		ImGui::SetDragDropPayload(DragAndDropType_ULangFilePath, &Tep, sizeof(const Path*));
		ImGui::EndDragDropSource();
	}

	if (Value)
	{
		const auto& Settings = UserSettings::GetSettings();
		String Args = UserSettings::GetOpenFileStringAsArgs(Settings, path, ImGuIHelper_Asset::ProjectData->Get_ProjectDir(), 0, 0);
		String Exe = Settings.CodeEditorPath;
		FileHelper::OpenExe(Exe, Args);
	}
	return false;
}
EditorEnd
