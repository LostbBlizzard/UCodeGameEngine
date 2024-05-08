#include "CoreModule.hpp"
#include "../ModuleNames.hpp"

#include "UCodeRunTime/ULibrarys/AssetManagement/SceneData.hpp"
#include "Helper/ImGuIHelper.hpp"
#include <EditorWindows/DragAndDropNames.hpp>
#include <EditorWindows/EditorStrings.hpp>

#include "UCodeRunTime/ULibrarys/Rendering/SpriteRenderer2d.hpp"
#include "UCodeRunTime/ULibrarys/Rendering/Camera2d.hpp"
#include "Helper/ImGuIHelper_Asset.hpp"
#include "EditorWindows/OtherTypes/RawEntityData.hpp"
#include "Editor/EditorAppCompoent.hpp"

#include "UCodeRunTime/ULibrarys/Serialization/SerlizeableType.hpp"

#include "UCodeRunTime/ULibrarys/AssetManagement/AssetRendering.hpp"
#include "UCodeRunTime/ULibrarys/Audio/AudioSystem.hpp"
#include <Imgui/misc/cpp/imgui_stdlib.h>
#include "Imgui/imgui_internal.h"
#include "Helper/UserSettings.hpp"
EditorStart


class ScencAssetFile :public UEditorAssetFileData
{
public:
	ScencAssetFile()
	{
		FileExtWithDot =UCode::Scene2dData::FileExtDot;
	}
	bool Draw(UEditorAssetDataConext& Data, const Path& path) override
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

	ExportFileRet ExportFile(const Path& path, const ExportFileContext& Item) override
	{
		UCodeGEStackFrame("SceneAsset:Export");
		std::filesystem::copy_file(path, Item.Output, std::filesystem::copy_options::overwrite_existing);
	
		UCode::Scene2dData V;
		UCode::Scene2dData::FromFile(V, path);

		UCode::GameRunTime runtime;
		auto scenc = UCode::Scene2dData::LoadScene(&runtime,V);
		UCode::Scene2dData::SaveScene(scenc,V, USerializerType::Bytes);

		//UCode::Scene2dData::ToFile(Item.Output, V, USerializerType::Bytes);

		ExportFileRet r;
		r._UID = V._UID;
		r.outpath = Item.Output;
		return r;
	}
	Optional<GetUIDInfo> GetFileUID(UEditorGetUIDContext& context) override
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
};



class SpriteRendererUEditorData : public UEditorComponentData
{
public:
	SpriteRendererUEditorData()
	{
		CompoentType = UCode::SpriteRenderer::Get_TypeID();
	}

	void DrawAsTree(const UEditorComponentDrawTreeData& Data, UCode::Compoent* Value) const override
	{
		UCode::SpriteRenderer* Component = (UCode::SpriteRenderer*)Value;

		ImGuIHelper_Asset::AsssetField("Shader", Component->shader);
		ImGuIHelper_Asset::AsssetField("Sprite", Component->sprite);

		ImGuIHelper::ColorField("Color", Component->color);
		ImGuIHelper::BoolEnumField("flipX", Component->flipX);
		ImGuIHelper::BoolEnumField("flipY", Component->flipY);

		ImGuIHelper_Asset::DrawLayerField("DrawLayer", Component->DrawLayer);
		ImGuIHelper::uInt8Field("DrawOrder", Component->DrawOrder);
	}

	void DrawInspect(const UEditorComponentDrawData& Data, UCode::Compoent* Value) const override
	{
		UCode::SpriteRenderer* Component = (UCode::SpriteRenderer*)Value;
		{
			ImGui::BeginDisabled(true);



			const float square_sz = ImGui::GetFrameHeight();
			ImGuIHelper::Image(AppFiles::sprite::Uility_image, { square_sz ,square_sz });
			ImGui::SameLine();

			String tep = "Compoent/SpriteRenderer";

			ImGuIHelper::ItemLabel(StringView("Type"), ImGuIHelper::ItemLabelFlag::Left);

			ImGui::PushItemWidth(ImGui::CalcItemWidth() - (ImGuIHelper::CheckBoxSizeWithPadding().x));
			ImGui::PushID(&tep);
			ImGui::InputText("", &tep);
			ImGui::PopID();
			ImGui::PopItemWidth();

			ImGui::SameLine();

			ImGui::EndDisabled();

			bool V = Component->Get_IsActive();
			ImGui::PushID(&V);
			ImGui::Checkbox("", &V);
			ImGui::PopID();
			Component->Set_CompoentActive(V);
		}

		Data.Draw->AsssetField("Shader", Component->shader);
		Data.Draw->AsssetField("Sprite", Component->sprite);

		Data.Draw->ColorField("Color", Component->color);
		Data.Draw->ToggleField("flipX", Component->flipX);
		Data.Draw->ToggleField("flipY", Component->flipY);

		Data.Draw->DrawLayerField("DrawLayer", Component->DrawLayer);
		Data.Draw->uInt8Field("DrawOrder", Component->DrawOrder);

	}
};


static const ImGuIHelper::EnumValue<UCode::Camera2d::CamType> CamTypes[] =
{
	{"Orthographic",UCode::Camera2d::CamType::Orthographic},
	{"Perspective",UCode::Camera2d::CamType::Perspective},
};
constexpr size_t CamTypesCount = sizeof(CamTypes) / sizeof(CamTypes[0]);
class CameraUEditorData : public UEditorComponentData
{
public:
	CameraUEditorData()
	{
		CompoentType = UCode::Camera2d::Get_TypeID();
	}


	void DrawAsTree(const UEditorComponentDrawTreeData& Data, UCode::Compoent* Value) const override
	{
		UCode::Camera2d* Component = (UCode::Camera2d*)Value;

		auto CamTV = Component->Get_CamType();
		ImGuIHelper::EnumField("Type", CamTV, CamTypes, CamTypesCount);
		Component->Set_CamType(CamTV);

		if (CamTV == UCode::Camera2d::CamType::Orthographic)
		{
			f32 V = Component->Get_Ortho_size();
			ImGuIHelper::f32Field("Ortho_Size", V);
			Component->Set_Ortho_size(V);
		}
		else
		{
			f32 V = Component->Get_Fov();
			ImGuIHelper::f32Field("Fov", V);
			Component->Set_Fov(V);
		}
	}

	void DrawInspect(const UEditorComponentDrawData& Data, UCode::Compoent* Value) const override
	{
		UCode::Camera2d* Component = (UCode::Camera2d*)Value;
		{
			ImGui::BeginDisabled(true);


			const float square_sz = ImGui::GetFrameHeight();
			ImGuIHelper::Image(AppFiles::sprite::Uility_image, { square_sz ,square_sz });
			ImGui::SameLine();

			String tep = "Compoent/Camera";

			ImGuIHelper::ItemLabel(StringView("Type"), ImGuIHelper::ItemLabelFlag::Left);

			ImGui::PushItemWidth(ImGui::CalcItemWidth() - (ImGuIHelper::CheckBoxSizeWithPadding().x));
			ImGui::PushID(&tep);
			ImGui::InputText("", &tep);
			ImGui::PopID();
			ImGui::PopItemWidth();

			ImGui::SameLine();

			ImGui::EndDisabled();

			bool V = Component->Get_IsActive();
			ImGui::PushID(&V);
			ImGui::Checkbox("", &V);
			ImGui::PopID();

		}
		auto CamTV = Component->Get_CamType();
		ImGuIHelper::EnumField("Type", CamTV, CamTypes, CamTypesCount);
		Component->Set_CamType(CamTV);
		if (CamTV == UCode::Camera2d::CamType::Orthographic)
		{
			f32 V = Component->Get_Ortho_size();
			Data.Draw->f32Field("Ortho_Size", V);
			Component->Set_Ortho_size(V);
		}
		else
		{
			f32 V = Component->Get_Fov();
			Data.Draw->f32Field("Fov", V);
			Component->Set_Fov(V);
		}
	}
};
namespace fs = std::filesystem;

struct SpriteItem
{
	UID uid;
	String spritename;

	Vec2i_t<u32> offset;
	Vec2i_t<u32> size;

	std::shared_ptr<UCode::SpriteAsset> _Asset;
};

EditorEnd


MakeSerlizeType(UCodeEditor::SpriteItem,
	Field("_UID", _this->uid);
	Field("_SpriteName", _this->spritename);
	Field("offset", _this->offset);
	Field("size", _this->size);
	)


	EditorStart
	
		
		class MP3AssetFile :public UEditorAssetFileData
	{
	public:
		MP3AssetFile()
		{
			FileExtWithDot = ".mp3";
			FileMetaExtWithDot = UEditorAssetFileData::DefaultMetaFileExtWithDot;
			CanHaveLiveingAssets = true;
		}

		class LiveingAsset :public UEditorAssetFile
		{
		public:
			UCode::AudioFile file;

			inline static UCode::AudioPlaySettings playsettings;
			void Init(const UEditorAssetFileInitContext& Context) override
			{
				file.Load(this->FileFullPath);
			}
			bool DrawButtion(const UEditorAssetDrawButtionContext& Item) override
			{
				return ImGuIHelper::ImageButton(Item.ObjectPtr,AppFiles::sprite::AudioIcon, *(ImVec2*)&Item.ButtionSize);
			}
			void DrawInspect(const UEditorAssetDrawInspectContext& Item) override
			{
				if (ImGui::Button("Play"))
				{

					auto audio = UCode::AudioSystem::Get(EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_EditorLibrary());
					audio->Play(file, playsettings);
				}
			}
		};

		ExportFileRet ExportFile(const Path& path, const ExportFileContext& Item) override
		{
			return ExportFileRet();
		}

		virtual Unique_ptr<UEditorAssetFile> GetMakeNewAssetFile() override
		{
			return Unique_ptr< UEditorAssetFile>(new LiveingAsset());
		}

	};
class PNGAssetFile :public UEditorAssetFileData
{
public:
	PNGAssetFile()
	{
		FileExtWithDot = ".png";
		FileMetaExtWithDot = UEditorAssetFileData::DefaultMetaFileExtWithDot;
		CanHaveLiveingAssets = true;
	}
	using Compression_t = int;
	enum class Compression :Compression_t
	{
		None,
		LowQuality,
		NormalQuality,
		HighQuality,
	};

	using Filter_t = UCode::Texture::Filter_t;
	using Filter = UCode::Texture::Filiter;
	struct TextureSettings
	{
		UID uid;
		bool ReadAndWrite = false;
		Compression compression = Compression::None;
		Filter filter = Filter::Point;
		u32 pixelperunit = 16;
		
		Vector<SpriteItem> sprites;
	};
	class LiveingPng :public UEditorAssetFile
	{
	public:

		LiveingPng()
		{
			HasSubAssets = true;

		}
		
		TextureSettings setting;
		Optional<UCode::TextureAsset> asset;
		bool IsLoadingTexture = false;

		inline static const ImGuIHelper::EnumValue<Compression> CompressionEnumValues[] =
		{
			{"None",Compression::None},
			{"LowQuality",Compression::LowQuality},
			{"NormalQuality",Compression::NormalQuality},
			{"HighQuality",Compression::HighQuality},
		};
		static constexpr size_t CompressionEnumValuesSize = sizeof(CompressionEnumValues) / sizeof(CompressionEnumValues[0]);


		inline static const ImGuIHelper::EnumValue<Filter> FilterEnumValues[] =
		{
			{"Point",Filter::Point},
			{"Bilinear",Filter::bilinear},
			{"Trilinear",Filter::trilinear},
		};
		static constexpr size_t FilterEnumValuesSize = sizeof(FilterEnumValues) / sizeof(FilterEnumValues[0]);

		void LoadAssetIfNeeded(SpriteItem& spr)
		{
			if (!spr._Asset.get())
			{
				auto& textureasset = asset.value();
				auto& tex = textureasset._Base;
				UCode::Sprite sprite(&tex, spr.offset.X, spr.offset.Y, spr.size.X, spr.size.Y);

				spr._Asset = std::make_shared<UCode::SpriteAsset>(sprite, textureasset.GetManaged());
				spr._Asset->Uid = spr.uid;


				auto runtimeproject = UCodeEditor::EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
				auto p = FileFullPath.generic_string();
				auto relpath = p.substr(runtimeproject->GetAssetsDir().generic_string().size());

				spr._Asset->ObjectPath = relpath + EditorIndex::SubAssetSeparator + spr.spritename + UCode::SpriteData::FileExtDot;
			}
		}

		Optional<UID> OpenSpriteEditor;
		void DrawSubAssets(const UEditorDrawSubAssetContext& Item) override
		{
			if (!asset.has_value())
			{
				if (fs::exists(this->FileMetaFullPath.value()))
				{
					fromfile(FileMetaFullPath.value(), setting);
				}
				else
				{
					auto runinfo = UCodeEditor::EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
					RemoveSubAssets(runinfo->GetAssetsDir(), runinfo->Get_AssetIndex());
					setting.uid = runinfo->GetNewUID();
					tofile(FileMetaFullPath.value(), setting);
				}



				UCode::TextureAsset V(UCode::Texture(this->FileFullPath));
				SetupTexture(&V._Base);
				asset = std::move(V);
			}

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
		void Init(const UEditorAssetFileInitContext& Context) override
		{
			this->FileMetaFullPath = this->FileFullPath.native() + Path(UEditorAssetFileData::DefaultMetaFileExtWithDot).native();

		}
		void SaveFile(const UEditorAssetFileSaveFileContext& Context) override
		{
			bool hasdiff = true;
			if (hasdiff) 
			{
				auto runprojectdata = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
				
				if (tofile(this->FileMetaFullPath.value(), setting))
				{
					auto& index =runprojectdata->Get_AssetIndex();
					
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

		static  bool fromfile(const Path& settings, TextureSettings& Out)
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
		static  bool tofile(const Path& settings,const TextureSettings& in)
		{
			USerializer serializer;

			serializer.Write("_UID", in.uid);
			serializer.Write("_ReadAndWrite", in.ReadAndWrite);

			serializer.Write("_Filter", *(Filter_t*)&in.filter);

			serializer.Write("_Compression", *(Compression_t*)&in.compression);

			serializer.Write("_PixelPerUnit", in.pixelperunit);

			serializer.Write("_Sprites",in.sprites);

			return serializer.ToFile(settings);
		}
		void RemoveSubAssets(const Path& AssetDir, EditorIndex& index)
		{
			auto relpath = Path(FileFullPath.native().substr(AssetDir.native().size()));

			index._Files.erase(std::remove_if(
				index._Files.begin(), index._Files.end(), [&relpath](EditorIndex::IndexFile& val)
				{
					return val.RelativePath == relpath;
				}), index._Files.end());

		}

		
		bool DrawButtion(const UEditorAssetDrawButtionContext& Item) override
		{

			if (!asset.has_value())
			{
				if (fs::exists(this->FileMetaFullPath.value()))
				{
					fromfile(FileMetaFullPath.value(),setting);
				}
				else
				{
					auto runinfo = UCodeEditor::EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
					RemoveSubAssets(runinfo->GetAssetsDir(),runinfo->Get_AssetIndex());
					setting.uid = Item._newuid();
					tofile(FileMetaFullPath.value(), setting);
				}

		
				
				UCode::TextureAsset V(UCode::Texture(this->FileFullPath));
				SetupTexture(&V._Base);
				asset = std::move(V);

				return ImGuIHelper::ImageButton(Item.ObjectPtr, AppFiles::sprite::Entity, *(ImVec2*)&Item.ButtionSize);
			}
			else
			{
				
				return ImGuIHelper::ImageButton(Item.ObjectPtr, &asset.value()._Base, *(ImVec2*)&Item.ButtionSize);
			}
		}
		void RemoveSprite(UID id)
		{
			auto runtimeproject = UCodeEditor::EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
			runtimeproject->Get_AssetIndex().RemoveIndexFileWithUID(id);
		}
		void SetupTexture(UCode::Texture* tex)
		{
			tex->PixelsPerUnit = setting.pixelperunit;
		}
		NullablePtr<UC::Asset> LoadAsset(const LoadAssetContext& Item)  override
		{
			if (!asset.has_value())
			{
				//TODO add logs when error
				if (fs::exists(this->FileMetaFullPath.value()))
				{
					fromfile(FileMetaFullPath.value(), setting);
				}
				else
				{
					auto runinfo = UCodeEditor::EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();
					RemoveSubAssets(runinfo->GetAssetsDir(),runinfo->Get_AssetIndex());
					setting.uid = runinfo->GetNewUID();
					tofile(FileMetaFullPath.value(), setting);
				}

				const static Color32 colordata = { 0,0,0,0 };

				UCode::TextureAsset V(UCode::Texture(1,1,&colordata));
				SetupTexture(&V._Base);
				asset = std::move(V);
				UC::AssetRendering::LoadTextureAsync(EditorAppCompoent::GetCurrentEditorAppCompoent()->GetGameRunTime()->Get_Library_Edit(), this->FileFullPath)
					.OnCompletedOnThread([this,v = asset.value().GetManaged()](Unique_ptr<UC::Texture>& text)
				{
					if (v.Has_Value())
					{
						v.Get_Value()->_Base = std::move(*text.get());
						IsLoadingTexture = false;
					}
				},UC::TaskType::Main);
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
		
		bool ShouldBeUnloaded(const UEditorAssetShouldUnloadContext& Context) override
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
		void DrawInspect(const UEditorAssetDrawInspectContext& Item) override
		{
			{
				auto& files = UCodeEditor::EditorAppCompoent::GetCurrentEditorAppCompoent()->GetPrjectFiles();
				files.AssetIsInUse(this);
			}

			ImGui::BeginDisabled(true);


			String tep ="Image/Png";
			Item.Drawer->StringField("Type",tep);
			ImGui::SameLine();
			ImGuIHelper::Image(AppFiles::sprite::Uility_image, { 20 ,20 });


			String tep2 = FileFullPath.filename().replace_extension("").generic_string();
			Item.Drawer->StringField("Name", tep2);


			ImGui::EndDisabled();
			static void* p = nullptr;

			if (ImGui::Button("Open Sprite Editor")  || OpenSpriteEditor)
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
						
						bool WithEmpty =false;
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
						file.RelativePath =  FileFullPath.generic_string().substr(assetdir.generic_string().size());
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

							if (ImGui::TreeNode(&Item,ImGuIHelper::ToCStr(StringView(Item.spritename))))
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
								if (ImGui::MenuItem("Destroy",str.c_str()) || USettings.IsKeybindActive(KeyBindList::Delete))
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
						if (ImGui::BeginPopup("SpriteSlice"))
						{
							auto& slicedata = windowdata.slicedata;

							ImGuIHelper::uInt32Field(StringView("Sprite Width"), slicedata.SpriteW);
							ImGuIHelper::uInt32Field(StringView("Sprite Height"), slicedata.SpriteH);

							ImGuIHelper::uInt32Field(StringView("Padding Sprite Width"), slicedata.SpritePadingX);
							ImGuIHelper::uInt32Field(StringView("Padding Sprite Height"), slicedata.SpritePadingY);

							ImGuIHelper::BoolEnumField(StringView("Include Empty"), slicedata.WithEmpty);

							ImGui::SameLine();
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
											bool isempty = false;

											if (isempty)
											{
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

									for (size_t i = 0; i < setting.sprites.size() - newitems.size(); i++)
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
								auto tep =ImGui::GetMouseDragDelta(ImGuiMouseButton_::ImGuiMouseButton_Right);
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

							ImU32 texturbordercolor =ImGuIHelper::ColorToImguiU32(Color32(217, 51, 0));
							ImU32 spritebordercolor =ImGuIHelper::ColorToImguiU32(Color32());
							ImU32 selecedspritebordercolor =ImGuIHelper::ColorToImguiU32( Color32(204, 161, 4));

							for (size_t i = 0; i < setting.sprites.size(); i++)
							{
								auto& Item = setting.sprites[i];
						
								float HOffset = Item.offset.Y * windowdata.texturescale;
								float WOffset = Item.offset.X * windowdata.texturescale;
								float HSize = Item.size.Y;
								float WSize = Item.size.X;
							
								auto diffx = windowdata.dragoffset.X/ windowdata.texturescale;
								auto diffy = windowdata.dragoffset.Y/ windowdata.texturescale;

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
		
	};

	virtual Unique_ptr<UEditorAssetFile> GetMakeNewAssetFile() override
	{
		return Unique_ptr< UEditorAssetFile>(new LiveingPng());
	}
	ExportFileRet ExportFile(const Path& path, const ExportFileContext& Item) override
	{
		UCodeGEStackFrame("PngAsset:Export");
		Path metapath = path.native() + Path(UEditorAssetFileData::DefaultMetaFileExtWithDot).native();

		TextureSettings settings;
		if (!LiveingPng::fromfile(metapath, settings))
		{
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


	Optional<GetUIDInfo> GetFileUID(UEditorGetUIDContext& context) override
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
};


class EntityAssetFile :public UEditorAssetFileData
{
public:
	EntityAssetFile()
	{
		FileExtWithDot = RawEntityData::FileExtDot;
		CanHaveLiveingAssets = true;
	}

	class Liveing :public UEditorAssetFile
	{
	public:

		Liveing()
		{

		}
		
		void Init(const UEditorAssetFileInitContext& Context) override
		{

		}
		void SaveFile(const UEditorAssetFileSaveFileContext& Context) override
		{

		}
		bool DrawButtion(const UEditorAssetDrawButtionContext& Item) override
		{
			return ImGuIHelper::ImageButton(Item.ObjectPtr, AppFiles::sprite::RawEntityData, *(ImVec2*)&Item.ButtionSize);;
		}
		void DrawInspect(const UEditorAssetDrawInspectContext& Item) override
		{
			ImGui::BeginDisabled(true);


			ImGuIHelper::Image(AppFiles::sprite::Uility_image, { 20 ,20 });
			ImGui::SameLine();

			String tep ="Entity/Prefab";
			Item.Drawer->StringField("Type", tep);


			String tep2 = FileFullPath.filename().replace_extension("").generic_string();
			Item.Drawer->StringField("Name", tep2);


			ImGui::EndDisabled();

			ImGui::Separator();

			//

			//
		}
	};

	virtual Unique_ptr<UEditorAssetFile> GetMakeNewAssetFile() override
	{
		return Unique_ptr<UEditorAssetFile>(new Liveing());
	}

};

CoreModule::CoreModule()
{
    Init();
}

CoreModule::~CoreModule()
{

}

void CoreModule::Init()
{
	ID = UCODEEDITOR_MODULENAME_CORE;

	{
		Assets[0] = Unique_ptr<UEditorAssetFileData>(new ScencAssetFile());
		Assets[1] = std::move(Unique_ptr<UEditorAssetFileData>(new PNGAssetFile()));
		Assets[2] = std::move(Unique_ptr<UEditorAssetFileData>(new EntityAssetFile()));
		Assets[3] = std::move(Unique_ptr<UEditorAssetFileData>(new MP3AssetFile()));
	}

	{
		Components[0] = std::move(Unique_ptr<UEditorComponentData>(new SpriteRendererUEditorData()));
		Components[1] = std::move(Unique_ptr<UEditorComponentData>(new CameraUEditorData()));
	}
}
EditorEnd