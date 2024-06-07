#include "TilemapWindow.hpp"
#include "UCodeRunTime/ULibrarys/Rendering/TileMapRenderer2d.hpp"
#include "Helper/ImGuIHelper.hpp"
#include "EditorWindows/BasicWindows/GameEditorWindow.hpp"
#include "Helper/ImGuIHelper_Asset.hpp"
#include "Imgui/imgui_internal.h"
#include "Editor/EditorAppCompoent.hpp"
#include "UCodeRunTime/ULibrarys/Serialization/Yaml_Implementation/AsssetPtr.hpp"
#include "UCodeRunTime/ULibrarys/Serialization/Bit_Implementation/AsssetPtr.hpp"
EditorStart
TilemapWindow::TilemapWindow(const NewEditorWindowData& windowdata) :EditorWindow(windowdata)
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


		if (currenttilemap)
		{
			currenttilemap->Showgrid = true;
		}
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
	{
		auto win = app->Get_Window<GameEditorWindow>();

		if (_toolbar == ToolBar::Draw)
		{
			if (!_CurrentTile.Has_Asset() && _CurrentTile.Has_UID())
			{
				auto tep = app->Get_AssetManager()->FindOrLoad_t<TileDataAsset>(_CurrentTile.Get_UID());

				if (tep.has_value())
				{
					_CurrentTile = tep.value()->GetManaged();
				}
			}
			if (_CurrentTile.Has_Asset())
			{
				if (!win->PreDraw.has_value())
				{
					win->PreDraw = [this, win, &currenttilemap](UC::RenderRunTime2d::DrawData& Data)
						{
							auto currententity = GameEditorWindow::GetCurrentSeclectedEntity();

							auto posin3d = win->Get_EditorSceneMIn3d();
							UC::TileMapRenderer* currenttilemap = nullptr;

							if (currententity.Has_Value())
							{
								currenttilemap = currententity.Get_Value()->GetCompent<UC::TileMapRenderer>().value_unchecked();


								if (currenttilemap)
								{
									currenttilemap->Showgrid = true;
								}
							}
							if (currenttilemap == nullptr) { return; }
							if (posin3d.has_value())
							{
								auto pos = posin3d.value();
								auto tilemap = currenttilemap;

								auto p = tilemap->GetTilePos({ pos.X,pos.Y });

								auto e = tilemap->NativeEntity();
								auto scale = e->LocalScale2D();
								float w = scale.X;
								float h = scale.Y;

								UC::RenderRunTime2d::DrawQuad2dData data(
									{ p.X + (w / 2),p.Y + (h / 2) }, { w,h }, { 0,0 });
								data.color.A = 0.5f;
								Data.Quad2d.push_back(data);

								if (ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left))
								{
									UC::TilePtr ptr;
									{
										UID val;
										//ptr = _CurrentTile.Get_Asset()->_Data;
									}
									UC::TileMapRenderer::Tile tile;

									tile.tile = ptr;
									tile.color = {};
									tile.xoffset = p.X;
									tile.yoffset = p.Y;

									tilemap->AddTile(tile);
								}
							}
						};
				}
			}
		}
	}
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
		static Optional<size_t> HoldingTile = {};
		bool dropedtile = false;
		if (HoldingTile.has_value())
		{
			if (!ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left))
			{
				dropedtile = true;
			}
		}
		for (size_t x = 0; x < MaxGridX; x++)
		{
			for (size_t y = 0; y < MaxGridY; y++)
			{
				auto xpos = x * GridSize;
				auto ypos = y * GridSize;


				list->AddRect(
					{ -ImGui::GetScrollX() + startpos.x + (float)xpos   ,-ImGui::GetScrollY() + startpos.y + (float)ypos },
					{ -ImGui::GetScrollX() + startpos.x + (float)(xpos + GridSize) ,-ImGui::GetScrollY() + startpos.y + (float)(ypos + GridSize) },
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

				if (ImGui::IsItemHovered())
				{

					if (ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
					{
						if (packtile.has_value())
						{
							_CurrentTile = packtile.value()->Ptr;
						}

						for (size_t i = 0; i < asset->_List.size(); i++)
						{
							auto& Item = asset->_List[i];

							if (Item.X == x && Item.Y == y)
							{
								HoldingTile = i;
								break;
							}
						}
					}

					if (!packtile.has_value() && HoldingTile.has_value())
					{
						auto& heldtile = asset->_List[HoldingTile.value()];

						ImGui::SetCursorPos({ oldcursorpos.x + (xpos),oldcursorpos.y + (ypos) });

						UC::Sprite* sp = nullptr;

						if (heldtile.Ptr.Has_Asset())
						{
							auto& spriteptr = heldtile.Ptr.Get_Asset()->_Data.Sprite;

							if (spriteptr.Has_Asset())
							{
								sp = spriteptr.Get_Asset();
							}
						}
						if (sp == nullptr)
						{
							sp = AppFiles::GetSprite(AppFiles::sprite::TileAsset);
						}

						ImGuIHelper::Image(sp, { GridSize,GridSize });
					}

					if (dropedtile)
					{
						auto& heldtile = asset->_List[HoldingTile.value()];
						heldtile.X = x;
						heldtile.Y = y;
						HoldingTile = {};
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
					bool wasset = ImGuIHelper_Asset::AsssetField("Set Tile", tile);

					if (wasset)
					{
						bool wasfound = false;

						bool isremove = tile.Has_UID() && tile.Get_UID() == UID();

						if (isremove)
						{
							wasfound = true;
							for (size_t i = 0; i < asset->_List.size(); i++)
							{
								auto& Item = asset->_List[i];

								if (Item.X == x && Item.Y == y)
								{
									asset->_List.erase(asset->_List.begin() + i);
									break;
								}
							}
						}
						else
						{

							for (auto& Item : asset->_List)
							{
								if (Item.X == x && Item.Y == y)
								{
									wasfound = true;

									Item.Ptr = std::move(tile);
									break;
								}
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

				if (ImGui::BeginDragDropTarget())
				{
					ImGuiDragDropFlags target_flags = 0;
					target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;
					target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DragAndDropType_AssetPath, target_flags))
					{
						const Path* fullpath = *(Path**)payload->Data;


						auto ext = Path(*fullpath).extension().generic_string();
						bool canbedroped = ext == TileData::FileExtDot || TileDataPack::FileExtDot;
						if (payload->IsDelivery())
						{
							if (canbedroped)
							{
								auto app = EditorAppCompoent::GetCurrentEditorAppCompoent();
								auto rundata = app->Get_RunTimeProjectData();

								auto rel = FileHelper::ToRelativePath(rundata->GetAssetsDir(), *fullpath).generic_string();
								auto& index = rundata->Get_AssetIndex();

								auto valop = index.FindFileRelativeAssetName(rel);
								if (valop.has_value())
								{
									auto& val = valop.value();


									if (ext == TileData::FileExtDot)
									{
										if (val.UserID.has_value())
										{
											if (packtile.has_value())
											{
												packtile.value()->Ptr = val.UserID.value();
											}
											else
											{
												TilePalette::PaletteItem item;
												item.X = x;
												item.Y = y;
												item.Ptr = val.UserID.value();
												asset->_List.push_back(std::move(item));
											}
										}
									}
									else if (ext == TileDataPack::FileExtDot)
									{
										if (val.UserID.has_value())
										{
											auto assetop = app->Get_AssetManager()->FindOrLoad_t<TilePackAsset>(val.UserID.value());

											if (assetop.has_value())
											{
												auto& packasset = assetop.value();

												for (auto& packItem : packasset->_Base.List)
												{
													NullablePtr<TilePalette::PaletteItem> Item;
													{
														for (auto& tile : asset->_List)
														{
															if (tile.X == packItem.X + x && tile.Y == packItem.Y + y)
															{
																Item = Nullableptr(&tile);
																break;
															}
														}
													}

													if (Item.has_value())
													{
														Item.value()->Ptr = packItem._Data._UID;
													}
													else
													{
														TilePalette::PaletteItem item;
														item.X = packItem.X +x;
														item.Y = packItem.Y + y;
														item.Ptr = packItem._Data._UID;
														asset->_List.push_back(std::move(item));
													}

												}
											}
										}
									}
								}
							}
						}

						if (canbedroped)
						{
							ImGuiContext& g = *GImGui;
							ImGui::RenderDragDropTargetRect(g.DragDropTargetRect, g.DragDropTargetClipRect);
						}
					}


					ImGui::EndDragDropTarget();
				}


				ImGui::PopID();
				I++;
			}
		}

		if (dropedtile)
		{
			HoldingTile = {};
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

