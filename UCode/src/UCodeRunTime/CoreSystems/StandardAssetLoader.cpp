#include "StandardAssetLoader.hpp"
#include "UCodeRunTime/Core/UModule.hpp"
CoreStart

Optional<Unique_ptr<Asset>> StandardAssetLoader::LoadAsset(const Path& Path)
{

	auto bytes = gamefiles->ReadGameFileAsBytes(Path);

	if (auto Val = UModules::GetAsset(Path.extension()))
	{
		UDeserializer V;
		V.SetData(bytes.AsView());
		return Val->LoadAsset(V);
	}
	return {};
	
}
CoreEnd