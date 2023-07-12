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
	auto Path = GetAssetsDir().native() + ToPathChar("src") + Path::preferred_separator;

	fs::path Path_t = Path;
	if (!fs::exists(Path_t))
	{
		fs::create_directories(Path_t);
	}

	return Path;
}
Path RunTimeProjectData::GetSrcLibsDir()
{
	auto Path = GetSrcDir().native() + ToPathChar("Libraries") + Path::preferred_separator;

	fs::path Path_t = Path;
	if (!fs::exists(Path_t))
	{
		fs::create_directories(Path_t);
	}

	return Path;
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
	auto Path = ProjectManger::GetProjectCachedDir(_ProjDir).native() + ToPathChar("OutPut") + Path::preferred_separator;;
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
Path RunTimeProjectData::GetGameLibDir()
{
	auto Path = GetCachedDir().native() + ToPathChar("GameLib/");

	fs::path Path_t = Path;
	if (!fs::exists(Path_t))
	{
		if (fs::create_directories(Path_t))
		{
			
			//Copy defaults.
		}GameLibManger::MakedafalultLibObjs(Path);
	}
	
	return Path;
}
Path RunTimeProjectData::GetULangIntDir()
{
	auto Path = GetCachedDir().native() + ToPathChar("ULang/Int/");

	fs::path Path_t = Path;
	if (!fs::exists(Path_t))
	{
		if (fs::create_directories(Path_t))
		{

			//Copy defaults.
		}GameLibManger::MakedafalultLibObjs(Path);
	}

	return Path;
}
Path RunTimeProjectData::GetULangOutDir()
{
	auto Path = GetGameLibDir().native() + ToPathChar("Lib/");
	fs::path Path_t = Path;
	if (!fs::exists(Path_t))
	{
		if (fs::create_directories(Path_t))
		{

			//Copy defaults.
		}GameLibManger::MakedafalultLibObjs(Path);
	}
	return Path;
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