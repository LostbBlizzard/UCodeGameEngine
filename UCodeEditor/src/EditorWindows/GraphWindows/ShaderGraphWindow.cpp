#include "ShaderGraphWindow.hpp"

namespace ed = ax::NodeEditor;
EditorStart
ShaderGraphWindow::ShaderGraphWindow(const NewEditorWindowData& windowdata)
	:EditorWindow(windowdata)
{
    _Shower.AddNode().Name = "One";
    _Shower.AddNode().Name = "Two";
    
    
    _Shower.AddPin(_Shower.GetNodeAtIndex(0),GraphShower::PinKind::Input).Name = "Hello";
}

ShaderGraphWindow::~ShaderGraphWindow()
{
}

void ShaderGraphWindow::UpdateWindow()
{
  
    //
    _Shower.Draw();
}


EditorWindowData ShaderGraphWindow::GetEditorData()
{
	return EditorWindowData("ShaderGraphWindow", MakeWin);
}

EditorWindow* ShaderGraphWindow::MakeWin(const NewEditorWindowData& windowdata)
{
	return new ShaderGraphWindow(windowdata);
}
EditorEnd