#include "EditorAssetLoader.hpp"

EditorStart

NullablePtr<UCode::Asset> EditorAssetLoader::LoadAssetPtr(const UID& Path)
{
  auto f =RunTimeProject->Get_AssetIndex().FindFileUsingID(Path);
  if (f.has_value())
  {
	  auto& fileindex = f.value();

	  auto v = Files->FindAssetFile(fileindex.RelativePath);
	  if (v.has_value())
	  {
		  auto& runingfiles = Files->_AssetFiles[v.value()];
		  if (runingfiles._ManageFile.Has_Value())
		  {
			  LoadAssetContext context;
			  return runingfiles._ManageFile.Get_Value()->LoadAsset(context); 
		  }
	  }
  }

	  
	return {};
}

NullablePtr<UCode::Asset> EditorAssetLoader::LoadAssetPtr(const Path& Path)
{
	UCodeGEToDo();
	return {};
}

EditorEnd
