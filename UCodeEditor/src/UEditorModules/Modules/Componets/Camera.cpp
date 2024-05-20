#include "Camera.hpp"
#include "Imgui/misc/cpp/imgui_stdlib.h"
EditorStart
CameraUEditorData::CameraUEditorData()
{
	CompoentType = UCode::Camera2d::Get_TypeID();
}

void CameraUEditorData::DrawAsTree(const UEditorComponentDrawTreeData& Data, UCode::Compoent* Value) const
{
	UCode::Camera2d* Component = (UCode::Camera2d*)Value;

	auto CamTV = Component->Get_CamType();
	ImGuIHelper::EnumField("Type", CamTV, CamTypes, CamTypesCount);
	Component->Set_CamType(CamTV);

	if (CamTV == UCode::Camera2d::CamType::Orthographic)
	{
		f32 V = Component->Get_Ortho_size();
		ImGuIHelper::f32Field("Ortho_Size", V);
		Component->Set_Ortho_size(V);
	}
	else
	{
		f32 V = Component->Get_Fov();
		ImGuIHelper::f32Field("Fov", V);
		Component->Set_Fov(V);
	}
	auto color = Component->Get_BackRoundClearColor();
	ImGuIHelper::ColorField("Backround Clear Color", color);
	Component->Set_BackRoundClearColor(color);
}

void CameraUEditorData::DrawInspect(const UEditorComponentDrawData& Data, UCode::Compoent* Value) const
{
	UCode::Camera2d* Component = (UCode::Camera2d*)Value;
	{
		ImGui::BeginDisabled(true);


		const float square_sz = ImGui::GetFrameHeight();
		ImGuIHelper::Image(AppFiles::sprite::Uility_image, { square_sz ,square_sz });
		ImGui::SameLine();

		String tep = "Compoent/Camera";

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

	}
	auto CamTV = Component->Get_CamType();
	ImGuIHelper::EnumField("Type", CamTV, CamTypes, CamTypesCount);
	Component->Set_CamType(CamTV);
	if (CamTV == UCode::Camera2d::CamType::Orthographic)
	{
		f32 V = Component->Get_Ortho_size();
		Data.Draw->f32Field("Ortho_Size", V);
		Component->Set_Ortho_size(V);
	}
	else
	{
		f32 V = Component->Get_Fov();
		Data.Draw->f32Field("Fov", V);
		Component->Set_Fov(V);
	}
	auto color = Component->Get_BackRoundClearColor();
	ImGuIHelper::ColorField("Backround Clear Color", color);
	Component->Set_BackRoundClearColor(color);

}

EditorEnd
