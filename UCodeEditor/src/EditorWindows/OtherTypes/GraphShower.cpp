#include "GraphShower.hpp"
EditorStart

namespace ed = ax::NodeEditor;
GraphShower::GraphShower()
{
	config.SettingsFile = nullptr;
	m_Context = ax::NodeEditor::CreateEditor(&config);
}

GraphShower::Node& GraphShower::AddNode()
{
	_nodes.push_back({});
	auto& r = _nodes.back();
	r.ID = NextID;
	NextID++;
	return r;
}
void GraphShower::Draw()
{
	ax::NodeEditor::SetCurrentEditor(m_Context);
	ax::NodeEditor::Begin("My Editor", ImVec2(0.0, 0.0f));
	for (auto& Item : _nodes)
	{
		auto Id = Item.ID;
		ed::BeginNode(Id);
		
		ImGui::Text(Item.Name.c_str());
		
		
		ed::EndNode();
	}



	if (ed::BeginCreate())
	{
		ed::PinId inputPinId, outputPinId;
		if (ed::QueryNewLink(&inputPinId, &outputPinId))
		{

		}
	}
	ed::EndCreate();

	// Handle deletion action
	if (ed::BeginDelete())
	{
		// There may be many links marked for deletion, let's loop over them.
		ed::LinkId deletedLinkId;
		while (ed::QueryDeletedLink(&deletedLinkId))
		{
			// If you agree that link can be deleted, accept deletion.
			if (ed::AcceptDeletedItem())
			{
				
			}

			// You may reject link deletion by calling:
			// ed::RejectDeletedItem();
		}
	}
	ed::EndDelete(); // Wrap up deletion acti

	ax::NodeEditor::End();

	ax::NodeEditor::SetCurrentEditor(nullptr);
}
GraphShower::Pin& GraphShower::AddPin(Node& node,GraphShower::PinKind type)
{
	Vector<Pin>& Pins = type == PinKind::Input ? node.Inputs : node.Outputs;
	
	Pins.push_back({});
	Pin& r =Pins.back();
	r.ID = NextID; NextID++;
	r.Kind = type;
	r.node = &node;

	return r;
}

GraphShower::~GraphShower()
{
	ed::DestroyEditor(m_Context);
}
EditorEnd
