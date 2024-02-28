#pragma once
#include  "UCodeRunTime/ULibrarys/AssetManagement/AssetManager.hpp"
#include "ProjectManagement/RunTimeProjectData.hpp"
#include "ImGuIHelper.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetRendering.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/CoreAssets.hpp"
EditorStart
class ImGuIHelper_Asset
{
public:
	static bool DrawLayerField(const char* FieldName, UCode::RenderRunTime2d::DrawLayer_t& Value);

	static bool AsssetField(const char* FieldName, UCode::TexturePtr& Value);
	static bool AsssetField(const char* FieldName, UCode::TextureAssetPtr& Value);

	static bool AsssetField(const char* FieldName, UCode::SpritePtr& Value);
	static bool AsssetField(const char* FieldName, UCode::SpriteAssetPtr& Value);

	static bool AsssetField(const char* FieldName, UCode::ShaderPtr& Value);
	static bool AsssetField(const char* FieldName, UCode::ShaderAssetPtr& Value);

	static bool AsssetField(const char* FieldName, UCode::ScencPtr& Value);
	static bool AsssetField(const char* FieldName, UCode::ScencAssetPtr& Value);


	static bool AnyAsssetField(UID& Value);
	static bool AnyAsssetField(StringView FieldName, UID& Value)
	{
		ImGuIHelper::Text(FieldName);
		ImGui::SameLine();
		return AnyAsssetField(Value);
	}
	static bool AnyAsssetsField(StringView FieldName,Vector<UID>& Value);


	inline static UCode::AssetManager* AssetManager =nullptr;
	inline static RunTimeProjectData* ProjectData = nullptr;
};
EditorEnd