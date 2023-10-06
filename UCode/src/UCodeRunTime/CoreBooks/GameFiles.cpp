#include "GameFiles.hpp"

#include <UCodeRunTime/StaticComponentsIndex.hpp>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <UCodeRunTime/ULibrarys/Serialization/Bit_Implementation/AsssetPtr.hpp>


#ifdef UCode_Build_Windows_OS
#include <Windows.h>
#include <shlobj.h>
#endif // UCode_Build_Windows_OS

Yaml_NotIMPlEMENTED(UCode::GameFileIndex);
CoreStart
namespace fs = std::filesystem;


void GameFilesData::GetFilesInDir(Vector<const GameFileIndex*>& Out, const Path& path) const
{

}

void GameFilesData::GetFilesWithExt(Vector<const GameFileIndex*>& Out, const Vector<const GameFileIndex*>& Files) const
{

}

void GameFilesData::GetFilesWithExt(Vector<const GameFileIndex*>& Out, const Vector<GameFileIndex>& Files) const
{

}

bool GameFilesData::MakeFile(const GameFilesData& data, const Path& Path, USerializerType Type)
{
	UCode::USerializer Serialz(Type);
	Serialize(data, Serialz);
	return Serialz.ToFile(Path);
}

bool GameFilesData::ReadFile(const Path& Path, GameFilesData& data)
{
	UCode::UDeserializer Deserialzer;
	if (Deserialzer.FromFile(Path, Deserialzer))
	{
		return Deserializ(Deserialzer, data);
	}
	return false;
}

bool GameFilesData::ReadFileKeepOpen(const Path& Path, FileBuffer& OutFile, GameFilesData& data)
{
	bool V = OutFile.Open(Path);
	if (V)
	{
		USerializerType SerialzerType = (USerializerType)OutFile.ReadByte();
		if (SerialzerType == USerializerType::YAML)
		{
			OutFile.Set_FileReadIndex(0);
			String _String;
			UDeserializer Deserialzer;

			OutFile.ReadFullFile(_String);
			Deserialzer.SetYAMLString(_String.substr(1));


			return Deserializ(Deserialzer, data);
		}
		else
		{
			UCODE_ENGINE_IMPlEMENTED_LATER;
		}
		return true;
	}
	return V;
}


template<> struct BitData<GameFileIndex>
{
	using _Type = GameFileIndex;
	static void ToBytes(BitMaker& This, const _Type& Value)
	{
		This.WriteType(Value.FileFullName);
		This.WriteType((u64)Value.FileOffset);
		This.WriteType((u64)Value.FileSize);
	}
	static void FromBytes(BitReader& This, _Type& Out)
	{
		This.ReadType(Out.FileFullName, Out.FileFullName);

		u64 V = 0;
		This.ReadType(V, V);
		Out.FileOffset = V;

		V = 0;
		This.ReadType(V, V);
		Out.FileSize = V;
	}
};


void GameFilesData::Serialize(const GameFilesData& data, USerializer& Out)
{
	const char* FileSignature = UCodeGameEngineFileSignature;//T=char [30]
	Out.Write("Signature", (String)FileSignature);

	Out.Write("version", UCodeGameEngineVersionNumber);
	Out.Write("Company", data.CompanyName);
	Out.Write("GameName", data.APPName);
	Out.Write("AssetDataType", (Type_t)data._Type);
	Out.Write("SceneToLoadOnPlay", data.SceneToLoadOnPlay);

	Out.Write("AssetsOffsets", data.Offsets);

	Out.Write("Data", data._Data);
}



bool GameFilesData::Deserializ(UDeserializer& In, GameFilesData& Out)
{
	String Sing;
	In.ReadType("Signature", Sing);
	if (Sing == UCodeGameEngineFileSignature)
	{
		VersionNumber_t Version = 0;
		In.ReadType("version", Version);
		In.ReadType("Company", Out.CompanyName);
		In.ReadType("GameName", Out.APPName);
		In.ReadType("AssetDataType", *(Type_t*)&Out._Type);
		In.ReadType("SceneToLoadOnPlay", Out.SceneToLoadOnPlay);
		In.ReadType("AssetsOffsets", Out.Offsets);

		In.ReadType("Data", Out._Data);
		return true;
	}


	return false;
}

constexpr StaticBooksIndex_t KeyIdex = (StaticBooksIndex_t)StaticBooksIndex::GameFiles;
void GameFiles::WriteBytes(const Byte* bytes, const size_t bytescount, const Path& Path)
{
	std::ofstream myfile(Path,std::ios::out | std::ios::binary);
	if (myfile.is_open())
	{
		myfile.clear();

		myfile.write((char*)bytes, (std::streamsize)bytescount);

		myfile.close();
	}
}
String GameFiles::ReadFileAsString(const Path& Path)
{
	String Text;
	String line;
	std::ifstream myfile(Path);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			Text += line + '\n';
		}
		myfile.close();
	}
	return Text;
}
Unique_Bytes GameFiles::ReadFileAsBytes(const Path& Path)
{
	std::ifstream File(Path, std::ios::binary);
	if (File.is_open())
	{
		Unique_Bytes Bits;
		File.seekg(0, File.end);
		size_t NewSize = File.tellg();
		File.seekg(0, File.beg);
		Bits.Resize(NewSize);
		File.read((char*)Bits.Data(), Bits.Size());

		return Bits;
	}
	else
	{
		return {};
	}
}
Unique_Bytes GameFiles::ReadFileAsBytes(const Path& path, size_t Offset, size_t Size)
{

	throw std::exception("bad");
	return Unique_Bytes();
}
String GameFiles::ReadGameFileAsString(const Path& path)
{
	if (_Data._Type == GameFilesData::Type::Redirect)
	{
		return GameFiles::ReadGameFileAsString(_Data._RedirectDir.native() + path.native());
	}
	else
	{
		throw std::exception("bad");
	}
}
Unique_Bytes GameFiles::ReadGameFileAsBytes(const Path& path)
{
	if (_Data._Type == GameFilesData::Type::Redirect)
	{
		return GameFiles::ReadFileAsBytes(_Data._RedirectDir.native() + path.native());
	}
	else
	{
		throw std::exception("bad");
	}
}
Unique_Bytes GameFiles::ReadGameFileAsBytes(const Path& Path, size_t Offset, size_t Size)
{
	throw std::exception("bad");
	return {};
}

bool GameFiles::MakeDir(const String& text)
{
	return fs::create_directory(text);
}
inline Path GameFiles::GetMovedFilesPath()
{
	return "GameFiles/";
}
inline Path GameFiles::GetMovedFilesPathAndMake()
{
	Path p = GetMovedFilesPath();
	if (!std::filesystem::exists(p))
	{
		std::filesystem::create_directories(p);
	}
	return p;
}
GameFiles::GameFiles(Gamelibrary* lib, const GameFilesData& Data) :libraryBook(lib),_Data(Data), _DefaultShader(nullptr)
{
	if (lib->Get_StaticComponent(KeyIdex)) { Destroy(this); return; }
	lib->SetStaticComponent(KeyIdex, this);
}
GameFiles::~GameFiles()
{
}
GameFiles* GameFiles::Get(Gamelibrary* lib)
{
	auto V = lib->Get_StaticComponent(KeyIdex);
	if (V) { return (GameFiles*)V; }

	GameFilesData Data = GameFilesData();
	GameFiles* r = new GameFiles(lib, Data);;

	lib->MoveBook(r);
	return r;

}
GameFiles* GameFiles::Find(const Gamelibrary* lib)
{
	auto V = lib->Get_StaticComponent(KeyIdex);
	return (GameFiles*)V; 
}
GameFiles* GameFiles::Init(Gamelibrary* lib,const GameFilesData& Data)
{

	auto GameF = lib->Get_StaticComponent(KeyIdex);
	if (GameF) { 
		((GameFiles*)GameF)->_Data = Data;
		return  (GameFiles*)GameF; 
	}

	GameFiles* newGameFiles =new GameFiles(lib,Data);


	lib->MoveBook(newGameFiles);
	return newGameFiles;
}

Shader* GameFiles::Get_DefaultShader()
{
	if (!_DefaultShader) 
	{
		_DefaultShader =std::make_unique<Shader>(Shader::dafalult_Vertex_shader_text, Shader::dafalult_fragment_shader_text);
	}
	return _DefaultShader.get();
}

void GameFiles::ReInit(const GameFilesData& data)
{
	_Data = data;
}

Path GameFiles::GetFileFullName(const Path& FilePath) const
{
	return _Data._RedirectDir.native() + FilePath.native();
}



Path GameFiles::Get_PersistentDataPath(const AppData& data)
{
	const String CompanyName = "company";
	const String APPName = "app";
	#ifdef UCode_Build_Windows_OS

	fs::path Persistentpath;
	PWSTR path_tmp;
	auto V = SHGetKnownFolderPath(FOLDERID_LocalAppDataLow, 0, nullptr, &path_tmp);


	/* Error check */
	if (V != S_OK)
	{

		//Um idk what to put here.
	}

	/* Convert the Windows path type to a C++ path */
	Persistentpath = path_tmp;

	/* Free memory :) */
	CoTaskMemFree(path_tmp);



	#endif 

	#if UCodeGameEngineDEBUG
	Persistentpath = (String)UCode_VS_PROJECTPATH + "Persistentpath/";
	#endif // DEBUG

	String S_Persistentpath = Persistentpath.generic_u8string();
	S_Persistentpath += '/' + CompanyName + '/' += APPName;//Comany
	fs::create_directories(S_Persistentpath);
	
	return S_Persistentpath;
}
const Path& GameFiles::Get_PersistentDataPath()
{


	if (!_PersistentDataPath.has_value()) 
	{
		UCODE_ENGINE_IMPlEMENTED_LATER;
	}


	auto& path = _CacheDataPath.value();
	fs::create_directories(path);
	return path;
}
const Path& GameFiles::Get_CacheDataPath()
{
	if (!_CacheDataPath.has_value()){
		_CacheDataPath = Get_PersistentDataPath().native() 
		+ Path("_Cache").native() + Path("/").native();
	}

	auto& path = _CacheDataPath.value();
	if (!fs::exists(path)) {fs::create_directory(path);}
	return path;
}
void GameFiles::Writetext(StringView text, const Path& Path)
{
	std::ofstream myfile(Path);
	if (myfile.is_open())
	{
		myfile << text;
		myfile.close();
	}
}

void GameFiles::SetFileBuffer(FileBuffer&& Buffer)
{
	_FileBuffer = std::move(Buffer);
}
AsynTask_t<Unique_Bytes> GameFiles::AsynReadGameFileFullBytes(const Path& path)
{
	auto threads = UCode::BookOfThreads::Get(Getlibrary());

	Path p = path;
	auto Func = [this,p]()
	{
		return GameFiles::ReadGameFileAsBytes(p);
	};
	UCode::Delegate<Unique_Bytes> V = Func;
	return threads->AddTask_t(TaskType::File_Input, std::move(V), {});
}
AsynTask_t<Unique_Bytes> GameFiles::AsynReadGameFileBytes(const Path& path, size_t Offset, size_t Bytes)
{
	auto threads = UCode::BookOfThreads::Get(Getlibrary());

	Path p = path;
	auto Func = [this,p, Offset, Bytes]()
	{
		return GameFiles::ReadGameFileAsBytes(p, Offset, Bytes);
	};
	UCode::Delegate<Unique_Bytes> V = Func;
	return threads->AddTask_t(TaskType::File_Input, std::move(V), {});
}
AsynTask_t<Unique_Bytes> GameFiles::AsynReadFileFullBytes(const Path& path)
{
	auto threads = UCode::BookOfThreads::Get(Getlibrary());

	Path p = path;
	auto Func = [p]()
	{
		return GameFiles::ReadFileAsBytes(p);
	};
	UCode::Delegate<Unique_Bytes> V = Func;
	return threads->AddTask_t(TaskType::File_Input, std::move(V), {});
}
AsynTask_t<Unique_Bytes> GameFiles::AsynReadFileBytes(const Path& path, size_t Offset, size_t Bytes)
{
	auto threads = UCode::BookOfThreads::Get(Getlibrary());

	Path p = path;
	auto Func = [p, Offset, Bytes]()
	{
		return GameFiles::ReadFileAsBytes(p,Offset,Bytes);
	};
	UCode::Delegate<Unique_Bytes> V = Func;
	return threads->AddTask_t(TaskType::File_Input, std::move(V), {});
}
AsynTask_t<String> GameFiles::AsynReadGameFileString(const Path& path)
{
	auto threads = UCode::BookOfThreads::Get(Getlibrary());

	Path p = path;
	auto Func = [this,p]()
	{
		return GameFiles::ReadGameFileAsString(p);
	};
	UCode::Delegate<String> V = Func;
	return threads->AddTask_t(TaskType::File_Input, std::move(V), {});
}
AsynTask_t<String> GameFiles::AsynReadFileString(const Path& path)
{
	auto threads = UCode::BookOfThreads::Get(Getlibrary());

	Path p = path;
	auto Func = [p]()
	{
		return GameFiles::ReadFileAsString(p);
	};
	UCode::Delegate<String> V = Func;
	return threads->AddTask_t(TaskType::File_Input, std::move(V), {});
}

Path GameFiles::GetGameFilePathByMove(const Path& path)
{
	if (_Data._Type == GameFilesData::Type::ThisFile)
	{
		auto OutPath = GetMovedFilesPath().native() + path.native();

		if (auto Val = _Data.GetFile(path))
		{
			auto Bits = ReadGameFileAsBytes(path);
			GameFiles::WriteBytes(Bits.Data(),Bits.Size(),OutPath);
		}

		return OutPath;
	}
	else 
	{
		return Path(_Data._RedirectDir.native() + path.native());
	}
}
AsynTask_t<Path> GameFiles::AsynGetGameFilePathByMove(const Path& path)
{
	auto threads = UCode::BookOfThreads::Get(Getlibrary());

	Path p = path;
	auto Func = [this,p]()
	{
		return this->GetGameFilePathByMove(p);
	};
	UCode::Delegate<Path> V = Func;
	return threads->AddTask_t(TaskType::File_Input, std::move(V), {});
}
//
CoreEnd



