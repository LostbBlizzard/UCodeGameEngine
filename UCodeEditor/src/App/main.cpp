
#include "App.hpp"

#ifdef PUBLISHED
#if UCodeGameEnginePlatformWindows

#include <Windows.h>
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	LPWSTR* argv;
	int argc;
	argv = CommandLineToArgvW(GetCommandLine(), &argc);
	return App::main(argc, (char**)&argv);
}
#endif 
#else
int main(int argc, char* argv[])
{
	return App::main(argc, argv);
}
#endif 
