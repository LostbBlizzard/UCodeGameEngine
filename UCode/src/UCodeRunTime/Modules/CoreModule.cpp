#include "CoreModule.hpp"


#include "../ULibrarys/Rendering/SpriteRenderer2d.hpp"
#include "../ULibrarys/Rendering/Camera2d.hpp"
#include "../ULibrarys/UCodeLang/ULangRunTime.hpp"
#include "../ULibrarys/UCodeLang/ScirptableObject.hpp"
#include "../ULibrarys/AssetManagement/EntityPlaceHolder.hpp"

#include "../ULibrarys/AssetManagement/CoreAssets.hpp"
#include "../ULibrarys/AssetManagement/UCodeLangAssets.hpp"
CoreStart
CoreModule::CoreModule()
{
	_Compoents[0] = UModuleComponentData(&SpriteRenderer::type_Data);
	_Compoents[1] = UModuleComponentData(&Camera2d::type_Data);
	_Compoents[2] = UModuleComponentData(&ULangScript::type_Data);
	_Compoents[3] = UModuleComponentData(&EntityPlaceHolder::type_Data);

	_Assets[0] = UModuleAssetData(Path(UCode::Scene2dData::FileExtDot),[](UDeserializer& serializer)
	{
			UCodeGEDebugStackFrame("AssetLoader:LoadAsset:SceneData");
			Optional<Unique_ptr<Asset>> r;
			UCode::Scene2dData scene;

			if (UCode::Scene2dData::FromString(scene, serializer))
			{
				Optional<Unique_ptr<Asset>> r;
				Unique_ptr<Asset> v =std::make_unique<ScencAsset>(std::move(scene));

				r = std::move(v);
				return r;
			}

			return r;
	});
	_Assets[1] = UModuleAssetData(Path(UCode::ScirptableObjectData::FileExtDot),[](UDeserializer& serializer)
	{
			UCodeGEDebugStackFrame("AssetLoader:LoadAsset:ScirptableObject");
			Optional<Unique_ptr<Asset>> r;
			UCode::ScirptableObjectData scene;


			UCode::ULangRunTime::Get(Gamelibrary::Current());//force load

			if (UCode::ScirptableObjectData::FromString(scene, serializer))
			{
				Optional<Unique_ptr<Asset>> r;
				
				UCode::ScirptableObject b;
				b.LoadScript(scene);
				Unique_ptr<Asset> v =std::make_unique<ScirptableObjectAsset>(std::move(b));

				r = std::move(v);
				return r;
			}

			return r;
	});
	_Assets[2] = UModuleAssetData(Path(UCode::TextureData::FileExtDot),[](UDeserializer& serializer)
	{
			UCodeGEDebugStackFrame("AssetLoader:LoadAsset:TextureAsset");
			Optional<Unique_ptr<Asset>> r;
			UCode::TextureData scene;

			if (UCode::TextureData::FromString(scene, serializer))
			{
				Optional<Unique_ptr<Asset>> r;
			
				if (scene._TextureType == ".png") 
				{
					//To Do Load Texture using Threads
					UCode::Texture b(UCode::PngDataSpan(spanof(scene._TextureData)));
					b.PixelsPerUnit = scene.PixelPerunit;

					if (!scene.ReadWrite)
					{
						b.FreeFromCPU();
					}
					Unique_ptr<Asset> v = std::make_unique<TextureAsset>(std::move(b));

					TextureAsset* tex = v->GetAssetAs<TextureAsset>().value().value();

					r = std::move(v);
					return r;
				}
			}

			return r;
	});
	_Assets[3] = UModuleAssetData(Path(UCode::SpriteData::FileExtDot),[](UDeserializer& serializer)
	{
			UCodeGEDebugStackFrame("AssetLoader:LoadAsset:SpriteAsset");
			Optional<Unique_ptr<Asset>> r;
			UCode::SpriteData scene;

			if (UCode::SpriteData::FromString(scene, serializer))
			{
				Optional<Unique_ptr<Asset>> r;
				
				auto assetloader = AssetManager::Get(Gamelibrary::Current());
				auto b = assetloader->FindOrLoad(scene._Texture);
				
				Optional<TextureAssetPtr> tex;
				if (b.has_value())
				{
					if (b.value().Has_Value())
					{
						auto val = b.value().Get_Value();
						auto val2 = val->GetAssetAs<TextureAsset>();
						if (val2.has_value())
						{
							tex = val2.value().value()->GetManaged();
						}
					}
				}

				if (tex.has_value()) 
				{
					auto texture = tex.value();

					UCode::Sprite b(&texture.Get_Value()->_Base, scene.OffsetX, scene.OffsetY, scene.SizeX, scene.SizeY);
					Unique_ptr<Asset> v = std::make_unique<UCode::SpriteAsset>(std::move(b),texture);

					r = std::move(v);
					return r;
				}
			}

			return r;
	}); 
}
CoreModule::~CoreModule()
{

}

CoreEnd