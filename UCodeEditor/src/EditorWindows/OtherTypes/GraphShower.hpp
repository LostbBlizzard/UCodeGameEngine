#pragma once

#include <Editor/EditorNamespace.hpp>
#include "EditorWindows/NodeEditor/Source/imgui_node_editor.h"
#include "Typedefs.hpp"
EditorStart

class GraphShower
{
public:
	enum class PinKind
	{
		Output,
		Input
	}; 
	enum class PinType
	{
		Flow,
		Bool,
		Int,
		Float,
		String,
		Object,
		Function,
		Delegate,
	};
	struct Node;
	struct Pin
	{
		ax::NodeEditor::PinId   ID;
		Node* Node;
		String Name;
		PinType     Type;
		PinKind     Kind;
		Pin()
		{

		}

	};
	struct Node
	{
		String Name;
		ax::NodeEditor::NodeId ID;
		Vector<Pin> Inputs;
		Vector<Pin> Outputs;
		Node()
		{

		}
	};
	

	GraphShower();
	Node& AddNode();
	Pin& AddPin(Node& node, PinKind type);
	void Draw();

	Node& GetNodeAtIndex(size_t I)
	{
		return _nodes[I];
	}

	~GraphShower();
private:
	ax::NodeEditor::Config config;
	ax::NodeEditor::EditorContext* m_Context = nullptr;
	Vector<Node> _nodes;
	size_t NextID = 0;
};
EditorEnd