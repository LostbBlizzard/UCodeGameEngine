#include "Loger.hpp"

#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"

#include "UCodeRunTime/UDefs.hpp"
#include <iostream>
#include <fstream>
CoreStart


#define FileOutPath "LogOut.txt"
struct FileOutInfo
{
	Path Loger_FileOutput = FileOutPath;
	std::wfstream File;

	void SetFile(const Path& Str)
	{
		Loger_FileOutput = Str;
		if (File.is_open())
		{
			File.close();
		}
	}
	
	std::wostream& OutStream()
	{
		if (!File.is_open())
		{
			File.open(Loger_FileOutput);
		}

		return File;
	}

	~FileOutInfo()
	{
		if (File.is_open())
		{
			File.close();
		}
	}
};
static FileOutInfo LogerFileOutInfo;

class IAppender :public plog::IAppender
{
public:
	~IAppender() override
	{

	}
	void write(const plog::Record& record) override
	{
		std::wostream& Output = std::wcout;
		#if UCodeGEDebug
		PushToStream(Output, record);
		#endif // DEBUG
		if (!LogerFileOutInfo.Loger_FileOutput.empty())
		{
			PushToStream(LogerFileOutInfo.OutStream(), record);
		}
	}
	void PushToStream(std::wostream& Output, const plog::Record& record)
	{
		Output << "[";

		tm t;
		plog::util::localtime_s(&t, &record.getTime().time);

		Output << t.tm_year + 1900 << PLOG_NSTR("/") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mon + 1 << PLOG_NSTR("/") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mday << PLOG_NSTR(";");
		Output << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << PLOG_NSTR(".") << std::setfill(PLOG_NSTR('0')) << std::setw(3) << static_cast<int> (record.getTime().millitm) << PLOG_NSTR(";");

		Output << "]";

		
		#if !PublishMode
		Output
			<< "~" << record.getFunc()
			//<< " -file: '" << record.getFile() << "'"
			<< ":" << record.getLine() << "";
		#endif	
		
		Output << ":\"";
		Output << record.getMessage() << "\"";


		Output << std::endl;
	}
};

void Loger::Log(const StringView& Msg, LogType Type)
{
	InitCheck();
    PLOG(GetPlogType(Type)) << Msg;

}
void Loger::Init()
{   
	static  IAppender O;
    plog::init(plog::debug,&O);
}
void Loger::SetLogOutfile(const Path& Str)
{
	LogerFileOutInfo.SetFile(Str);
}
CoreEnd