#pragma once
#include "APITypeDefs.hpp"
#include "UCodeLang/UCodeAnalyzer/CppHelper.hpp"
UCodeAPIStart


#define UCodeLangExclude


using LogType_t = int;
UCodeLangExportSymbol("UCodeGameEngine") enum class LogType :LogType_t
{
	//For Debuging and testing.
	Log,

	//For Somehting risky or could cause an Error. something may be wourng
	Warning,

	//A Recoverable Error may be cause by an exception or a painc.
	Error,
	
	//Very bad Errors that may cause put the GameRunTime to be an unstable state or cause a crash. 
	Fatal,


	UCodeLangExclude MinValues = (LogType_t)Log,
	UCodeLangExclude MaxValues = (LogType_t)Fatal,
};

/// <summary>
///	This is a Class for Debuging.
/// </summary>
UCodeLangExportSymbol("UCodeGameEngine") struct Debug
{
	/// <summary>
	/// Does nothing but add returns Value + 5.This is for Testing the API.
	/// </summary>
	/// <param name="Value"></param>
	/// <returns></returns>
	UCodeLangExport inline static int APIPing(int Value) { return Value + 5; }



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
	UCodeLangExport static void LogEx(const StringView Msg,LogType logType);
};


UCodeAPIEnd