#include "TilePalette.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include "Tile.hpp"
#include "Imgui/imgui_internal.h"
EditorStart

TileAssetFile::TileAssetFile()
{
	CanHaveLiveingAssets = true;
	CallLiveingAssetsWhenUpdated = true;
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
		_Asset.Uid = _Asset._Base._UID;
	}

}

bool TileAssetFile::Liveing::DrawButtion(const UEditorAssetDrawButtionContext& Item)
{
	UCode::Sprite* thumbnail = nullptr;

	bool hasSprite = false;
	auto& spriteptr = _Asset._Base._Data.Sprite;

	if (!spriteptr.Has_Asset() && spriteptr.Has_UID())
	{
		auto app = EditorAppCompoent::GetCurrentEditorAppCompoent();

		auto opasset = app->Get_AssetManager()->FindOrLoad_t<UC::SpriteAsset>(spriteptr.Get_UID());
		if (opasset.has_value())
		{
			spriteptr = opasset.value().value()->GetManaged();
		}
	}
	hasSprite = spriteptr.Has_Asset();

	auto predrawpos = ImGui::GetCursorPos();
	if (hasSprite)
	{
		{
			auto imagescale = 3;
			auto minimagesize = Item.ButtionSize / imagescale;
			auto predrawasucode = Vec2(predrawpos.x, predrawpos.y);

			auto newcursorpos = predrawasucode;
			newcursorpos += {Item.ButtionSize.X - (minimagesize.X / 2), 0};

			ImGui::SetCursorPos(ImVec2(newcursorpos.X, newcursorpos.Y));

			ImGuIHelper::Image(AppFiles::sprite::TileAsset, *(ImVec2*)&minimagesize);

			ImGui::SetCursorPos(predrawpos);
		}

		thumbnail = spriteptr.Get_Asset();
	}
	else
	{

		thumbnail = AppFiles::GetSprite(AppFiles::sprite::TileAsset);
	}


	bool r = ImGuIHelper::ImageButton(Item.ObjectPtr, thumbnail, *(ImVec2*)&Item.ButtionSize);
	if (ImGui::BeginDragDropSource())
	{
		static Path tepAssetPath;
		tepAssetPath = this->FileFullPath.native();
		Path* p = &tepAssetPath;
		bool OnDropable = ImGui::SetDragDropPayload(DragAndDropType_AssetPath, &p, sizeof(UCode::Path*));

		auto& g = *GImGui;
		auto ImageHight = ImGui::GetFrameHeight();


		Vec2 imagesize = { ImageHight,ImageHight };

		auto tilename = this->FileFullPath.filename().replace_extension().generic_string();
		if (OnDropable)
		{
			String Text = "Drop " + tilename + " Here?";

			ImGuIHelper::Text(Text);
		}
		else
		{
			ImGuIHelper::Text(tilename);
		}

		ImGui::SameLine();
		ImGuIHelper::Image(thumbnail, *(ImVec2*)&imagesize);
		
		ImGui::EndDragDropSource();
	}

	return r;
}
		
bool TileAssetFile::Liveing::ShouldBeUnloaded(const UEditorAssetShouldUnloadContext& Context)
{
	return _Asset.Get_Managed().GetCounter() == 1;
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

	ImGui::Separator();


	auto& Data = _Asset._Base._Data;
	ImGuIHelper_Asset::AsssetField("Sprite", Data.Sprite);
	ImGuIHelper::ColorField(StringView("Color"), Data.Color);
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


