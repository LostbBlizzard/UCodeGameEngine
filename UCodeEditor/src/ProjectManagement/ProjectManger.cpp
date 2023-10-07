#include "ProjectManger.hpp"
#include <fstream>
#include "ProjectData.hpp"
#include "EditorIndex.hpp"
EditorStart



namespace fs = std::filesystem;


Path ProjectManger::GetProjectInitObjsDir(const Path&  ProjectDir)
{
	return GetProjectCachedDir(ProjectDir).native() + Path("Init").native() + Path::preferred_separator;
}

Path ProjectManger::GetProjectCachedDir(const Path&  ProjectDir)
{
	return  ProjectDir.native() + Path("CachedData").native() + Path::preferred_separator;
}

Path ProjectManger::GetProjectAssetsDir(const Path&  ProjectDir)
{
	return ProjectDir.native() + Path("Assets").native() + Path::preferred_separator;
}

Path ProjectManger::GetProjectDataPath(const Path&  ProjectDir)
{
	return ProjectDir.native() + Path(ProjectData::FileName).native();
}

UCODE_EDITOR_NODISCARD Path ProjectManger::GetAssetIndexPath(const Path& ProjectDir)
{
	return GetProjectCachedDir(ProjectDir).native() + Path(EditorIndex::FileName).native();
}

Path ProjectManger::GetProjectPrefsDir(const Path&  ProjectDir)
{
	return ProjectDir.native() + Path("Prefs").native() + Path::preferred_separator;
}

bool ProjectManger::MakeNewProject(const Path& Dir, const String& ProjectName)
{

	ProjectData Data = ProjectData();
	Data._ProjectName = ProjectName;



	const Path ProjectDir_s = Dir.native() + Path(ProjectName).native() + Path::preferred_separator;
	const Path ProjectDataPath_s = GetProjectDataPath(ProjectDir_s);
	


	

	if (fs::create_directory(ProjectDir_s))
	{
		if (!ProjectData::WriteToFile(ProjectDataPath_s, Data)) { return false; }
		const fs::path AssetsDir = GetProjectAssetsDir(ProjectDir_s);
		if (fs::create_directory(AssetsDir))
		{

			//Just for Organization
			fs::create_directory(AssetsDir / "Packages");
			fs::create_directory(AssetsDir / "Scripts");
			fs::create_directory(AssetsDir / "Audio");
			fs::create_directory(AssetsDir / "Audio" / "Music");
			fs::create_directory(AssetsDir / "Audio" / "Sounds");
			fs::create_directory(AssetsDir / "Art");
			fs::create_directory(AssetsDir / "Art" / "Fonts");
			fs::create_directory(AssetsDir / "GameStorage");
			fs::create_directory(AssetsDir / "GameData");


			fs::create_directory(AssetsDir / "EditorTools");
		}
	}
	return true;
}

EditorEnd

