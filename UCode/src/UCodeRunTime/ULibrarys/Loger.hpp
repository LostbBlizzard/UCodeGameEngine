#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>

#include "UCodeRunTime/BasicTypes.hpp"
#include "plog/Log.h"

#include "UCodeLang/LangCore/LangDef.hpp"
CoreStart


#define UCodeGELog_t(type,x) Core::Loger::InitCheck(); PLOG(Core::Loger::GetPlogType(type)) << x;
#define UCodeGELog(x)  UCodeGELog_t(Core::LogType::Default,x)
#define UCodeGEError(x)  UCodeGELog_t(Core::LogType::Error,x)
#define UCodeGEFatal(x)  UCodeGELog_t(Core::LogType::Fatal,x)
#define UCodeGEWarning(x)  UCodeGELog_t(Core::LogType::Warning,x)

enum class LogType : Byte
{
	Fatal,
	Error,
	Warning,
	Info,

	Default = (Byte)LogType::Info,
};
class Loger
{
public:
	static constexpr plog::Severity GetPlogType(LogType Type)
	{
		switch (Type)
		{
		case UCode::LogType::Fatal:
			return plog::Severity::fatal;
		case UCode::LogType::Error:
			return plog::Severity::error;
		case UCode::LogType::Warning:
			return plog::Severity::warning;
		case UCode::LogType::Info:
		default:
			return plog::Severity::info;
		}
	}
	using String = std::string;
	using StringView = std::string_view;
	using Path = std::filesystem::path;

	static void Log(const String& Msg, LogType Type = LogType::Default)
	{
		Log(StringView(Msg), Type);
	}
	static void Log(const char* Msg, LogType Type = LogType::Default)
	{
		Log(StringView(Msg), Type);
	}

	static void Log(const StringView& Msg, LogType Type = LogType::Default);



	static void Init();
	static inline void InitCheck()
	{
		if (!IsInit)
		{
			IsInit = true;
			Init();
		}
	};
	static void SetLogOutfile(const Path& Str);

	using ListnerCallBack = Delegate<void, StringView>;
	using CallBackKey = int;

	//Callback will allways be on main thread
	static CallBackKey AddListener(ListnerCallBack callback);
	static void RemoveListener(CallBackKey key);
private:

	inline static bool IsInit = false;
};

/// <summary>
///	This is a Class for Debuging.
/// </summary>
UCodeLangExportSymbol("UCodeGameEngine") struct Debug
{
	///Makes a Log to the Console
	UCodeLangExport inline static void Log(const String& Msg) { return LogEx(StringView(Msg), LogType::Info); }
	//Makes a Log to the Console
	UCodeLangExport inline static void Log(const StringView Msg) { return LogEx(Msg, LogType::Info); }


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
	static void LogEx(const StringView Msg, LogType logType)
	{
		return Loger::Log(Msg, logType);
	}
};


CoreEnd