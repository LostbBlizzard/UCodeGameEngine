#include "EditorAssetLoader.hpp"

EditorStart

NullablePtr<UCode::Asset> EditorAssetLoader::LoadAssetPtr(const UID& Path)
{
	auto f = RunTimeProject->Get_AssetIndex().FindFileUsingID(Path);
	if (f.has_value())
	{
		auto& fileindex = f.value();



		auto fullpath = RunTimeProject->GetAssetsDir() / fileindex.RelativePath;

		auto v = Files->FindAssetFile(fullpath);

		
		if (!v.has_value())
		{
			auto Modules = UEditorModules::GetModules();
			for (size_t i = 0; i < Modules.Size(); i++)
			{
				auto& item = Modules[i];


				auto AssetDataList = item->GetAssetData();
				UCode::Path FileExt = UCode::Path(fileindex.RelativePath).extension();
				auto Info = item->GetAssetDataUsingExt(FileExt);

				if (Info.has_value())
				{
					auto Data = AssetDataList[Info.value()];
					if (Data->CanHaveLiveingAssets)
					{
						auto Ptr = Data->GetMakeNewAssetFile();
						Ptr->FileFullPath = fullpath;
						UEditorAssetFileInitContext InitContext;
						Ptr->Init(InitContext);


						ProjectFiles::AssetFile tep1;
						tep1.Set(std::move(Ptr));
						tep1.LastUsed = ProjectFiles::AssetFileMaxLastUsed;

						Files->_AssetFiles.push_back(std::move(tep1));
						v = Files->_AssetFiles.size() - 1;
						break;
					}
				}
			}
		}
			
		if (v.has_value())
		{
			auto& runingfiles = Files->_AssetFiles[v.value()];
			if (runingfiles._ManageFile.Has_Value())
			{
				LoadAssetContext context;
				context._AssetToLoad = Path;
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
