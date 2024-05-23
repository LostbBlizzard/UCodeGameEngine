#include "SpriteRenderer.hpp"
#include "UCodeRunTime/ULibrarys/Rendering/SpriteRenderer2d.hpp"
#include "Imgui/misc/cpp/imgui_stdlib.h"
EditorStart
SpriteRendererUEditorData::SpriteRendererUEditorData()
{
	CompoentType = UCode::SpriteRenderer::Get_TypeID();
}

void SpriteRendererUEditorData::DrawAsTree(const UEditorComponentDrawTreeData& Data, UCode::Compoent* Value) const
{
	UCode::SpriteRenderer* Component = (UCode::SpriteRenderer*)Value;

	ImGuIHelper_Asset::AsssetField("Shader", Component->shader);
	ImGuIHelper_Asset::AsssetField("Sprite", Component->sprite);

	ImGuIHelper::ColorField("Color", Component->color);
	ImGuIHelper::BoolEnumField("flipX", Component->flipX);
	ImGuIHelper::BoolEnumField("flipY", Component->flipY);

	ImGuIHelper_Asset::DrawLayerField("DrawLayer", Component->DrawLayer);
	ImGuIHelper::uInt8Field("DrawOrder", Component->DrawOrder);
}

void SpriteRendererUEditorData::DrawInspect(const UEditorComponentDrawData& Data, UCode::Compoent* Value) const
{
	UCode::SpriteRenderer* Component = (UCode::SpriteRenderer*)Value;
	{
		ImGui::BeginDisabled(true);



		const float square_sz = ImGui::GetFrameHeight();
		ImGuIHelper::Image(AppFiles::sprite::Uility_image, { square_sz ,square_sz });
		ImGui::SameLine();

		String tep = "Compoent/SpriteRenderer";

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
	ImGuIHelper_Asset::AsssetField("Sprite", Component->sprite);

	ImGuIHelper::ColorField("Color", Component->color);
	ImGuIHelper::BoolEnumField("flipX", Component->flipX);
	ImGuIHelper::BoolEnumField("flipY", Component->flipY);

	ImGuIHelper_Asset::DrawLayerField("DrawLayer", Component->DrawLayer);
	ImGuIHelper::DrawOrdeField("DrawOrder", Component->DrawOrder);

}
EditorEnd
