#include "PNGAssetFile.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include <EditorWindows/DragAndDropNames.hpp>
#include "Imgui/imgui_internal.h"
#include "stb_image/stb_image.h"
#include "Helper/UserSettings.hpp"
EditorStart
PNGAssetFile::PNGAssetFile()
{
	FileExtWithDot = ".png";
	FileMetaExtWithDot = UEditorAssetFileData::DefaultMetaFileExtWithDot;
	CanHaveLiveingAssets = true;
}

PNGAssetFile::LiveingPng::LiveingPng()
{
	HasSubAssets = true;

}

void PNGAssetFile::LiveingPng::LoadAssetIfNeeded(SpriteItem& spr)
{
	if (!spr._Asset.get())
	{
		auto& textureasset = asset.value();
		auto& tex = textureasset._Base;
		UCode::Sprite sprite(&tex, spr.offset.X, spr.offset.Y, spr.size.X, spr.size.Y);

		auto manage = textureasset.GetManaged();
		spr._Asset = std::make_shared<UCode::SpriteAsset>(std::move(sprite), manage);
		spr._Asset->Uid = spr.uid;


		auto runtimeproject = UCodeEditor::EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
		auto p = FileFullPath.generic_string();
		auto relpath = p.substr(runtimeproject->GetAssetsDir().generic_string().size());

		spr._Asset->ObjectPath = relpath + EditorIndex::SubAssetSeparator + spr.spritename + UCode::SpriteData::FileExtDot;
	}
}

void PNGAssetFile::LiveingPng::DrawSubAssets(const UEditorDrawSubAssetContext& Item)
{
	if (isbroken) { return; }

	LoadAssetContext context;
	context._AssetToLoad = this->setting.uid;
	LoadAsset(context);

	for (auto& Spr : setting.sprites)
	{
		UEditorDrawSubAssetContext::DoneDraw ondraw;
		ondraw.AssetName = Spr.spritename;
		ondraw.OnAssetRename = [](String&)
			{

			};
		ondraw.OnDestroy = []()
			{

			};

		UCode::Sprite sp{ &asset.value()._Base,(i32)Spr.offset.X,(i32)Spr.offset.Y,(i32)Spr.size.X,(i32)Spr.size.Y };

		auto predrawpos = ImGui::GetCursorPos();
		{
			auto imagescale = 3;
			auto minimagesize = Item.ButtionSize / imagescale;
			auto predrawasucode = Vec2(predrawpos.x, predrawpos.y);

			auto newcursorpos = predrawasucode;
			newcursorpos += {Item.ButtionSize.X - (minimagesize.X / 2), 0};

			ImGui::SetCursorPos(ImVec2(newcursorpos.X, newcursorpos.Y));

			ImGuIHelper::Image(AppFiles::sprite::SpriteAsset, *(ImVec2*)&minimagesize);

			ImGui::SetCursorPos(predrawpos);
		}
		if (ImGuIHelper::ImageButton(&Spr, &sp, *(ImVec2*)&Item.ButtionSize))
		{
			auto App = EditorAppCompoent::GetCurrentEditorAppCompoent();
			auto inpswin = App->Get_Window<InspectWindow>();


			static auto Func = App->GetPrjectFiles()._newuid;
			InspectWindow::InspectData V;
			V._Data = Item._ManageFile;
			V._Draw = [](InspectWindow::InspectDrawer& data)
				{
					UEditorAssetDrawInspectContext Data;
					Data.Drawer = &data;
					Data._newuid = Func;

					auto AssetFile = data.GetPtr().As_ptr<UEditorAssetFile>();
					if (AssetFile.Has_Value())
					{
						AssetFile.Get_Value()->DrawInspect(Data);
					}
					else
					{
						data.SetPtrNull();
					}
				};

			inpswin->Inspect(V);

			OpenSpriteEditor = Spr.uid;
		}

		if (ImGui::BeginDragDropSource())
		{
			static Path tepAssetPath;
			tepAssetPath = this->FileFullPath.native() + Path(EditorIndex::SubAssetSeparator).native() + Path(Spr.spritename).native() + Path(UCode::SpriteData::FileExtDot).native();
			Path* p = &tepAssetPath;
			bool OnDropable = ImGui::SetDragDropPayload(DragAndDropType_AssetPath, &p, sizeof(UCode::Path*));

			auto& g = *GImGui;
			auto ImageHight = ImGui::GetFrameHeight();


			Vec2 imagesize = { ImageHight,ImageHight };
			if (OnDropable)
			{
				String Text = "Drop " + Spr.spritename + "Here?";

				ImGuIHelper::Text(Text);
				ImGui::SameLine();
				ImGuIHelper::Image(&sp, *(ImVec2*)&imagesize);
			}
			else
			{
				ImGuIHelper::Text(Spr.spritename);
				ImGui::SameLine();
				ImGuIHelper::Image(&sp, *(ImVec2*)&imagesize);
			}

			ImGui::EndDragDropSource();
		}

		Item.OnDoneDrawingAssetButton(ondraw);
	}
}

void PNGAssetFile::LiveingPng::Init(const UEditorAssetFileInitContext& Context)
{
	this->FileMetaFullPath = this->FileFullPath.native() + Path(UEditorAssetFileData::DefaultMetaFileExtWithDot).native();

}

void PNGAssetFile::LiveingPng::SaveFile(const UEditorAssetFileSaveFileContext& Context)
{
	if (isbroken) { return; }

	bool hasdiff = true;
	if (hasdiff)
	{
		auto runprojectdata = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();

		if (tofile(this->FileMetaFullPath.value(), setting))
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
		else
		{
			auto p = FileFullPath.generic_string();
			auto relpath = p.substr(runprojectdata->GetAssetsDir().generic_string().size());
			UCodeGEError("Saveing Asset for " << relpath << " Failed");
		}


	}
}

bool PNGAssetFile::LiveingPng::fromfile(const Path& settings, TextureSettings& Out)
{
	UDeserializer serializer;
	if (UDeserializer::FromFile(settings, serializer))
	{
		serializer.ReadType("_UID", Out.uid);
		serializer.ReadType("_ReadAndWrite", Out.ReadAndWrite);

		serializer.ReadType("_Filter", *(Filter_t*)&Out.filter);

		serializer.ReadType("_Compression", *(Compression_t*)&Out.compression);

		serializer.ReadType("_PixelPerUnit", Out.pixelperunit);

		serializer.ReadType("_Sprites", Out.sprites);
		return true;
	}
	return false;
}

bool PNGAssetFile::LiveingPng::tofile(const Path& settings, const TextureSettings& in)
{
	USerializer serializer;

	serializer.Write("_UID", in.uid);
	serializer.Write("_ReadAndWrite", in.ReadAndWrite);

	serializer.Write("_Filter", *(Filter_t*)&in.filter);

	serializer.Write("_Compression", *(Compression_t*)&in.compression);

	serializer.Write("_PixelPerUnit", in.pixelperunit);

	serializer.Write("_Sprites", in.sprites);

	return serializer.ToFile(settings);
}

void PNGAssetFile::LiveingPng::RemoveSubAssets(const Path& AssetDir, EditorIndex& index)
{
	auto relpath = Path(FileFullPath.native().substr(AssetDir.native().size()));

	index._Files.erase(std::remove_if(
		index._Files.begin(), index._Files.end(), [&relpath](EditorIndex::IndexFile& val)
		{
			return val.RelativePath == relpath;
		}), index._Files.end());

}

bool PNGAssetFile::LiveingPng::DrawButtion(const UEditorAssetDrawButtionContext& Item)
{
	LoadAssetContext context;
	context._AssetToLoad = setting.uid;
	LoadAsset(context);


	auto predrawpos = ImGui::GetCursorPos();
	{
		auto imagescale = 3;
		auto minimagesize = Item.ButtionSize / imagescale;
		auto predrawasucode = Vec2(predrawpos.x, predrawpos.y);

		auto newcursorpos = predrawasucode;
		newcursorpos += {Item.ButtionSize.X - (minimagesize.X / 2), 0};

		ImGui::SetCursorPos(ImVec2(newcursorpos.X, newcursorpos.Y));

		ImGuIHelper::Image(AppFiles::sprite::TextureAsset, *(ImVec2*)&minimagesize);

		ImGui::SetCursorPos(predrawpos);
	}
	return ImGuIHelper::ImageButton(Item.ObjectPtr, &asset.value()._Base, *(ImVec2*)&Item.ButtionSize);
}

void PNGAssetFile::LiveingPng::RemoveSprite(UID id)
{
	auto runtimeproject = UCodeEditor::EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
	runtimeproject->Get_AssetIndex().RemoveIndexFileWithUID(id);
}

inline void PNGAssetFile::LiveingPng::SetupTexture(UCode::Texture* tex)
{
	tex->PixelsPerUnit = setting.pixelperunit;
}

inline NullablePtr<UC::Asset> PNGAssetFile::LiveingPng::LoadAsset(const LoadAssetContext& Item)
{
	if (!asset.has_value())
	{
		auto runinfo = UCodeEditor::EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
		if (std::filesystem::exists(this->FileMetaFullPath.value()))
		{
			if (!fromfile(FileMetaFullPath.value(), setting))
			{
				auto relfilepath = FileHelper::ToRelativePath(runinfo->GetAssetsDir(), this->FileMetaFullPath.value());
				UCodeGEError("unable to load .meta file data " << relfilepath);
				isbroken = true;
			}
		}
		else
		{
			RemoveSubAssets(runinfo->GetAssetsDir(), runinfo->Get_AssetIndex());
			setting.uid = runinfo->GetNewUID();

			bool isbadimage = false;
			SpriteItem value;
			{
				int width, height, channels;
				String str = this->FileFullPath.generic_string();
				if (!stbi_info(str.c_str(), &width, &height, &channels))
				{
					isbadimage = true;
					isbroken = true;

					auto relfilepath = FileHelper::ToRelativePath(runinfo->GetAssetsDir(), this->FileFullPath);
					UCodeGEError("unable to load .png image " << relfilepath);
				}
				value.uid = runinfo->GetNewUID();
				value.spritename = this->FileFullPath.filename().replace_extension().generic_string();
				value.size = { (u32)width,(u32)height };
			}
			auto& assetindex = runinfo->Get_AssetIndex();



			if (!isbadimage)
			{
				{
					EditorIndex::IndexFile file;

					auto assetdir = runinfo->GetAssetsDir();
					file.RelativePath = FileFullPath.generic_string().substr(assetdir.generic_string().size());
					file.RelativeAssetName = file.RelativePath + EditorIndex::SubAssetSeparator + value.spritename + UCode::SpriteData::FileExtDot;
					file.UserID = value.uid;

					assetindex._Files.push_back(std::move(file));
				}
				setting.sprites.push_back(std::move(value));
				tofile(FileMetaFullPath.value(), setting);
			}
		}

		UCode::TextureAsset V(UCode::Texture::MakeNewPlaceHolderTexture());
		SetupTexture(&V._Base);
		asset = std::move(V);

		auto lib = EditorAppCompoent::GetCurrentEditorAppCompoent()->GetGameRunTime()->Get_Library_Edit();

		UCode::Threads* threads = UCode::Threads::Get(lib);
		Delegate<Vector<Byte>> Func = [lib, path = this->FileFullPath]()
			{
				UCode::GameFiles* f = UCode::GameFiles::Get(lib);
				auto bytes = f->ReadFileAsBytes(path);

				return bytes.MoveToVector();
			};
		Delegate<Unique_ptr<UCode::Texture>, Vector<Byte>&&> Func2 = [](Vector<Byte>&& Bits) mutable
			{

				auto teptex = new UCode::Texture();
				teptex->SetTexture(UC::PngDataSpan(BytesView::Make(Bits.data(), Bits.size())));


				return Unique_ptr<UCode::Texture>(teptex);
			};
		Delegate<Unique_ptr<UCode::Texture>, Unique_ptr<UCode::Texture>> Func3 = [](Unique_ptr<UCode::Texture>&& Tex) mutable
			{
				Tex->InitTexture();

				return std::move(Tex);
			};

		Delegate<void, Unique_ptr<UC::Texture>&&> Func4 = [this, v = asset.value().GetManaged()](Unique_ptr<UC::Texture>&& text)
			{
				if (v.Has_Value())
				{
					auto& tex = v.Get_Value()->_Base;
					tex = std::move(*text.get());
					SetupTexture(&tex);
					IsLoadingTexture = false;

					tex.FreeFromCPU();
				}
			};

		auto vg = asset.value().GetManaged();


		threads->AddTask_t(UCode::TaskType::File_Input, std::move(Func), {})
			.ContinueOnThread<Unique_ptr<UCode::Texture>>(UCode::TaskType::DataProcessing, std::move(Func2))
			.ContinueOnThread<Unique_ptr<UCode::Texture>>(UCode::TaskType::Rendering, std::move(Func3))
			.OnCompletedOnThread(Func4, UC::TaskType::Main).Start();
	}
	if (Item._AssetToLoad == setting.uid)
	{
		return UC::Nullableptr<UCode::Asset>(asset.value().GetAsset());
	}
	else
	{
		for (auto& spr : setting.sprites)
		{
			if (Item._AssetToLoad == spr.uid)
			{
				LoadAssetIfNeeded(spr);
				auto asset = spr._Asset.get();

				return asset->GetAsset();
			}
		}
	}
	return {};
}

bool PNGAssetFile::LiveingPng::ShouldBeUnloaded(const UEditorAssetShouldUnloadContext& Context)
{
	for (auto& Item : setting.sprites)
	{
		bool isinuse = Item._Asset.get() && Item._Asset->Get_Managed().GetCounter() > 1;

		if (isinuse)
		{
			return false;
		}
	}
	return true;
}

void PNGAssetFile::LiveingPng::DrawInspect(const UEditorAssetDrawInspectContext& Item)
{
	if (isbroken) {
		ImGuIHelper::Text(StringView("Image is Broken"));
		return;
	}
	{
		auto& files = UCodeEditor::EditorAppCompoent::GetCurrentEditorAppCompoent()->GetPrjectFiles();
		files.AssetIsInUse(this);
	}

	ImGui::BeginDisabled(true);


	String tep = "Image/Png";
	ImGuIHelper::InputText("Type", tep);
	ImGui::SameLine();
	auto imageh = ImGui::GetFrameHeight();
	ImGuIHelper::Image(AppFiles::sprite::Uility_image, { imageh ,imageh });


	String tep2 = FileFullPath.filename().replace_extension("").generic_string();
	ImGuIHelper::InputText("Name", tep2);


	ImGui::EndDisabled();
	static void* p = nullptr;

	if (ImGui::Button("Open Sprite Editor",{ImGui::GetContentRegionAvail().x,imageh}) || OpenSpriteEditor)
	{
		p = this;
		ImGui::OpenPopup("SpriteAssetEditor");
	}

	ImGui::Separator();
	auto runtimeproject = UCodeEditor::EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();

	if ((p == this))
	{
		ImGui::Begin("SpriteAssetEditor");
		{
			struct SliceWindowData
			{
				u32 SpriteW = 16;
				u32 SpriteH = 16;

				u32 SpritePadingX = 0;
				u32 SpritePadingY = 0;

				bool WithEmpty = false;
			};
			struct EditorWindowData
			{
				float texturescale = 5;
				Vec2 dragoffset;
				Optional<size_t> SelcedSpriteIndex;
				SliceWindowData slicedata;
			};
			static EditorWindowData windowdata;

			ImGuiWindowFlags flags = ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar;
			flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse;
			auto& USettings = UserSettings::GetSettings();

			bool newspritebuttion = ImGui::Button("New Sprite");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				String tip = "ShortCut '" + USettings.KeyBinds[(size_t)KeyBindList::New].ToString() + "'";
				ImGui::SetTooltip(tip.c_str());
			}
			if (newspritebuttion || (ImGui::IsWindowFocused() && USettings.IsKeybindActive(KeyBindList::New)))
			{
				SpriteItem val;
				val.size = { 16,16 };
				val.offset = { 0,0 };

				val.spritename = "New Sprite " + std::to_string(setting.sprites.size());

				UID newid = runtimeproject->GetNewUID();
				val.uid = newid;


				auto& assetindex = runtimeproject->Get_AssetIndex();
				EditorIndex::IndexFile file;

				auto assetdir = runtimeproject->GetAssetsDir();
				file.RelativePath = FileFullPath.generic_string().substr(assetdir.generic_string().size());
				file.RelativeAssetName = file.RelativePath + EditorIndex::SubAssetSeparator + val.spritename + UCode::SpriteData::FileExtDot;
				file.UserID = newid;

				assetindex._Files.push_back(std::move(file));
				setting.sprites.push_back(std::move(val));
			}
			ImGui::SameLine();
			if (ImGui::Button("Slice"))
			{
				ImGui::OpenPopup("SpriteSlice");
			}
			ImGui::SameLine();
			if (ImGui::Button("Reset Mouse Drag"))
			{
				windowdata.texturescale = 5;
				windowdata.dragoffset = {};
			}
			ImGui::Columns(2);
			{
				ImGui::BeginChild("Tp");

				auto tex = &asset.value()._Base;
				bool hasoneopen = false;

				for (size_t i = 0; i < setting.sprites.size(); i++)
				{
					auto& Item = setting.sprites[i];

					UCode::Sprite sp{ tex,(i32)Item.offset.X,(i32)Item.offset.Y,(i32)Item.size.X,(i32)Item.size.Y };



					auto& g = *GImGui;
					ImGuIHelper::Image(&sp, ImVec2(20, g.FontSize + g.Style.FramePadding.y * 2)); ImGui::SameLine();


					bool focusitem = false;
					if (OpenSpriteEditor.has_value())
					{
						if (OpenSpriteEditor.value() == Item.uid)
						{
							focusitem = true;
							ImGui::SetWindowFocus();
							ImGui::SetNextItemOpen(true);
							OpenSpriteEditor = {};
						}
						else
						{
							ImGui::SetNextItemOpen(false);
						}
					}

					if (ImGui::TreeNode(&Item, ImGuIHelper::ToCStr(StringView(Item.spritename))))
					{
						if (focusitem)
						{
							ImGui::FocusItem();
						}
						hasoneopen = true;
						windowdata.SelcedSpriteIndex = i;

						if (ImGuIHelper::InputText("Sprite Name", Item.spritename))
						{
							auto& assetindex = runtimeproject->Get_AssetIndex();
							auto f = assetindex.FindFileUsingID(Item.uid);
							if (f.has_value())
							{
								auto& file = f.value();
								file.RelativeAssetName = file.RelativePath + EditorIndex::SubAssetSeparator + Item.spritename + UCode::SpriteData::FileExtDot;

								if (Item._Asset.get())
								{
									auto& asset = *Item._Asset.get();
									asset.ObjectPath = file.RelativeAssetName;
								}
							}

						}

						if (ImGuIHelper::uInt32Field("X offset", Item.offset.X))
						{
							auto texw = tex->Get_Width();
							auto spw = Item.size.X;

							Item.offset.X = std::min(Item.offset.X, texw - spw);

							if (Item._Asset.get())
							{
								auto& asset = *Item._Asset.get();
								asset._Base.Set_Xoffset(Item.offset.X);
							}
						}

						if (ImGuIHelper::uInt32Field("Y offset", Item.offset.Y))
						{
							auto texh = tex->Get_Height();
							auto sph = Item.size.Y;

							Item.offset.Y = std::min(Item.offset.Y, texh - sph);

							if (Item._Asset.get())
							{
								auto& asset = *Item._Asset.get();
								asset._Base.Set_Yoffset(Item.offset.Y);
							}
						}

						if (ImGuIHelper::uInt32Field("Width", Item.size.X))
						{
							auto texh = tex->Get_Width();
							auto spx = Item.offset.X;

							Item.size.X = std::min(Item.size.X, texh + spx);

							if (Item._Asset.get())
							{
								auto& asset = *Item._Asset.get();
								asset._Base.Set_Width(Item.size.X);
							}
						}

						if (ImGuIHelper::uInt32Field("Height", Item.size.Y))
						{
							auto texw = tex->Get_Height();
							auto spy = Item.offset.Y;

							Item.size.Y = std::min(Item.size.Y, texw + spy);

							if (Item._Asset.get())
							{
								auto& asset = *Item._Asset.get();
								asset._Base.Set_Width(Item.size.Y);
							}
						}

						ImGuIHelper::Image(&sp, { 125 ,125 });
						ImGui::TreePop();
					}

					bool remove = false;
					if (ImGui::IsItemFocused())
					{
						if (USettings.IsKeybindActive(KeyBindList::Delete))
						{
							RemoveSprite(Item.uid);
							setting.sprites.erase(setting.sprites.begin() + i);
							remove = true;
						}
					}

					if (ImGuIHelper::BeginPopupContextItem(Item.spritename.c_str()))
					{
						ImGuIHelper::Text(StringView("Sprite Options"));

						auto str = USettings.KeyBinds[(size_t)KeyBindList::Delete].ToString();
						if (ImGui::MenuItem("Destroy", str.c_str()) || USettings.IsKeybindActive(KeyBindList::Delete))
						{
							RemoveSprite(Item.uid);
							setting.sprites.erase(setting.sprites.begin() + i);
							remove = true;

							ImGui::CloseCurrentPopup();
						}

						ImGui::EndPopup();
					}

					if (remove)
					{
						break;
					}
				}

				if (!hasoneopen)
				{
					windowdata.SelcedSpriteIndex = {};
				}
				ImGui::EndChild();
			}
			ImGui::NextColumn();
			{
				ImGui::SetNextWindowSize({ 450,180 });
				if (ImGui::BeginPopup("SpriteSlice"))
				{
					auto& slicedata = windowdata.slicedata;

					ImGuIHelper::uInt32Field(StringView("Sprite Width"), slicedata.SpriteW);
					ImGuIHelper::uInt32Field(StringView("Sprite Height"), slicedata.SpriteH);

					ImGuIHelper::uInt32Field(StringView("Padding Sprite Width"), slicedata.SpritePadingX);
					ImGuIHelper::uInt32Field(StringView("Padding Sprite Height"), slicedata.SpritePadingY);

					ImGuIHelper::BoolEnumField(StringView("Include Empty"), slicedata.WithEmpty);

					if (ImGui::Button("Slice"))
					{
						struct SliceItem
						{
							Vec2i_t<u32> offset;
							Vec2i_t<u32> size;
						};

						auto textureW = asset.value()._Base.Get_Width();
						auto textureH = asset.value()._Base.Get_Height();
						Vector<SliceItem> newitems;

						Optional<UC::Texture> colordata;

						for (size_t y = 0; y < textureH; y += slicedata.SpriteH)
						{
							for (size_t x = 0; x < textureW; x += slicedata.SpriteW)
							{

								SliceItem item;
								item.offset.X = x;
								item.offset.Y = y;

								item.size.X = slicedata.SpriteW;
								item.size.Y = slicedata.SpriteH;

								if (!slicedata.WithEmpty)
								{
									bool isempty = true;
									{
										if (!colordata.has_value())
										{
											colordata = UC::Texture(FileFullPath);
										}
										auto& tex = colordata.value();
										auto colors = tex.Get_ColorData();

										for (size_t pixely = item.offset.Y; pixely < item.size.Y + item.offset.Y; pixely++)
										{
											for (size_t pixelx = item.offset.X; pixelx < item.size.X + item.offset.X; pixelx++)
											{

												auto index = UC::Texture::GetPixelIndex(pixelx, pixely, textureW, textureH);
												auto& pixelcolor = colors[index];

												if (pixelcolor != Color32(0, 0, 0, 0))
												{
													isempty = false;
													break;
												}
											}

											if (isempty == false)
											{
												break;
											}
										}
									}

									if (isempty)
									{
										x += slicedata.SpritePadingX;
										y += slicedata.SpritePadingY;

										continue;
									}
								}

								newitems.push_back(std::move(item));
								x += slicedata.SpritePadingX;
								y += slicedata.SpritePadingY;


							}
						}


						setting.sprites.reserve(newitems.size());

						auto& assetindex = runtimeproject->Get_AssetIndex();

						if (newitems.size() < setting.sprites.size())
						{
							for (size_t i = newitems.size(); i < setting.sprites.size(); i++)
							{
								auto& itemtoremove = setting.sprites[i];
								RemoveSprite(itemtoremove.uid);
							}

							auto count = setting.sprites.size();
							for (size_t i = 0; i < count - newitems.size(); i++)
							{
								setting.sprites.pop_back();
							}
						}

						for (size_t i = 0; i < newitems.size(); i++)
						{
							auto& newitem = newitems[i];
							bool isnewitem = i >= setting.sprites.size();
							if (isnewitem)
							{
								SpriteItem item;
								item.offset = newitem.offset;
								item.size = newitem.size;
								item.spritename = FileFullPath.filename().replace_extension("").generic_string() + " " + std::to_string(i);

								UID newid = UCodeEditor::EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData()->GetNewUID();

								item.uid = newid;
								EditorIndex::IndexFile file;

								auto assetdir = runtimeproject->GetAssetsDir();
								file.RelativePath = FileFullPath.generic_string().substr(assetdir.generic_string().size());
								file.RelativeAssetName = file.RelativePath + EditorIndex::SubAssetSeparator + item.spritename + UCode::SpriteData::FileExtDot;
								file.UserID = newid;

								assetindex._Files.push_back(std::move(file));

								setting.sprites.push_back(std::move(item));
							}
							else
							{
								auto& olditem = setting.sprites[i];

								olditem.offset = newitem.offset;
								olditem.size = newitem.size;

								if (olditem._Asset.get())
								{
									auto& asset = *olditem._Asset.get();
									asset._Base.Set_Xoffset(olditem.offset.X);
									asset._Base.Set_Yoffset(olditem.offset.Y);
									asset._Base.Set_Width(olditem.size.X);
									asset._Base.Set_Height(olditem.size.Y);
								}
							}
						}
					}

					ImGui::EndPopup();
				}
				ImGui::BeginChild("test", {}, 0, flags);
				{
					auto list = ImGui::GetWindowDrawList();
					auto startpos = ImGui::GetWindowPos();

					if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered())
					{
						windowdata.texturescale += ImGui::GetIO().MouseWheel;
						auto tep = ImGui::GetMouseDragDelta(ImGuiMouseButton_::ImGuiMouseButton_Right);
						windowdata.dragoffset += *(Vec2*)&tep;
						windowdata.texturescale = std::max(windowdata.texturescale, 0.5f);
					}

					auto& myasset = asset.value();
					Vec2 objectsize = { (f32)myasset._Base.Get_Width(),(f32)myasset._Base.Get_Height() };
					objectsize *= windowdata.texturescale;

					auto newpos = ImGui::GetCursorPos();
					auto diffx = windowdata.dragoffset.X / windowdata.texturescale;
					auto diffy = windowdata.dragoffset.Y / windowdata.texturescale;

					newpos.x += diffx;
					newpos.y += diffy;
					ImGui::SetCursorPos(newpos);

					ImU32 texturbordercolor = ImGuIHelper::ColorToImguiU32(Color32(217, 51, 0));
					ImU32 spritebordercolor = ImGuIHelper::ColorToImguiU32(Color32());
					ImU32 selecedspritebordercolor = ImGuIHelper::ColorToImguiU32(Color32(204, 161, 4));

					for (size_t i = 0; i < setting.sprites.size(); i++)
					{
						auto& Item = setting.sprites[i];

						float HOffset = Item.offset.Y * windowdata.texturescale;
						float WOffset = Item.offset.X * windowdata.texturescale;
						float HSize = Item.size.Y;
						float WSize = Item.size.X;

						auto diffx = windowdata.dragoffset.X / windowdata.texturescale;
						auto diffy = windowdata.dragoffset.Y / windowdata.texturescale;

						bool isseleced = false;
						if (windowdata.SelcedSpriteIndex.has_value())
						{
							isseleced = i == windowdata.SelcedSpriteIndex.value();
						}
						auto color = isseleced ? selecedspritebordercolor : spritebordercolor;

						list->AddRect({ startpos.x + WOffset + diffx ,
							startpos.y + HOffset + diffy },
							{ startpos.x + WOffset + diffx + (WSize * windowdata.texturescale),
							startpos.y + HOffset + diffy + (HSize * windowdata.texturescale) }, color);
					}

					if (windowdata.SelcedSpriteIndex.has_value())
					{
						if (windowdata.SelcedSpriteIndex.value() > setting.sprites.size())
						{
							windowdata.SelcedSpriteIndex = {};
						}
					}
					auto tex = &asset.value()._Base;
					{//
						float HOffset = 0 * windowdata.texturescale;
						float WOffset = 0 * windowdata.texturescale;
						float HSize = tex->Get_Height();
						float WSize = tex->Get_Width();

						auto diffx = windowdata.dragoffset.X / windowdata.texturescale;
						auto diffy = windowdata.dragoffset.Y / windowdata.texturescale;

						list->AddRect({ startpos.x + WOffset + diffx ,
							startpos.y + HOffset + diffy },
							{ startpos.x + WOffset + diffx + (WSize * windowdata.texturescale),
							startpos.y + HOffset + diffy + (HSize * windowdata.texturescale) }, texturbordercolor);

					}

					ImGuIHelper::Image(tex, *(ImVec2*)&objectsize);
				}
			}
			ImGui::EndChild();

		}
		ImGui::End();
	}
	//

	if (ImGuIHelper::BoolEnumField("ReadAndWrite", setting.ReadAndWrite))
	{

	}
	if (ImGuIHelper::EnumField("Compression", setting.compression, CompressionEnumValues, CompressionEnumValuesSize))
	{

	}
	if (ImGuIHelper::EnumField("Filter", setting.filter, FilterEnumValues, FilterEnumValuesSize))
	{

	}
	if (ImGuIHelper::uInt32Field("Pixel Per Unit", setting.pixelperunit))
	{
		asset.value()._Base.PixelsPerUnit = setting.pixelperunit;
	}
	//
}

ExportFileRet PNGAssetFile::ExportFile(const Path& path, const ExportFileContext& Item)
{
	UCodeGEStackFrame("PngAsset:Export");
	Path metapath = path.native() + Path(UEditorAssetFileData::DefaultMetaFileExtWithDot).native();

	TextureSettings settings;
	if (!LiveingPng::fromfile(metapath, settings))
	{
		ExportFileRet r;
		ExportError error;
		error.message = "Unable unable to Read/Parse " + metapath.generic_string();
		error.filepath = metapath;
		r.errors.push_back(std::move(error));
		return r;
		UCodeGEToDo();
	}
	ExportFileRet r;
	r._UID = settings.uid;
	r.WasUpdated = true;
	r._SubAssets.reserve(settings.sprites.size());

	String filename = Item.Output.generic_string();
	Path outtexpath = filename.substr(0, filename.find_last_of('.')) + UCode::TextureData::FileExtDot;

	bool textureneedsupdate = true;
	if (textureneedsupdate)
	{
		UCode::TextureData _Data;
		_Data.ReadWrite = settings.ReadAndWrite;
		_Data.PixelPerunit = settings.pixelperunit;
		_Data.Filter = settings.filter;
		_Data._TextureType = ".png";

		auto byte = UCode::GameFiles::ReadFileAsBytes(path);
		_Data._TextureData.reserve(byte.Size());

		for (auto& Item : byte.AsView())
		{
			_Data._TextureData.push_back(Item);
		}

		if (!UCode::TextureData::ToFile(outtexpath, _Data, USerializerType::Bytes))
		{
			UCodeGEToDo();
		}
	}
	r.outpath = outtexpath;

	for (auto& spriteItem : settings.sprites)
	{
		Path outpath = Item.Output.native() + Path(spriteItem.spritename).native() + Path(UCode::SpriteData::FileExtDot).native();

		bool spriteneedsupdate = true;

		if (spriteneedsupdate) {
			UCode::SpriteData spritedata;
			spritedata.OffsetX = spriteItem.offset.X;
			spritedata.OffsetY = spriteItem.offset.Y;
			spritedata.SizeX = spriteItem.size.X;
			spritedata.SizeY = spriteItem.size.Y;
			spritedata._Texture = r._UID.value();

			if (!UCode::SpriteData::ToFile(outpath, spritedata, USerializerType::Bytes))
			{
				UCodeGEToDo();
			}
		}

		ExportFileRet::SubAsset val;
		val._UID = spriteItem.uid;
		val._Path = outpath;

		r._SubAssets.push_back(std::move(val));
	}


	return  r;
}

Optional<GetUIDInfo> PNGAssetFile::GetFileUID(UEditorGetUIDContext& context)
{
	TextureSettings setting;
	Path metapath = context.AssetPath.native() + Path(UEditorAssetFileData::DefaultMetaFileExtWithDot).native();

	if (LiveingPng::fromfile(metapath, setting))
	{
		GetUIDInfo val;
		val._MainAssetID = setting.uid;
		val._SubAssets.reserve(setting.sprites.size());

		for (auto& Item : setting.sprites)
		{
			GetSubAssetData sub;
			sub._ID = Item.uid;
			sub._SubAssetName = Item.spritename + UCode::SpriteData::FileExtDot;

			val._SubAssets.push_back(std::move(sub));
		}

		return val;
	}
	else
	{
		GetUIDInfo val;
		auto newid = context.GetNewUID();
		setting.uid = newid;

		LiveingPng::tofile(metapath, setting);

		val._MainAssetID = newid;
		return val;
	}
	return {};
}
EditorEnd
