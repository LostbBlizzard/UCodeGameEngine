#include "RunTimeProjectData.hpp"
#include "ProjectManagement/ProjectManger.hpp"
#include "GameLibManger.hpp"
#include <filesystem>
#include <ProjectManagement/ProjectFiles.hpp>
EditorStart
namespace fs = std::filesystem;
RunTimeProjectData::RunTimeProjectData():_UIDGenerator(UCode::Random::RandomSeed())
{
}
void RunTimeProjectData::SetProject(const ProjectData& Data, const Path&  ProjDir, ProjectFiles& files)
{
	_Data = Data;
	_ProjDir = ProjDir;

	auto assetpath = GetAssetsDir();
	
	_Watcher.init(assetpath);

	files.Set_ProjDir(assetpath);
	files._newuid = [this] {return GetNewUID(); };
	{
		auto Path = GetAssetIndexPath();

		if (!fs::exists(Path)) 
		{
			_Index._Files.clear();
			
			files.ReIndex(_Index, [this] {return GetNewUID(); },Data._SerializeType);

			EditorIndex::ToFile(_Index, Path);
		}
		else
		{
			EditorIndex::FromFile(_Index,Path);
		}
	}
}

void RunTimeProjectData::SetProjectToNull()
{
	
	_ProjDir = "";
	_Watcher.CloseWatcher();
}

Path RunTimeProjectData::GetAssetsDir() const
{
	auto Path = ProjectManger::GetProjectAssetsDir(_ProjDir);


	fs::path Path_t = Path;

	if (!fs::exists(Path_t))
	{
		if (!fs::create_directories(Path_t))
		{
			//idk then
		}
	}

	return Path;
}
Path RunTimeProjectData::GetSrcDir() const
{
	auto path = GetAssetsDir().native() + Path("Source").native() + Path::preferred_separator;

	Path Path_t = path;
	if (!fs::exists(Path_t))
	{
		fs::create_directories(Path_t);
	}

	return path;
}
Path RunTimeProjectData::GetSrcLibsDir() const
{
	auto path = GetSrcDir().native() + Path("Libraries").native() + Path::preferred_separator;

	Path Path_t = path;
	if (!fs::exists(Path_t))
	{
		fs::create_directories(Path_t);
	}

	return path;
}
Path RunTimeProjectData::GetCachedDir() const
{
	auto Path = ProjectManger::GetProjectCachedDir(_ProjDir);

	fs::path Path_t = Path;

	if (!fs::exists(Path_t))
	{
		if (!fs::create_directories(Path_t))
		{
			//idk then
		}
	}
	
	return Path;
}

Path RunTimeProjectData::GetCachedAssetsDir() const
{
	auto Path = ProjectManger::GetProjectCachedDir(_ProjDir) / "assets";

	fs::path Path_t = Path;

	if (!fs::exists(Path_t))
	{
		if (!fs::create_directories(Path_t))
		{
			//idk then
		}
	}
	
	return Path;


}
Path RunTimeProjectData::GetOutDir() const
{
	auto path = ProjectManger::GetProjectCachedDir(_ProjDir).native() + Path("output").native() + Path::preferred_separator;;
	fs::path Path_t = path;

	if (!fs::exists(Path_t))
	{
		if (!fs::create_directories(Path_t))
		{
			//idk then
		}
	}
	return path;
}
Path RunTimeProjectData::GetGameLibDir() const
{
	auto path = GetCachedDir().native() + Path("gamelib/").native();

	fs::path Path_t = path;
	if (!fs::exists(Path_t))
	{
		if (fs::create_directories(Path_t))
		{
			
			//Copy defaults.
		}GameLibManger::MakedafalultLibObjs(path);
	}
	
	return path;
}
Path RunTimeProjectData::GetULangIntDir() const
{
	auto path = GetCachedDir().native() + Path("ulang/").native();

	Path Path_t = path;
	if (!fs::exists(Path_t))
	{
		if (fs::create_directories(Path_t))
		{

			//Copy defaults.
		}GameLibManger::MakedafalultLibObjs(path);
	}

	return path;
}
Path RunTimeProjectData::GetULangOutDir() const
{
	auto path = GetGameLibDir().native() + Path("bin/").native();
	Path Path_t = path;
	if (!fs::exists(Path_t))
	{
		if (fs::create_directories(Path_t))
		{

			//Copy defaults.
		}GameLibManger::MakedafalultLibObjs(path);
	}
	return path;
}
Path RunTimeProjectData::Get_ProjectPrefsDir()
{
	auto Path = ProjectManger::GetProjectPrefsDir(_ProjDir);
	fs::path Path_t = Path;

	if (!fs::exists(Path_t))
	{
		if (!fs::create_directories(Path_t))
		{
			//idk then
		}
	}
	return Path;
}
UCode::UID RunTimeProjectData::GetNewUID()
{
	UCode::UID id;
	UCode::UID::MakeNew(_UIDGenerator,id);
	return id;
}

UCODE_EDITOR_NODISCARD Path RunTimeProjectData::GetProjectDataPath()
{
	return ProjectManger::GetProjectDataPath(_ProjDir);
}

UCODE_EDITOR_NODISCARD Path RunTimeProjectData::GetAssetIndexPath()
{
	return ProjectManger::GetAssetIndexPath(_ProjDir);
}

EditorEnd