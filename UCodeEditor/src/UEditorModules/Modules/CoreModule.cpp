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
                String Text = "Drop Scene Here?";
                ImGui::Text(Text.c_str());
            }
            else
            {
                String Text = "Draging Scene";
                ImGui::Text(Text.c_str());
            }

            ImGui::EndDragDropSource();
        }
	}

	ExportFileRet ExportFile(const Path& path, const ExportFileContext& Item) override
	{
		UCode::Scene2dData V;
		UCode::Scene2dData::FromFile(V, path);

		UCode::GameRunTime runtime;
		auto scenc = UCode::Scene2dData::LoadScene(&runtime,V);
		UCode::Scene2dData::SaveScene(scenc,V, USerializerType::Bytes);

		UCode::Scene2dData::ToFile(Item.Output, V, USerializerType::Bytes);

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
	struct TextureSettings
	{
		UID uid;
		bool ReadAndWrite = false;
		Compression compression = Compression::None;
		Filter filter = Filter::Point;
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

			ImGui::Separator();

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