#include "LineRenderer.hpp"
#include "UCodeRunTime/ULibrarys/Rendering/LineRenderer.hpp"
#include "Imgui/misc/cpp/imgui_stdlib.h"
EditorStart
 LineRendererUEditorData::LineRendererUEditorData()
{
	CompoentType = UCode::LineRenderer::Get_TypeID();
}

void LineRendererUEditorData::DrawAsTree(const UEditorComponentDrawTreeData& Data, UCode::Compoent* Value) const
{
	UCode::LineRenderer* Component = (UCode::LineRenderer*)Value;

	ImGuIHelper_Asset::AsssetField("Shader", Component->shader);

	ImGuIHelper::ColorField("Color", Component->color);
	ImGuIHelper::f32Field("Line Thickness", Component->LineThickness);

	ImGuIHelper_Asset::DrawLayerField("DrawLayer", Component->DrawLayer);
	ImGuIHelper::uInt8Field("DrawOrder", Component->DrawOrder);
	ImGuIHelper::Vec2Field("Start", Component->_Start);
	ImGuIHelper::Vec2Field("End", Component->_End);
}

void LineRendererUEditorData::DrawInspect(const UEditorComponentDrawData& Data, UCode::Compoent* Value) const
{
	UCode::LineRenderer* Component = (UCode::LineRenderer*)Value;
	{
		ImGui::BeginDisabled(true);

		const float square_sz = ImGui::GetFrameHeight();
		ImGuIHelper::Image(AppFiles::sprite::Uility_image, { square_sz ,square_sz });
		ImGui::SameLine();

		String tep = "Compoent/LineRender";

		ImGuIHelper::ItemLabel(StringView("Type"), ImGuIHelper::ItemLabelFlag::Left);

		ImGui::PushItemWidth(ImGui::CalcItemWidth() - (ImGuIHelper::CheckBoxSizeWithPadding().x));
		ImGui::PushID(&tep);
		ImGui::InputText("", &tep);
		ImGui::PopID();
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::EndDisabled();

		bool V = Component->Get_IsActive();
		ImGui::PushID(&V);
		ImGui::Checkbox("", &V);
		ImGui::PopID();
		Component->Set_CompoentActive(V);
	}
	ImGuIHelper_Asset::AsssetField("Shader", Component->shader);
	
	ImGuIHelper_Asset::DrawLayerField("DrawLayer", Component->DrawLayer);
	ImGuIHelper::DrawOrdeField("DrawOrder", Component->DrawOrder);
	
	ImGuIHelper::ColorField("Color", Component->color);
	ImGuIHelper::f32Field("Line Thckness", Component->LineThickness);
	
	ImGuIHelper::Vec2Field("Start", Component->_Start);
	ImGuIHelper::Vec2Field("End", Component->_End);
}

EditorEnd
