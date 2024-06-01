#include "ULangScript.hpp"
#include "UCodeRunTime/ULibrarys/UCodeLang/ULangRunTime.hpp"
#include "ULang/UCodeDrawer.hpp"
#include "ULang/UCompiler.hpp"
#include "Imgui/misc/cpp/imgui_stdlib.h"
#include "UEditorModules/Modules/CodeModule.hpp"
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
		ImGui::BeginDisabled(Component->HasScript());


		const float square_sz = ImGui::GetFrameHeight();
		ImGuIHelper::Image(AppFiles::sprite::ULangScript, { square_sz ,square_sz });
		ImGui::SameLine();

		String tep = Component->GetClassName();

		ImGuIHelper::ItemLabel(StringView("Type"), ImGuIHelper::ItemLabelFlag::Left);

		ImGui::PushItemWidth(ImGui::CalcItemWidth() - (ImGuIHelper::CheckBoxSizeWithPadding().x));
		ImGui::PushID(&tep);

		if (!Component->HasScript())
		{
			const Vector<const UCodeLang::AssemblyNode*>& vaildcompoents = CodeModule::GetAllVaildCompoents();
			ImGuIHelper::ObjectFieldData data;
			data.OnObjectInList = [Component](void* Ptr, void* Object, bool Listmode, const String& Find) -> bool
				{
					const UCodeLang::AssemblyNode* item = *(const UCodeLang::AssemblyNode**)Object;
	
					auto imagesize = ImGui::GetFrameHeight();
					ImGuIHelper::Image(AppFiles::sprite::ULangScript, { imagesize,imagesize });

					ImGui::SameLine();
					bool r =ImGui::Selectable(item->FullName.c_str(),false);
					if (r)
					{
						Component->LoadScript(item);
					}
					return r;
				};

			ImGuIHelper::DrawObjectField(&tep, vaildcompoents.data(), vaildcompoents.size()
				,sizeof(const UCodeLang::AssemblyNode*), data, AppFiles::sprite::ULangScript, tep);
		}
		else 
		{
			ImGui::InputText("", &tep);
		}
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
