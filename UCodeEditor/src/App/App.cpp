#include "App.hpp"
#include "../Editor/EditorApp.hpp"


#if UCodeGameEngineDEBUG
#include "tests/tests.hpp"
#endif // DEBUG

int App::main(int argc, char* argv[])
{
	int a = 0;

	#ifndef UCodeGameEngineDEBUG
	try
	#endif // DEBUG

	{
		for (size_t i = 0; i < argc; i++)
		{
			UCode::StringView Str = argv[i];

			#if UCodeGameEngineDEBUG
			if (Str == "--RunTests")
			{
				return UCodeEditor::Tests::RunTests();
			}
			#endif // DEBUG
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
