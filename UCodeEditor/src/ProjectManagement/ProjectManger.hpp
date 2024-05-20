#pragma once


#include <UCodeRunTime/BasicTypes.hpp>

#include <Editor/EditorNamespace.hpp>
#include <filesystem>
#include <Typedefs.hpp>
EditorStart
class FileExt
{
public:
	inline static const char* Data = "UData";
	inline static const char* Prefs = "UPref";
	
	inline static const char* Chahe = "UCach";
	inline static const char* Tep = "UTep";
};

class ProjectManger
{
public:
	UCODE_EDITOR_NODISCARD static Path GetProjectInitObjsDir(const Path&  ProjectDir);

	UCODE_EDITOR_NODISCARD static Path GetProjectCachedDir(const Path&  ProjectDir);

	UCODE_EDITOR_NODISCARD static Path GetProjectAssetsDir(const Path&  ProjectDir);

	UCODE_EDITOR_NODISCARD static Path GetProjectDataPath(const Path&  ProjectDir);

	UCODE_EDITOR_NODISCARD static Path GetProjectLockPath(const Path& ProjectDir);

	UCODE_EDITOR_NODISCARD static Path GetAssetIndexPath(const Path& ProjectDir);

	UCODE_EDITOR_NODISCARD static Path GetProjectPrefsDir(const Path&  ProjectDir);

	enum class ProjectType
	{
		Empty,
		PlugIn,
		ExternalProjectType,
	};


	UCODE_EDITOR_NODISCARD static bool MakeNewProject(const Path&  Dir, const String&  ProjectName,ProjectType ProjectType,Optional<String> ExternalProjectID);
};
EditorEnd

