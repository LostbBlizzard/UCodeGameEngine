#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>

#include <plog/Log.h>
#include "UCodeRunTime/BasicTypes.hpp"
CoreStart


#define UCODE_ENGINE_LOG_t(type,x) Core::Loger::InitCheck(); PLOG(Core::Loger::GetPlogType(type)) << x;
#define UCODE_ENGINE_LOG(x)  UCODE_ENGINE_LOG_t(Core::LogType::Default,x)
#define UCODE_ENGINE_ERROR(x)  UCODE_ENGINE_LOG_t(Core::LogType::Error,x)
#define UCODE_ENGINE_FATAL(x)  UCODE_ENGINE_LOG_t(Core::LogType::Fatal,x)
#define UCODE_ENGINE_Warning(x)  UCODE_ENGINE_LOG_t(Core::LogType::Warning,x)

enum class LogType : Byte
{
	Fatal,
	Error,
	Warning,
	Info,

	Default = LogType::Info,
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
	
	static void Log(const String& Msg, LogType Type = LogType::Default)
	{
		Log(StringView(Msg),Type);
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
	
private:
	
	inline static bool IsInit = false;
};

CoreEnd