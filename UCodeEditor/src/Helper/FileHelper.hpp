#pragma once
#include "Editor/EditorNamespace.hpp"
#include "UCodeRunTime/RunTimeBasicTypes/String.hpp"
#include <filesystem>
#include <Typedefs.hpp>
EditorStart
class FileHelper
{
public:
	enum class OpenFileResult
	{
		ERROR,       /* programmatic error */
		OKAY,        /* user pressed okay, or successful return */
		CANCEL       /* user pressed cancel */
	};
	struct OpenFileData
	{
		OpenFileResult Result = OpenFileResult::ERROR;
		String Path;
	};

	typedef UInt8 FileTypesOptions_t;
	enum class FileTypesOptions : FileTypesOptions_t
	{
		None = 0,
		png = 1 << 1,
		jpg = 1 << 2,
		bmp = 1 << 3,
		tga = 1 << 4,
	};
	static String GetNewFileName(const String&  path);
	static String GetNewFileName(const String&  path,const String&  ext);
	static String ToRelativePath(const String&  RootPath, const String&  Path);
	static StringView ToRelativePath(const StringView& RootPath, const StringView& Path);
	static Path ToRelativePath(const Path& RootPath, const Path& Path);
	static void ReverseString(String& r);
	static String GetFileExt(const String&  text);
	static Path GetFileExt(const Path& text)
	{
		return text.extension();
	}
	static OpenFileData OpenFileFromUser(FileTypesOptions_t filetypes = (FileTypesOptions_t)FileTypesOptions::None, const Path&  defaultPath = "");
	static OpenFileData OpenFileFromUser(const Path&  FileTypewithnoDot, const Path&  defaultPath = "");
	static OpenFileData OpenDirFromUser(const Path&  defaultPath = "");
	static OpenFileData SaveToFilePathFromUser(FileTypesOptions_t filetypes = (FileTypesOptions_t)FileTypesOptions::None, const Path&  defaultPath = "");
	static void OpenPathinFiles(const Path&  text);

	static Path Get_PersistentDataPath();
	static bool TrashFile(const Path& File);
	static bool TrashDir(const Path& Dir);
	static bool RenameFile(const Path& Filepath, const Path& NewFilePath);
	enum class FileType :unsigned char
	{
		Null,
		File,
		Dir,
		File_Image_Type,
		SceneData,
		RawEntityData,
		ULangFile,
		ULangLib,
		UPugin,
		Dll_File,
		metaFile,
		Color,
		TxtFile,
		glslFile,
	};
	static FileType GetFileType(const String& Ext);

	static void OpenExe(const Path& path,const String& Args);
private:
	static void Setfilters(const FileTypesOptions_t& filetypes, String&  filterList);
};


EditorEnd

