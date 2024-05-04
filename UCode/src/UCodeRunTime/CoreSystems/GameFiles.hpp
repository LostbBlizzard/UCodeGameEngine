#pragma once


#include "../Core/GameRunTime.hpp"

#include <UCodeRunTime/BasicTypes.hpp>
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/ULibrarys/Rendering/GpuTypes/Shader.hpp>
#include <UCodeRunTime/AppData.hpp>

#include "UCodeRunTime/ULibrarys/Serialization/USerializer.hpp"
#include "UCodeRunTime/ULibrarys/Serialization/FileBuffer.hpp"
#include <fstream>
#include "Threads.hpp"
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

	//may be nullptr
	const GameFileIndex* GetFile(const Path& path) const
	{
		for (auto& Item : Offsets)
		{
			if (Item.FileFullName == path)
			{
				return &Item;
			}
		}
		return nullptr;
	}
	Vector<const GameFileIndex*> GetFilesInDir(const Path& path)
	{
		Vector<const GameFileIndex*> R;
		GetFilesInDir(R, path);

		return R;
	}
	void GetFilesInDir(Vector<const GameFileIndex*>& Out, const Path& path) const;
	void GetFilesWithExt(Vector<const GameFileIndex*>& Out, const Vector<GameFileIndex>& Files) const;
	void GetFilesWithExt(Vector<const GameFileIndex*>& Out, const Vector<const GameFileIndex*>& Files) const;

	static bool MakeFile(const GameFilesData& data,const Path& Path,USerializerType Type);
	static bool ReadFile(const Path& Path, GameFilesData& data);
	static bool ReadFileKeepOpen(const Path& Path, FileBuffer& OutFile, GameFilesData& data);

	static void Serialize(const GameFilesData& data, USerializer& Out);
	static bool Deserializ(UDeserializer& In, GameFilesData& Out);


	static std::ofstream StartWritingBytes(const GameFilesData& data,const Path& path,size_t BufferSize);

	void SetRedirectDir(const Path& path)
	{
		_Type = Type::Redirect;
		_RedirectDir = path;
	}

	inline static const char* FileDataName = "GameFiles.data";
	inline static const char* FileExtWithDot = ".data";

	//a folder were important files and defaults. 
	static Path GetUCodeDir()
	{
		return ".ucode";
	}

	static bool PackDir(const Path& dirpath, const Path& outfile);
	static bool UnPackToDir(const Path& datafile,const Path& dirpath);
private:
};

class GameFiles :private System
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

	String ReadGameFileAsString(const Path& Path);
	Unique_Bytes ReadGameFileAsBytes(const Path& Path);
	Unique_Bytes ReadGameFileAsBytes(const Path& Path, size_t Offset, size_t Size);
	bool CopyGameFileTo(const Path& path, const Path& outpath);

	Path GetGameFilePathByMove(const Path& path);
	Path GetFileFullName(const Path& FilePath) const;

	static Path Get_PersistentDataPath(const AppData& data);

	const Path& Get_PersistentDataPath();
	const Path& Get_CacheDataPath();

	Shader* Get_DefaultShader();

	void ReInit(const GameFilesData& data);
	void SetFileBuffer(FileBuffer&& Buffer);


	const GameFilesData& Get_FilesData()
	{
		return _MainState.Unsafe_GetBaseType()._Data;
	}
	
	AsynTask_t<Unique_Bytes> AsynReadGameFileFullBytes(const Path& Path);
	AsynTask_t<Unique_Bytes> AsynReadGameFileBytes(const Path& Path, size_t Offset, size_t Bytes);
	AsynTask_t<String> AsynReadGameFileString(const Path& Path);

	AsynTask_t<Unique_Bytes> AsynReadFileFullBytes(const Path& Path);
	AsynTask_t<Unique_Bytes> AsynReadFileBytes(const Path& Path, size_t Offset, size_t Bytes);

	AsynTask_t<String> AsynReadFileString(const Path& Path);

	AsynTask_t<Path> AsynGetGameFilePathByMove(const Path& path);

	Path GetMovedFilesPath();
	Path GetMovedFilesPathAndMake();
	bool GameFileExist(const Path& path);
private:
	GameFiles(Gamelibrary* lib, const GameFilesData& Data);
	~GameFiles() override;

	void GetCurrentFileBuffer(std::function<void(FileBuffer&)> Scope);
	Mutex<FileBuffer> _FileBuffer;
	Vector<Mutex<Optional<FileBuffer>>> _OtherThreadsBuffers;
	struct MainGameFilesState
	{
		GameFilesData _Data;
		Optional<Path> _PersistentDataPath;
		Optional<Path> _CacheDataPath;
		Unique_ptr<Shader> _DefaultShader;
	};
	Mutex<MainGameFilesState> _MainState;
};
CoreEnd

