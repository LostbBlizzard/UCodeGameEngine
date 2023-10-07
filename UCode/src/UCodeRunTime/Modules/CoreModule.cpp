#include "CoreModule.hpp"


#include "../ULibrarys/Rendering/SpriteRenderer2d.hpp"
#include "../ULibrarys/Rendering/Camera2d.hpp"
#include "../ULibrarys/UCodeLang/ULangRunTime.hpp"
#include "../ULibrarys/AssetManagement/CoreAssets.hpp"
CoreStart
CoreModule::CoreModule()
{
	_Compoents[0] = UModuleComponentData(&SpriteRenderer::type_Data);
	_Compoents[1] = UModuleComponentData(&Camera2d::type_Data);
	_Compoents[2] = UModuleComponentData(&ULangScript::type_Data);

	_Assets[0] = UModuleAssetData(Path(UCode::Scene2dData::FileExtDot),[](UDeserializer& serializer)
	{
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
}
CoreModule::~CoreModule()
{

}
CoreEnd