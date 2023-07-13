#pragma once


#include "../Core/GameRunTime.hpp"

#include <UCodeRunTime/RunTimeBasicTypes/String.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/Ref.hpp>
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/ULibrarys/Rendering/GpuTypes/Shader.hpp>
#include <UCodeRunTime/AppData.hpp>

#include "UCodeRunTime/ULibrarys/Serialization/USerializer.hpp"
#include "UCodeRunTime/ULibrarys/Serialization/FileBuffer.hpp"
#include <fstream>
#include "BookOfThreads.hpp"
CoreStart

struct GameFileIndex
{	
	Path FileFullName;
	size_t FileOffset=0;
	size_t FileSize = 0;

	inline Path Get_FileName() const
	{
		return FileFullName.filename();
	}
	inline Path Get_FileExtension() const
	{
		return FileFullName.extension();
	}
	inline Path Get_FileDirectory() const
	{
		return FileFullName.root_directory();
	}
	
};
class GameFilesData
{
public:
	using Type_t = unsigned char;
	enum class Type :Type_t
	{
		Null,
		ThisDir,
		ThisFile,
		Redirect,
	};
	GameFilesData():_Type(Type::Null), _RedirectDir(){}

	

	Type _Type;
	Path _RedirectDir;

	String CompanyName;
	String APPName;
	UID SceneToLoadOnPlay;
	
	Vector<GameFileIndex> Offsets;
	Vector<Byte> _Data;

	static bool MakeFile(const GameFilesData& data,const Path& Path,USerializerType Type);
	static bool ReadFile(const Path& Path, GameFilesData& data);
	static bool ReadFileKeepOpen(const Path& Path, FileBuffer& OutFile, GameFilesData& data);

	static void Serialize(const GameFilesData& data, USerializer& Out);
	static bool Deserializ(UDeserializer& In, GameFilesData& Out);

	void SetRedirectDir(const Path& path)
	{
		_Type = Type::Redirect;
		_RedirectDir = path;
	}

	inline static const char* FileDataName = "GameFiles.data";


	//a folder were important files and defaults. 
	static Path GetUCodeDir()
	{
		return ".UCode";
	}
private:
};

class GameFiles :private libraryBook
{
public:
	typedef unsigned char Byte;

	static GameFiles* Get(Gamelibrary* lib);
	static GameFiles* Find(const Gamelibrary* lib);
	static GameFiles* Init(Gamelibrary* lib, const GameFilesData& Data);
	

	static void Writetext(StringView text, const Path& Path);
	inline static void WriteBytes(const void* bytes, const size_t bytescount, const Path& Path)
	{
		WriteBytes((Byte*)bytes, bytescount, Path);
	}
	static void WriteBytes(const Byte* bytes, const size_t bytescount, const Path& Path);
	
	static bool MakeDir(const String& text);
	
	static String ReadFileAsString(const Path& Path);
	static Unique_Bytes ReadFileAsBytes(const Path& Path);
	static Unique_Bytes ReadFileAsBytes(const Path& Path,size_t Offset,size_t Size);

	Path GetFilePathByMove(const Path& path);
	Path GetFileFullName(const Path& FilePath) const;

	static Path Get_PersistentDataPath(const AppData& data);

	const Path& Get_PersistentDataPath();
	const Path& Get_CacheDataPath();

	Shader* Get_DefaultShader();

	void ReInit(const GameFilesData& data);
	void SetFileBuffer(FileBuffer&& Buffer);


	const GameFilesData& Get_FilesData()
	{
		return _Data;
	}
	
	AsynTask_t<Unique_Bytes> AsynReadGameFileFullBytes(const Path& Path);
	AsynTask_t<Unique_Bytes> AsynReadGameFileBytes(const Path& Path, size_t Offset, size_t Bytes);
	AsynTask_t<String> AsynReadGameFileString(const Path& Path);

	AsynTask_t<Unique_Bytes> AsynReadFileFullBytes(const Path& Path);
	AsynTask_t<Unique_Bytes> AsynReadFileBytes(const Path& Path, size_t Offset, size_t Bytes);

	AsynTask_t<String> AsynReadFileString(const Path& Path);

	AsynTask_t<Path> AsynGetFilePathByMove(const Path& path);
private:
	GameFiles(Gamelibrary* lib, const GameFilesData& Data);
	~GameFiles() override;

	FileBuffer _FileBuffer;
	GameFilesData _Data;
	Path _PersistentDataPath;
	Path _CacheDataPath;
	Unique_ptr<Shader> _DefaultShader;
};
CoreEnd

