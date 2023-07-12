#pragma once
#include "../EditorWindow.hpp"

#include <Editor/EditorNamespace.hpp>
#include "EditorWindows/OtherTypes/GraphShower.hpp"
EditorStart
class ShaderGraphWindow :public EditorWindow
{
public:
	ShaderGraphWindow(const NewEditorWindowData& windowdata);
	~ShaderGraphWindow() override;
	void UpdateWindow() override;

	
	static EditorWindowData GetEditorData();

	UCODE_EDITOR_FORCEINLINE static String GetOutFile(const String& File)
	{
		return File + ".glsl";
	}


private:
	static EditorWindow* MakeWin(const NewEditorWindowData& windowdata);
	GraphShower _Shower;
};

EditorEnd