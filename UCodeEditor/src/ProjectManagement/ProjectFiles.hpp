#pragma once

#include <Editor/EditorNamespace.hpp>
#include <UCodeRunTime/BasicTypes.hpp>
#include "UEditorModules/UEditorModule.hpp"

EditorStart

class EditorIndex;

class ProjectFiles
{
public:
	static constexpr float AssetFileMaxLastUsed = 30;
	struct AssetFile
	{
		Unique_ptr<UEditorAssetFile> _File;
		ManagedPtr<UEditorAssetFile> _ManageFile;

		float LastUsed = AssetFileMaxLastUsed;

		void Set(Unique_ptr<UEditorAssetFile> File)
		{
			_ManageFile.Set_Value(File.get());
			_File = std::move(File);
		}
		AssetFile() = default;
		AssetFile(AssetFile&& ToCopyFrom) = default;
		AssetFile& operator=(AssetFile&& ToCopyFrom) = default;

		~AssetFile()
		{
			if (_File.get())
			{
				_ManageFile.Set_Value(nullptr);
			}
		}
	};
	Vector<AssetFile> _AssetFiles;

	Optional<size_t> FindAssetFile(const Path& path);

	void Update(float UpateDelta);


	void Set_ProjDir(const Path& Proj)
	{
		ProjDir = Proj;
	}

	void ReIndex(EditorIndex& index, std::function<UID()> _newuid);
	std::function<UID()> _newuid;
private:
	Path ProjDir;
};
EditorEnd

