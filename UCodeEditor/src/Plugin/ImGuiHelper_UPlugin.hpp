#include "UPlugin.hpp"

EditorStart
struct ImGuiHelper_UPlugin
{
	static bool DrawUPluginField(StringView FeildName,UPluginID& value);

	static bool DrawUPluginFieldVector(StringView FeildName, Vector<UPluginID>& value);
};
EditorEnd