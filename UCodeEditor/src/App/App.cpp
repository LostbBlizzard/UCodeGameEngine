#include "App.hpp"
#include "../Editor/EditorApp.hpp"
#include "../Helper/StringHelper.hpp"

#include "UCodeRunTime/CoreSystems/GameFiles.hpp"

#include "../BuildSytems/BuildSytemManger.hpp"

#include "zip.h"
#include "../Helper/NetworkHelper.hpp"
#include "../Helper/FileHelper.hpp"
#include "../Helper/UserSettings.hpp"
#include "../Helper/AppFiles.hpp"
#if UCodeGEWindows
#include <Windows.h>
#endif

#if UCodeGEDebug
#include "tests/tests.hpp"
#endif // DEBUG

using String_view = UCode::StringView;
using Path = UCode::Path;
using StringHelper = UCodeEditor::StringHelper;

template<typename T>
bool InList(const T* buffer, size_t buffersize, T Value)
{
	for (size_t i = 0; i < buffersize; i++)
	{
		if (buffer[i] == Value) { return true; }
	}
	return false;
}

bool IsList(const char* List, char V)
{
	return InList(List, strlen(List), V);
}

bool IsList(String_view List, char V)
{
	return InList(List.data(), List.size(), V);
}

#define Letersdef "QWERTYUIOPASDFGHJKLZXCVBNM" "qwertyuiopasdfghjklzxcvbnm"
#define Numdef "1234567890"
#define NameChardef "_"

#define Worddef Letersdef Numdef NameChardef

#define PathDef Letersdef Numdef ":/\\.+()-"

String_view GetWord_t(String_view& Line, String_view GoodChars_t)
{
	bool Reading = false;
	size_t goodindex = 0;
	for (size_t i = 0; i < Line.size(); i++)
	{
		char V = Line[i];
		bool GoodChar = IsList(GoodChars_t, V);
		if (!GoodChar && Reading == true)
		{
			auto r = String_view(&Line[goodindex], i - goodindex);
			Line = Line.substr(i);
			return r;
		}
		else
		{
			if (GoodChar)
			{
				if (Reading == false) {
					goodindex = i;
				}
				Reading = true;
			}
		}
	}

	if (Reading) {
		auto r2 = String_view(&Line[goodindex]);
		Line = Line.substr(Line.size());
		return r2;
	}
	else
	{
		auto r2 = String_view();
		return r2;
	}
}
String_view GetWord(String_view& Line)
{
	return GetWord_t(Line, Worddef);
}

String_view GetPath(String_view& Line)
{
	return GetWord_t(Line, PathDef);
}

int App::main(int argc, char* argv[])
{
	namespace UC = UCode;
	namespace UE = UCodeEditor;
	int a = 0;

#ifndef UCodeGEDebug
	try
#endif // DEBUG

	{

		//while (true);

		UC::String StrV;
		for (size_t i = 1; i < argc; i++)
		{
			StrV += String_view(argv[i]);
			if (i + 1 < argc)
			{
				StrV += ' ';
			}
		}
		UC::StringView Str = StrV;
#if UCodeGEDebug
		if (Str == "--RunTests")
		{
			return UCodeEditor::Tests::RunTests();
		}
#endif // DEBUG
		bool argupdatepassed = false;
		if (StringHelper::StartsWith(Str, "pack"))
		{
			Str = Str.substr(sizeof("pack"));
			Path pin = GetPath(Str);
			Path pout = GetPath(Str);

			return UC::GameFilesData::PackDir(pin, pout) == true ? EXIT_SUCCESS : EXIT_FAILURE;
		}
		else if (StringHelper::StartsWith(Str, "unpack"))
		{
			Str = Str.substr(sizeof("unpack"));
			Path pin = GetPath(Str);
			Path pout = GetPath(Str);

			return UC::GameFilesData::UnPackToDir(pin, pout) == true ? EXIT_SUCCESS : EXIT_FAILURE;
		}
		else if (StringHelper::StartsWith(Str, "zip"))
		{
			Str = Str.substr(sizeof("zip"));
			Path pin = GetPath(Str);
			Path pout = GetPath(Str);

			UC::Vector<UC::String> FileName;

			for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(pin))
			{
				if (dirEntry.is_regular_file())
				{
					FileName.push_back(dirEntry.path().generic_string());
					std::cout << dirEntry.path().generic_string() << "\n";
				}
			}


			int exitcode;
			{
				UC::Vector<const char*> FileNamesptr;
				FileNamesptr.resize(FileName.size());

				for (size_t i = 0; i < FileName.size(); i++)
				{
					FileNamesptr[i] = FileName[i].c_str();
				}

				exitcode = zip_create(pout.generic_string().c_str(), FileNamesptr.data(), FileNamesptr.size()) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
			}


			return exitcode;
		}
		else if (StringHelper::StartsWith(Str, "unzip"))
		{
			Str = Str.substr(sizeof("unzip"));
			UC::String pin = UC::String(GetPath(Str));
			UC::String pout = UC::String(GetPath(Str));


			return zip_extract(pin.c_str(), pout.c_str(), nullptr, nullptr) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
		}
		else if (StringHelper::StartsWith(Str, "build"))
		{
			Str = Str.substr(sizeof("build"));
			Path project = GetPath(Str);

			bool isvaildproject = false;

			if (std::filesystem::exists(project) && std::filesystem::is_directory(project))
			{
				isvaildproject = std::filesystem::exists(project / UE::ProjectData::FileName);
			}

			if (!isvaildproject)
			{
				std::cerr << "Project path \"" << project << "\" must be a directory of a vaild UCodeGameEngine Project\n";
				return EXIT_FAILURE;
			}

			auto lockpath = UE::ProjectManger::GetProjectLockPath(project);
			{
				bool hasprojectlock = std::filesystem::exists(project);
				if (hasprojectlock)
				{
					std::error_code errorcode = {};
					if (std::filesystem::remove(lockpath, errorcode) == true)
					{
						hasprojectlock = false;
					}
				}

				if (hasprojectlock)
				{
					std::cerr << "Project is already Opened";
					return EXIT_FAILURE;
				}
			}
			auto projectlock = std::ofstream(lockpath);

			auto oldstr = Str;
			Path target = GetPath(Str);

			if (!target.empty())
			{
				auto tep = target.generic_string();
				if (tep[0] == '-')
				{
					Str = oldstr;
					target.clear();
				}
			}

			UE::BuildSytemManger build;

			if (!target.empty())
			{
				if (target == "windows")
				{
					UE::WindowsBuildSetings settings;


					build.Setings.Settings = std::move(settings);
				}
				else if (target == "linux")
				{
					UE::LinuxBuildSetings settings;


					build.Setings.Settings = std::move(settings);
				}
				else if (target == "maxos")
				{
					UE::MacOsBuildSetings settings;


					build.Setings.Settings = std::move(settings);
				}
				else if (target == "web")
				{
					UE::WebBuildSetings settings;


					build.Setings.Settings = std::move(settings);
				}
				else if (target == "android")
				{
					UE::AndroidBuildSetings settings;


					build.Setings.Settings = std::move(settings);
				}
				else if (target == "ios")
				{
					UE::IOSBuildSetings settings;


					build.Setings.Settings = std::move(settings);
				}
				else if (target == "custom")
				{
					UE::CustomBuildSetings settings;


					build.Setings.Settings = std::move(settings);
				}
				else
				{
					return EXIT_FAILURE;
				}
			}
			else
			{
#if UCodeGEWindows
				UE::WindowsBuildSetings settings;


				build.Setings.Settings = std::move(settings);
#elif UCodeGELinux
				UE::LinuxBuildSetings settings;


				build.Setings.Settings = std::move(settings);
#elif UCodeGEMacOS
				UE::MacOsBuildSetings settings;


				build.Setings.Settings = std::move(settings);
#elif UCodeGEWasm
				UE::WebBuildSetings settings;


				build.Setings.Settings = std::move(settings);
#endif
			}
			UE::Optional<Path> CopyToPath;
			bool printoutputpath = false;

			{
				auto t = GetPath(Str);
				if (t == "-o")
				{
					CopyToPath = GetPath(Str);
				}
				else if (t == "-p")
				{
					printoutputpath = true;
				}
			}

			if (CopyToPath.has_value())
			{
				auto& copyto = CopyToPath.value();


				if (!std::filesystem::exists(copyto))
				{
					std::filesystem::create_directories(copyto);
				}
				else
				{
					if (!std::filesystem::is_directory(copyto)) {
						std::cerr << "Output path \"" << copyto << "\" must be a directory\n";

						return EXIT_FAILURE;
					}
				}
			}
			{
				build.Setings._InputDir = project / "Assets";
				build.Setings._OutDir = UE::ProjectManger::GetProjectCachedDir(project).native() + Path("output").native();
				build.Setings.TemporaryPlatfromPath = UE::ProjectManger::GetProjectCachedDir(project).native() + Path("build").native();
				build.Setings.TemporaryGlobalPath = UE::ProjectManger::GetProjectCachedDir(project).native() + (Path("build") / "global").native();

				UE::ProjectData proj;

				UE::ProjectData::ReadFromFile(project / UE::ProjectData::FileName, proj);
				build.Setings._OutName = proj._ProjectName;
			}

			UE::BuildSytemManger::BuildRet r;
			{
				UC::Gamelibrary lib;
				UE::AppFiles::Init(&lib);


				auto _run = std::make_unique<UCode::GameRunTime>();
				_run->Init();

				//setup Opengl Context
				auto _render = std::make_unique<UCode::RenderAPI::Render>();
				_render->PreInit();

				UCode::RenderAPI::WindowData windowdata;
				windowdata.GenNewWindow = false;
				windowdata.ImGui_Init = false;
				_render->Init(windowdata, _run.get());

				{
					r = build.BuildProject();
				}

				_render->EndRender();
			}


			if (r.IsError())
			{
				auto& err = r.GetError();

				for (auto& Item : err.Errors)
				{

					if (Item.filepath.has_value())
					{
						std::cerr << "(";
						std::cerr << Item.filepath.value();

						if (Item.lineNumber.has_value())
						{
							std::cerr << Item.lineNumber.value();
						}

						std::cerr << "):";
					}
					std::cerr << Item.message;
					std::cerr << '\n';
				}
			}

			if (r.IsValue() && CopyToPath.has_value())
			{
				auto srcpath = r.GetValue().parent_path();
				auto& outpath = CopyToPath.value();

				using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
				for (const auto& dirEntry : recursive_directory_iterator(srcpath))
				{
					auto filepath = dirEntry.path();
					auto relpath = filepath.native().substr(srcpath.native().size());
					auto outfilepath = outpath;
					outfilepath += relpath;

					std::filesystem::create_directories(outfilepath.parent_path());
					std::filesystem::copy(filepath, outfilepath, std::filesystem::copy_options::overwrite_existing);
				}
			}
			else if (r.IsValue() && printoutputpath)
			{
				std::cout << r.GetValue().parent_path();
			}


			projectlock.close();
			std::filesystem::remove(lockpath);

			return r.IsValue() ? EXIT_SUCCESS : EXIT_FAILURE;
		}
		else if (StringHelper::StartsWith(Str, "uninstall"))
		{
			Path exepath = std::filesystem::absolute(argv[0]);

#if UCodeGEWindows
			auto unstallerpath = UE::FileHelper::GetEditorGlobalDirectory() / "unins000.exe";

			UC::String cmd = unstallerpath.generic_string();

			cmd += " /VERYSILENT";

			system(cmd.c_str());

			Path removeexepath = UE::FileHelper::GetEditorGlobalDirectory() / "remov_exe.bat";

			UC::String batfiletxt = "ping 127.0.0.1 -n 1 > nul \n";
			batfiletxt += "del \"" + removeexepath.generic_string() + "\" \n";
			batfiletxt += "del \"" + exepath.generic_string() + "\" \n";
			batfiletxt += "DEL /F /Q /S " + UE::FileHelper::GetEditorGlobalDirectory().generic_string();
			batfiletxt += "\* \n";
			batfiletxt += "for /D %%i in (" + UE::FileHelper::GetEditorGlobalDirectory().generic_string() + ") do RD /S /Q \"%%i\"";

			UC::GameFiles::Writetext(batfiletxt, removeexepath);

			Path c = "open";
			ShellExecute(NULL, c.c_str(), removeexepath.c_str(), NULL, NULL, SW_HIDE);
			return EXIT_SUCCESS;
#else

#endif
		}
		else if (StringHelper::StartsWith(Str, "update"))
		{
			argupdatepassed = true;
		}

		UC::String ProPath = "";
		if (argc > 1) { ProPath = argv[1]; }

		if (argupdatepassed == false)
		{
			UE::EditorApp app = UE::EditorApp();
			app.Run(ProPath);
		}

		{
			bool allowsautoupdate = UE::UserSettings::GetSettings().allowautoudate;

#if UCodeGEDebug
			allowsautoupdate = false;
#endif

			if (allowsautoupdate || (UE::UserSettings::updateonclose || argupdatepassed))
			{
				auto vop = UE::NetworkHelper::GetNewestVersion();
				if (vop.has_value())
				{
					auto v = vop.value();

					auto current = UE::Version::CurrentUCodeVersion();
					bool isversionbiger = current < v;

					if (isversionbiger)
					{
						auto changelog = UE::NetworkHelper::GetNewestChangeLog();

#if UCodeGEWindows
						auto installer = UE::NetworkHelper::GetNewestWindowInstall();
#else 
						auto installer = UE::NetworkHelper::GetNewestUnixInstallPath();
#endif

						if (changelog.has_value() && installer.has_value())
						{
							auto change = changelog.value();
							auto in = installer.value();

							UC::GameFiles::Writetext(change, "changelog.md");


#if UCodeGEWindows
							auto exe = in.generic_string() + ".exe";
							std::filesystem::rename(in, exe);
							installer = {};

							UE::FileHelper::OpenExeSubProcess(exe, "/VERYSILENT");
#else

#endif
							int a = 0;
						}

						if (installer.has_value())
						{
							std::filesystem::remove(installer.value());
						}

					}

				}
			}
		}
		return EXIT_SUCCESS;
	}

#ifndef UCodeGEDebug
	catch (const std::exception& ex)
	{
		UC::Loger::Log((UC::String)"App crashed :" + (UC::String)ex.what(), UC::LogType::Fatal);

		while (true)
		{

		}

		return EXIT_FAILURE;
}
#endif // DEBUG

}
