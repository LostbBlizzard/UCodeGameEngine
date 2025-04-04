#include "TilePack.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include "../Art/PNGAssetFile.hpp"
#include "Helper/UserSettings.hpp"
#include "Imgui/imgui_internal.h"
EditorStart

TilePackAssetFile::TilePackAssetFile()
{
	CanHaveLiveingAssets = true;
	CallLiveingAssetsWhenUpdated = true;
	this->FileExtWithDot = TileDataPack::FileExtDot;
}
TilePackAssetFile::Liveing::Liveing()
{
	HasSubAssets = true;

 }
TilePackAssetFile::Liveing::~Liveing()
{


}
void TilePackAssetFile::Liveing::Init(const UEditorAssetFileInitContext& Context)
{
	auto runprojectdata = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
	if (!TileDataPack::FromFile(_Data._Base, this->FileFullPath,runprojectdata->Get_ProjData()._SerializeType))
	{
		auto p = FileFullPath.generic_string();
		auto relpath = p.substr(runprojectdata->GetAssetsDir().generic_string().size());
		UCodeGEError("Unable to Read/Parse for " << relpath << " Failed");
	}
	else
	{
		_Data.Uid = _Data._Base._UID;
		OnFileLoaded();
	}

}
bool TilePackAssetFile::Liveing::DrawButtion(const UEditorAssetDrawButtionContext& Item)
{
	bool r = ImGuIHelper::ImageButton(Item.ObjectPtr, AppFiles::sprite::TilePack, *(ImVec2*)&Item.ButtionSize);

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
		ImGuIHelper::Image(AppFiles::sprite::TilePack, *(ImVec2*)&imagesize);
		
		ImGui::EndDragDropSource();
	}

	return r;
}

void DrawInspectTile(TileDataPtr  ptr, TileDataPack::PackTile& tile)
{
	if (!ptr.Has_Asset())
	{
		return;
	}
	ImGui::BeginDisabled(true);

	String tep = "Tilemap/Tile";
	ImGuIHelper::InputText("Type", tep);
	ImGui::SameLine();
	auto imageh = ImGui::GetFrameHeight();
	ImGuIHelper::Image(AppFiles::sprite::TilePalette, { imageh ,imageh });


	String tep2 = tile._Name;
	ImGuIHelper::InputText("Name", tep2);


	ImGui::EndDisabled();

	ImGui::Separator();


	auto& Data = ptr.Get_Asset()->_Base;
	if (ImGuIHelper_Asset::AsssetField("Sprite", Data.Sprite))
	{
		tile._Data._Base.Sprite = Data.Sprite;
	}
	if (ImGuIHelper::ColorField(StringView("Color"), Data.color))
	{
		tile._Data._Base.color = Data.color;
	}

}
void TilePackAssetFile::Liveing::RenameTile(size_t Index, const String& newname)
{
	auto& Item = _Data._Base.List[Index];
	auto& AssetItem = _Assets[Index];

	Item._Name = newname;

	auto runtimeproject = UCodeEditor::EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
	auto& assetindex = runtimeproject->Get_AssetIndex();
	auto f = assetindex.FindFileUsingID(Item._Data._UID);
	if (f.has_value())
	{
		auto& file = f.value();
		file.RelativeAssetName = file.RelativePath + EditorIndex::SubAssetSeparator + newname + TileData::FileExtDot;

		if (AssetItem.has_value())
		{
			auto& asset = AssetItem.value();
			asset.ObjectPath = file.RelativeAssetName;
		}
	}
}
void TilePackAssetFile::Liveing::RemoveTile(size_t Index)
{
	auto& Item = _Data._Base.List[Index];
	auto& AssetItem = _Assets[Index];

	auto runtimeproject = UCodeEditor::EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
	auto& assetindex = runtimeproject->Get_AssetIndex();
	assetindex.RemoveIndexFileWithUID(Item._Data._UID);

	_Assets.erase(_Assets.begin() + Index);
	_Data._Base.List.erase(_Data._Base.List.begin() + Index);
}
void TilePackAssetFile::Liveing::LoadAssetAt(size_t Index)
{
	auto& Item = _Data._Base.List[Index];
	auto& AssetItem = _Assets[Index];


	TileDataAsset data;
	data._UID =Item._Data._UID;
	data.Uid = data._UID;
	data._Base = Item._Data._Base;


	AssetItem = std::move(data);
}
void TilePackAssetFile::Liveing::OnFileLoaded()
{
	auto runprojectdata = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
	auto& assetindex = runprojectdata->Get_AssetIndex();

	for (auto& Item : _Data._Base.List)
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
	_Assets.resize(_Data._Base.List.size());
}
bool TilePackAssetFile::Liveing::ShouldBeUnloaded(const UEditorAssetShouldUnloadContext& Context)
{
	for (auto& Item : _Assets)
	{

		if (Item.has_value())
		{
			bool isinuse = Item.value().GetManaged().GetCounter() > 1;
			if (isinuse)
			{
				return false;
			}
		}
	}
	return true;
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
	bool focuswin = false;
	if (ImGui::Button("Open", { ImGui::GetContentRegionAvail().x,ImGui::GetFrameHeight() }))
	{
		isopen = true;
		focuswin = true;
	}

	ImGui::Separator();

	auto app = EditorAppCompoent::GetCurrentEditorAppCompoent();
	auto runprojectdata = app->Get_RunTimeProjectData();
	auto assetmanger = app->Get_AssetManager();

	if (ImGui::Begin(PopUpName, &isopen))
	{
		if (focuswin)
		{
			ImGui::SetWindowFocus();
		}
		struct TilePackPopupWindowData
		{

		};
		static TilePackPopupWindowData PopupWindow;

		auto& _Data = this->_Data._Base;
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

		auto& settings = UserSettings::GetSettings();
		
		ImGui::Columns(2,nullptr,false);

		ImGui::SetColumnOffset(1, 150.0f);
		{
			ImGui::BeginChild("test2", {}, 0, flags);	
	
			{
				ImVec2 size = { ImGui::GetFrameHeight(),ImGui::GetFrameHeight() };
				for (size_t i = 0; i < _Data.List.size(); i++)
				{
					auto& Item = _Data.List[i];
					auto& AssetItem = _Assets[i];

					UCode::Sprite* spr = nullptr;
					auto& spriteptr = Item._Data._Base.Sprite;
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
					ImGuIHelper::Image(spr, size);
					ImGui::SameLine();
					static Optional<size_t> isrenameing;

					bool isrenamemode = isrenameing.has_value() && isrenameing.value() == i;
					if (isrenamemode)
					{
						static String newname;
						newname = Item._Name;
						ImGui::SetKeyboardFocusHere();
						bool renameing = true;

						auto style = ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_ChildBg);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, style);
	
						ImGuIHelper::DrawRenameName(newname, renameing);
						
						ImGui::PopStyleColor();
						if (renameing == false)
						{
							RenameTile(i,newname);
							isrenameing = {};
						}
					}
					else
					{
						if (ImGui::Selectable(Item._Name.c_str(), false))
						{
							if (!AssetItem.has_value())
							{
								LoadAssetAt(i);
							}
							auto win = app->Get_Window<InspectWindow>();
							InspectWindow::InspectData data;
							data._Data = UC::AnyManagedPtr::As(AssetItem.value().Get_Managed());
							data._Data2 = &Item;
							data._Draw = [](InspectWindow::InspectDrawer& draw)
								{
									TileDataPtr ptr = UC::AnyManagedPtr::As<TileDataAsset>(draw.GetPtr());
									TileDataPack::PackTile* tile = (TileDataPack::PackTile*)draw.GetData();
									DrawInspectTile(ptr, *tile);
								};
							win->Inspect(data);
						}
					}
					bool isrename = false;
					bool iscopy = false;
					bool isdelete = false;
					if (ImGui::IsItemFocused())
					{
						if (settings.IsKeybindActive(KeyBindList::Rename))
						{
							isrename = true;
						}
						if (settings.IsKeybindActive(KeyBindList::Delete))
						{
							isdelete = true;
						}
						if (settings.IsKeybindActive(KeyBindList::Copy))
						{
							iscopy = true;
						}
					}

					ImGui::PushID(&Item);
					if (ImGuIHelper::BeginPopupContextItem("TileItem"))
					{
						String str = settings.KeyBinds[(size_t)KeyBindList::Rename].ToString();
						if (ImGui::MenuItem("Rename", str.c_str()) || settings.IsKeybindActive(KeyBindList::Rename))
						{
							isrename = true;
							ImGui::CloseCurrentPopup();
						}
						str = settings.KeyBinds[(size_t)KeyBindList::Copy].ToString();
						if (ImGui::MenuItem("Copy", str.c_str()) || settings.IsKeybindActive(KeyBindList::Copy))
						{
							iscopy = true;
							ImGui::CloseCurrentPopup();
						}
						str = settings.KeyBinds[(size_t)KeyBindList::Delete].ToString();
						if (ImGui::MenuItem("Delete", str.c_str()) || settings.IsKeybindActive(KeyBindList::Delete))
						{
							isdelete = true;
							ImGui::CloseCurrentPopup();
						}

						ImGui::EndPopup();
					}
					ImGui::PopID();
					if (isrename)
					{
						isrenameing = i;
					}
					if (isdelete)
					{
						RemoveTile(i);
					}
					if (iscopy)
					{
						Path subassetpath = Path(FileFullPath).native() + Path(EditorIndex::SubAssetSeparator).native()
							+ Path(Item._Name).native() + Path(TileData::FileExtDot).native();

						UserSettings::SetCopyBufferAsValue("AssetPath", subassetpath);
					}
				}
			}
			bool newtile = false;
			bool pastetile = false;
			bool canpaste = false;

			auto runtimeprojectdata = app->Get_RunTimeProjectData();
			{
				auto pathop =  UserSettings::ReadCopyBufferAs<Path>("AssetPath");
				if (pathop.has_value())
				{
					auto relpath = FileHelper::ToRelativePath(runtimeprojectdata->GetAssetsDir(), pathop.value()).generic_string();
					auto v = runtimeprojectdata->Get_AssetIndex().FindFileRelativeAssetName(relpath);
					if (v.has_value())
					{
						auto& Item = pathop.value();
						auto ext = Item.extension();
						if (ext == UCode::SpriteData::FileExtDot)
						{
							canpaste = true;
						}
						if (ext == UCode::TileData::FileExtDot)
						{
							canpaste = true;
						}
					}
				}
			}

			
			ImGui::InvisibleButton("TileSpace", ImGui::GetContentRegionAvail());
			
			if (ImGui::IsItemFocused())
			{
				if (settings.IsKeybindActive(KeyBindList::New))
				{
					newtile= true;
				}
				if (canpaste && settings.IsKeybindActive(KeyBindList::Paste))
				{
					pastetile = true;
				}
			}
			if (ImGuIHelper::BeginPopupContextItem("TileList"))
			{
				String str = settings.KeyBinds[(size_t)KeyBindList::New].ToString();
				if (ImGui::MenuItem("New", str.c_str()) || settings.IsKeybindActive(KeyBindList::New))
				{
					newtile = true;
					ImGui::CloseCurrentPopup();
				}

				str = settings.KeyBinds[(size_t)KeyBindList::Paste].ToString();
				if (ImGui::MenuItem("Paste", str.c_str(),nullptr,canpaste) || settings.IsKeybindActive(KeyBindList::Paste))
				{
					pastetile = true;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			if (newtile)
			{
				TileDataPack::PackTile tile;
				tile._Name = "New Tile";
				NewTile(std::move(tile));
			}
			if (pastetile)
			{
				auto pathop = UserSettings::ReadCopyBufferAs<Path>("AssetPath");
				if (pathop.has_value())
				{
					auto relpath = FileHelper::ToRelativePath(runtimeprojectdata->GetAssetsDir(), pathop.value()).generic_string();
					auto v = runtimeprojectdata->Get_AssetIndex().FindFileRelativeAssetName(relpath);
					if (v.has_value())
					{
						auto& file = v.value();
						auto& Item = pathop.value();

						auto ext = Item.extension();
						if (ext == UCode::SpriteData::FileExtDot)
						{
							TileDataPack::PackTile tile;
							tile._Name = "New Tile";//Should be SpriteName

							if (file.UserID.has_value())
							{
								tile._Data._Base.Sprite = file.UserID.value();
							}
							NewTile(std::move(tile));
						}
						if (ext == UCode::TileData::FileExtDot)
						{
							TileDataPack::PackTile tile;
							tile._Name = "New Tile";//Should be SpriteName

							if (file.UserID.has_value()) 
							{
								auto tiledata = app->Get_AssetManager()->FindOrLoad_t<TileDataAsset>(file.UserID.value());
								if (tiledata)
								{
									tile._Data = *tiledata.value().value();
								}
							}
							NewTile(std::move(tile));
						}
					}
				}
			}
			ImGui::EndChild();

		}
		ImGui::NextColumn();
		{	
			bool showtilepreview = false;
			
			static bool shouldcloseAtlastSettings = false;
			ImGuIHelper_Asset::AsssetField("Texture Atlas", this->_Data._Base._BaseTexture);


			NullablePtr<PNGAssetFile::TextureSettings>  texturesettingsop;
			
			if (_Data._BaseTexture.Has_UID()) 
			{
				texturesettingsop = PNGAssetFile::GetTextureSettings(_Data._BaseTexture.Get_UID());
			}
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
						auto texturesettings = texturesettingsop.value();
						shouldcloseAtlastSettings = true;

						_Data.List.reserve(texturesettings->sprites.size());
						_Assets.resize(texturesettings->sprites.size());

						auto run = app->Get_RunTimeProjectData();
						auto& index = run->Get_AssetIndex();

						i32 tilesize = 0;
						if (texturesettings->sprites.size())
						{
							tilesize = texturesettings->sprites.front().size.X;
							if (tilesize == 0)
							{
								tilesize = 1;
							}
						}

						for (size_t i = 0; i < std::min(_Data.List.size(),texturesettings->sprites.size()); i++)
						{
							auto& item = texturesettings->sprites[i];
							auto& LastOut = _Data.List[i];

							LastOut._Name = item.spritename;
							LastOut._Data._Base.Sprite = item.uid;
							LastOut.X = item.offset.X / tilesize;
							LastOut.Y = item.offset.Y / tilesize;

							auto fileop = index.FindFileUsingID(LastOut._Data._UID);

							if (fileop.has_value())
							{
								auto& file = fileop.value();

								file.RelativeAssetName = file.RelativePath;
								file.RelativeAssetName += EditorIndex::SubAssetSeparator;
								file.RelativeAssetName += LastOut._Name;
								file.RelativeAssetName += TileData::FileExtDot;
							}
						}
						auto relpath = FileHelper::ToRelativePath(run->GetAssetsDir(), this->FileFullPath).generic_string();

						
						for (size_t i = _Data.List.size(); i < texturesettings->sprites.size(); i++)
						{
							auto& item = texturesettings->sprites[i];

							TileDataPack::PackTile newtile;
							newtile._Name = item.spritename;
							newtile._Data._UID = app->Get_RunTimeProjectData()->GetNewUID();
							newtile._Data._Base.Sprite = item.uid;	
							newtile.X = item.offset.X / tilesize;
							newtile.Y = item.offset.Y / tilesize;

							EditorIndex::IndexFile file;
							file.UserID = newtile._Data._UID;
							file.RelativePath = relpath;
							file.RelativeAssetName = relpath + EditorIndex::SubAssetSeparator + newtile._Name;
							file.RelativeAssetName += TileData::FileExtDot;
							
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

								_Assets.pop_back();

								index.RemoveIndexFileWithUID(Item._Data._UID);
							}
						}

					}
					ImGui::TreePop();
				}

			}

			ImGui::BeginChild("test", {}, 0, flags);
			{
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
					size_t MaxGridX = 5;
					size_t MaxGridY = 5;
					size_t Padding = 2;
					for (auto& Item : _Data.List)
					{
						if (Item.X > MaxGridX - Padding)
						{
							MaxGridX = Item.X + Padding;
						}
						if (Item.Y > MaxGridY - Padding)
						{
							MaxGridY = Item.Y + Padding;
						}
					}
					float GridSize = 50;

					ImU32 spritebordercolor = ImGuIHelper::ColorToImguiU32(Color32());
					ImU32 selecedspritebordercolor = ImGuIHelper::ColorToImguiU32(Color32(204, 161, 4));
					ImU32  GridColor = spritebordercolor;

					auto list = ImGui::GetWindowDrawList();

					static Optional<size_t> HoldingTile = {};
					bool dropedtile = false;
					if (HoldingTile.has_value())
					{
						if (!ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left))
						{
							dropedtile = true;
						}
					}

					auto startpos = ImGui::GetWindowPos();
					auto oldpos = ImGui::GetCursorPos();
	
					size_t G = 0;
					for (size_t x = 0; x < MaxGridX; x++)
					{
						for (size_t y = 0; y < MaxGridY; y++)
						{
							auto xpos = x * GridSize;
							auto ypos = y * GridSize;
							list->AddRect(
								{ startpos.x + (float)xpos   ,startpos.y + (float)ypos },
								{ startpos.x + (float)(xpos + GridSize) , startpos.y + (float)(ypos + GridSize) },
								GridColor
							);

							if (HoldingTile.has_value())
							{
								ImGui::SetCursorPos({ oldpos.x + (xpos),oldpos.y + (ypos) });
								ImGui::PushID(G);
								ImGui::InvisibleButton("gg", { GridSize,GridSize });
								ImGui::PopID();

								G++;

								if (ImGui::IsItemHovered())
								{
									auto& CurrentItem = _Data.List[HoldingTile.value()];
									
									UC::Sprite* spr = nullptr;
									{
										if (CurrentItem._Data._Base.Sprite.Has_Asset())
										{
											spr = CurrentItem._Data._Base.Sprite.Get_Asset();
										}
										if (spr == nullptr)
										{
											spr = AppFiles::GetSprite(AppFiles::sprite::TileAsset);
										}
									}

									ImGui::SetCursorPos({ oldpos.x + (xpos),oldpos.y + (ypos) });
									ImGuIHelper::Image(spr, ImVec2(GridSize, GridSize));
									if (dropedtile)
									{
										CurrentItem.X = x;
										CurrentItem.Y = y;
									}
								}
							}
						}
					}


					if (dropedtile)
					{
						HoldingTile = {};
					}
				
					for (size_t i = 0; i < _Data.List.size(); i++)
					{
						auto& Item = _Data.List[i];

						UC::Sprite* spr = nullptr;
						{
							auto& SpritePtr = Item._Data._Base.Sprite;
							if (SpritePtr.Has_UID() && !SpritePtr.Has_Asset())
							{
								auto val = app->Get_AssetManager()->FindOrLoad_t<UC::SpriteAsset>(SpritePtr.Get_UID());
								if (val.has_value())
								{
									SpritePtr = val.value()->GetManaged();
								}
							}

							if (SpritePtr.Has_Asset())
							{
								spr = SpritePtr.Get_Asset();
							}
						
							if (spr == nullptr)
							{
								spr = AppFiles::GetSprite(AppFiles::sprite::TileAsset);
							}
						}

						ImGui::SetCursorPos({ oldpos.x + (Item.X * GridSize),oldpos.y + (Item.Y * GridSize) });
						ImGuIHelper::Image(spr, ImVec2(GridSize, GridSize));

						if (!HoldingTile.has_value()) 
						{
							if (ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left) && ImGui::IsItemHovered())
							{
								HoldingTile = i;
							}
						}
					}
				}
			}
			ImGui::EndChild();
		}

		

	}
	ImGui::End();
}
NullablePtr<UCode::Asset> TilePackAssetFile::Liveing::LoadAsset(const LoadAssetContext& Item) 
{
	if (_Data._Base._UID == Item._AssetToLoad)
	{
		return _Data.GetAsset();
	}
	for (size_t i = 0; i < this->_Data._Base.List.size(); i++)
	{
		auto& ListItem = _Data._Base.List[i];
		auto& Asset = _Assets[i];

		if (ListItem._Data._UID == Item._AssetToLoad)
		{
			if (!Asset.has_value())
			{
				LoadAssetAt(i);
			}
			return Asset.value().GetAsset();
		}
	}
	return {};
}
void TilePackAssetFile::Liveing::FileUpdated()
{
	auto runprojectdata = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();

	if (!TileDataPack::FromFile(_Data._Base, this->FileFullPath, runprojectdata->Get_ProjData()._SerializeType))
	{

		auto p = FileFullPath.generic_string();
		auto relpath = p.substr(runprojectdata->GetAssetsDir().generic_string().size());
		UCodeGEError("Unable to Read/Parse for " << relpath << " Failed");
	}
	else
	{
		OnFileLoaded();
	}
}
void TilePackAssetFile::Liveing::SaveFile(const UEditorAssetFileSaveFileContext& Context) 
{
	auto runprojectdata = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();

	if (!TileDataPack::ToFile(this->FileFullPath,_Data._Base, runprojectdata->Get_ProjData()._SerializeType))
	{
		auto p = FileFullPath.generic_string();
		auto relpath = p.substr(runprojectdata->GetAssetsDir().generic_string().size());

		UCodeGEError("Unable to Saveing for " << relpath << " Failed");
	}
	_Assets.resize(_Data._Base.List.size());
}
void TilePackAssetFile::Liveing::DrawSubAssets(const UEditorDrawSubAssetContext& Item)
{
	auto app = EditorAppCompoent::GetCurrentEditorAppCompoent();
	for (size_t i = 0; i < _Data._Base.List.size(); i++)
	{
		auto& ListItem = _Data._Base.List[i];
		auto& AssetItem = _Assets[i];

		if (!AssetItem.has_value())
		{
			LoadAssetAt(i);
		}
		auto& Asset = AssetItem.value();
		UEditorDrawSubAssetContext::DoneDraw ondraw;
		ondraw.AssetName = ListItem._Name + TileData::FileExtDot;
		ondraw.OnAssetRename = [](String&)
			{

			};
		ondraw.OnDestroy = []()
			{

			};

		UCode::Sprite* Spr = nullptr;
		{
			auto& SpritePtr = ListItem._Data._Base.Sprite;

			if (SpritePtr.Has_UID() && !SpritePtr.Has_Asset())
			{
				auto val = app->Get_AssetManager()->FindOrLoad_t<UC::SpriteAsset>(SpritePtr.Get_UID());
				if (val.has_value())
				{
					SpritePtr = val.value()->GetManaged();
				}
			}

			if (SpritePtr.Has_Asset())
			{
				Spr = SpritePtr.Get_Asset();
			}
		}
		auto predrawpos = ImGui::GetCursorPos();
		if (Spr != nullptr)
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

		if (Spr == nullptr)
		{
			Spr = AppFiles::GetSprite(AppFiles::sprite::TileAsset);
		}
		if (ImGuIHelper::ImageButton(&ListItem, Spr, *(ImVec2*)&Item.ButtionSize))
		{
			auto inpswin = app->Get_Window<InspectWindow>();


			static auto Func = app->GetPrjectFiles()._newuid;

			InspectWindow::InspectData data;
			data._Data = UC::AnyManagedPtr::As(AssetItem.value().Get_Managed());
			data._Data2 = &ListItem;
			data._Draw = [](InspectWindow::InspectDrawer& draw)
				{
					TileDataPtr ptr = UC::AnyManagedPtr::As<TileDataAsset>(draw.GetPtr());
					TileDataPack::PackTile* tile = (TileDataPack::PackTile*)draw.GetData();
					DrawInspectTile(ptr, *tile);
				};

			inpswin->Inspect(data);

		}

		
		if (ImGui::BeginDragDropSource())
		{
			static Path tepAssetPath;
			tepAssetPath = this->FileFullPath.native() + Path(EditorIndex::SubAssetSeparator).native() + Path(ListItem._Name).native() + Path(TileData::FileExtDot).native();
			Path* p = &tepAssetPath;
			bool OnDropable = ImGui::SetDragDropPayload(DragAndDropType_AssetPath, &p, sizeof(UCode::Path*));

			auto& g = *GImGui;
			auto ImageHight = ImGui::GetFrameHeight();


			Vec2 imagesize = { ImageHight,ImageHight };
			if (OnDropable)
			{
				String Text = "Drop " + ListItem._Name + " Here?";

				ImGuIHelper::Text(Text);
				ImGui::SameLine();
				ImGuIHelper::Image(Spr, *(ImVec2*)&imagesize);
			}
			else
			{
				ImGuIHelper::Text(ListItem._Name);
				ImGui::SameLine();
				ImGuIHelper::Image(Spr, *(ImVec2*)&imagesize);
			}

			ImGui::EndDragDropSource();
		}
		

		ImGui::PushID(&ListItem);
		Item.OnDoneDrawingAssetButton(ondraw);
		ImGui::PopID();
	}
}
void TilePackAssetFile::Liveing::NewTile(TileDataPack::PackTile&& tile)
{
	auto app = EditorAppCompoent::GetCurrentEditorAppCompoent();
	auto runtimeprojectdata = app->Get_RunTimeProjectData();
	tile._Data._UID = runtimeprojectdata->GetNewUID();

	auto& index = app->Get_RunTimeProjectData()->Get_AssetIndex();
	EditorIndex::IndexFile file;
	file.UserID = tile._Data._UID;
	file.RelativePath = FileHelper::ToRelativePath(runtimeprojectdata->GetAssetsDir(), FileFullPath).generic_string();
	file.RelativeAssetName = file.RelativePath;
	file.RelativeAssetName += EditorIndex::SubAssetSeparator + tile._Name;

	index._Files.push_back(std::move(file));
	_Data._Base.List.push_back(std::move(tile));
	_Assets.push_back({});
}
ExportFileRet TilePackAssetFile::ExportFile(const Path& path, const ExportFileContext& Item)
{
	return ExportFileRet();
}

Optional<GetUIDInfo> TilePackAssetFile::GetFileUID(UEditorGetUIDContext& context)
{
	TileDataPack palette;
	if (TileDataPack::FromFile(palette, context.AssetPath,context.ProjectSerializerType))
	{
		GetUIDInfo info;
		info._MainAssetID = palette._UID;

		info._SubAssets.reserve(palette.List.size());
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


