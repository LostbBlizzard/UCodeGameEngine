#include "ProjectFiles.hpp"
#include "EditorIndex.hpp"

EditorStart

Optional<size_t> ProjectFiles::FindAssetFile(const Path& path)
{
	for (size_t i = 0; i < _AssetFiles.size(); i++)
	{
		auto& Item = _AssetFiles[i];

		if (Item._File->FileFullPath == path)
		{
			return i;
		}
	}
	return {};
}

void ProjectFiles::AssetIsInUse(UEditorAssetFile* file)
{
	for (auto& Item : _AssetFiles)
	{
		if (Item._ManageFile.Has_Value() && Item._ManageFile.Get_Value() == file)
		{
			Item.LastUsed = AssetFileMaxLastUsed;
			break;
		}
	}
}

void ProjectFiles::Update(float UpateDelta)
{
	for (auto i = _AssetFiles.size() - 1; i != (std::vector<AssetFile>::size_type) - 1; i--)
	{
		auto& Item = _AssetFiles[i];

		if (Item.LastUsed < 0)
		{
			UEditorAssetShouldUnloadContext shouldunload;
			if (Item._File->ShouldBeUnloaded(shouldunload))
			{
				UEditorAssetFileSaveFileContext Context;
				Item._File->SaveFile(Context);

				_AssetFiles.erase(_AssetFiles.begin() + i);
			}
			else
			{
				Item.LastUsed = ProjectFiles::AssetFileMaxLastUsed;
			}
		}
		else
		{
			bool isinuse = Item._ManageFile.GetCounter() > 1;
			if (isinuse) 
			{
				Item.LastUsed = ProjectFiles::AssetFileMaxLastUsed;
			}
			else
			{
				Item.LastUsed -= UpateDelta;
			}
		}
	}
}

void ProjectFiles::ReIndex(EditorIndex& index, std::function<UID()> _newuid)
{
	using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

	auto Modules = UEditorModules::GetModules();


	index._Files.clear();
	for (const auto& dirEntry : recursive_directory_iterator(ProjDir))
	{
		Path FileExt = dirEntry.path().extension();
		if (dirEntry.is_regular_file())
		{
			EditorIndex::IndexFile Index;

			auto relative = dirEntry.path().generic_string().substr(ProjDir.native().size());

			Vector<GetSubAssetData> subassets;
			EditorIndex::UpdateFile(Index, dirEntry.path(), relative,subassets);

			for (auto& Item : subassets)
			{
				EditorIndex::IndexFile subIndex;
				subIndex.UserID = Item._ID;
				subIndex.RelativePath = Index.RelativePath;
				subIndex.RelativeAssetName = Index.RelativePath + EditorIndex::SubAssetSeparator + Item._SubAssetName;

				index._Files.push_back(std::move(subIndex));
			}
			index._Files.push_back(std::move(Index));
		}
	}
}

EditorEnd