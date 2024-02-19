#include "Loger.hpp"

#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"

#include "UCodeRunTime/UDefs.hpp"
#include "UCodeRunTime/CoreSystems/Threads.hpp"
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

struct CallBackItem
{
	Loger::CallBackKey key;
	Loger::ListnerCallBack callback;
};

static Vector<CallBackItem> CallBacksItems;
constexpr Loger::CallBackKey NullCallBack = 0;
static Loger::CallBackKey NextKey = 0;


class IAppender :public plog::IAppender
{
public:
	~IAppender() override
	{

	}
	void write(const plog::Record& record) override
	{
		#if DebugMode
		std::wostream& Output = std::wcerr;
		PushToStream(Output, record);
		#endif // DEBUG
	
		if (!LogerFileOutInfo.Loger_FileOutput.empty())
		{
			PushToStream(LogerFileOutInfo.OutStream(), record);
		}

		std::basic_stringstream<wchar_t> str;
		PushToStream(str, record);

		String msg = Path(str.str()).generic_string();

		auto docallfunc = [msg]()
		{
			for (auto& Item : CallBacksItems)
			{
				Item.callback(msg);
			}
		};
	
		if (Threads::Get_Threads())
		{
			if (Threads::IsOnMainThread())
			{
				docallfunc();
			}
			else
			{
				Threads::Get_Threads()->AddTask(UCode::TaskType::Main, std::bind(docallfunc), {});
			}
		}
	}
	void PushToStream(std::wostream& Output, const plog::Record& record)
	{
		Output << "[";

		tm t;
		plog::util::localtime_s(&t, &record.getTime().time);

		//Output << t.tm_year + 1900 << PLOG_NSTR("/") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mon + 1 << PLOG_NSTR("/") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mday << PLOG_NSTR(";");
		Output << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << PLOG_NSTR(".") << std::setfill(PLOG_NSTR('0')) << std::setw(3) << static_cast<int> (record.getTime().millitm);

		Output << "]";


		#if !PublishMode
		/*	Output
			<< "~" << record.getFunc()
			//<< " -file: '" << record.getFile() << "'"
			<< ":" << record.getLine() << "";
		*/
		#endif	

		Output << ":";
		Output << record.getMessage();


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
	plog::init(plog::debug, &O);
}
void Loger::SetLogOutfile(const Path& Str)
{
	LogerFileOutInfo.SetFile(Str);
}
Loger::CallBackKey Loger::AddListener(ListnerCallBack callback)
{
	#ifdef DebugMode
	if (!Threads::IsOnMainThread())
	{
		UCodeGEError("Adding Log Listener on Non-Main Thread is Undefined");

		return NullCallBack;
	}
	#endif

	if (NextKey == NullCallBack)
	{
		NextKey++;
	}
	auto newcallbackkey = NextKey;
	NextKey++;

	CallBackItem item;
	item.key = newcallbackkey;
	item.callback = callback;

	CallBacksItems.push_back(std::move(item));

	return newcallbackkey;
}
void Loger::RemoveListener(CallBackKey key)
{
	#ifdef DebugMode
	if (!Threads::IsOnMainThread())
	{
		UCodeGEError("Removeing Log Listener on Non-Main Thread is Undefined");

		if (NullCallBack)
		{
			return;
		}
	}
	#endif

	for (size_t i = 0; i < CallBacksItems.size(); i++)
	{
		auto& Item = CallBacksItems[i];
		if (Item.key == key)
		{
			CallBacksItems.erase(CallBacksItems.begin() + i);
			return;
		}
	}

	#ifdef DebugMode
	UCodeGEError("Could not Remove Log Listener using key. The key may be invaild");
	#else 
	UCodeGEUnreachable();
	#endif
}
CoreEnd