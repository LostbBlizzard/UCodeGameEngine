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
	return  ProjectDir.native() + Path("Cache").native() + Path::preferred_separator;
}

Path ProjectManger::GetProjectAssetsDir(const Path&  ProjectDir)
{
	return ProjectDir.native() + Path("Assets").native() + Path::preferred_separator;
}

Path ProjectManger::GetProjectDataPath(const Path&  ProjectDir)
{
	return ProjectDir.native() + Path(ProjectData::FileName).native();
}

Path ProjectManger::GetAssetIndexPath(const Path& ProjectDir)
{
	return GetProjectCachedDir(ProjectDir).native() + Path(EditorIndex::FileName).native();
}

Path ProjectManger::GetProjectPrefsDir(const Path&  ProjectDir)
{
	return ProjectDir.native() + Path("Prefs").native() + Path::preferred_separator;
}

bool ProjectManger::MakeNewProject(const Path& Dir, const String& ProjectName,ProjectType ProjectType,Optional<String> ExternalProjectID)
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

			if (ProjectType == ProjectType::Empty)
			{

				//Just for Organization	
				fs::create_directory(AssetsDir / "Source");
				fs::create_directory(AssetsDir / "Source" / "Libraries");

				fs::create_directory(AssetsDir / "Audio");
				fs::create_directory(AssetsDir / "Audio" / "Music");
				fs::create_directory(AssetsDir / "Audio" / "Sounds");

				fs::create_directory(AssetsDir / "Art");
				fs::create_directory(AssetsDir / "Art" / "Fonts");
				fs::create_directory(AssetsDir / "Art" / "Shaders");

				fs::create_directory(AssetsDir / "GameStorage");
				fs::create_directory(AssetsDir / "GameData");

				fs::create_directory(AssetsDir / "EditorTools");
			}
			else if (ProjectType == ProjectType::PlugIn)
			{
				fs::create_directory(AssetsDir / "Source");
				fs::create_directory(AssetsDir / "Source" / "Libraries");

				fs::create_directory(AssetsDir / "Art");
				
				fs::create_directory(AssetsDir / "EditorTools");
			}
			else if (ProjectType == ProjectType::ExternalProjectType)
			{
				auto& ProjectTypeID = ExternalProjectID.value();

			}
			else
			{
				UCodeGEUnreachable();
			}
	
			{
				String gitnore;

				gitnore += "Cache \n";
				gitnore += "Prefs \n";
				std::ofstream out(Path(ProjectDir_s.native() + Path(".gitignore").native()));
				out << gitnore;
				out.close();
			}
		}

		return true;
	}
	
}

EditorEnd

