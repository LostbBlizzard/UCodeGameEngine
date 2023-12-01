#include "App.hpp"
#include "../Editor/EditorApp.hpp"
#include "../Helper/StringHelper.hpp"

#include "UCodeRunTime/CoreSystems/GameFiles.hpp"

#if UCodeGameEngineDEBUG
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
	int a = 0;
	#ifndef UCodeGameEngineDEBUG
	try
#endif // DEBUG

	{
		
		//while (true);
		UCode::String StrV;
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
		UCode::StringView Str = StrV;

		#if UCodeGameEngineDEBUG
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

			return UCode::GameFilesData::PackDir(pin, pout) == true ? EXIT_SUCCESS : EXIT_FAILURE;
		}
		else if (StringHelper::StartsWith(Str, "unpack"))
		{
			Str = Str.substr(sizeof("unpack"));
			Path pin = GetPath(Str);
			Path pout = GetPath(Str);

			return UCode::GameFilesData::UnPackToDir(pin, pout) == true ? EXIT_SUCCESS : EXIT_FAILURE;
		}



		UCode::String ProPath = "";
		if (argc > 1) { ProPath = argv[1]; }

		UCodeEditor::EditorApp app = UCodeEditor::EditorApp();
		app.Run(ProPath);
		return EXIT_SUCCESS;
	}

	#ifndef UCodeGameEngineDEBUG
	catch (const std::exception& ex)
	{
		UCode::Loger::Log( (UCode::String)"App crashed :" + (UCode::String)ex.what(), UCode::LogType::Fatal);

		while (true)
		{

		}

		return EXIT_FAILURE;
	}
	#endif // DEBUG

}
