#pragma once
#include  "UCodeRunTime/ULibrarys/AssetManagement/AssetManager.hpp"
#include "ProjectManagement/RunTimeProjectData.hpp"
#include "ImGuIHelper.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetRendering.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/CoreAssets.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AudioAssets.hpp"
#include "UEditorModules/Modules/Assets/Tilemap/TilePaletteData.hpp"
EditorStart
class ImGuIHelper_Asset
{
public:
	static bool DrawLayerField(const char* FieldName, UCode::RenderRunTime2d::DrawLayer_t& Value);

	static bool AsssetField(const char* FieldName, UCode::TexturePtr& Value);
	static bool AsssetField(const char* FieldName, UCode::TextureAssetPtr& Value);
	static bool AssetField(UCode::TexturePtr& Value);
	
	static bool AsssetField(const char* FieldName, UCode::SpritePtr& Value);
	static bool AsssetField(const char* FieldName, UCode::SpriteAssetPtr& Value);
	static bool AssetField(UCode::SpritePtr& Value);

	static bool AsssetField(const char* FieldName, UCode::ShaderPtr& Value);
	static bool AsssetField(const char* FieldName, UCode::ShaderAssetPtr& Value);

	static bool AsssetField(const char* FieldName, UCode::ScencPtr& Value);
	static bool AsssetField(const char* FieldName, UCode::ScencAssetPtr& Value);
	static bool AssetField(UCode::ScencPtr& Value);

	static bool AsssetField(const char* FieldName, TilePalettePtr& Value);
	static bool AsssetField(const char* FieldName, TilePaletteAssetPtr& Value);
	static bool AssetField(TilePalettePtr& Value);
	
	static bool AsssetField(const char* FieldName, TileDataPtr& Value);
	static bool AsssetField(const char* FieldName, TileDataAssetPtr& Value);
	static bool AssetField(TileDataPtr& Value);

	static bool AsssetField(const char* FieldName, UC::AudioAssetPtr& Value);
	static bool AsssetField(const char* FieldName, UC::AudioPtr& Value);
	static bool AssetField(UC::AudioPtr& Value);


	static bool AnyAsssetField(UID& Value);
	static bool AnyAsssetField(StringView FieldName, UID& Value)
	{
		ImGuIHelper::Text(FieldName);
		ImGui::SameLine();
		return AnyAsssetField(Value);
	}
	static bool AnyAsssetsField(StringView FieldName,Vector<UID>& Value);

	static bool IconField(StringView FieldName, UCode::SpritePtr& Value,ImVec2 IconSize);

	inline static UCode::AssetManager* AssetManager =nullptr;
	inline static RunTimeProjectData* ProjectData = nullptr;

	enum class AssetSpriteType
	{
		Icon,
		AssetType,
	};
	static UCode::Sprite* GetAssetSpriteFromUID(const UID& value,AssetSpriteType Type);
};
EditorEnd