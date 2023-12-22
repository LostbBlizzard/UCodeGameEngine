#include "Inspect_Entity2d.hpp"
#include "ImGui.hpp"
#include "UCodeRunTime/ULibrarys/Rendering_Library.hpp"

#include "EditorWindows/EditorStrings.hpp" 

#include "UCodeRunTime/ULibrarys/UCodeLang/ULangRunTime.hpp"

#include "Helper/ImGuIHelper.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include "MakeableCompoents.hpp"
#include "imgui_internal.h"
#include "Helper/ImGuIHelper_Asset.hpp"
#include <stack>
#include "UEditorModules/UEditorModule.hpp"
#include "ULang/UCompiler.hpp"
EditorStart



InspectWindow::InspectData Inspect_Entity2d::Get(UCode::EntityPtr Item)
{
    AnyManagedPtr PtrData = AnyManagedPtr::As_ptr(Item);
    return { PtrData,Insp_ };
}
InspectWindow::InspectData Inspect_Entity2d::Get(UCode::Entity* Item)
{
    return Get(Item->NativeManagedPtr());
}
void Inspect_Entity2d::Insp_(InspectWindow::InspectDrawer& Draw)
{

    UCode::EntityPtr& Ptr = Draw.GetPtr().As_ptr<UCode::Entity>();
    if (!Ptr.Has_Value())
    {
        Draw.SetPtrNull(); return;
    }
    UCode::Entity* item = Ptr.Get_Value();


    {
        ImGui::BeginDisabled(true);


        ImGuIHelper::Image(AppFiles::sprite::Entity, { 20 ,20 });
        ImGui::SameLine();

        String tep ="Entity";
        ImGuIHelper::InputText("Type",tep);
        ImGui::EndDisabled();

        ImGui::SameLine();

        bool V = item->GetActive();
        ImGuIHelper::ToggleField("Active", V);
        item->SetActive(V);


        ImGuIHelper::InputText("Name", item->NativeName());
    }
   
    bool Is2D = true;
    auto Cam = UCode::Camera2d::FindMainCam(item->NativeGameRunTime());
    if (Cam)
    {
        Is2D = Cam->Get_CamType() == UCode::Camera2d::CamType::Orthographic;
    }
    if (!item->NativeParent())
    {
        if (Is2D)
        {
            Draw.Vec2Field("Position", item->localposition2d());
            Draw.Vec2Field("Rotation", item->localrotation2d());
            Draw.Vec2Field("Scale", item->localscale2d());
        }
        else 
        {
            Draw.Vec3Field("Position", item->localposition());
            Draw.Vec3Field("Rotation", item->localrotation());
            Draw.Vec3Field("Scale", item->localscale());
        }
    }
    else
    {
        if (Is2D)
        {
            Draw.Vec2Field("Local Position", item->localposition2d());
            Draw.Vec2Field("Local Rotation", item->localrotation2d());
            Draw.Vec2Field("Local Scale", item->localscale2d());

            ImGui::Separator();


            Vec2 Tep = item->worldposition2d();

            if (Draw.Vec2Field("World Position", Tep))
            {
                item->worldposition(Tep);
            }

            Tep = item->worldrotation2d();

            if (Draw.Vec2Field("World Rotation", Tep))
            {
                item->worldrotation(Tep);
            }

            Tep = item->worldscale2d();
            if (Draw.Vec2Field("World Scale", Tep))
            {
                item->worldscale(Tep);
            }
        }
        else
        {
            Draw.Vec3Field("Local Position", item->localposition());
            Draw.Vec3Field("Local Rotation", item->localrotation());
            Draw.Vec3Field("Local Scale", item->localscale());

            ImGui::Separator();


            Vec3 Tep = item->worldposition();

            if (Draw.Vec3Field("World Position", Tep))
            {
                item->worldposition(Tep);
            }

            Tep = item->worldrotation();

            if (Draw.Vec3Field("World Rotation", Tep))
            {
                item->worldrotation(Tep);
            }

            Tep = item->worldscale();

            if (Draw.Vec3Field("World Scale",Tep))
            {
                item->worldscale(Tep);
            }
        }
    }
    ImGui::Separator();

    for (auto& Item : item->NativeCompoents())
    {
        Inspect_Compoent2d::Insp_(Item.get(), Draw);
        ImGui::Separator();
    }


    ImGui::Separator();
    if (ImGui::BeginMenu("Add Compoent"))
    {
        Inspect_Compoent2d::ShowAddCompoenList(item);
        ImGui::EndMenu();
    }
}

AppFiles::sprite  Inspect_Compoent2d::GetCompoentSprite(UCode::UComponentsID Type)
{
    AppFiles::sprite r;

    /*
    switch (Type)
    {
    case UCode::UComponentsID::Camera2d:
        r = AppFiles::sprite::Camera2d;
        break;
    case UCode::UComponentsID::SpriteRenderer:
        r = AppFiles::sprite::SpriteRenderer;
        break;
    case UCode::UComponentsID::ULangScript:
        r = AppFiles::sprite::ULangScript;
        break;
    case UCode::UComponentsID::ULangRunTime:
        r = AppFiles::sprite::Uility_image;
        break;
    case UCode::UComponentsID::Position2dCompoent:
    case UCode::UComponentsID::Position3dCompoent:
    case UCode::UComponentsID::Scale2dCompoent:
    case UCode::UComponentsID::Scale3dCompoent:
    case UCode::UComponentsID::Rotation3dCompoent:
    case UCode::UComponentsID::EntityNameCompoent:
    case UCode::UComponentsID::SceneNameCompont:
        r = AppFiles::sprite::Property;
        break;
    default:
        r = AppFiles::sprite::Null;
        break;
    }
    */

    return AppFiles::sprite::Uility_image;
}
InspectWindow::InspectData Inspect_Compoent2d::Get(UCode::Compoent* Item)
{
    auto V = Item->Get_ManagedPtr();
    AnyManagedPtr& R = AnyManagedPtr::As_ptr(V);
    return { R,Insp_ };
}
void Inspect_Compoent2d::DrawCompoentButtion(UCode::Compoent* Item, EditorAppCompoent* App)
{
    auto TypeData = Item->Get_CompoentTypeData();
  

   
    auto Type = TypeData->_Type;
    const char* CompoentName = Get_CompoentName(Item);

    ImGui::PushID(Item);   
    
   
    bool IsNotActive = false;
    if (IsNotActive)
    {
        ImGuiContext& g = *GImGui;
        g.DisabledAlphaBackup = g.Style.Alpha;
        g.Style.Alpha *= g.Style.DisabledAlpha;
    }

    bool node_open = ImGuIHelper::TreeNode(Item,CompoentName, GetCompoentSprite(Type));

    if (IsNotActive) 
    {
        ImGuiContext& g = *GImGui;
        g.Style.Alpha = g.DisabledAlphaBackup;
    }
    if (ImGui::BeginPopupContextItem())
    {
        ImGui::Text(CompoentName);  ImGui::SameLine(); ImGui::Text("options");
        ImGui::Separator();
        if (ImGui::MenuItem("Inspect"))
        {
            auto inpswin = App->Get_Window<InspectWindow>();
            inpswin->Inspect(Inspect_Compoent2d::Get(Item));
        }
        if (ImGui::MenuItem("Destroy")) { UCode::Compoent::Destroy(Item); }
        ImGui::EndPopup();
    }
    
  
   
     
 
    if (node_open)
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();

        auto Value = UEditorModules::GetComponentData(TypeData->_Type);
        if (Value) 
        {
            Value->DrawAsTree({}, Item);
        }
        else
        {
            ImGui::Text("[Cannot Draw Compoent Propertys]");
        }

        ImGui::TreePop();
    }

    ImGui::PopID();
    
}
const char* Inspect_Compoent2d::Get_CompoentName(UCode::Compoent* Item)
{
    return Get_CompoentName(Item->Get_CompoentTypeData()->_Type);
}
const char* Inspect_Compoent2d::Get_CompoentName(const UCode::UComponentsID& Type)
{
    return Type.c_str();
}

void Inspect_Compoent2d::Insp_(InspectWindow::InspectDrawer& Draw)
{
    UCode::Compoent_Ptr& item =Draw.GetPtr().As_ptr<UCode::Compoent>();

    if (item.Has_Value()) 
    {
        Insp_(item.Get_Value(), Draw);
    }
    else
    {
        Draw.SetPtrNull(); return;
    }
}
void Inspect_Compoent2d::Insp_(UCode::Compoent* item, InspectWindow::InspectDrawer& Draw)
{
    auto TypeData = item->Get_CompoentTypeData();

    auto Value = UEditorModules::GetComponentData(TypeData->_Type);
    if (Value)
    {
        UEditorComponentDrawData DrawInfo;
        DrawInfo.Draw = &Draw;
        Value->DrawInspect(DrawInfo, item);
    }
    else
    {
        Draw.DrawText("Cannot Inspect This Compoent2d");
    }

}
void Inspect_Compoent2d::ShowAddCompoenList(UCode::Entity* item)
{
    ImVec2 Size = { 20,20 };
    struct Lib
    {
        bool IsOpen;
        String MenuName;
    };
    static std::stack<Lib> MenuData;//TODO ReMove This
    for (size_t i = 0; i < MakeableCompoents::Compoents_Size; i++)
    {
        
        auto& Item = MakeableCompoents::Compoents[i];
        if (Item.Id == MakeableCompoents::NewMenuName)
        {
            bool V = ImGui::BeginMenu(Item.Name.c_str());
            MenuData.push({ V ,Item.Name});
            continue;
        }
        else if (Item.Id == MakeableCompoents::EndMenuName)
        {
            auto& V = MenuData.top();
            if (V.IsOpen)
            {
                if (V.MenuName == CustomMenuName)
                {
                    ImGui::Separator();

                    auto ULang = UCode::ULangRunTime::Get(item->NativeGameRunTime()->Get_Library_Edit());
                    auto& Assembly = ULang->Get_Assembly();


                    for (const auto& Item : Assembly.Classes)
                    {
                        if (Item->Get_Type() == UCodeLang::ClassType::Class) 
                        {
                            if (UCompiler::IsAComponent(*Item, Assembly))
                            {
                                auto Spr = Inspect_Compoent2d::GetCompoentSprite(UCode::ULangScript::Get_TypeID());
                                ImGuIHelper::Image(Spr, Size);
                                ImGui::SameLine();
                                if (ImGui::Button(Item->FullName.size() ? Item->FullName.c_str() : "##"))
                                {
                                    auto script = item->AddCompoent<UCode::ULangScript>();
                                    script->LoadScript(Item.get());
                                }
                            }
                        }
                    }
                }


                ImGui::EndMenu();
            }
            
            continue;
        }
        if (MenuData.size() == 0 || MenuData.top().IsOpen)
        {
            auto Spr = Inspect_Compoent2d::GetCompoentSprite(Item.Id);



            ImGuIHelper::Image(Spr, Size);
            ImGui::SameLine();
            if (ImGui::Button(Item.Name.c_str()))
            {
                Item.MakeCompoent(item);
            }
        }
        
    }
}
EditorEnd


