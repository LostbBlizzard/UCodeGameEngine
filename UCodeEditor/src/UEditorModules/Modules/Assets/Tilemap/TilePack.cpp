#include "TilePack.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include "../Art/PNGAssetFile.hpp"
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


	const char* PopUpName = "TilePackPopup";

	static bool isopen = false;
	if (ImGui::Button("Open", { ImGui::GetContentRegionAvail().x,ImGui::GetFrameHeight() }))
	{
		isopen = true;
		ImGui::OpenPopup(PopUpName);
	}

	ImGui::Separator();

	auto app = EditorAppCompoent::GetCurrentEditorAppCompoent();
	auto runprojectdata = app->Get_RunTimeProjectData();
	auto assetmanger = app->Get_AssetManager();
	if (isopen && ImGui::Begin(PopUpName))
	{

		struct TilePackPopupWindowData
		{

		};
		static TilePackPopupWindowData PopupWindow;

		if (_Data._BaseTexture.Has_UID() && !_Data._BaseTexture.Has_Asset())
		{
			auto val = assetmanger->FindOrLoad_t<UC::TextureAsset>(_Data._BaseTexture.Get_UID());

			if (val.has_value())
			{
				_Data._BaseTexture = val.value()->GetManaged();
			}
		}

		//ImGui::Separator();

		bool hastexture = _Data._BaseTexture.Has_Asset();

		ImGuiWindowFlags flags = ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar;
		flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse;

		ImGui::Columns(2);
        ImGui::SetColumnOffset(1, 150.0f);
		{
			ImGui::BeginChild("test2", {}, 0, flags);

			ImVec2 size = { ImGui::GetFrameHeight(),ImGui::GetFrameHeight() };
			for (auto& Item : _Data.List)
			{

				UCode::Sprite* spr = nullptr;
				auto& spriteptr = Item._Data._Data.Sprite;
				if (spriteptr.Has_UID() && !spriteptr.Has_Asset())
				{
					auto tep = app->Get_AssetManager()->FindOrLoad_t<UC::SpriteAsset>(spriteptr.Get_UID());
					if (tep.has_value())
					{
						spriteptr = tep.value()->GetManaged();
					}
				}
				
				if (spriteptr.Has_Asset())
				{
					spr = spriteptr.Get_Asset();
				}

				if (spr == nullptr)
				{
					spr = AppFiles::GetSprite(AppFiles::sprite::TileAsset);
				}
				ImGuIHelper::Image(spr,size);	
				ImGui::SameLine();

				if (ImGui::Selectable(Item._Name.c_str(), false))
				{

				}
			}
			ImGui::EndChild();
		}
		ImGui::NextColumn();
		{	
			bool showtilepreview = false;
			
			static bool shouldcloseAtlastSettings = false;
			ImGuIHelper_Asset::AsssetField("Texture Atlas", this->_Data._BaseTexture);


			auto texturesettingsop = PNGAssetFile::GetTextureSettings(_Data._BaseTexture.Get_UID());
			if (_Data._BaseTexture.Has_Asset()) 
			{
				if (shouldcloseAtlastSettings)
				{
					shouldcloseAtlastSettings = false;

					ImGui::SetNextItemOpen(false);
				}
				if (texturesettingsop.has_value() && ImGui::TreeNode("Atlas Settings"))
				{
					showtilepreview = true;

					if (ImGui::Button("Make Tiles from Atlas",{ImGui::GetContentRegionAvail().x,ImGui::GetFrameHeight()}))
					{
						auto& texturesettings = texturesettingsop.value();
						shouldcloseAtlastSettings = true;

						_Data.List.reserve(texturesettings->sprites.size());
						_Assets.reserve(texturesettings->sprites.size());
						_Assets.clear();

						auto run = app->Get_RunTimeProjectData();
						auto& index = run->Get_AssetIndex();
						for (size_t i = 0; i < _Data.List.size(); i++)
						{
							auto& item = texturesettings->sprites[i];
							auto& LastOut = _Data.List[i];

							LastOut._Name = item.spritename;
							LastOut._Data._Data.Sprite = item.uid;

							auto fileop = index.FindFileUsingID(LastOut._Data._UID);

							if (fileop.has_value())
							{
								auto& file = fileop.value();

								file.RelativeAssetName = file.RelativePath;
								file.RelativeAssetName += EditorIndex::SubAssetSeparator;
								file.RelativeAssetName += LastOut._Name;
							}
						}
						auto relpath = FileHelper::ToRelativePath(run->GetAssetsDir(), this->FileFullPath).generic_string();
						for (size_t i = _Data.List.size(); i < texturesettings->sprites.size(); i++)
						{
							auto& item = texturesettings->sprites[i];

							TileDataPack::PackTile newtile;
							newtile._Name = item.spritename;
							newtile._Data._UID = app->Get_RunTimeProjectData()->GetNewUID();
							newtile._Data._Data.Sprite = item.uid;	

							EditorIndex::IndexFile file;
							file.UserID = newtile._Data._UID;
							file.RelativePath = relpath;
							file.RelativeAssetName = relpath + EditorIndex::SubAssetSeparator + newtile._Name;

							
							_Data.List.push_back(std::move(newtile));
							index._Files.push_back(std::move(file));
						}
						if (_Data.List.size() > texturesettings->sprites.size())
						{
							size_t remove = _Data.List.size() - texturesettings->sprites.size();
							for (size_t i = 0; i < remove; i++)
							{
								auto& Item = _Data.List.back();
								_Data.List.pop_back();

								index.RemoveIndexFileWithUID(Item._Data._UID);
							}
						}

					}
					ImGui::TreePop();
				}

			}

			ImGui::BeginChild("test", {}, 0, flags);

			if (showtilepreview)
			{
				if (hastexture && texturesettingsop.has_value())
				{
					auto texture = _Data._BaseTexture.Get_Asset();
					auto texturesettings = texturesettingsop.value().value();

					auto list = ImGui::GetWindowDrawList();
					auto startpos = ImGui::GetWindowPos();



					auto maxsize = ImGui::GetContentRegionAvail();

					Vec2 objectsize = { (f32)texture->Get_Width(),(f32)texture->Get_Height() };

					auto AspectRatio = objectsize.X / objectsize.Y;
					float texturescale = 10 * 0.5f;
					Vec2 dragoffset;
					objectsize.X *= texturescale;
					objectsize.Y *= texturescale;

					ImU32 texturbordercolor = ImGuIHelper::ColorToImguiU32(Color32(217, 51, 0));
					ImU32 spritebordercolor = ImGuIHelper::ColorToImguiU32(Color32());
					ImU32 selecedspritebordercolor = ImGuIHelper::ColorToImguiU32(Color32(204, 161, 4));


					for (size_t i = 0; i < texturesettings->sprites.size(); i++)
					{
						auto& Item = texturesettings->sprites[i];

						float HOffset = Item.offset.Y * texturescale;
						float WOffset = Item.offset.X * texturescale;
						float HSize = Item.size.Y;
						float WSize = Item.size.X;

						auto diffx = dragoffset.X / texturescale;
						auto diffy = dragoffset.Y / texturescale;

						bool isseleced = false;

						auto color = isseleced ? selecedspritebordercolor : spritebordercolor;

						list->AddRect({ startpos.x + WOffset + diffx ,
							startpos.y + HOffset + diffy },
							{ startpos.x + WOffset + diffx + (WSize * texturescale),
							startpos.y + HOffset + diffy + (HSize * texturescale) }, color);
					}

					ImGuIHelper::Image(texture, ImVec2(objectsize.X, objectsize.Y));
				}
			}
			else
			{

			}
			ImGui::EndChild();
		}


		ImGui::End();
	}
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


