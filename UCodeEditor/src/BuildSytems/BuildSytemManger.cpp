#include "BuildSytemManger.hpp"
#include "Helper/AppFiles.hpp"
#include "UCodeRunTime/CoreSystems/GameFiles.hpp"
#include <filesystem>

#include "UCodeRunTime/ULibrarys/AssetManagement/AssetManager.hpp"
#include "Helper/FileHelper.hpp"

#include "UEditorModules/UEditorModule.hpp"
#include "zip.h"

#include <UCodeRunTime/CoreSystems/StandardAssetLoader.hpp>
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
BuildSytemManger::BuildRet BuildSytemManger::BuildProject()
{
	if (auto Val = Setings.Settings.IfType<WindowsBuildSetings>())
	{
		return Build(*Val);
	}
	else if (auto Val = Setings.Settings.IfType<LinuxBuildSetings>())
	{
		return Build(*Val);
	}
	else if (auto Val = Setings.Settings.IfType<MacOsBuildSetings>())
	{
		return Build(*Val);
	}
	else if (auto Val = Setings.Settings.IfType<WebBuildSetings>())
	{
		return Build(*Val);
	}
	else if (auto Val = Setings.Settings.IfType<AndroidBuildSetings>())
	{
		return Build(*Val);
	}
	else if (auto Val = Setings.Settings.IfType<IOSBuildSetings>())
	{
		return Build(*Val);
	}
	else
	{
		UCodeGEUnreachable();
	}
}
void BuildSytemManger::Reset()
{

}
BuildSytemManger::BuildRet BuildSytemManger::Build(const WindowsBuildSetings& setings)
{
	Path OutDir = Path(Setings._OutDir.native() + Path::preferred_separator);
	
	
	Path AddedPath;
	{
		AddedPath += "Windows";

		if (setings.architecture == WindowsBuildSetings::Architecture::x86_64)
		{
			AddedPath += "-X86_64";
		}
		else
		{
			AddedPath += "-X86";
		}
		if (setings.DebugBuild)
		{
			AddedPath += "-Debug";
		}
		else
		{
			AddedPath += "-Publish";
		}
	}
	OutDir += AddedPath;

	Platfromteppath = Setings.TemporaryPlatfromPath / AddedPath;

	const Path ExePath = OutDir / Path(Setings._OutName).concat(".exe").native();
	const Path GameFilesDataPath = OutDir / Path(UCode::GameFilesData::FileDataName).native();

	Path baseexepath;


	if (setings.DebugBuild)
	{
		if (setings.architecture == WindowsBuildSetings::Architecture::x86_64)
		{
			baseexepath = Path("bin/UCAppWinDebug86X64.exe");
		}
		else
		{
			baseexepath = Path("bin/UCAppWinDebug86X32.exe");
		}
	}
	else
	{
		if (setings.architecture == WindowsBuildSetings::Architecture::x86_64)
		{
			baseexepath = Path("bin/UCAppWinPub86X64.exe");
		}
		else
		{

			baseexepath = Path("bin/UCAppWinPub86X32.exe");
		}
	}


	//
	Settings.AddSetting("target.platform", "windows");
	Settings.AddSetting("target.bitmode",setings.architecture == WindowsBuildSetings::Architecture::x86_64 ? "64" : "32");
	Settings.AddSetting("ucodelang.target","interpreter");

	//
	
	fs::create_directories(OutDir);
	fs::create_directories(Platfromteppath);
	fs::create_directories(Setings.TemporaryGlobalPath);

	RemoveFileInPath(OutDir);


	if (fs::exists(ExePath))
	{
		fs::remove(ExePath);
	}
	const auto ExeTepPath = AppFiles::CopyFile(baseexepath, ExePath);
	

	const auto SerializerMode = UCode::USerializerType::Bytes;
	return BuildProjectGameData(GameFilesDataPath, SerializerMode);
}
BuildSytemManger::BuildRet BuildSytemManger::Build(const LinuxBuildSetings& setings)
{
	return Empty();
}
BuildSytemManger::BuildRet BuildSytemManger::Build(const MacOsBuildSetings& setings)
{
	return Empty();
}
BuildSytemManger::BuildRet BuildSytemManger::BuildProjectGameData(const Path& GameFilesDataPath,USerializerType SerializerMode)
{
	const Path ChachPath = Setings.TemporaryGlobalPath.native() + Path("Chache.data").native();
	ExportChacheFile Chache;
	ExportChacheFile::FromFile(ChachPath, Chache);

	auto ModulesList = UCodeEditor::UEditorModules::GetModules();

	ExportEditorContext Export;
	Export.AssetPath = Setings._InputDir;
	Export.TemporaryGlobalPath = Setings.TemporaryGlobalPath;
	Export.TemporaryPlatfromPathSpecificPath = Platfromteppath;
	Export.ChachInfo = &Chache;
	Export.settings = std::move(Settings);

	fs::create_directories(Export.TemporaryGlobalPath);

	Vector<ExportEditorReturn> ModulesRets;

	Optional<ExportErrors> errors;
	for (size_t i = 0; i < ModulesList.Size(); i++)
	{
		auto& Item = ModulesList[i];

		auto r = std::move(Item->ExportEditor(Export));
		if (r.IsError())
		{
			if (errors.has_value())
			{
				errors.value().Add(std::move(r.GetError()));
			}
			else
			{
				errors = std::move(r.GetError());
			}
		}
		else {
			ModulesRets.push_back(r.GetValue());
		}
	}

	if (errors.has_value())
	{
		return errors.value();
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
		
		#if UCodeGEDebug
		fs::copy_file(GameFilesDataPath, "../UCodeApp" / Path(UCode::GameFilesData::FileDataName),fs::copy_options::overwrite_existing);
		#endif
	}
	return Empty();
}
BuildSytemManger::BuildRet BuildSytemManger::Build(const WebBuildSetings& setings)
{
	const Path GameFilesDataPath = Setings.TemporaryPlatfromPath / Path(UCode::GameFilesData::FileDataName).native();

	
	fs::create_directories(Setings._OutDir);
	fs::create_directories(Setings.TemporaryPlatfromPath);
	fs::create_directories(Setings.TemporaryGlobalPath);
	
	RemoveFileInPath(Setings._OutDir);


	const auto SerializerMode = UCode::USerializerType::Bytes;
	BuildProjectGameData(GameFilesDataPath, SerializerMode);

	{
		const Path IndexPath = Setings._OutDir / Path(Setings._OutName).concat(".html").native();
		auto Txt = AppFiles::ReadFileAsString(Path("buildtemplates/web/indextemplate.html"));
		UCode::GameFiles::Writetext(Txt, IndexPath);
	}

	{
		fs::create_directory(Setings._OutDir / "data");
	}

	return Empty();
}
BuildSytemManger::BuildRet BuildSytemManger::Build(const AndroidBuildSetings& setings)
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
		BuildProjectGameData(ApkInfoDir / "assets" / UCode::GameFilesData::FileDataName, SerializerMode);
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

	return Empty();
}
BuildSytemManger::BuildRet BuildSytemManger::Build(const IOSBuildSetings& setings)
{
	return Empty();
}
BuildSytemManger::BuildRet BuildSytemManger::Build(const CustomBuildSetings& setings)
{
	return Empty();
}
EditorEnd