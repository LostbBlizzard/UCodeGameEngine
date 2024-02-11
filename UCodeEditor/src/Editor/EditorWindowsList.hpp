#pragma once

#include <Editor/EditorNamespace.hpp>
#include <EditorWindows/EditorWindow.hpp>
EditorStart
class EditorWindowsList
{
public:
	struct NewMenu 
	{ 
		const StringView Name;
		NewMenu(StringView name)
			:Name(name)
		{
		}
	};
	struct EndMenu {};
	
	const static Variant<EditorWindowData,NewMenu,EndMenu>* EditorWindows;
	const static size_t EditorWindows_Size;
};
EditorEnd

