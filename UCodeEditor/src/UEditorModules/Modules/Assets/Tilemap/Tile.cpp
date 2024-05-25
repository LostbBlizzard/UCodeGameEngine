#include "TilePalette.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include "Tile.hpp"
EditorStart

TileAssetFile::TileAssetFile()
{
	CanHaveLiveingAssets = true;
	FileExtWithDot = TileData::FileExtDot;
}

TileAssetFile::~TileAssetFile()
{
}

bool TileAssetFile::Draw(UEditorAssetDataConext& Data, const Path& path)
{
	return false;
}
ExportFileRet TileAssetFile::ExportFile(const Path& path, const ExportFileContext& Item)
{
	return ExportFileRet();
}
Optional<GetUIDInfo> TileAssetFile::GetFileUID(UEditorGetUIDContext& context)
{
	TileData palette;
	if (TileData::FromFile(palette, context.AssetPath))
	{
		GetUIDInfo info;
		info._MainAssetID = palette._UID;

		return info;
	}
	return Optional<GetUIDInfo>();
}
TileAssetFile::Liveing::Liveing()
{

}
TileAssetFile::Liveing::~Liveing()
{

}
		
void TileAssetFile::Liveing::Init(const UEditorAssetFileInitContext& Context)
{
	auto runprojectdata = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
	if (!TileData::FromFile(_Asset._Base, this->FileFullPath))
	{

		auto p = FileFullPath.generic_string();
		auto relpath = p.substr(runprojectdata->GetAssetsDir().generic_string().size());
		UCodeGEError("Unable to Read/Parse for " << relpath << " Failed");
	}
	else
	{
		auto& assetindex = runprojectdata->Get_AssetIndex();
		if (!assetindex.FindFileUsingID(_Asset._Base._UID).has_value())
		{
			EditorIndex::IndexFile file;

			auto assetdir = runprojectdata->GetAssetsDir();
			file.RelativePath = FileFullPath.generic_string().substr(assetdir.generic_string().size());
			file.RelativeAssetName = file.RelativePath;
			file.UserID = _Asset._Base._UID;
			assetindex._Files.push_back(std::move(file));
		}
	}

}

bool TileAssetFile::Liveing::DrawButtion(const UEditorAssetDrawButtionContext& Item)
{
	return ImGuIHelper::ImageButton(Item.ObjectPtr, AppFiles::sprite::TilePalette, *(ImVec2*)&Item.ButtionSize);

}
void TileAssetFile::Liveing::DrawInspect(const UEditorAssetDrawInspectContext& Item)
{
	ImGui::BeginDisabled(true);


	String tep = "Tilemap/Tile";
	ImGuIHelper::InputText("Type", tep);
	ImGui::SameLine();
	auto imageh = ImGui::GetFrameHeight();
	ImGuIHelper::Image(AppFiles::sprite::TilePalette, { imageh ,imageh });


	String tep2 = FileFullPath.filename().replace_extension("").generic_string();
	ImGuIHelper::InputText("Name", tep2);


	ImGui::EndDisabled();

}
NullablePtr<UCode::Asset> TileAssetFile::Liveing::LoadAsset(const LoadAssetContext& Item)
{
	return _Asset.GetAsset();
}
void TileAssetFile::Liveing::FileUpdated()
{
	if (!TileData::FromFile(_Asset._Base, this->FileFullPath))
	{
		auto runprojectdata = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();

		auto p = FileFullPath.generic_string();
		auto relpath = p.substr(runprojectdata->GetAssetsDir().generic_string().size());
		UCodeGEError("Unable to Read/Parse for " << relpath << " Failed");
	}
}
void TileAssetFile::Liveing::SaveFile(const UEditorAssetFileSaveFileContext& Context)
{
	auto runprojectdata = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();

	if (!TileData::ToFile(this->FileFullPath, _Asset._Base, runprojectdata->Get_ProjData()._SerializeType))
	{
		auto p = FileFullPath.generic_string();
		auto relpath = p.substr(runprojectdata->GetAssetsDir().generic_string().size());

		UCodeGEError("Unable to Saveing for " << relpath << " Failed");
	}

}


EditorEnd


