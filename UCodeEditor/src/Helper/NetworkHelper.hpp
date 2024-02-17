#pragma once
#include <Typedefs.hpp>

EditorStart

struct NetworkHelper
{
	static Optional<Version> GetNewestVersion();
	
	static Optional<String> GetNewestChangeLog();

	//shell script
	static Optional<String> GetNewestUnixInstall();

	static Optional<Path> GetNewestUnixInstallPath();
	//exe path
	static Optional<Path> GetNewestWindowInstall();
};

EditorEnd
