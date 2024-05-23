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
#include "UEditorModules/Modules/CodeModule.hpp"
#include <Imgui/misc/cpp/imgui_stdlib.h>
#include "Helper/UserSettings.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/EntityPlaceHolder.hpp"
#include "EditorWindows/ProjectManagement/ProjectFilesWindow.hpp"
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

    auto isprefab = item->GetCompent<UCode::EntityPlaceHolder>().value_unchecked();
    {


        const float square_sz = ImGui::GetFrameHeight();
        ImGuIHelper::Image(isprefab ? AppFiles::sprite::RawEntityData : AppFiles::sprite::Entity , { square_sz ,square_sz });
        ImGui::SameLine();

        ImGuIHelper::ItemLabel(StringView("Name"), ImGuIHelper::ItemLabelFlag::Left);

        ImGui::PushItemWidth(ImGui::CalcItemWidth() - (ImGuIHelper::CheckBoxSizeWithPadding().x));
        ImGuIHelper::InputText(item->NativeName());
        ImGui::PopItemWidth();

        ImGui::SameLine();


        bool V = item->GetActive();
        ImGui::PushID(&V);
        ImGui::Checkbox("", &V);
        ImGui::PopID();
		item->SetActive(V);
    }
   
    bool Is2D = true;
    auto Cam = UCode::Camera2d::FindMainCam(item->NativeGameRunTime());
    if (Cam)
    {
        Is2D = Cam->Get_CamType() == UCode::Camera2d::CamType::Orthographic;
    }
    if (!item->NativeParent())
    {

        if (isprefab)
        {
            ImGuIHelper::ItemLabel("Prefab", ImGuIHelper::ItemLabelFlag::Left);
            UID id = isprefab->_id;


            auto buttioncount = 3;

            auto h = ImGui::GetFrameHeight();
            auto fieldwidthleft = ImGui::GetCurrentWindow()->Size.x - ImGui::GetCursorPosX();
            auto w = (fieldwidthleft / buttioncount) - ImGui::GetStyle().ItemSpacing.x;


            if (ImGui::Button("Open",{w,h}))
            {

            }
            ImGui::SameLine();
            if (ImGui::Button("Select", { w,h }))
            {
                auto files = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_Window<ProjectFilesWindow>();
                files->OpenAndFocusOnAsset(id);
            }
            
            ImGui::SameLine();

            bool wasupdated = true;
            
            if (wasupdated) 
            {
                auto type = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData()->Get_ProjData()._SerializeType;
                isprefab->UpdateChanges(type);
            }

            const char* overidepopupname = "overridepopup";
            bool haschanges = isprefab->_change.HasChanges();


            if (ImGui::Button(haschanges ? "Overides*" : "Overrides", { w,h }))
            {
                 ImGui::OpenPopup(overidepopupname);
            }

            ImGui::SetNextWindowSize({ 325,175 });
            if (ImGui::BeginPopup(overidepopupname))
            {
                OnOverridePopup(haschanges, overidepopupname, item, isprefab);



                ImGui::EndPopup();
            }
            
        }
    
        if (Is2D)
        {
            ImGuIHelper::Vec2Field("Position", *(Vec2*)&item->NativeLocalPosition());
            ImGuIHelper::Vec2Field("Rotation", *(Vec2*)&item->NativeLocalRotation());
            ImGuIHelper::Vec2Field("Scale", *(Vec2*)&item->NativeLocalScale());
        }
        else 
        {
            ImGuIHelper::Vec3Field("Position", item->NativeLocalPosition());
            ImGuIHelper::Vec3Field("Rotation", item->NativeLocalRotation());
            ImGuIHelper::Vec3Field("Scale", item->NativeLocalScale());
        }
    }
    else
    {
        if (Is2D)
        {
            /*
            Draw.Vec2Field("Local Position", item->localposition2d());
            Draw.Vec2Field("Local Rotation", item->localrotation2d());
            Draw.Vec2Field("Local Scale", item->localscale2d());
            */

            //ImGui::Separator();


            Vec2 Tep = item->WorldPosition2D();
            Vec2 Tep1 = item->WorldPosition2D();
            Vec2 Tep2 = item->WorldPosition2D();

            if (ImGuIHelper::Vec2Field("Position", Tep))
            {
                item->WorldPosition2D(Tep);
            }

            Tep1 = item->WorldRotation2D();

            if (ImGuIHelper::Vec2Field("Rotation", Tep1))
            {
                item->WorldRotation(Tep1);
            }

            Tep2 = item->WorldScale2D();
            if (ImGuIHelper::Vec2Field("Scale", Tep2))
            {
                item->WorldScale(Tep2);
            }
        }
        else
        {
            Vec3 Tep = item->WorldPosition();
            Vec3 Tep1 = item->WorldRotation();
            Vec3 Tep2 = item->WorldScale();

            if (ImGuIHelper::Vec3Field("Position", Tep))
            {
                item->WorldPosition(Tep);
            }

            Tep = item->WorldRotation();

            if (ImGuIHelper::Vec3Field("Rotation", Tep1))
            {
                item->WorldRotation(Tep);
            }

            Tep2 = item->WorldScale();

            if (ImGuIHelper::Vec3Field("Scale", Tep2))
            {
                item->WorldScale(Tep2);
            }
        }
    }
    ImGui::Separator();

    for (auto& Item : item->NativeCompoents())
    {
        if (Item->Get_CompoentTypeData() == &UC::EntityPlaceHolder::type_Data)
        {
            continue;
        }
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

void Inspect_Entity2d::OnOverridePopup(bool haschanges, const char* overidepopupname, UCode::Entity* item, UCode::EntityPlaceHolder* isprefab)
{
    ImGuIHelper::Text(StringView("Overides"));

    ImGui::BeginDisabled(!haschanges);
    //if (ImGuIHelper::TreeNode(overidepopupname, StringView(item->NativeName()), AppFiles::sprite::Entity))
    {
        auto type = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData()->Get_ProjData()._SerializeType;
        auto& changes = isprefab->_change;

        Vector<UC::Entity*> EntitysToShows;
        Vector<UC::Compoent*> CompoentsToShow;
        Unordered_map<UC::Entity*, UC::EntityPlaceHolderChanges::PlaceHolderChangeProps> ChangeType;
        Unordered_map<UC::Compoent*, UC::EntityPlaceHolderChanges::PlaceHolderChangeProps> ChangeTypeForCompoent;
        for (auto& change : changes._changes)
        {
            auto e = isprefab->GetEntity(type, change);

            if (e.has_value())
            {
                auto eval = e.value().entity;
                bool hasentity = false;
                for (auto& Item : EntitysToShows)
                {
                    if (Item == eval)
                    {
                        hasentity = true;
                        break;
                    }
                }
                if (!hasentity) {
                    ChangeType.AddValue(e.value().entity, e.value().props);
                    EntitysToShows.push_back(eval);
                }
            }

            auto c = isprefab->GetCompoent(type, change);

            if (c.has_value())
            {
                auto cval = c.value().compoent;
                bool hascompent = false;
                for (auto& Item : CompoentsToShow)
                {
                    if (Item == cval)
                    {
                        hascompent = true;
                        break;
                    }
                }

                if (!hascompent)
                {
                    ChangeTypeForCompoent.AddValue(cval, c.value().props);
                    CompoentsToShow.push_back(cval);
                }

                bool hasentity = false;
                for (auto& Item : EntitysToShows)
                {
                    if (Item == cval->NativeEntity())
                    {
                        hasentity = true;
                        break;
                    }
                }
                if (!hasentity)
                {
                    EntitysToShows.push_back(cval->NativeEntity());
                }
            }
        }

        //get root entitys
        {
            auto copy = EntitysToShows;
            for (auto& Item : copy)
            {
                auto p = Item->NativeParent();
                while (p)
                {
                    bool hasentity = false;
                    for (auto& Item : EntitysToShows)
                    {
                        if (Item == p)
                        {
                            hasentity = true;
                            break;
                        }
                    }
                    if (!hasentity)
                    {
                        EntitysToShows.push_back(p);
                    }

                    p = p->NativeParent();
                }
            }
        }

        std::function<void(UC::Entity*)> OnEveryEntity;

        UserSettings& settings = UserSettings::GetSettings();
        OnEveryEntity = [&](UC::Entity* e)
            {
                bool show = false;
                {
                    for (auto& Item : EntitysToShows)
                    {
                        if (Item == e)
                        {
                            show = true;
                            break;
                        }
                    }
                }
                if (show)
                {
                    if (ChangeType.HasValue(e))
                    {
                        auto& type = ChangeType.GetValue(e);
                        if (type == UC::EntityPlaceHolderChanges::PlaceHolderChangeProps::AddEntity)
                        {
                            ImGuIHelper::Text(StringView("+")); ImGui::SameLine();
                            auto& g = *GImGui;
                            auto SizeY = g.FontSize + g.Style.FramePadding.y * 2;
                            ImGuIHelper::Image(AppFiles::sprite::Entity, ImVec2(SizeY, SizeY));

                            ImGui::SameLine();
                            ImGui::Selectable(e->NativeName().c_str());

                            bool Focus = false;
                            bool Undo = false;
                            if (ImGui::IsItemFocused())
                            {
                                if (settings.IsKeybindActive(KeyBindList::Inspect))
                                {
                                    Focus = true;
                                }


                                if (settings.IsKeybindActive(KeyBindList::Delete))
                                {
                                    Undo = true;
                                }


                            }
                            ImGui::PushID(e);
                            if (ImGuIHelper::BeginPopupContextItem("addedtype"))
                            {
                                auto str = settings.KeyBinds[(size_t)KeyBindList::Inspect].ToString();
                                if (ImGui::MenuItem("Focus", str.c_str()) || settings.IsKeybindActive(KeyBindList::Inspect))
                                {
                                    Focus = true;
                                    ImGui::CloseCurrentPopup();
                                }

                                str = settings.KeyBinds[(size_t)KeyBindList::Delete].ToString();
                                if (ImGui::MenuItem("Undo Change", str.c_str()) || settings.IsKeybindActive(KeyBindList::Delete))
                                {
                                    Undo = true;
                                    ImGui::CloseCurrentPopup();
                                }

                                ImGui::EndPopup();
                            }
                            ImGui::PopID();
                        }
                        else
                        {
                            UCodeGEUnreachable();
                        }
                    }
                    else
                    {
                        if (ImGuIHelper::TreeNode(e, StringView(e->NativeName()), AppFiles::sprite::Entity))
                        {
                            for (auto& e : e->NativeGetEntitys())
                            {
                                OnEveryEntity(e.get());
                            }
                            auto& Compoents = e->NativeCompoents();


                            for (size_t i = 0; i < Compoents.size(); i++)
                            {
                                auto Item2 = Compoents[i].get();
                                bool show = false;
                                {
                                    for (auto& Item : CompoentsToShow)
                                    {
                                        if (Item == Item2)
                                        {
                                            show = true;
                                            break;
                                        }
                                    }
                                }

                                if (show)
                                {
                                    if (ChangeTypeForCompoent.HasValue(Item2))
                                    {
                                        auto& type = ChangeTypeForCompoent.GetValue(Item2);

                                        if (type == UC::EntityPlaceHolderChanges::PlaceHolderChangeProps::AddCompent)
                                        {
                                            String CompoentName = Item2->Get_CompoentTypeData()->_Type;
                                            auto v = Inspect_Compoent2d::GetCompoentSprite(Item2->Get_CompoentTypeData()->_Type);


                                            ImGuIHelper::Text(StringView("+")); ImGui::SameLine();
                                            auto& g = *GImGui;
                                            auto SizeY = g.FontSize + g.Style.FramePadding.y * 2;
                                            ImGuIHelper::Image(v, ImVec2(SizeY, SizeY));

                                            ImGui::SameLine();
                                            ImGui::Selectable(CompoentName.c_str());

                                            bool Focus = false;
                                            bool Undo = false;
                                            if (ImGui::IsItemFocused())
                                            {
                                                if (settings.IsKeybindActive(KeyBindList::Inspect))
                                                {
                                                    Focus = true;
                                                }


                                                if (settings.IsKeybindActive(KeyBindList::Delete))
                                                {
                                                    Undo = true;
                                                }


                                            }

                                            ImGui::PushID(Item2);
                                            if (ImGuIHelper::BeginPopupContextItem("addedtype"))
                                            {
                                                auto str = settings.KeyBinds[(size_t)KeyBindList::Inspect].ToString();
                                                if (ImGui::MenuItem("Focus", str.c_str()) || settings.IsKeybindActive(KeyBindList::Inspect))
                                                {
                                                    Focus = true;
                                                    ImGui::CloseCurrentPopup();
                                                }

                                                str = settings.KeyBinds[(size_t)KeyBindList::Delete].ToString();
                                                if (ImGui::MenuItem("Undo Change", str.c_str()) || settings.IsKeybindActive(KeyBindList::Delete))
                                                {
                                                    Undo = true;
                                                    ImGui::CloseCurrentPopup();
                                                }

                                                ImGui::EndPopup();
                                            }
                                            ImGui::PopID();
                                        }
                                    }
                                    else
                                    {

                                    }
                                }
                            }
                            ImGui::TreePop();
                        }
                    }
                }
            };

        OnEveryEntity(item);
        //ImGui::TreePop();
    }
    ImGui::EndDisabled();

    ImGui::Separator();
    auto h = ImGui::GetFrameHeight();
    auto fieldwidthleft = ImGui::GetCurrentWindow()->Size.x - ImGui::GetCursorPosX();

    auto buttionw = (fieldwidthleft / 2) - ImGui::GetStyle().ItemSpacing.x * 2;

    ImGui::BeginDisabled(!haschanges);
    if (ImGui::Button("Revert", { buttionw,h }))
    {
        isprefab->RemoveChanges();
        ImGui::CloseCurrentPopup();
    }
    ImGui::EndDisabled();

    ImGui::SameLine();
    if (ImGui::Button("Unpack Prefab", { buttionw,h }))
    {
        UC::Compoent::Destroy(isprefab);
        ImGui::CloseCurrentPopup();
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
    

    
    auto& g = *GImGui;
	auto SizeY = g.FontSize + g.Style.FramePadding.y * 2;
	ImGuIHelper::Image(GetCompoentSprite(Type), ImVec2(SizeY, SizeY)); ImGui::SameLine();
   
    
    bool node_open = ImGui::TreeNode(Item, CompoentName);
    if (ImGui::IsItemFocused() && !ImGui::GetIO().WantTextInput)
    {
        auto& settings = UserSettings::GetSettings();


        if (settings.IsKeybindActive(KeyBindList::Inspect))
        {
            auto inpswin = App->Get_Window<InspectWindow>();
            inpswin->Inspect(Inspect_Compoent2d::Get(Item));
        }
        if (settings.IsKeybindActive(KeyBindList::Copy))
        {
            UCode::Scene2dData::Compoent_Data v;
            UCode::Scene2dData::SaveCompoentData(Item, v, USerializerType::YAML);

            USerializer V(USerializerType::YAML);
            V.Write("UData", v);
            V.Write("UType", "Compoent");

            auto copytext = V.Get_TextMaker().c_str();

            ImGui::SetClipboardText(copytext); 
        }
        if (settings.IsKeybindActive(KeyBindList::Paste))
        {
 
        }
        if (settings.IsKeybindActive(KeyBindList::Delete))
        {
            UCode::Compoent::Destroy(Item); 
        }
    }

    if (IsNotActive) 
    {
        ImGuiContext& g = *GImGui;
        g.Style.Alpha = g.DisabledAlphaBackup;
    }
    if (ImGuIHelper::BeginPopupContextItem())
    {
        ImGuIHelper::Text(StringView(CompoentName));  ImGui::SameLine(); ImGuIHelper::Text(StringView("Options"));
        ImGui::Separator();

        auto& settings = UserSettings::GetSettings();
        auto keybindstring = settings.KeyBinds[(size_t)KeyBindList::Inspect].ToString();

        if (ImGui::MenuItem("Inspect", keybindstring.c_str()) || settings.IsKeybindActive(KeyBindList::Inspect))
        {
            auto inpswin = App->Get_Window<InspectWindow>();
            inpswin->Inspect(Inspect_Compoent2d::Get(Item));
        }


        keybindstring = settings.KeyBinds[(size_t)KeyBindList::Copy].ToString();
        if (ImGui::MenuItem("Copy", keybindstring.c_str()) || settings.IsKeybindActive(KeyBindList::Copy))
        {
            UCode::Scene2dData::Compoent_Data v;
            UCode::Scene2dData::SaveCompoentData(Item, v, USerializerType::YAML);

            USerializer V(USerializerType::YAML);
            V.Write("UData", v);
            V.Write("UType", "Compoent");

            auto copytext = V.Get_TextMaker().c_str();

            ImGui::SetClipboardText(copytext);
        }


        keybindstring = settings.KeyBinds[(size_t)KeyBindList::Paste].ToString();
        if (ImGui::MenuItem("Paste", keybindstring.c_str()) || settings.IsKeybindActive(KeyBindList::Paste))
        {

        }


        keybindstring = settings.KeyBinds[(size_t)KeyBindList::Delete].ToString();
        if (ImGui::MenuItem("Destroy", keybindstring.c_str()) || settings.IsKeybindActive(KeyBindList::Inspect)) 
        {
            UCode::Compoent::Destroy(Item); 
        }
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
            ImGuIHelper::Text(StringView("[Cannot Draw Compoent Propertys]"));
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
    
    {
        auto Value = UEditorModules::GetComponentData(TypeData->_Type);
        if (Value)
        {
            UEditorComponentDrawData DrawInfo;
            DrawInfo.Draw = &Draw;
            Value->DrawInspect(DrawInfo, item);
        }
        else
        {
            ImGuIHelper::Text(StringView("Cannot Inspect This Compoent"));
        }

    }
}
bool Inspect_Compoent2d::ShowAddCompoenList(UCode::Entity* item)
{
    auto imageh = ImGui::GetFrameHeight();
    ImVec2 Size = { imageh,imageh };
    struct Lib
    {
        bool IsOpen;
        String MenuName;
    };
    static std::stack<Lib> MenuData;//TODO ReMove This
    bool  addconpoent = false;
    for (size_t i = 0; i < MakeableCompoents::Compoents_Size; i++)
    {

        auto& Item = MakeableCompoents::Compoents[i];
        if (Item.Id == MakeableCompoents::NewMenuName)
        {
            bool V = ImGui::BeginMenu(Item.Name.c_str());
            MenuData.push({ V ,Item.Name });
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

                    const Vector<const UCodeLang::AssemblyNode*>& vaildcompoents = CodeModule::GetAllVaildCompoents();

                    for (const auto& Item : vaildcompoents)
                    {
                        if (Item->Get_Type() == UCodeLang::ClassType::Class)
                        {
                            auto Spr = Inspect_Compoent2d::GetCompoentSprite(UCode::ULangScript::Get_TypeID());
                            ImGuIHelper::Image(Spr, Size);
                            ImGui::SameLine();
                            if (ImGui::Button(Item->FullName.size() ? Item->FullName.c_str() : "##"))
                            {
                                auto script = item->AddCompoent<UCode::ULangScript>();
                                script->LoadScript(Item);
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
            if (ImGui::MenuItem(Item.Name.c_str()))
            {
                Item.MakeCompoent(item);
                addconpoent = true;
            }
        }

    }
    return addconpoent;
}
EditorEnd


