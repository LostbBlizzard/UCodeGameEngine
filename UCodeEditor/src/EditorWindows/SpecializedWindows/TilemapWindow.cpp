#include "TilemapWindow.hpp"
#include "UCodeRunTime/ULibrarys/Rendering/TileMapRenderer2d.hpp"
#include "Helper/ImGuIHelper.hpp"
#include "EditorWindows/BasicWindows/GameEditorWindow.hpp"
#include "Helper/ImGuIHelper_Asset.hpp"

#include "Editor/EditorAppCompoent.hpp"
#include "UCodeRunTime/ULibrarys/Serialization/Yaml_Implementation/AsssetPtr.hpp"
#include "UCodeRunTime/ULibrarys/Serialization/Bit_Implementation/AsssetPtr.hpp"
EditorStart
TilemapWindow::TilemapWindow(const NewEditorWindowData& windowdata):EditorWindow(windowdata)
{

}
TilemapWindow::~TilemapWindow()
{
}

void TilemapWindow::UpdateWindow()
{
	auto currententity = GameEditorWindow::GetCurrentSeclectedEntity();
	UC::TileMapRenderer* currenttilemap = nullptr;

	if (currententity.Has_Value())
	{
		currenttilemap = currententity.Get_Value()->GetCompent<UC::TileMapRenderer>().value_unchecked();
	}
	if (currenttilemap == nullptr)
	{
		ImGuIHelper::Text(StringView("Seclect a Tilemap in Scene Hierarchy"));

        ImGui::Separator();
	}

	ImGui::BeginDisabled(currenttilemap == nullptr);
	{
		if (ImGui::RadioButton("Draw", _toolbar == ToolBar::Draw)) { _toolbar = ToolBar::Draw; }
        ImGui::SameLine();
	
		if (ImGui::RadioButton("Copy", _toolbar == ToolBar::Erase)) { _toolbar = ToolBar::Copy; }
        ImGui::SameLine();
	
		if (ImGui::RadioButton("Move", _toolbar == ToolBar::Move)) { _toolbar = ToolBar::Move; }
        ImGui::SameLine();	

		if (ImGui::RadioButton("Erase", _toolbar == ToolBar::Erase)) { _toolbar = ToolBar::Erase; }	
    
        ImGui::Separator();
	}
	ImGui::EndDisabled();
	{
		if (!_CurrentTilePalette.Has_Asset() && _CurrentTilePalette.Has_UID())
		{
			auto app = EditorAppCompoent::GetCurrentEditorAppCompoent();
			auto tep = app->Get_AssetManager()->FindOrLoad_t<TilePaletteAsset>(_CurrentTilePalette.Get_UID());
			if (tep.has_value())
			{
				_CurrentTilePalette = tep.value()->GetManaged();
			}
		}

		ImGuIHelper_Asset::AsssetField("Palette", _CurrentTilePalette);
	}
	
	auto app = EditorAppCompoent::GetCurrentEditorAppCompoent();
	if (_CurrentTilePalette.Has_Asset())
	{
		ImGuIHelper_Asset::AsssetField("Tile", _CurrentTile);
		ImGui::BeginChild("test", ImGui::GetContentRegionAvail(), 0, flags);

		//ImGui::Text("5");
		auto asset = _CurrentTilePalette.Get_Asset();

		size_t MaxGridX = 5;
		size_t MaxGridY = 5;
		size_t Padding = 2;

		auto list = ImGui::GetWindowDrawList();

		for (auto& Item : asset->_List)
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

		ImU32 spritebordercolor = ImGuIHelper::ColorToImguiU32(Color32());
		ImU32 selecedspritebordercolor = ImGuIHelper::ColorToImguiU32(Color32(204, 161, 4));
		ImU32  GridColor = spritebordercolor;

		float GridSize = 50;


		auto startpos = ImGui::GetWindowPos();
		auto oldcursorpos = ImGui::GetCursorPos();
		size_t I = 0;
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

				ImGui::SetCursorPos({ oldcursorpos.x + (xpos),oldcursorpos.y + (ypos) });
				ImGui::PushID(I);
				
				NullablePtr<TilePalette::PaletteItem> packtile;
				{
					for (auto& Item : asset->_List)
					{
						if (Item.X == x && Item.Y == y)
						{
							packtile = Nullableptr(&Item);
							break;
						}
					}

				}

				if (packtile.has_value())
				{
					auto& tile = packtile.value();

					UC::Sprite* spr = nullptr;

					if (tile->Ptr.Has_UID() && !tile->Ptr.Has_Asset())
					{
						auto tep = app->Get_AssetManager()->FindOrLoad_t<TileDataAsset>(tile->Ptr.Get_UID());

						if (tep.has_value())
						{
							tile->Ptr = tep.value()->GetManaged();
						}
					}


					if (tile->Ptr.Has_Asset())
					{
						auto& spritesptr = tile->Ptr.Get_Asset()->_Data.Sprite;

						if (!spritesptr.Has_Asset() && spritesptr.Has_UID())
						{

							auto tep = app->Get_AssetManager()->FindOrLoad_t<UC::SpriteAsset>(spritesptr.Get_UID());

							if (tep.has_value())
							{
								spritesptr = tep.value()->GetManaged();
							}
						}
						if (spritesptr.Has_Asset())
						{
							spr = spritesptr.Get_Asset();
						}
					}

					if (spr == nullptr)
					{
						spr = AppFiles::GetSprite(AppFiles::sprite::TileAsset);
					}

					ImGuIHelper::Image(spr, { GridSize,GridSize });
				}
				else
				{
					ImGui::InvisibleButton("gg", { GridSize,GridSize });
				}


				static bool justopened = false;
				static TileDataPtr tile;

				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
				{
					if (packtile.has_value()) 
					{
						_CurrentTile = packtile.value()->Ptr;
					}

				}
				float popwindowwdth = 400;
				ImGui::SetNextWindowSize({ popwindowwdth,50 });
				if (ImGuIHelper::BeginPopupContextItem("Test"))
				{
					if (justopened == false)
					{
						for (auto& Item : asset->_List)
						{
							if (Item.X == x && Item.Y == y)
							{
								tile = Item.Ptr;
								break;
							}
						}
						justopened = true;
					}
					//ImGui::SetNextItemWidth(popwindowwdth/4);
					bool wasset = ImGuIHelper_Asset::AsssetField("Set TIle", tile);

					if (wasset)
					{
						bool wasfound = false;
						for (auto& Item : asset->_List)
						{
							if (Item.X == x && Item.Y == y)
							{
								wasfound = true;

								Item.Ptr = std::move(tile); 
								break;
							}
						}

						if (wasfound == false)
						{
							TilePalette::PaletteItem item;
							item.X = x;
							item.Y = y;
							item.Ptr = std::move(tile);
							asset->_List.push_back(std::move(item));
						}
						tile = TileDataPtr();
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
				else
				{
					justopened = false;
				}

				ImGui::PopID();
				I++;
			}
		}

		ImGui::EndChild();
	}
}

void TilemapWindow::OnSaveWindow(USerializer& Input)
{
	Input.Write("_CurrentTilePalette", _CurrentTilePalette);
	Input.Write("_CurrentTile", _CurrentTile);
}

void TilemapWindow::OnLoadWindow(UDeserializer& JsonToOutof)
{
	JsonToOutof.ReadType("_CurrentTilePalette", _CurrentTilePalette,_CurrentTilePalette);
	JsonToOutof.ReadType("_CurrentTile", _CurrentTile,_CurrentTile);
}

EditorWindowData TilemapWindow::GetEditorData()
{
	return EditorWindowData("TilemapWindow",MakeWin);
}

EditorWindow* TilemapWindow::MakeWin(const NewEditorWindowData& windowdata)
{
	return new TilemapWindow(windowdata);
}
EditorEnd

