#include "InspectWindow.hpp"
#include "ImGui.hpp"
#include <Helper/ImGuIHelper.hpp>
#include <Editor/EditorAppCompoent.hpp>
#include "Helper/ImGuIHelper_Asset.hpp"
EditorStart

InspectWindow::InspectWindow(const NewEditorWindowData& windowdata) : EditorWindow(windowdata), _Inspect(), _InspectDrawer(this)
{
	WindowName = "InspectWindow";
}

InspectWindow::~InspectWindow()
{
}

void InspectWindow::UpdateWindow()
{
	if (_Inspect._Draw != nullptr)
	{
		if (_Updated)
		{
			_Updated = false;
			ImGui::SetWindowFocus();
		}
		_Inspect._Draw(_InspectDrawer);
	}
}
EditorWindowData InspectWindow::GetEditorData()
{
	return EditorWindowData("InspectWindow", MakeWin);
}
EditorWindow* InspectWindow::MakeWin(const NewEditorWindowData& windowdata)
{
	return new InspectWindow(windowdata);
}
void InspectWindow::DrawText(const char* text)
{
	ImGuIHelper::Text(StringView(text));
}
bool InspectWindow::Buttion(const char* FieldName)
{
	return ImGui::Button(FieldName);
}
bool InspectWindow::ToggleField(const char* FieldName, bool& Value)
{
	return ImGuIHelper::ToggleField(FieldName, Value);
}
bool InspectWindow::StringField(const char* FieldName, String&  Value)
{
	return ImGuIHelper::InputText(FieldName,Value);
}
bool InspectWindow::Vec2Field(const char* FieldName, UCode::Vec2& Value)
{
	return ImGuIHelper::Vec2Field(FieldName,Value);
}
bool InspectWindow::Vec2Field(const char* FieldName, UCode::Vec2i& Value)
{
	return ImGuIHelper::Vec2Field(FieldName, Value);
}
bool InspectWindow::Vec3Field(const char* FieldName, UCode::Vec3& Value)
{
	return ImGuIHelper::Vec3Field(FieldName, Value);
}
bool InspectWindow::Vec3Field(const char* FieldName, UCode::Vec3i& Value)
{
	return ImGuIHelper::Vec3Field(FieldName, Value);
}
bool InspectWindow::ColorField(const char* FieldName, UCode::ColorRGBA& Value)
{
	return ImGuIHelper::ColorField(FieldName, Value);
}
bool InspectWindow::ColorField(const char* FieldName, UCode::ColorRGB& Value)
{
	return ImGuIHelper::ColorField(FieldName, Value);
}
bool InspectWindow::ColorField(const char* FieldName, UCode::Color32& Value)
{
	return ImGuIHelper::ColorField(FieldName, Value);
}
bool InspectWindow::ColorField(const char* FieldName, UCode::Color24& Value)
{
	return ImGuIHelper::ColorField(FieldName, Value);
}
bool InspectWindow::uInt64Field(const char* FieldName, u64& Value)
{
	return ImGuIHelper::uInt64Field(FieldName, Value);
}
bool InspectWindow::uInt32Field(const char* FieldName, u32& Value)
{
	return ImGuIHelper::uInt32Field(FieldName, Value);
}
bool InspectWindow::uInt16Field(const char* FieldName,u16& Value)
{
	return ImGuIHelper::uInt16Field(FieldName, Value);
}
bool InspectWindow::uInt8Field(const char* FieldName,u8& Value)
{
	return ImGuIHelper::uInt8Field(FieldName, Value);
}
bool InspectWindow::Int64Field(const char* FieldName, i64& Value)
{
	return ImGuIHelper::Int64Field(FieldName, Value);
}
bool InspectWindow::f32Field(const char* FieldName, f32& Value)
{
	return ImGuIHelper::f32Field(FieldName, Value);
}
bool InspectWindow::f64Field(const char* FieldName, f64& Value)
{
	return ImGuIHelper::f64Field(FieldName, Value);
}
bool InspectWindow::BoolField(const char* FieldName, bool& Value)
{
	return  ImGuIHelper::BoolEnumField(FieldName, Value);
}
bool InspectWindow::CharField(const char* FieldName, char& Value)
{
	return ImGuIHelper::CharField(FieldName,Value);
}
bool InspectWindow::DrawLayerField(const char* FieldName,u8& Value)
{
	return ImGuIHelper_Asset::DrawLayerField(FieldName,Value);
}
bool InspectWindow::Int32Field(const char* FieldName, i32& Value)
{
	return ImGuIHelper::Int32Field(FieldName, Value);
}
bool InspectWindow::Int16Field(const char* FieldName, i16& Value)
{
	return ImGuIHelper::Int16Field(FieldName, Value);
}
bool InspectWindow::Int8Field(const char* FieldName, i8& Value)
{
	return ImGuIHelper::Int8Field(FieldName, Value);
}
EditorEnd


