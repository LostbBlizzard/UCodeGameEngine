#pragma once
#include "../EditorWindow.hpp"

#include <Editor/EditorNamespace.hpp>
#include <UCodeLang/LangCore.hpp>
EditorStart

class UCodeGraph :public EditorWindow
{
public:
	UCodeGraph(const NewEditorWindowData& windowdata);
	~UCodeGraph() override;
	void UpdateWindow() override;

	static EditorWindowData GetEditorData();

	UCODE_EDITOR_FORCEINLINE static String GetOutFile(const String& File)
	{
		return File + UCodeLang::FileExt::SourceFileWithDot;
	}
private:
	static EditorWindow* MakeWin(const NewEditorWindowData& windowdata);
};

EditorEnd

