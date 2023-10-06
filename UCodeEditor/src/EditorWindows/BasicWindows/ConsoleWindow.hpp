#pragma once
#include <EditorWindows/EditorWindow.hpp>
#include <Editor/EditorNamespace.hpp>
EditorStart


class ConsoleWindow :public EditorWindow
{
public:
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
		LogType _Type;

		Optional<Delegate<void,ConsoleWindow&,Log&>> _OnOpen;
	};

	ConsoleWindow(const NewEditorWindowData& windowdata);
	~ConsoleWindow() override;
	void UpdateWindow() override;
	static EditorWindowData GetEditorData();

	void AddLog(Log Msg);

	
	void ClearLogs()
	{
		_Items.clear();
		_LookingAtLog = nullptr;
	}
	void FocusWindow()
	{
		_FocusWindow = true;
	}
private:
	static EditorWindow* MakeWin(const NewEditorWindowData& windowdata);
	UCode::Vector<Log> _Items;
	String _filter;
	String _ConsoleIn;
	Log* _LookingAtLog;
	bool _FocusWindow = false;
};
EditorEnd
