#include "UModuleFile.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include "ULang/UCodeDrawer.hpp"
EditorStart

UModuleFile::UModuleFile()
{
	FileExtWithDot = UCodeLang::ModuleFile::FileExtWithDot;
	CanHaveLiveingAssets = true;
	CallLiveingAssetsWhenUpdated = true;
}

void UModuleFile::Liveing::Init(const UEditorAssetFileInitContext& Context)
{
	if (!UCodeLang::ModuleFile::FromFile(&_Module, FileFullPath))
	{
		auto runprojectdata = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
		auto p = FileFullPath.generic_string();
		auto relpath = p.substr(runprojectdata->GetAssetsDir().generic_string().size());

		UCodeGEError("Unable to open/read " << relpath << " Failed")
	}

}

void UModuleFile::Liveing::FileUpdated()
{
	if (justsaved == true)
	{
		justsaved = false;
		return;
	}
	auto runprojectdata = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
	_Module = UCodeLang::ModuleFile();
	if (!UCodeLang::ModuleFile::FromFile(&_Module, FileFullPath))
	{
		auto p = FileFullPath.generic_string();
		auto relpath = p.substr(runprojectdata->GetAssetsDir().generic_string().size());
		UCodeGEError("Saveing Asset for " << relpath << " Failed")
	}
	else
	{
		auto& index = runprojectdata->Get_AssetIndex();

		auto p = FileFullPath.generic_string();
		auto relpath = p.substr(runprojectdata->GetAssetsDir().generic_string().size());
		auto op = index.FindFileRelativeAssetName(relpath);

		if (op.has_value())
		{
			auto& ind = op.value();
			ind.FileLastUpdatedTime = std::filesystem::last_write_time(FileFullPath).time_since_epoch().count();
			ind.FileSize = std::filesystem::file_size(FileFullPath);
		}
	}
}

void UModuleFile::Liveing::SaveFile(const UEditorAssetFileSaveFileContext& Context)
{
	if (updated)
	{
		updated = false;
		justsaved = true;

		if (!UCodeLang::ModuleFile::ToFile(&_Module, FileFullPath))
		{
			UCodeGEError("Unable to Save \"" << FileFullPath << "\"")
		}
	}
}

bool UModuleFile::Liveing::DrawButtion(const UEditorAssetDrawButtionContext& Item)
{
	return ImGuIHelper::ImageButton(Item.ObjectPtr, AppFiles::sprite::ULangModule, *(ImVec2*)&Item.ButtionSize);;
}

void UModuleFile::Liveing::DrawInspect(const UEditorAssetDrawInspectContext& Item)
{
	ImGui::BeginDisabled(true);

	ImGuIHelper::Image(AppFiles::sprite::ULangModule, { 20 ,20 });
	ImGui::SameLine();

	String tep = "UCodeLang/ModuleFile";
	ImGuIHelper::InputText("Type", tep);

	String tep2 = FileFullPath.filename().replace_extension("").generic_string();
	ImGuIHelper::InputText("Name", tep2);


	ImGui::EndDisabled();

	ImGui::Separator();

	if (ImGuIHelper::InputText("BaseNameSpace", _Module.ModuleNameSpace))
	{
		updated = true;
	}

	bool Unsafe = !_Module.RemoveUnSafe;

	if (ImGuIHelper::BoolEnumField("Allow Unsafe", Unsafe))
	{
		_Module.RemoveUnSafe = !Unsafe;
		updated = true;
	}

	ImGui::Separator();
	if (UCodeDrawer::DrawModuleFileDeps("Dependencies", _Module.ModuleDependencies))
	{
		updated = true;
	}
}
EditorEnd
