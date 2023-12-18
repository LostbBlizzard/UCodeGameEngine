#include "App.hpp"
#include "../Editor/EditorApp.hpp"
#include "../Helper/StringHelper.hpp"

#include "UCodeRunTime/CoreSystems/GameFiles.hpp"

#include "../BuildSytems/BuildSytemManger.hpp"

#include "zip.h"

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
			char v;
			std::cin >> v;
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

			Path target = GetPath(Str);
			
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
			
			
			return build.BuildProject() ? EXIT_SUCCESS : EXIT_FAILURE;
		}


		UC::String ProPath = "";
		if (argc > 1) { ProPath = argv[1]; }

		UE::EditorApp app = UE::EditorApp();
		app.Run(ProPath);
		return EXIT_SUCCESS;
	}

	#ifndef UCodeGEDebug
	catch (const std::exception& ex)
	{
		UC::Loger::Log( (UC::String)"App crashed :" + (UC::String)ex.what(), UC::LogType::Fatal);

		while (true)
		{

		}

		return EXIT_FAILURE;
	}
	#endif // DEBUG

}
