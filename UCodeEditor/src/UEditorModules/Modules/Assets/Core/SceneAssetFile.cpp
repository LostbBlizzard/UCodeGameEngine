#include "SceneAssetFile.hpp"
EditorStart
ScencAssetFile::ScencAssetFile()
{
	FileExtWithDot = UCode::Scene2dData::FileExtDot;
}

bool ScencAssetFile::Draw(UEditorAssetDataConext& Data, const Path& path)
{
	if (ImGuIHelper::ImageButton(Data.ObjectPtr, AppFiles::sprite::Scene2dData, *(ImVec2*)&Data.ButtionSize))
	{




		return false;
	}


	if (ImGui::BeginDragDropSource())
	{
		const Path* Value = &path;
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
}

ExportFileRet ScencAssetFile::ExportFile(const Path& path, const ExportFileContext& Item)
{
	UCodeGEStackFrame("SceneAsset:Export");
	std::filesystem::copy_file(path, Item.Output, std::filesystem::copy_options::overwrite_existing);

	UCode::Scene2dData V;
	UCode::Scene2dData::FromFile(V, path);

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
	if (UCode::Scene2dData::FromFile(V, context.AssetPath))
	{
		GetUIDInfo val;
		val._MainAssetID = V._UID;

		return val;
	}
	return {};
}
EditorEnd
