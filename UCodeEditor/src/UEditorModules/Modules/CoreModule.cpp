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
            if (OnDropable)
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
		std::filesystem::copy_file(path, Item.Output, std::filesystem::copy_options::overwrite_existing);
	
		UCode::Scene2dData V;
		UCode::Scene2dData::FromFile(V, path);

		UCode::GameRunTime runtime;
		auto scenc = UCode::Scene2dData::LoadScene(&runtime,V);
		UCode::Scene2dData::SaveScene(scenc,V, USerializerType::Bytes);

		//UCode::Scene2dData::ToFile(Item.Output, V, USerializerType::Bytes);

		ExportFileRet r;
		r._UID = V._UID;
		return r;
	}
	Optional<UID> GetFileUID(UEditorGetUIDContext& context) override
	{
		UCode::Scene2dData V;
		UCode::Scene2dData::FromFile(V, context.AssetPath);
		return V._UID;
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


			ImGuIHelper::Image(AppFiles::sprite::Uility_image, { 20 ,20 });
			ImGui::SameLine();

			String tep = "Compoent/SpriteRenderer";
			ImGuIHelper::InputText("Type", tep);

			ImGui::EndDisabled();
			ImGui::SameLine();
			bool V = Component->Get_IsActive();
			ImGuIHelper::ToggleField("Active", V);
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


			ImGuIHelper::Image(AppFiles::sprite::Uility_image, { 20 ,20 });
			ImGui::SameLine();

			String tep = "Compoent/Camera";
			ImGuIHelper::InputText("Type", tep);
			ImGui::EndDisabled();
			ImGui::SameLine();
			bool V = Component->Get_IsActive();
			ImGuIHelper::ToggleField("Active", V);
			Component->Set_CompoentActive(V);

			
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

	using Filter_t = int;
	enum class Filter :Filter_t
	{
		Point,
		bilinear,
		trilinear,
	};
	struct SpriteItem
	{
		UID uid;
		String spritename;

		Vec2i_t<u32> offset;
		Vec2i_t<u32> size;
	};
	struct TextureSettings
	{
		UID uid;
		bool ReadAndWrite = false;
		Compression compression = Compression::None;
		Filter filter = Filter::Point;
		Vector<SpriteItem> sprites;
	};
	class LiveingPng :public UEditorAssetFile
	{
	public:

		LiveingPng()
		{

		}
		
		TextureSettings setting;
		Optional<UCode::TextureAsset> asset;
		

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

	
		void Init(const UEditorAssetFileInitContext& Context) override
		{
			this->FileMetaFullPath = this->FileFullPath.native() + Path(UEditorAssetFileData::DefaultMetaFileExtWithDot).native();

		}
		void SaveFile(const UEditorAssetFileSaveFileContext& Context) override
		{
			tofile(this->FileMetaFullPath.value(), setting);
		}

		static  bool fromfile(const Path& settings, TextureSettings& Out)
		{
			UDeserializer serializer;
			if (UDeserializer::FromFile(settings, serializer))
			{
				serializer.ReadType("_Uid", Out.uid);
				serializer.ReadType("_ReadAndWrite", Out.ReadAndWrite);

				serializer.ReadType("_Filter", *(Filter_t*)&Out.filter);

				serializer.ReadType("_Compression", *(Compression_t*)&Out.compression);

				return true;
			}
			return false;
		}
		static  bool tofile(const Path& settings,const TextureSettings& in)
		{
			USerializer serializer;

			serializer.Write("_Uid", in.uid);
			serializer.Write("_ReadAndWrite", in.ReadAndWrite);

			serializer.Write("_Filter", *(Filter_t*)&in.filter);

			serializer.Write("_Compression", *(Compression_t*)&in.compression);


			return serializer.ToFile(settings);
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
					setting.uid = Item._newuid();
					tofile(FileMetaFullPath.value(), setting);
				}

		
				
				UCode::TextureAsset V;
				V._Base = UCode::Texture(this->FileFullPath);
				asset = std::move(V);

				return ImGuIHelper::ImageButton(Item.ObjectPtr, AppFiles::sprite::Entity, *(ImVec2*)&Item.ButtionSize);
			}
			else
			{
				
				return ImGuIHelper::ImageButton(Item.ObjectPtr, &asset.value()._Base, *(ImVec2*)&Item.ButtionSize);
			}
		}
		void DrawInspect(const UEditorAssetDrawInspectContext& Item) override
		{
			ImGui::BeginDisabled(true);


			ImGuIHelper::Image(AppFiles::sprite::Uility_image, { 20 ,20 });
			ImGui::SameLine();

			String tep ="Image/Png";
			Item.Drawer->StringField("Type",tep);


			String tep2 = FileFullPath.filename().replace_extension("").generic_string();
			Item.Drawer->StringField("Name", tep2);


			ImGui::EndDisabled();
			static void* p = nullptr;

			if (ImGui::Button("Open Sprite Editor"))
			{
				p = this;
				ImGui::OpenPopup("SpriteAssetEditor");
			}

			ImGui::Separator();

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
						SliceWindowData slicedata;
					};
					static EditorWindowData windowdata;

					ImGuiWindowFlags flags = ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar;
					flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse;
				
					if (ImGui::Button("New Sprite"))
					{
						SpriteItem val;
						val.size = { 16,16 };
						val.offset = { 0,0 };

						val.spritename = "New Sprite " + std::to_string(setting.sprites.size());

						UID newid = UCodeEditor::EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData()->GetNewUID();
						val.uid = newid;

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

						for (size_t i = 0; i < setting.sprites.size(); i++)
						{ 
							auto& Item = setting.sprites[i];
						
							UCode::Sprite sp{ &asset.value()._Base,(i32)Item.offset.X,(i32)Item.offset.Y,(i32)Item.size.X,(i32)Item.size.Y };
							if (ImGuIHelper::TreeNode(&Item,StringView(Item.spritename), &sp))
							{
								ImGuIHelper::InputText("Sprite Name", Item.spritename);
								ImGuIHelper::uInt32Field("X offset", Item.offset.X);
								ImGuIHelper::uInt32Field("Y offset", Item.offset.Y);
								ImGuIHelper::uInt32Field("Width", Item.size.X);
								ImGuIHelper::uInt32Field("Height", Item.size.Y);

								ImGuIHelper::Image(&sp, { 125,125 });
								ImGui::TreePop();
							}

							bool remove = false;

							if (ImGui::BeginPopupContextItem(Item.spritename.c_str()))
							{
								ImGuIHelper::Text(StringView("Sprite options"));

								if (ImGui::MenuItem("Destroy"))
								{
									setting.sprites.erase(setting.sprites.begin() + i);
									remove = true;

								}
								
								ImGui::EndPopup();
							}
							
							if (remove)
							{
								break;
							}
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


								if (newitems.size() < setting.sprites.size())
								{
									for (size_t i = newitems.size(); i < setting.sprites.size(); i++)
									{
										auto& itemtoremove = setting.sprites[i];


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

										setting.sprites.push_back(std::move(item));
									}
									else
									{
										auto& olditem = setting.sprites[i];

										olditem.offset = newitem.offset;
										olditem.size = newitem.size;
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
								windowdata.dragoffset += *(Vec2*)&ImGui::GetMouseDragDelta(ImGuiMouseButton_::ImGuiMouseButton_Right);
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

							for (auto& Item : setting.sprites)
							{
								float HOffset = Item.offset.Y * windowdata.texturescale;
								float WOffset = Item.offset.X * windowdata.texturescale;
								float HSize = Item.size.Y;
								float WSize = Item.size.X;
							
								auto diffx = windowdata.dragoffset.X/ windowdata.texturescale;
								auto diffy = windowdata.dragoffset.Y/ windowdata.texturescale;
								list->AddRect({ startpos.x + WOffset + diffx ,
												startpos.y + HOffset + diffy },
											  { startpos.x + WOffset + diffx + (WSize * windowdata.texturescale),
												startpos.y + HOffset + diffy + (HSize * windowdata.texturescale) }, 0xFFFFFFFF);
							}

							ImGuIHelper::Image(&asset.value()._Base, *(ImVec2*)&objectsize);
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
			//
		}
		
	};

	virtual Unique_ptr<UEditorAssetFile> GetMakeNewAssetFile() override
	{
		return Unique_ptr< UEditorAssetFile>(new LiveingPng());
	}
	Optional<UID> GetFileUID(UEditorGetUIDContext& context) override
	{
		TextureSettings setting;
		Path metapath = context.AssetPath.native() + Path(UEditorAssetFileData::DefaultMetaFileExtWithDot).native();
		
		if (LiveingPng::fromfile(metapath, setting))
		{
			return setting.uid;
		}
		else
		{
			auto newid = context.GetNewUID();
			setting.uid = newid;

			LiveingPng::tofile(metapath, setting);
			return newid;
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
	}

	{
		Components[0] = std::move(Unique_ptr<UEditorComponentData>(new SpriteRendererUEditorData()));
		Components[1] = std::move(Unique_ptr<UEditorComponentData>(new CameraUEditorData()));
	}
}
EditorEnd