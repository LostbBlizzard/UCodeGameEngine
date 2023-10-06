#pragma once
#include "Typedefs.hpp"

EditorStart

enum class PlatformType :u16
{
	Null,
	UCodeEditor,
	Windows,

	ThisPlatfrom = Windows,

};
enum class BitSizeType :u16
{
	Null,
	Bit32,
	Bit64,

	ThisPlatfrom = Bit64,
};
enum class ConfigurationType :u16
{
	Null,
	Debug,
	Release,
	Published,
};

struct WindowsBuildSetings
{
	enum class Architecture
	{
		x86_64,
		x86,

		#if UCode_Build_Windows_OS
		#if UCode_Build_32Bit
		Native = (int)Architecture::x86,
		Default = (int)Native,
		#else
		Native = (int)Architecture::x86_64,
		Default = (int)Architecture::Native,
		#endif // 
		#else
		Default = (int)Architecture::x86_64;//most people on 64 bit
		#endif
	};

	bool DebugBuild = true;
	Architecture architecture = Architecture::Default;
};
struct WebBuildSetings
{

};
struct AndroidBuildSetings
{

};


struct BuildSetings
{
	using SettingsType = Variant<WindowsBuildSetings, WebBuildSetings, AndroidBuildSetings>;
	SettingsType Settings;

	String _OutName;
	Path _InputDir;
	Path TemporaryPlatfromPath;
	Path TemporaryGlobalPath;
	Path _OutDir;
};
EditorEnd