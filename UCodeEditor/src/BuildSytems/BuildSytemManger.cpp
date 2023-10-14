#include "BuildSytemManger.hpp"
#include "Helper/AppFiles.hpp"
#include "UCodeRunTime/CoreBooks/GameFiles.hpp"
#include <filesystem>

#include "UCodeRunTime/ULibrarys/AssetManagement/AssetManager.hpp"
#include "Helper/FileHelper.hpp"

#include "UEditorModules/UEditorModule.hpp"
#include "zip.h"

#include <UCodeRunTime/CoreBooks/StandardAssetLoader.hpp>
EditorStart

namespace fs = std::filesystem;

void RemoveFileInPath(const Path& dir_path)
{
	if (fs::exists(dir_path))
	{
		for (const auto& entry : std::filesystem::directory_iterator(dir_path))
		{
			std::filesystem::remove_all(entry.path());
		}
	}
}
void BuildSytemManger::BuildProject()
{
	if (auto Val = Setings.Settings.IfType<WindowsBuildSetings>())
	{
		Build(*Val);
	}
	else if (auto Val = Setings.Settings.IfType<WebBuildSetings>())
	{
		Build(*Val);
	}
	else if (auto Val = Setings.Settings.IfType<AndroidBuildSetings>())
	{
		Build(*Val);
	}
	else
	{
		UCodeGameEngineUnreachable();
	}
}
void BuildSytemManger::Reset()
{

}
void BuildSytemManger::Build(const WindowsBuildSetings& setings)
{
	const Path ExePath = Setings._OutDir / Path(Setings._OutName).concat(".exe").native();
	const Path GameFilesDataPath = Setings._OutDir / Path(UCode::GameFilesData::FileDataName).native();
	const auto ExeTepPath = AppFiles::ReadFileAsBytes(Path("bin/UCAppWinDebug86X64.exe"));

	fs::create_directories(Setings._OutDir);
	fs::create_directories(Setings.TemporaryPlatfromPath);
	fs::create_directories(Setings.TemporaryGlobalPath);

	RemoveFileInPath(Setings._OutDir);

	if (fs::exists(ExePath))
	{
		fs::remove(ExePath);
	}
	UCode::GameFiles::WriteBytes(ExeTepPath.Data(), ExeTepPath.Size(), ExePath);
	

	const auto SerializerMode = UCode::USerializerType::Bytes;
	BuildProjectGlobalWasGameData(GameFilesDataPath, SerializerMode);

}
void BuildSytemManger::BuildProjectGlobalWasGameData(const Path& GameFilesDataPath,USerializerType SerializerMode)
{
	const Path ChachPath = Setings.TemporaryGlobalPath.native() + Path("Chache.data").native();
	ExportChacheFile Chache;
	ExportChacheFile::FromFile(ChachPath, Chache);

	auto ModulesList = UCodeEditor::UEditorModules::GetModules();

	ExportEditorContext Export;
	Export.AssetPath = Setings._InputDir;
	Export.TemporaryGlobalPath = Setings.TemporaryGlobalPath;
	Export.ChachInfo = &Chache;

	fs::create_directories(Export.TemporaryGlobalPath);

	Vector<ExportEditorReturn> ModulesRets;
	for (size_t i = 0; i < ModulesList.Size(); i++)
	{
		auto& Item = ModulesList[i];


		ModulesRets.push_back(std::move(Item->ExportEditor(Export)));
	}

	ProjectData prj;
	ProjectData::ReadFromFile(Path(Export.AssetPath).native() + Path("../").native() + Path(ProjectData::FileName).native(), prj);


	auto Uidmappath = UCode::GameFilesData::GetUCodeDir() / UCode::StandardAssetLoader::UIdMap::FileWithDot;
	UCode::StandardAssetLoader::UIdMap IDMap;
	{
		UCode::GameFilesData GameData;
		GameData._Type = UCode::GameFilesData::Type::ThisFile;


		for (auto& Item : ModulesRets)
		{
			if (ModulesRets.size())
			{
				UCode::GameFilesData data;
				if (UCode::GameFilesData::ReadFile(Item.OutputModuleFile, data)) 
				{

					size_t OffsetToAdd = GameData._Data.size();
					size_t MyDataSize = data._Data.size();
					for (auto& Item : data.Offsets)
					{
						if (Item.FileFullName == Uidmappath)
						{
							UCode::StandardAssetLoader::UIdMap myidmap;
							UDeserializer deserializer;
							deserializer.SetBytes(BytesView::Make(&data._Data[Item.FileOffset],Item.FileSize));

							myidmap.Deserialize(deserializer);

							for (auto& Item : myidmap._Paths) 
							{
								IDMap._Paths[Item.first] = std::move(Item.second);
							}

							MyDataSize -= Item.FileSize;//Will allways be last.
							continue;
						}

						GameData.Offsets.push_back(std::move(Item));


						auto& MovedItem = GameData.Offsets.back();
						MovedItem.FileOffset += OffsetToAdd;
					}

					GameData._Data.resize(GameData._Data.size() + MyDataSize);
					memcpy(GameData._Data.data() + OffsetToAdd, data._Data.data(), MyDataSize);
				}
			}
		}

		{
			UCode::USerializer serializer(USerializerType::Bytes);

			IDMap.Serialize(serializer);

			auto bytes = serializer.Get_BitMaker().Get_Bytes();


			UCode::GameFileIndex index;
			index.FileOffset = GameData._Data.size();
			index.FileFullName = UCode::GameFilesData::GetUCodeDir() / UCode::StandardAssetLoader::UIdMap::FileWithDot;
			index.FileSize = bytes.size();

			GameData.Offsets.push_back(std::move(index));

			for (size_t i = 0; i < bytes.size(); i++)
			{
				auto& Item = bytes[i];

				GameData._Data.push_back(Item);
			}
		}

		ExportChacheFile::ToFile(ChachPath, Chache);

		if (fs::exists(GameFilesDataPath))
		{
			fs::remove(GameFilesDataPath);
		}


		GameData.APPName = prj._ProjectName;
		GameData.CompanyName = prj._CompanyName;
		GameData.SceneToLoadOnPlay = prj.StartScene;

		UCode::GameFilesData::MakeFile(GameData, GameFilesDataPath, SerializerMode);
		
		#if UCodeGameEngineDEBUG
		fs::copy_file(GameFilesDataPath, "../UCodeApp" / Path(UCode::GameFilesData::FileDataName),fs::copy_options::overwrite_existing);
		#endif
	}
}
void BuildSytemManger::Build(const WebBuildSetings& setings)
{
	const Path GameFilesDataPath = Setings.TemporaryPlatfromPath / Path(UCode::GameFilesData::FileDataName).native();

	
	fs::create_directories(Setings._OutDir);
	fs::create_directories(Setings.TemporaryPlatfromPath);
	fs::create_directories(Setings.TemporaryGlobalPath);
	
	RemoveFileInPath(Setings._OutDir);


	const auto SerializerMode = UCode::USerializerType::Bytes;
	BuildProjectGlobalWasGameData(GameFilesDataPath, SerializerMode);

	{
		const Path IndexPath = Setings._OutDir / Path(Setings._OutName).concat(".html").native();
		auto Txt = AppFiles::ReadFileAsString(Path("buildtemplates/web/indextemplate.html"));
		UCode::GameFiles::Writetext(Txt, IndexPath);
	}

	{
		fs::create_directory(Setings._OutDir / "data");
	}
}
void BuildSytemManger::Build(const AndroidBuildSetings& setings)
{
	const Path ApkPath = Setings._OutDir / Path(Setings._OutName).concat(".apk").native();

	const Path ApkInfoDir = Setings.TemporaryPlatfromPath / "apkinfo";
	
	fs::create_directories(Setings._OutDir);
	fs::create_directories(Setings.TemporaryPlatfromPath);
	fs::create_directories(Setings.TemporaryGlobalPath);
	
	RemoveFileInPath(Setings._OutDir);
	RemoveFileInPath(ApkInfoDir);


	const auto SerializerMode = UCode::USerializerType::Bytes;


	//apk
	fs::create_directories(ApkInfoDir / "assets");
	{
		BuildProjectGlobalWasGameData(ApkInfoDir / "assets" / UCode::GameFilesData::FileDataName, SerializerMode);
	}
	fs::create_directories(ApkInfoDir / "lib");
	{
		fs::create_directories(ApkInfoDir / "lib" / "armeabi-v7a");
		{

		}


		fs::create_directories(ApkInfoDir / "lib" / "arm64-v8a");
		{

		}


		fs::create_directories(ApkInfoDir / "lib" / "x86");
		{

		}


		fs::create_directories(ApkInfoDir / "lib" / "x86_64");
		{

		}
	}
	fs::create_directories(ApkInfoDir / "META-INF");
	{
		{
			const Path path = (ApkInfoDir / "META-INF" / "MANIFEST.MF");
			auto Txt =AppFiles::ReadFileAsString(Path("buildtemplates/android/MANIFEST.MF"));
			UCode::GameFiles::Writetext(Txt, path);
		}
	}

	//classes.dex
	//The classes compiled in the dex file format executed by Android Runtime (or by Dalvik virtual machine used in Android 4.4 KitKat).
	{

	}


	//ApkInfoDir / "resources.arsc"
	//a file containing precompiled resources, such as binary XML for example.
	{

	}
	UCode::GameFiles::Writetext("but cool stuff here.", ApkInfoDir / "AndroidManifest.xml");

	struct zip_t* zip = zip_open(ApkPath.generic_string().c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
	for (const auto& entry : std::filesystem::recursive_directory_iterator(ApkInfoDir))
	{
		if (entry.is_regular_file()) {

			auto path = entry.path().generic_string();
			Path Repath = path.substr(ApkInfoDir.generic_string().size() + 1);
			zip_entry_open(zip, Repath.generic_string().c_str());

			zip_entry_fwrite(zip, path.c_str());

			zip_entry_close(zip);
		}
	}
	zip_close(zip);
}
EditorEnd