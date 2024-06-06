#pragma once
#include "UEditorModules/Modules/CoreModule.hpp" 
#include "UCodeRunTime/ULibrarys/AssetManagement/SceneData.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/RawEntityData.hpp"
EditorStart

struct RenderFrameData
{
	Vec3 CamPos;
	float CamWidth = 500;
	float CamHeight = 500;
	float CamOrth = 5;
};

UC::ImageData RenderFrame(RenderFrameData& Data, UC::RenderRunTime2d::DrawData& drawdata, UC::Entity* newentity);


class EntityAssetFile :public UEditorAssetFileData
{
public:
	EntityAssetFile();

	class Liveing :public UEditorAssetFile
	{
	public:

		Liveing();
		~Liveing();
		UC::RawEntityDataAsset _asset;
		UC::EntityPtr _entity;
		Vector<Byte> _assetasbytes;
		Optional<UC::Texture> _Thumbnail;
		Optional<UC::Sprite> _ThumbnailSprite;
		bool hascalledloadthumnail = false;

		UC::RunTimeScene* GetTepScene();
		UC::Entity* GetAsEntity();
		Vector<Byte> GetAssetAsBytes(UC::Scene2dData::Entity_Data& data);
		void Init(const UEditorAssetFileInitContext& Context) override;

		Path ThumbnailPath();
		void LoadThumbnail();
		void SaveFile(const UEditorAssetFileSaveFileContext& Context) override;
		bool DrawButtion(const UEditorAssetDrawButtionContext& Item) override;
		USerializerType GetSerializerTypeForAsset();
		bool WasEntityUpdated(UC::Entity* e);
		void SaveEntity(UC::Entity* e);
		void DrawInspect(const UEditorAssetDrawInspectContext& Item) override;
		void FileUpdated() override;
		NullablePtr<UCode::Asset> LoadAsset(const LoadAssetContext& Item) override;
	};



	Unique_ptr<UEditorAssetFile> GetMakeNewAssetFile() override
	{
		return Unique_ptr<UEditorAssetFile>(new Liveing());
	}

	Optional<GetUIDInfo> GetFileUID(UEditorGetUIDContext& context) override;

};
EditorEnd
