#include "SceneAssetFile.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include "EditorWindows/BasicWindows/GameEditorWindow.hpp"
EditorStart
ScencAssetFile::ScencAssetFile()
{
	FileExtWithDot = UCode::Scene2dData::FileExtDot;
	CanHaveLiveingAssets = true;
}


ExportFileRet ScencAssetFile::ExportFile(const Path& path, const ExportFileContext& Item)
{
	UCodeGEStackFrame("SceneAsset:Export");
	std::filesystem::copy_file(path, Item.Output, std::filesystem::copy_options::overwrite_existing);

	UCode::Scene2dData V;
	UCode::Scene2dData::FromFile(V, path,Item.ProjectSerializerType);

	UCode::GameRunTime runtime;
	auto scenc = UCode::Scene2dData::LoadScene(&runtime, V);
	UCode::Scene2dData::SaveScene(scenc, V, USerializerType::Bytes);

	//UCode::Scene2dData::ToFile(Item.Output, V, USerializerType::Bytes);

	ExportFileRet r;
	r._UID = V._UID;
	r.outpath = Item.Output;
	return r;
}

Optional<GetUIDInfo> ScencAssetFile::GetFileUID(UEditorGetUIDContext& context)
{
	UCode::Scene2dData V;
	if (UCode::Scene2dData::FromFile(V, context.AssetPath,context.ProjectSerializerType))
	{
		GetUIDInfo val;
		val._MainAssetID = V._UID;

		return val;
	}
	return {};
}
Unique_ptr<UEditorAssetFile> ScencAssetFile::GetMakeNewAssetFile()
{
	return std::make_unique<ScencAssetFile::LiveingAsset>();
}
void ScencAssetFile::LiveingAsset::Init(const UEditorAssetFileInitContext& Context) 
{

}
void ScencAssetFile::LiveingAsset::DrawInspect(const UEditorAssetDrawInspectContext& Item) 
{
	ImGui::BeginDisabled(true);

	String tep = "Core/SceneAsset";
	ImGuIHelper::InputText("Type", tep);
	ImGui::SameLine();
	auto imageh = ImGui::GetFrameHeight();
	ImGuIHelper::Image(AppFiles::sprite::TilePalette, { imageh ,imageh });


	String tep2 = FileFullPath.filename().replace_extension("").generic_string();
	ImGuIHelper::InputText("Name", tep2);


	ImGui::EndDisabled();

	ImGui::Separator();


	if (ImGui::Button("Open in GameEditor", { ImGui::GetContentRegionAvail().x,ImGui::GetFrameHeight() }))
	{
		TryLoadAsset();

		auto window = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_Window<GameEditorWindow>();
		window->OpenScencAtPathRemoveOthers(file._Base._UID);
	}
}

NullablePtr<UCode::Asset> ScencAssetFile::LiveingAsset::LoadAsset(const LoadAssetContext& Item)
{
	TryLoadAsset();
	if (Item._AssetToLoad == file._Base._UID)
	{
		return file.GetAsset();
	}
	return {};
}

void ScencAssetFile::LiveingAsset::TryLoadAsset()
{
	auto runprojectdata = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
	if (!UCode::Scene2dData::FromFile(file._Base, this->FileFullPath,runprojectdata->Get_ProjData()._SerializeType))
	{
		auto p = FileFullPath.generic_string();
		auto relpath = p.substr(runprojectdata->GetAssetsDir().generic_string().size());
		UCodeGEError("Unable to Read/Parse for " << relpath << " Failed");
	}
	else
	{
		file.Uid = file._Base._UID;
	}
}

bool ScencAssetFile::LiveingAsset::DrawButtion(const UEditorAssetDrawButtionContext& Item)
{
	bool  r = ImGuIHelper::ImageButton(this,AppFiles::sprite::Scene2dData, *(ImVec2*)&Item.ButtionSize);

	if (ImGui::BeginDragDropSource())
	{
		const Path* Value = &this->FileFullPath;
		bool OnDropable = ImGui::SetDragDropPayload(DragAndDropType_Scene2dPathType, &Value, sizeof(Path*));
		bool OnDropable2 = ImGui::SetDragDropPayload(DragAndDropType_AssetPath, &Value, sizeof(Path*));
		if (OnDropable || OnDropable2)
		{
			ImGuIHelper::Text(StringView("Drop Scene Here?"));
		}
		else
		{
			ImGuIHelper::Text(StringView("Draging Scene"));
		}

		ImGui::EndDragDropSource();
	}
	return r;
}

EditorEnd
