#include "UCodeGraph.hpp"
EditorStart
UCodeGraph::UCodeGraph(const NewEditorWindowData& windowdata)
	:EditorWindow(windowdata)
{
}
UCodeGraph::~UCodeGraph()
{
}

void UCodeGraph::UpdateWindow()
{
}

EditorWindowData UCodeGraph::GetEditorData()
{
	return EditorWindowData("UCodeGraph", MakeWin);
}

EditorWindow* UCodeGraph::MakeWin(const NewEditorWindowData& windowdata)
{
	return new UCodeGraph(windowdata);
}

EditorEnd


