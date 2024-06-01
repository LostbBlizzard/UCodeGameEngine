#include "ULangScript.hpp"
#include "UCodeRunTime/ULibrarys/UCodeLang/ULangRunTime.hpp"
#include "ULang/UCodeDrawer.hpp"
#include "ULang/UCompiler.hpp"
#include "Imgui/misc/cpp/imgui_stdlib.h"
EditorStart

ULangScriptUEditorData::ULangScriptUEditorData()
{
	CompoentType = UCode::ULangScript::Get_TypeID();
}

void ULangScriptUEditorData::DrawAsTree(const UEditorComponentDrawTreeData& Data, UCode::Compoent* Value) const
{
	UCode::ULangScript* Component = (UCode::ULangScript*)Value;

	if (Component->HasScript())
	{
		void* Ptr = Component->Get_UObj();
		auto ClassType = Component->Get_ClassData();
		auto& Assembly = Component->GetULang()->Get_Assembly();

		UCodeDrawer::DrawClass(Ptr, *ClassType, Assembly);
	}
	else
	{
		if (Component->Get_ClassData())
		{
			Component->ReLoad();
		}
	}
}

void ULangScriptUEditorData::DrawInspect(const UEditorComponentDrawData& Data, UCode::Compoent* Value) const
{
	UCode::ULangScript* Component = (UCode::ULangScript*)Value;
	{
		ImGui::BeginDisabled(true);


		const float square_sz = ImGui::GetFrameHeight();
		ImGuIHelper::Image(AppFiles::sprite::Uility_image, { square_sz ,square_sz });
		ImGui::SameLine();

		String tep = Component->GetClassName();

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
	auto ULang = Component->GetULang();

	if (!Component->HasScript())
	{
		if (Component->HasClass())
		{
			Component->ReLoad();
			return;
		}
		auto& Assembly = ULang->Get_Assembly();


		for (const auto& Item : Assembly.Classes)
		{
			if (Item->Get_Type() == UCodeLang::ClassType::Class)
			{
				if (UCompiler::IsAComponent(*Item, Assembly))
				{
					if (ImGui::MenuItem(Item->FullName.size() ? Item->FullName.c_str() : "noname"))
					{
						Component->LoadScript(Item.get());
					}
				}
			}
		}
	}
	else
	{
		auto& Assembly = ULang->Get_Assembly();
		void* Ptr = Component->Get_UObj();
		auto ClassType = Component->Get_ClassData();

		UCodeDrawer::DrawClass(Ptr, *ClassType, Assembly);
	}
}

EditorEnd
