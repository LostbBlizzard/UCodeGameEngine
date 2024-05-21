#pragma once
#include <EditorWindows/EditorWindow.hpp>
#include <Editor/EditorNamespace.hpp>
#include <UCodeRunTime/ULibrarys/Loger.hpp>
EditorStart


class ConsoleWindow :public EditorWindow
{
public:
	using LogMessageID = int;
	enum class LogType :unsigned char
	{
		Log,
		Error,
		Warning,
		ConsoleInput,
	};
	struct Log
	{
		String Text;
		String MoreText;
		LogType _Type = LogType::Log;
		LogMessageID _MessageID = {};

		Optional<Delegate<void,ConsoleWindow&,Log&>> _OnOpen;
	};

	ConsoleWindow(const NewEditorWindowData& windowdata);
	~ConsoleWindow() override;
	void UpdateWindow() override;
	static EditorWindowData GetEditorData();

	ConsoleWindow::LogMessageID AddLog(Log Msg);

	
	void RemoveLog(Span<ConsoleWindow::LogMessageID> Log);
	void RemoveLog(ConsoleWindow::LogMessageID Log)
	{
		RemoveLog(Span<LogMessageID>::Make(&Log, 1));
	}
	void ClearLogs()
	{
		_Items.clear();
		_LookingAtLog = nullptr;
	}
	void FocusWindow()
	{
		_FocusWindow = true;
	}
	void ClearLogs(std::function<bool(const Log& item)> ToRemove);
private:
	static EditorWindow* MakeWin(const NewEditorWindowData& windowdata);
	UCode::Vector<Log> _Items;
	String _filter;
	String _ConsoleIn;
	Log* _LookingAtLog;
	bool _FocusWindow = false;
	UCode::Loger::CallBackKey logkey;
	LogMessageID _NextLogID = {};
	LogMessageID GetNextLogID() 
	{
		auto r = _NextLogID;
		_NextLogID++;
		return r;
	}
};
EditorEnd
