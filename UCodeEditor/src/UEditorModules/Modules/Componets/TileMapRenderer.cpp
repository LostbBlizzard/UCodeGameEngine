#include "TileMapRenderer.hpp"
#include "UCodeRunTime/ULibrarys/Rendering/TileMapRenderer2d.hpp"
#include "Imgui/misc/cpp/imgui_stdlib.h"
EditorStart
TileMapRenderUEditorData::TileMapRenderUEditorData()
{
	CompoentType = UCode::TileMapRenderer::Get_TypeID();
}
void TileMapRenderUEditorData::DrawAsTree(const UEditorComponentDrawTreeData& Data, UCode::Compoent* Value) const
{
	UCode::TileMapRenderer* ValueAs = (UCode::TileMapRenderer*)Value;
}

void TileMapRenderUEditorData::DrawInspect(const UEditorComponentDrawData& Data, UCode::Compoent* Value) const
{
	UCode::TileMapRenderer* ValueAs = (UCode::TileMapRenderer*)Value;
	{
		ImGui::BeginDisabled(true);



		const float square_sz = ImGui::GetFrameHeight();
		ImGuIHelper::Image(AppFiles::sprite::Uility_image, { square_sz ,square_sz });
		ImGui::SameLine();

		String tep = "Compoent/TileMapRender";

		ImGuIHelper::ItemLabel(StringView("Type"), ImGuIHelper::ItemLabelFlag::Left);

		ImGui::PushItemWidth(ImGui::CalcItemWidth() - (ImGuIHelper::CheckBoxSizeWithPadding().x));
		ImGui::PushID(&tep);
		ImGui::InputText("", &tep);
		ImGui::PopID();
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::EndDisabled();

		bool V = Value->Get_IsActive();
		ImGui::PushID(&V);
		ImGui::Checkbox("", &V);
		ImGui::PopID();
		Value->Set_CompoentActive(V);
	}
}

EditorEnd
