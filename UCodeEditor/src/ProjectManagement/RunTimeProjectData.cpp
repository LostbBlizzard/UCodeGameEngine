#include "RunTimeProjectData.hpp"
#include "ProjectManagement/ProjectManger.hpp"
#include "GameLibManger.hpp"
#include <filesystem>
EditorStart
namespace fs = std::filesystem;
RunTimeProjectData::RunTimeProjectData():_UIDGenerator(UCode::Random::RandomSeed())
{
}
void RunTimeProjectData::SetProject(const ProjectData& Data, const Path&  ProjDir)
{
	_Data = Data;
	_ProjDir = ProjDir;
	_Watcher.init(ProjDir);


	{
		auto Path = GetAssetIndexPath();

		if (!fs::exists(Path)) 
		{
			_Index._Files.clear();
			EditorIndex::ToFile(_Index, Path);
			_Index.GetDataFromDir(GetAssetsDir());
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

Path RunTimeProjectData::GetAssetsDir()
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
Path RunTimeProjectData::GetSrcDir()
{
	auto path = GetAssetsDir().native() + Path("src").native() + Path::preferred_separator;

	Path Path_t = path;
	if (!fs::exists(Path_t))
	{
		fs::create_directories(Path_t);
	}

	return path;
}
Path RunTimeProjectData::GetSrcLibsDir()
{
	auto path = GetSrcDir().native() + Path("Libraries").native() + Path::preferred_separator;

	Path Path_t = path;
	if (!fs::exists(Path_t))
	{
		fs::create_directories(Path_t);
	}

	return path;
}
Path RunTimeProjectData::GetCachedDir()
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
UCODE_EDITOR_NODISCARD Path RunTimeProjectData::GetOutDir()
{
	auto path = ProjectManger::GetProjectCachedDir(_ProjDir).native() + Path("OutPut").native() + Path::preferred_separator;;
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
Path RunTimeProjectData::GetGameLibDir()
{
	auto path = GetCachedDir().native() + Path("GameLib/").native();

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
Path RunTimeProjectData::GetULangIntDir()
{
	auto path = GetCachedDir().native() + Path("ULang/Int/").native();

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
Path RunTimeProjectData::GetULangOutDir()
{
	auto path = GetGameLibDir().native() + Path("Lib/").native();
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