#pragma once
#include "APITypeDefs.hpp"
#include "UCodeLang/LangCore/LangDef.hpp"
UCodeAPIStart



using LogType_t = int;
UCodeLangExportSymbol("UCodeGameEngine") enum class LogType : Byte
{
	//For Debuging and testing.
	Log,

	//For Somehting risky or could cause an Error. something may be wourng
	Warning,

	//A Recoverable Error may be cause by an exception or a painc.
	Error,
	
	//Very bad Errors that may cause put the GameRunTime to be an unstable state or cause a crash. 
	Fatal,


	MinValues = (LogType_t)Log,
	MaxValues = (LogType_t)Fatal,
};

/// <summary>
///	This is a Class for Debuging.
/// </summary>
UCodeLangExportSymbol("UCodeGameEngine") struct Debug
{
	///Makes a Log to the Console
	UCodeLangExport inline static void Log(const String& Msg) { return LogEx(StringView(Msg),LogType::Log); }
	//Makes a Log to the Console
	UCodeLangExport inline static void Log(const StringView Msg) { return LogEx(Msg, LogType::Log); }


	///Makes a Warning to the Console
	UCodeLangExport inline static void LogWarning(const String& Msg) { return LogEx(StringView(Msg), LogType::Warning); }
	//Makes a Warning to the Console
	UCodeLangExport inline static void LogWarning(const StringView Msg) { return LogEx(Msg, LogType::Warning); }
	
	///Makes a Fatal Error to the Console
	UCodeLangExport inline static void LogError(const String& Msg) { return LogEx(StringView(Msg), LogType::Error); }
	//Makes a Fatal Error to the Console
	UCodeLangExport inline static void LogError(const StringView Msg) { return LogEx(Msg, LogType::Error); }

	///Makes a Fatal Error to the Console
	UCodeLangExport inline static void LogFatalError(const String& Msg) { return LogEx(StringView(Msg), LogType::Fatal); }
	//Makes a Fatal Error to the Console
	UCodeLangExport inline static void LogFatalError(const StringView Msg) { return LogEx(Msg, LogType::Fatal); }

	//Makes a Log to the Console
	static void LogEx(const StringView Msg,LogType logType);
};


/*

*/
UCodeLangExportSymbol("UCodeGameEngine") enum class InputKey : Byte
{
	Null,

	//Mouse
	UCodeLangExclude MOUSE_BUTTON_LEFT,
	UCodeLangExclude MOUSE_BUTTON_RIGHT,
	UCodeLangExclude MOUSE_BUTTON_MIDDLE,
	UCodeLangExclude MOUSE_SCROLL_UP,
	UCodeLangExclude MOUSE_SCROLL_Down,

	//Or
	Space, Tab, Escapw, Enter, Backspace, Delete,
	//Other
	RightArrow, LeftArrow, DownArrow, UpArrow,
	//Numbers
	Key_0, Key_1, Key_2, Key_3, Key_4, Key_5, Key_6, Key_7, Key_8, Key_9,
	//Leters
	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

	MaxSize,
};

UCodeLangExportSymbol("UCodeGameEngine") enum class MouseButtion : Byte
{
	Null,
	Left,
	Right,
	Middle,
	Scrollup,
	ScrollDown,
	MaxSize,
};

/// <summary>
///	This is a Class for Input.
/// </summary>
UCodeLangExportSymbol("UCodeGameEngine") struct Input
{
	static bool IsKeyDown(InputKey Key);
	static bool IsMouseKeyDown(MouseButtion Key);
	static Vec2 GetMousePosition();
};

UCodeAPIEnd