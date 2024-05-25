#include "TilePack.hpp"
#include "Editor/EditorAppCompoent.hpp"
EditorStart

TilePackAssetFile::TilePackAssetFile()
{
	CanHaveLiveingAssets = true;
	this->FileExtWithDot = TileDataPack::FileExtDot;
}
TilePackAssetFile::Liveing::Liveing()
{

 }
TilePackAssetFile::Liveing::~Liveing()
{


}
void TilePackAssetFile::Liveing::Init(const UEditorAssetFileInitContext& Context)
{
	auto runprojectdata = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
	if (!TileDataPack::FromFile(_Data, this->FileFullPath))
	{
		auto p = FileFullPath.generic_string();
		auto relpath = p.substr(runprojectdata->GetAssetsDir().generic_string().size());
		UCodeGEError("Unable to Read/Parse for " << relpath << " Failed");
	}
	else
	{
		auto& assetindex = runprojectdata->Get_AssetIndex();

		for (auto& Item : _Data.List) 
		{
			if (!assetindex.FindFileUsingID(Item._Data._UID).has_value())
			{
				EditorIndex::IndexFile file;

				auto assetdir = runprojectdata->GetAssetsDir();
				file.RelativePath = FileFullPath.generic_string().substr(assetdir.generic_string().size());
				file.RelativePath += EditorIndex::SubAssetSeparator + Item._Name;
				file.RelativeAssetName = file.RelativePath;
				file.UserID = Item._Data._UID;
				assetindex._Files.push_back(std::move(file));
			}
		}
	}

}
bool TilePackAssetFile::Liveing::DrawButtion(const UEditorAssetDrawButtionContext& Item)
{
	return ImGuIHelper::ImageButton(Item.ObjectPtr, AppFiles::sprite::TilePack, *(ImVec2*)&Item.ButtionSize);
}
void TilePackAssetFile::Liveing::DrawInspect(const UEditorAssetDrawInspectContext& Item) 
{
	ImGui::BeginDisabled(true);

	String tep = "Tilemap/TilePack";
	ImGuIHelper::InputText("Type", tep);
	ImGui::SameLine();
	auto imageh = ImGui::GetFrameHeight();
	ImGuIHelper::Image(AppFiles::sprite::TilePack, { imageh ,imageh });


	String tep2 = FileFullPath.filename().replace_extension("").generic_string();
	ImGuIHelper::InputText("Name", tep2);


	ImGui::EndDisabled();

	ImGui::Separator();



}
NullablePtr<UCode::Asset> TilePackAssetFile::Liveing::LoadAsset(const LoadAssetContext& Item) 
{
	for (size_t i = 0; i < this->_Data.List.size(); i++)
	{
		auto& ListItem = _Data.List[i];
		auto& Asset = _Assets[i];

		if (ListItem._Data._UID == Item._AssetToLoad)
		{
			return Asset.GetAsset();
		}
	}
	return {};
}
void TilePackAssetFile::Liveing::FileUpdated() 
{
	if (!TileDataPack::FromFile(_Data, this->FileFullPath))
	{
		auto runprojectdata = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();

		auto p = FileFullPath.generic_string();
		auto relpath = p.substr(runprojectdata->GetAssetsDir().generic_string().size());
		UCodeGEError("Unable to Read/Parse for " << relpath << " Failed");
	}
}
void TilePackAssetFile::Liveing::SaveFile(const UEditorAssetFileSaveFileContext& Context) 
{
	auto runprojectdata = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();

	if (!TileDataPack::ToFile(this->FileFullPath,_Data, runprojectdata->Get_ProjData()._SerializeType))
	{
		auto p = FileFullPath.generic_string();
		auto relpath = p.substr(runprojectdata->GetAssetsDir().generic_string().size());

		UCodeGEError("Unable to Saveing for " << relpath << " Failed");
	}
	_Assets.resize(_Data.List.size());
}
ExportFileRet TilePackAssetFile::ExportFile(const Path& path, const ExportFileContext& Item)
{
	return ExportFileRet();
}

Optional<GetUIDInfo> TilePackAssetFile::GetFileUID(UEditorGetUIDContext& context)
{
	TileDataPack palette;
	if (TileDataPack::FromFile(palette, context.AssetPath))
	{
		GetUIDInfo info;
		//info._MainAssetID = palette._UID;

		info._SubAssets.resize(palette.List.size());
		for (auto& Item : palette.List)
		{
			GetSubAssetData val;
			val._ID = Item._Data._UID;
			val._SubAssetName = Item._Name;
			val._SubAssetName += TileData::FileExtDot;
		
			info._SubAssets.push_back(std::move(val));
		}
		return info;
	}
	return Optional<GetUIDInfo>();
}
EditorEnd


