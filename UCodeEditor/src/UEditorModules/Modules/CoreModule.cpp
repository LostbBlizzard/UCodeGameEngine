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
			ImGuIHelper::InputText("Type", (String)"Compoent/SpriteRenderer");

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
			float32 V = Component->Get_Ortho_size();
			ImGuIHelper::float32Field("Ortho_Size", V);
			Component->Set_Ortho_size(V);
		}
		else
		{
			float32 V = Component->Get_Fov();
			ImGuIHelper::float32Field("Fov", V);
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
			ImGuIHelper::InputText("Type", (String)"Compoent/Camera");
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
			float32 V = Component->Get_Ortho_size();
			Data.Draw->float32Field("Ortho_Size", V);
			Component->Set_Ortho_size(V);
		}
		else
		{
			float32 V = Component->Get_Fov();
			Data.Draw->float32Field("Fov", V);
			Component->Set_Fov(V);
		}
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

	class LiveingPng :public UEditorAssetFile
	{
	public:

		LiveingPng()
		{

		}
		enum class Compression
		{
			None,
			LowQuality,
			NormalQuality,
			HighQuality,
		};
		enum class Filter
		{
			Point,
			bilinear,
			trilinear,
		};
		bool ReadAndWrite = false;
		Compression compression = Compression::None;
		Filter filter = Filter::Point;


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

		}
		void SaveFile(const UEditorAssetFileSaveFileContext& Context) override
		{

		}
		bool DrawButtion(const UEditorAssetDrawButtionContext& Item) override
		{
			return ImGuIHelper::ImageButton(Item.ObjectPtr, AppFiles::sprite::Entity, *(ImVec2*)&Item.ButtionSize);;
		}
		void DrawInspect(const UEditorAssetDrawInspectContext& Item) override
		{
			ImGui::BeginDisabled(true);


			ImGuIHelper::Image(AppFiles::sprite::Uility_image, { 20 ,20 });
			ImGui::SameLine();
			Item.Drawer->StringField("Type", (String)"Image/Png");

			Item.Drawer->StringField("Name", FileFullPath.filename().replace_extension("").generic_string());


			ImGui::EndDisabled();

			ImGui::Separator();

			//

			ImGuIHelper::BoolEnumField("ReadAndWrite", ReadAndWrite);
			ImGuIHelper::EnumField("Compression", compression, CompressionEnumValues, CompressionEnumValuesSize);
			ImGuIHelper::EnumField("Filter", filter, FilterEnumValues, FilterEnumValuesSize);
			//
		}
	};

	virtual Unique_ptr<UEditorAssetFile> GetMakeNewAssetFile() override
	{
		return Unique_ptr< UEditorAssetFile>(new LiveingPng());
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
			Item.Drawer->StringField("Type", (String)"Entity/Prefab");

			Item.Drawer->StringField("Name", FileFullPath.filename().replace_extension("").generic_string());


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