#pragma once


#include <UCodeRunTime/App.hpp>
#include "EditorAppCompoent.hpp"
#include <Editor/EditorNamespace.hpp>
#include "Typedefs.hpp"
EditorStart
class EditorApp
{
public:
	EditorApp();
	~EditorApp();

	void Run(const String& ProjPath);
private:
	UCode::UApp _App;
};
EditorEnd

