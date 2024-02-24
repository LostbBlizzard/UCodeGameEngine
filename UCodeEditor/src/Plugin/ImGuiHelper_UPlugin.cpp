#include "ImGuiHelper_UPlugin.hpp"
#include "Helper/ImGuIHelper.hpp"
EditorStart

bool ImGuiHelper_UPlugin::DrawUPluginField(StringView FeildName, UPluginID& value)
{
	return false;
}

bool ImGuiHelper_UPlugin::DrawUPluginFieldVector(StringView FeildName, Vector<UPluginID>& value)
{
	ImGuIHelper::Text(FeildName);

	return false;
}

EditorEnd
