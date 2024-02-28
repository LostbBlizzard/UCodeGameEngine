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

void ProjectFiles::Update(float UpateDelta)
{
	for (auto i = _AssetFiles.size() - 1; i != (std::vector<AssetFile>::size_type) - 1; i--)
	{
		auto& Item = _AssetFiles[i];

		if (Item.LastUsed < 0)
		{
			UEditorAssetFileSaveFileContext Context;
			Item._File->SaveFile(Context);

			_AssetFiles.erase(_AssetFiles.begin() + i);
		}
		else
		{
			Item.LastUsed -= UpateDelta;
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

			auto tep = dirEntry.last_write_time();
			Index.FileLastUpdatedTime = *(u64*)&tep;
			Index.FileHash = 0;
			Index.UserID = {};
			Index.FileSize = (u64)dirEntry.file_size();
			Index.RelativePath = dirEntry.path().generic_string().substr(ProjDir.native().size());

            bool FoundIt = false;
            for (size_t i = 0; i < Modules.Size(); i++)
            {
                auto& item = Modules[i];
                auto AssetDataList = item->GetAssetData();

                auto Info = item->GetAssetDataUsingExt(FileExt);
				if (Info.has_value())
				{
					auto Data = AssetDataList[Info.value()];
					FoundIt = true;

					UEditorGetUIDContext context;
					context.AssetPath = dirEntry.path();
					context._newuid = _newuid;

					auto op = Data->GetFileUID(context);

					if (op.has_value())
					{
						Index.UserID = op.value();
					}


					break;
				}
            }

			index._Files.push_back(std::move(Index));
		}
	}
}

EditorEnd