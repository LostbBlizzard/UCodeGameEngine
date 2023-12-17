#include "FileHelper.hpp"
#include <OtherLibrarys/Nativefiledialog/nfd.h>
#include <OtherLibrarys/Nativefiledialog/nfd_common.h>



//File exts
#include <EditorWindows/OtherTypes/RawEntityData.hpp>
#include <EditorWindows/OtherTypes/ImageData.hpp>
#include <ULang/UPlugin.hpp>
#include <EditorWindows/OtherTypes/ColorData.hpp>
#include <UCodeRunTime/ULibrarys/AssetManagement/SceneData.hpp>
#include <UCodeLang/LangCore/FileHelper.hpp>

#include <UCodeRunTime/CoreSystems/Threads.hpp>

#if UCodeGEWindows
#include <Windows.h>
#include <shellapi.h>
#endif // DEBUG
namespace fs = std::filesystem;
EditorStart





Path FileHelper::GetNewFileName(const Path&  path)
{
	Path newPath = path;
	if (!fs::exists(newPath)) { return newPath; }
	for (size_t i = 0; i < 100; i++)
	{
		newPath = path.native() + Path(' ' + std::to_string(i)).native();
		if (!fs::exists(newPath)) { return newPath; }
	}
	return path;
}

Path FileHelper::GetNewFileName(const Path&  path, const Path&  ext)
{
	Path newPath = path.native() + Path(ext).native();
	if (!fs::exists(newPath)) { return newPath; }
	for (size_t i = 0; i < 100; i++)
	{
		newPath = path.native() + Path(' ' + std::to_string(i)).native() + ext.native();
		if (!fs::exists(newPath)) { return newPath; }
	}
	return path;
}

Path FileHelper::ToRelativePath(const Path&  RootPath, const Path&  Path)
{
	if (Path.native().length() < RootPath.native().length())
	{
		return Path;
	}
	return Path.native().substr(RootPath.native().length());
}

PathSpan FileHelper::ToRelativePath(const PathSpan& RootPath, const PathSpan& Path)
{
	if (Path.length() < RootPath.length())
	{
		return Path;
	}
	return Path.substr(RootPath.length());
}


void FileHelper::ReverseString(String&  r)
{
	size_t LastI = r.size() -1;
	for (size_t i = 0; i < r.size() / 2;)
	{
		char A = r[i];
		char B = r[LastI];

		r[i] = B;
		r[LastI] = A;

		i++;
		LastI--;
	}
}
FileHelper::OpenFileData FileHelper::OpenFileFromUser(FileTypesOptions_t filetypes,const Path&  defaultPath)
{

	nfdchar_t* outPath = NULL;

	String filterList;

	Setfilters(filetypes, filterList);

	String TepString;
	const char* FilePath = nullptr;
	if (defaultPath.native().size() != 0) 
	{
		TepString = defaultPath.generic_string();
		FilePath = TepString.c_str();
	}


	nfdresult_t result = NFD_OpenDialog(filterList.c_str(), FilePath, &outPath);

	FileHelper::OpenFileData r;
	if (outPath) { r.Path = UCode::String(outPath); NFDi_Free(outPath); }
	r.Result = (FileHelper::OpenFileResult)result;//same type
	
	
	return r;
}

FileHelper::OpenFileData FileHelper::OpenFileFromUser(const Path&  FileType, const Path&  defaultPath)
{
	nfdchar_t* outPath = NULL;

	String filterList;

	String TepString;
	const char* FilePath = nullptr;
	if (defaultPath.native().size() != 0) 
	{
		TepString = defaultPath.generic_string();
		FilePath = TepString.c_str();
	}
	if (!fs::exists(FilePath))
	{
		FilePath = nullptr;
	}

	nfdresult_t result = NFD_OpenDialog(FileType.generic_string().c_str(), FilePath, &outPath);

	FileHelper::OpenFileData r;
	if (outPath) { r.Path = UCode::String(outPath); NFDi_Free(outPath); }
	r.Result = (FileHelper::OpenFileResult)result;//same type


	return r;
}

FileHelper::OpenFileData FileHelper::OpenDirFromUser(const Path&  defaultPath)
{
	nfdchar_t* outPath = NULL;

	String TepString;
	const char* FilePath = nullptr;

	{
		TepString = defaultPath.generic_string();
		FilePath = TepString.c_str();
	}
	if (!fs::exists(FilePath))
	{
		FilePath = nullptr;
	}


	nfdresult_t result = NFD_PickFolder(FilePath, &outPath);

	FileHelper::OpenFileData r;
	if (outPath) { r.Path = UCode::String(outPath); NFDi_Free(outPath); }
	r.Result = (FileHelper::OpenFileResult)result;//same type


	return r;
}

FileHelper::FileType FileHelper::GetFileType(const String& Ext)
{

	if (Ext == ".png") {return FileType::File_Image_Type; }
	else if (Ext == RawEntityData::FileExtDot) { return FileType::RawEntityData; }
	else if (Ext == UCode::Scene2dData::FileExtDot) { return FileType::SceneData; }
	else if (Ext == UCodeLang::FileExt::SourceFileWithDot) { return  FileType::ULangFile; }
	else if (Ext == UCodeLang::FileExt::LibWithDot) { return FileType::ULangLib; }
	else if (Ext == UPlugin::FileExWithDot) { return FileType::UPugin; }
	else if (Ext == ".dll") { return FileType::Dll_File; }
	else if (Ext == ColorData::FileExtDot) { return FileType::Color; }
	else if (Ext == TextureData::FileExtDot) { return FileType::metaFile; }
	else if (Ext == ".txt") { return FileType::TxtFile; }
	else if (Ext == ".glsl") { return FileType::glslFile; }
	else { return FileType::File; }
}

void FileHelper::OpenExe(const Path& path, const String& Args)
{
#if UCodeGEWindows
	Path t = "open";
	Path args =Path(Args.c_str());
	ShellExecute(NULL, t.c_str(), path.c_str(), args.c_str(), NULL, SW_SHOWDEFAULT);
#endif 
}

void FileHelper::OpenExeSubProcess(const Path& path, const String& Args)
{
	String SysCall = path.generic_string() + " " + Args;
	system(SysCall.c_str());
}

void FileHelper::Setfilters(const FileHelper::FileTypesOptions_t& filetypes, String&  filterList)
{
	if (filetypes & (FileTypesOptions_t)FileTypesOptions::png) { filterList += ",png"; }
	if (filetypes & (FileTypesOptions_t)FileTypesOptions::jpg) { filterList += ",jpg"; }
	if (filetypes & (FileTypesOptions_t)FileTypesOptions::bmp) { filterList += ",bmp"; }
	if (filetypes & (FileTypesOptions_t)FileTypesOptions::tga) { filterList += ",tga"; }
}

FileHelper::OpenFileData FileHelper::SaveToFilePathFromUser(FileTypesOptions_t filetypes, const Path&  defaultPath)
{
	nfdchar_t* outPath = NULL;

	String filterList;

	Setfilters(filetypes, filterList);


	String TepString;
	const char* FilePath = nullptr;
	if (defaultPath.native().size() != 0) 
	{
		TepString = defaultPath.generic_string();
		FilePath = TepString.c_str();
	}

	nfdresult_t result = NFD_SaveDialog(filterList.c_str(), FilePath, &outPath);

	FileHelper::OpenFileData r;
	if (outPath) { r.Path = UCode::String(outPath); NFDi_Free(outPath); }
	r.Result = (FileHelper::OpenFileResult)result;//same type


	return r;
}

void FileHelper::OpenPathinFiles(const Path&  Dir)
{
	
	#if UCodeGEWindows
	ShellExecuteA(NULL, "open", (LPSTR)Dir.generic_string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
	
#else
	UCodeGEThrow("OpenPathinFiles");
#endif
	
}

Path FileHelper::Get_PersistentDataPath()
{
	//UCode::AppData Data;
	//Data.CompanyName = "Lost blizzard";
	//Data.APPName = "UCodeEditor";
	Path R;// = UCode::GameFiles::Get_PersistentDataPath(Data);
	
	#if UCodeGEDebug
	R = UCode_VS_PROJECTPATH "PersistentData/";
	#endif // DEBUG
	if (!fs::exists(R))
	{
		if (fs::create_directory(R)) {

		}
	}
	return R;
}

bool FileHelper::TrashFile(const Path& File)
{
	return fs::remove(File);
	#if UCodeGEWindows

	auto widestr = File.native();

	SHFILEOPSTRUCT fileOp;
	fileOp.hwnd = NULL;
	fileOp.wFunc = FO_DELETE;
	fileOp.pFrom = widestr.c_str();
	fileOp.pTo = NULL;
	fileOp.fFlags = FOF_ALLOWUNDO | FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_SILENT;
	int result = SHFileOperation(&fileOp);

	if (result != 0) {
		return false;
	}
	else {
		return true;
	}
	return false;
#else
	UCodeGEThrow("TrashFile")
#endif

}

bool FileHelper::TrashDir(const Path& Dir)
{
	return TrashFile(Dir);
}

bool FileHelper::RenameFile(const Path& Filepath, const Path& NewFilePath)
{
	fs::rename(Filepath, NewFilePath);
	return true;
}




EditorEnd

