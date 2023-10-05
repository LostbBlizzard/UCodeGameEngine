#include "Debuging.hpp"
#include "UCodeRunTime/ULibrarys/Loger.hpp"
UCodeAPIStart

UCode::LogType To(LogType Value)
{
	switch (Value)	
	{
	case UCode::ULangAPI::LogType::Log:return UCode::LogType::Info;
	case UCode::ULangAPI::LogType::Warning:return UCode::LogType::Warning;
	case UCode::ULangAPI::LogType::Error:return UCode::LogType::Error;
	case UCode::ULangAPI::LogType::Fatal:return UCode::LogType::Fatal;
		
	default:
		#if UCodeGameEngineDEBUG
		throw std::exception("not added");
		#endif // DEBUG
		break;
	}
}

void Debug::LogEx(const StringView Msg, LogType logType)
{
	String NewMSG =String(Msg);

	//TODO add stack frames to MSG. 

	UCode::Loger::Log(NewMSG,To(logType));
	return UCodeLangExport void();
}

UCodeAPIEnd

