#include "GameEditorWindow.hpp"


#include "ImGui.hpp"
#include "Imgui/imgui_internal.h"
#include <UCodeRunTime/ULibrarys/Input_Library.hpp>
#include <UCodeRunTime/Rendering/InputHelper.hpp>
#include "Editor/EditorApp.hpp"
#include <Helper/FileHelper.hpp>
#include "Helper/ImGuIHelper.hpp"
#include "EditorWindows/InspectTypes/Inspect_Entity2d.hpp"
#include <EditorWindows/OtherTypes/RawEntityData.hpp>
#include <EditorWindows/DragAndDropNames.hpp>

#include <EditorWindows/EditorStrings.hpp>
#include <ULang/UCompiler.hpp>

#include <OtherLibrarys/ImGuizmo/ImGuizmo.h>
EditorStart

EditorWindow* GameEditorWindow::MakeWin(const NewEditorWindowData& windowdata)
{
    return new GameEditorWindow(windowdata);
}
GameEditorWindow::GameEditorWindow(const NewEditorWindowData& windowdata) : EditorWindow(windowdata)
{
    WindowName = "GameEditorWindow";
    CallBegin = false;
}

GameEditorWindow::~GameEditorWindow()
{
    UnLoadRender();
    UnLoadRunTime();
    if (_SceneData) {
        delete _SceneData;
    }
}
void GameEditorWindow::UpdateWindow()
{
    ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, { 0.0f,0.0f });

    if (ImGui::Begin(Get_ImGuiName().c_str()))
    {
        SceneEditorTab(); 
       
    } ImGui::End();
   

    ImGui::PopStyleVar();



    if (ImGui::Begin("Hierarchy"))
    {
        Scenehierarchy();
        
    }ImGui::End();


    ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, { 0.0f,0.0f });

    if (ImGui::Begin("Game"))
    {
        //    UnLoadSceneCamera();
        GameTab();
       
    } ImGui::End();

    ImGui::PopStyleVar();

}

void GameEditorWindow::OnSaveWindow(USerializer& JsonToSaveIn)
{
    auto Assespath = Get_App()->Get_RunTimeProjectData()->GetAssetsDir();
    if (_SceneData && _UseingScenePath.has_value())
    {
        auto PathString = FileHelper::ToRelativePath(Assespath, _UseingScenePath.value());
       
        
        JsonToSaveIn.Write("_ScenePath",PathString); 
        
        SaveScene();
    }
   
}

void GameEditorWindow::OnLoadWindow(UDeserializer& JsonToOutof)
{
    auto Assespath = Get_App()->Get_RunTimeProjectData()->GetAssetsDir();
    Path PathString = "";
    
    JsonToOutof.ReadType("_ScenePath", PathString, PathString);
    Path NewString = Assespath.native() + Path(PathString).native();

    OpenScencAtPath(NewString);
}

void GameEditorWindow::Scenehierarchy()
{

    if (ImGui::BeginTabBar("?2")) 
    {


        if (ImGui::BeginTabItem("SceneData"))
        {
            ShowSceneData();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("RunTime GameLibrary"))
        {
            ShowRunTimeGameLibrary();
            ImGui::EndTabItem();
        }




        ImGui::EndTabBar();
    }
}


void GameEditorWindow::SetCopy(const UCode::Scene2dData::Entity_Data Entity)
{
    _HasCopy = true;
    _CopyedEntity = Entity;
}

void GameEditorWindow::SetCopy(const UCode::Entity* Entity)
{
    UCode::Scene2dData::Entity_Data v;
    UCode::Scene2dData::SaveEntityData(Entity, v, USerializerType::Fastest);
    SetCopy(v);
}

void GameEditorWindow::SceneCameraGetInputs()
{
    const float TimeFromlastFrame = Get_App()->GetGameRunTime()->Get_GameTime().UpateDelta;

    UCode::Vec2& CamPos = *(UCode::Vec2*)&_SceneCameraData._Pos.X;

    ImGuiKey Up, Down, lift, right;
    Up = ImGuiKey::ImGuiKey_W;
    Down = ImGuiKey::ImGuiKey_S;
    lift = ImGuiKey::ImGuiKey_A;
    right = ImGuiKey::ImGuiKey_D;
    float CamSpeed = 2.0f;
    float ScrollSizeSpeed = 2.5f;

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImGuiIO& io = ImGui::GetIO();

    auto M_pos = g.MouseLastValidPos;
    auto M_wheelX = io.MouseWheelH;
    auto M_wheelY = io.MouseWheel;


    bool UpV = ImGui::IsKeyDown(Up);
    bool DownV = ImGui::IsKeyDown(Down);
    bool liftV = ImGui::IsKeyDown(lift);
    bool rightV = ImGui::IsKeyDown(right);



    UCode::Vec2 CamVelocity = { 0,0 };
    if (UpV) { CamVelocity.Y += 1; }
    if (DownV) { CamVelocity.Y -= 1; }
    if (liftV) { CamVelocity.X -= 1; }
    if (rightV) { CamVelocity.X += 1; }
    float NewCamSpeed = CamSpeed * (_SceneCameraData.Orth_Size / 7) + 2;

    CamPos += CamVelocity.Get_Normalize() * NewCamSpeed * TimeFromlastFrame;
    auto NewSize = _SceneCameraData.Orth_Size - M_wheelY * ScrollSizeSpeed;

    if (NewSize > 0)
    {
        _SceneCameraData.Orth_Size = NewSize;
    }

    auto iSDraging = ImGui::IsMouseDragging(ImGuiMouseButton_::ImGuiMouseButton_Right);
    static UCode::Vec2 CamOnDrag;
    static bool SetDragPos = false;
    if (iSDraging)
    {
        if (SetDragPos)
        {
            auto Drag = ImGui::GetMouseDragDelta(ImGuiMouseButton_::ImGuiMouseButton_Right);
            auto DragAsVec2 = (*(UCode::Vec2*)&Drag);

            DragAsVec2.Y = -DragAsVec2.Y;
            float Div = 7;

            CamPos = CamOnDrag - (DragAsVec2 / Div) * (_SceneCameraData.Orth_Size / 18);
        }
        else
        {
            SetDragPos = true;
            CamOnDrag = CamPos;
        }
    }
    else
    {
        SetDragPos = false;
    }


    //

    auto& Pos = _SceneCamera->NativeEntity()->localposition();
    Pos = CamPos;



    _SceneCamera->Set_Ortho_size(_SceneCameraData.Orth_Size);
}

void GameEditorWindow::SceneEditorTab()
{
    
    SceneEditorBar();

    if (_GameRunTime == nullptr)
    {
        LoadRunTime(); LoadRender(false);
    }
    

    auto Viewportsize = ImGui::GetContentRegionAvail();
    auto p = ImGui::GetCursorPos();

    if (_WindowType != GameWindowType::EditorWindow)
    {
        UpdateAndShowGameImg();
    }
    if (_GameRender == nullptr) { LoadRender(false); }
    LoadSceneCamera();
    auto runtime = UCode::RenderRunTime2d::GetRenderRunTime(_GameRunTime.get());
    
  

    UpdateRunTimeWindowSize(Viewportsize, _SceneCamera);
    runtime->UpdateDrawData();  
    
   
    
    {
        if (SelectedObject.Has_Value())
        {
            auto Entity = SelectedObject.Get_Value();

            auto pos = Entity->worldposition();
            auto rot = Entity->worldrotation();
            auto scl = Entity->worldscale();


            UCode::Mat4 matrix;

            float* matrixfptr = (float*)&matrix;

            ImGuizmo::RecomposeMatrixFromComponents(
                &pos.X,
                &rot.X,
                &scl.X,matrixfptr);


            ImGuizmo::OPERATION mode = ImGuizmo::OPERATION::BOUNDS;

            switch (_ToolBar)
            {
            case UCodeEditor::GameEditorWindow::ToolBarType::Select:
                mode = ImGuizmo::OPERATION::BOUNDS;
                break;
            case UCodeEditor::GameEditorWindow::ToolBarType::Move:
                mode = ImGuizmo::OPERATION::TRANSLATE;
                break;
            case UCodeEditor::GameEditorWindow::ToolBarType::Scale:
                mode = ImGuizmo::OPERATION::SCALE;
                break;
            case UCodeEditor::GameEditorWindow::ToolBarType::Rotate:
                mode = ImGuizmo::OPERATION::ROTATE;
                break;
            default:
                UCodeGEUnreachable();
                break;
            }


            ImGuizmo::SetOrthographic(_SceneCamera->Get_CamType() ==UCode::Camera2d::CamType::Orthographic);
            ImGuizmo::BeginFrame();

            auto tep= ImGui::GetWindowPos();
        
            ImGuizmo::SetRect(tep.x,(- tep.x/2.5), Viewportsize.x, Viewportsize.y);

            auto vm =_SceneCamera->Get_ViewMatrix();
            auto pm =_SceneCamera->Get_ProjectionMatrix();
            
            bool updated = ImGuizmo::Manipulate(
             (float*)&vm,
             (float*)&pm
                , mode,ImGuizmo::MODE::WORLD, matrixfptr);


            if (updated)
            {
                ImGuizmo::DecomposeMatrixToComponents(matrixfptr, &pos.X, &rot.X, &scl.X);
                Entity->localposition() = pos;
                Entity->localrotation() = rot;
                Entity->localscale() = scl;
            }
        }
    }
   {
        for (auto& Item : _GameRunTime->Get_Scenes())
        {
            for (auto& Item : Item->Get_Entitys())
            {
                if (Item->GetCompent<UCode::Camera2d>())
                {
                    UCode::RenderRunTime2d::DrawQuad2dData data = { Item->localposition2d(),{1,1},{0,90} };
                    data.color.A = 0.05f;
                   // data.Spr = AppFiles::GetSprite(AppFiles::sprite::Scene2dData);
                    runtime->DrawQuad2d(data);
                }
            }
        }
    }
     _GameRender->Draw(runtime->Get_DrawData(),_SceneCamera); 
  
    size_t SceneTex = _SceneCamera->Get_Buffer().Get_TextureId();


    auto tep = ImGui::GetCursorPos();

    Vec2 V = *(Vec2*)&tep;
    ImGui::Image((ImTextureID)SceneTex, Viewportsize, { 0,0 }, { 1,-1 });
    bool IsOverImage = ImGui::IsItemHovered();
    if (IsOverImage) 
    {
        Vec2 Mpos = MousePosFromImage(V, *(Vec2*)&Viewportsize);

    }


    if (!_IsRuningGame)
    {
        _GameRunTime->DestroyNullScenes();
    }
    ImGuiIO& io = ImGui::GetIO();
    bool IsFocused = ImGui::IsItemHovered();
    if (IsFocused)
    {
        SceneCameraGetInputs();
    }
    
}
void GameEditorWindow::SceneEditorBar()
{

    if (_SceneCamera)
    {
        bool isPerspective = _SceneCamera->Get_CamType() == UCode::Camera2d::CamType::Perspective;

        if (ImGui::RadioButton("2D", !isPerspective)) _SceneCamera->Set_CamType(UCode::Camera2d::CamType::Orthographic);
        ImGui::SameLine();
        if (ImGui::RadioButton("3D", isPerspective)) _SceneCamera->Set_CamType(UCode::Camera2d::CamType::Perspective);

        ImGui::SameLine();//ImGui::Separator(); 

        if (ImGui::RadioButton("Select", _ToolBar == ToolBarType::Select))_ToolBar = ToolBarType::Select;
        ImGui::SameLine();
        if (ImGui::RadioButton("Move", _ToolBar == ToolBarType::Move))_ToolBar = ToolBarType::Move;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", _ToolBar == ToolBarType::Rotate))_ToolBar = ToolBarType::Rotate;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", _ToolBar == ToolBarType::Scale))_ToolBar = ToolBarType::Scale;


       
        ImGui::Separator();
    }
}
void GameEditorWindow::UpdateRunTimeWindowSize(ImVec2& Viewportsize, UCode::Camera2d* runtime)
{
    if (Viewportsize.x <= 0) { Viewportsize.x = 1; }
    if (Viewportsize.y <= 0) { Viewportsize.y = 1; }
    runtime->API_Set_WindowSize((int)Viewportsize.x, (int)Viewportsize.y);
}
void GameEditorWindow::UnLoadSceneCamera()
{
    if (_SceneCamera)
    {
        UCode::Entity::Destroy(_SceneCamera->NativeEntity()); 
        _SceneCamera->UnSetAsMainCam();
        _SceneCamera = nullptr;
       
    }
}
void GameEditorWindow::LoadSceneCamera()
{
    if (_SceneCamera == nullptr)
    {
        auto Entity = _GameRunTime->NewEntityOnRunTimeScene();
        _SceneCamera = Entity->AddCompoent<UCode::Camera2d>();
        _SceneCamera->UnSetAsMainCam();
    }
}
void GameEditorWindow::ShowSceneData()
{
    bool HasSceneRightPrivilege = true;//add this to stop updateing the same scene data file.
  


    


    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));

    ImGuiTableFlags flags =   ImGuiTableFlags_Resizable;
    if (_GameRunTime) 
    {
        if (ImGui::BeginTable("split", 1, flags))
        {
            auto& Scenes = _GameRunTime->Get_Scenes();
           

          
            for (size_t i = 0; i < Scenes.size(); i++)
            {
                auto& Item = Scenes[i];

                ShowScene(Item.get());
            }

            ImGui::EndTable();
        }
       
    }
   
    ImGui::PopStyleVar();
    
    
    auto Size = ImGui::GetContentRegionAvail();
    if (Size.x == 0 || Size.y == 0) { Size = { 1,1 }; }
    ImGui::InvisibleButton("Data", Size);
    DropSceneFromPath();
    if (ImGui::BeginPopupContextItem())
    {
        ImGui::Text("RunTime options");
        ImGui::Separator();
        if (ImGui::MenuItem("Add New Scene"))
        {
            auto* e = _GameRunTime->Add_NewScene();
            e->Get_Name() = UnNamedScene;
            if (_GameRunTime->Get_Scenes().size() == 1)
            {
                if (_SceneData == nullptr)
                {
                    _SceneData = new  UCode::Scene2dData();
                    
                    UCode::Scene2dData::SaveScene(e, *_SceneData,USerializerType::Fastest);
                }

                    
                _SceneDataAsRunTiime = e;
            }
        }
        ImGui::BeginDisabled(_IsRuningGame);

        if (ImGui::MenuItem("ReLoad RunTime"))
        {
            UnLoadRunTime();
            LoadRunTime();
        }

        ImGui::EndDisabled();

        ImGui::EndPopup();
    }
}
void GameEditorWindow::DropSceneFromPath()
{
    if (ImGui::BeginDragDropTarget())
    {
        ImGuiDragDropFlags target_flags = 0;
        target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DragAndDropType_Scene2dPathType, target_flags))
        {


            if (payload->IsDelivery())
            {
                const Path* DropItem = *(Path**)payload->Data;
                const Path& Path = *DropItem;
              

                OpenScencAtPath(Path);

                
            }

        }
        ImGui::EndDragDropTarget();
    }
}
void GameEditorWindow::ShowScene(UCode::RunTimeScene* Item)
{ 
    if (Item->Get_IsDestroyed()) { return; }
    
    // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
    ImGui::PushID(Item);
    // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();


    const String& SceneName = Item->Get_Name();
    auto style = ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_WindowBg);
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, style);
    bool node_open;
    bool ShowingTree = false;

    if (IsRenameing && IsSelected(Item))
    {
        node_open = WasSelectedObjectOpened;
        ImGuIHelper::DrawRenameTree(Item->Get_Name(), node_open, IsRenameing);
    }
    else
    {
        auto Data = ImGuIHelper::TreeNode(Item, SceneName.c_str(),AppFiles::sprite::Scene2dData);
        node_open = Data;
        ShowingTree = true;
    }
    ImGui::PopStyleColor();

    if (ImGui::BeginPopupContextItem())
    {
        ImGui::Text("Scene options");
        ImGui::Separator();
        if (ImGui::MenuItem("Rename"))
        {
            SetScelected(Item);
            IsRenameing = true;
            WasSelectedObjectOpened = node_open;

        }
        if (ImGui::MenuItem("Add Entity"))
        {
            UCode::Entity* e = Item->NewEntity();
            e->NativeName() = UnNamedEntity;
            e->worldposition(_SceneCameraData._Pos);
        }
        if (ImGui::MenuItem("Paste-Entity","Ctrl+V",nullptr, _HasCopy))
        {
            auto pastedentity = Item->NewEntity();
            UCode::Scene2dData::LoadEntity(pastedentity, _CopyedEntity);


            {
                UndoData undo;

                auto _CopyedEntitytep = _CopyedEntity;
                auto ePtr = pastedentity->NativeManagedPtr();
                
                undo._UndoCallBack = [_CopyedEntitytep, ePtr](UndoData& This)
                {
                    if (ePtr.Has_Value())
                    {
                        UCode::Entity::Destroy(ePtr.Get_Value());

                        auto ScenePtr = ePtr.Get_Value()->NativeScene()->Get_ManagedPtr();

                        This._RedoCallBack = [ePtr, ScenePtr, _CopyedEntitytep](UndoData& This)
                        {
                            if (ScenePtr.Has_Value())
                            {
                            UCode::Scene2dData::LoadEntity(ScenePtr.Get_Value()->NewEntity(), _CopyedEntitytep);
                            };
                        };
                    }
                };
                Get_App()->AddUndo(undo);
            }
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Destroy"))
        {
            UCode::RunTimeScene::Destroy(Item);

        }
        ImGui::EndPopup();
    }

    if (ImGui::BeginDragDropTarget())
    {
        ImGuiDragDropFlags target_flags = 0;
        target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DragAndDropType_RawEntity2dPathType, target_flags))
        {


            if (payload->IsDelivery())
            {
                const String* DropItem = *(UCode::String**)payload->Data;
                const UCode::String& Path = *DropItem;
                RawEntityData Data;
                if (RawEntityData::ReadFromFile(Path, Data))
                {
                    UCode::Entity* e = Item->NewEntity();
                    UCode::Scene2dData::LoadEntity(e, Data.Get_Value());

                }

            }

        }
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DragAndDropType_Entity2dType, target_flags))
        {
            if (payload->IsDelivery())
            {
                UCode::Entity* DropItem = *(UCode::Entity**)payload->Data;
                UCode::RunTimeScene::MoveEntity(DropItem, Item);
            }

        }
        

        bool OnDropable = ImGui::SetDragDropPayload(DragAndDropType_Scene2dType, &Item, sizeof(UCode::RunTimeScene*));
        if (OnDropable)
        {
            String Text = "Drop " + SceneName + "Here?";
            ImGui::Text(Text.c_str());
        }
        else
        {
            String Text = "Draging " + SceneName;
            ImGui::Text(Text.c_str());
        }

        ImGui::EndDragDropTarget();
    }

    if (ImGui::BeginDragDropSource())
    {

        bool OnDropable = ImGui::SetDragDropPayload(DragAndDropType_Scene2dType, &Item, sizeof(UCode::RunTimeScene*));
        if (OnDropable)
        {
            String Text = "Drop " + SceneName + " Here?";
            ImGui::Text(Text.c_str());
        }
        else
        {
            String Text = "Draging " + SceneName;
            ImGui::Text(Text.c_str());
        }

        ImGui::EndDragDropSource();
    }



    if (node_open)
    {

        auto& Enitys = Item->Get_Entitys();
        for (size_t i2 = 0; i2 < Enitys.size(); i2++)
        {
            auto& Enity = Enitys[i2];

            ShowEntityData(Enity.get());
        }

        if (ShowingTree) {
            ImGui::TreePop();
        }
    }
    ImGui::PopID();
}


void GameEditorWindow::ShowEntityData(UCode::Entity* Item)
{
    if (Item->Get_IsDestroyed()) { return; }
    
    ImGui::PushID(Item);

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();

   
 
    //
    AppFiles::sprite _SprToShow = AppFiles::sprite::Entity;
    auto& Co = Item->NativeCompoents();
   //
    auto& g = *GImGui;
       
    //
    String& EntityName =Item->NativeName();
    auto style = ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_WindowBg);
  //  ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, style);
    bool node_open;
    bool ShowingTree = false;

    bool IsSelectedEntity = IsSelected(Item);
    {
        ImGui::SetItemAllowOverlap();

 
        
        if (IsRenameing && IsSelectedEntity)
        {
            node_open = WasSelectedObjectOpened;
            ImGuIHelper::DrawRenameTree(Item->NativeName(), node_open, IsRenameing);
        }
        else
        {
            ImGuiTreeNodeFlags TreeFlags = IsSelectedEntity ? ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected : 0;
            TreeFlags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow;
            TreeFlags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_SpanAvailWidth;
            node_open = ImGuIHelper::TreeNode(Item, Item->NativeName().c_str(), AppFiles::sprite::Entity); 
            ShowingTree = true;
        }
        
        
        if (IsSelectedEntity==false)
        {
            if (ImGui::IsItemClicked())
            {
                SetScelected(Item);
                auto inpswin = Get_App()->Get_Window<InspectWindow>();
                inpswin->Inspect(Inspect_Entity2d::Get(Item));

            }    
        }
       

       
        ImGui::SameLine();

    }

 //   ImGui::PopStyleColor();

    
    if (ImGui::BeginDragDropTarget())
    {
        ImGuiDragDropFlags target_flags = 0;
        target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DragAndDropType_RawEntity2dPathType, target_flags))
        {
            if (payload->IsDelivery())
            {
                const String* DropItem = *(UCode::String**)payload->Data;
                const UCode::String& Path = *DropItem;
                RawEntityData Data;//should get from Asset DataBasce
                if (RawEntityData::ReadFromFile( Path, Data)) 
                {
                    UCode::RunTimeScene* runtime = Item->NativeScene();
                    UCode::Entity* e = runtime->NewEntity();
                    UCode::Scene2dData::LoadEntity(e, Data.Get_Value());
                    UCode::RunTimeScene::MoveEntity(e, Item, node_open);
                }

            }
        }
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DragAndDropType_Entity2dType, target_flags))
        {
            if (payload->IsDelivery())
            {
                UCode::Entity* DropItem = *(UCode::Entity**)payload->Data;
                UCode::RunTimeScene::MoveEntity(DropItem, Item, node_open);
            }

        }
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DragAndDropType_ULangFilePath, target_flags))
        {
            if (payload->IsDelivery())
            {
                const Path* DropItem = *(UCode::Path**)payload->Data;
                const UCode::Path& Str = *DropItem;
                
                auto PorjDat = Get_ProjectData();
                auto Intermediate = UCompiler::GetIntermediate(Str, PorjDat);
                bool ItWork = false;
                if (Intermediate.has_value())
                {
                    UCodeLang::UClib Lib;
                    if (UCodeLang::UClib::FromFile(&Lib, *Intermediate))
                    {
                        Vector<UCodeLang::ReflectionCustomTypeID> NodeTypeIDs;
                        for (auto& Node : Lib._Assembly.Classes)
                        {
                            if (Node->Get_Type() == UCodeLang::ClassType::Class)
                            {
                                auto& Classnode = Node->Get_ClassData();
                                NodeTypeIDs.push_back(Classnode.TypeID);
                            }
                        }

                        {
                            auto& Assembly =UCode::ULangRunTime::Get(Get_GameLib())->Get_Assembly();
                            Vector<UCodeLang::ReflectionCustomTypeID> New;
                            for (auto& Item : NodeTypeIDs)
                            {
                                auto SameTypeButIn = Assembly.Find_Node(NodeTypeIDs.front());
                                if (SameTypeButIn)
                                {
                                    if (UCompiler::IsAComponent(*SameTypeButIn, Assembly)) 
                                    {
                                        New.push_back(Item);
                                    }
                                }
                            }
                            NodeTypeIDs = std::move(New);
                        }

                        if (NodeTypeIDs.size())
                        {
                            if (NodeTypeIDs.size() == 1)
                            {
                                auto Scr = Item->AddCompoent<UCode::ULangScript>();
                                auto& Assembly = Scr->GetULang()->Get_Assembly();

                                auto SameTypeButIn = Assembly.Find_Node(NodeTypeIDs.front());
                                if (SameTypeButIn)
                                {
                                    Scr->LoadScript(SameTypeButIn);
                                }
                                else
                                {
                                    //how
                                }
                            }
                            else
                            {
                                _PickComponent = std::move(NodeTypeIDs);
                                SetScelected(Item);
                            }
                        }
                        else
                        {
                            Path RelativePath = Str.native().substr(PorjDat->GetAssetsDir().native().size());
                            UCodeGELog("Cant find Component for " + RelativePath.generic_string());
                        }
                    }
                }

                if (!ItWork)
                {
                    Path RelativePath = Str.native().substr(PorjDat->GetAssetsDir().native().size());
                    UCodeGELog("Cant find Object-file for " + RelativePath.generic_string());
                }
            }

        }


        ImGui::EndDragDropTarget();
    }
    if (ImGui::BeginDragDropSource())
    {
        bool OnDropable = ImGui::SetDragDropPayload(DragAndDropType_Entity2dType, &Item, sizeof(UCode::Entity*));
        if (OnDropable)
        {
            String Text = "Drop " + EntityName + " Here?";
            ImGui::Text(Text.c_str());
        }
        else
        {
            String Text = "Draging " + EntityName;
            ImGui::Text(Text.c_str());
        }

        ImGui::EndDragDropSource();
    }



    if (_PickComponent.has_value() && IsSelected(Item))
    {
        if (ImGui::BeginChild("PickComponent"))
        {
            ImGui::Text("Pick Component");
            auto& Assembly = UCode::ULangRunTime::Get(Get_GameLib())->Get_Assembly();

            for (auto& Item2 : _PickComponent.value())
            {
                auto Node = Assembly.Find_Node(Item2);
                if (Node)
                {
                    bool Selected =ImGui::Button(Node->FullName.c_str());

                    if (Selected)
                    {
                        auto Scr = Item->AddCompoent<UCode::ULangScript>();
                        auto& Assembly = Scr->GetULang()->Get_Assembly();

                        Scr->LoadScript(Assembly.Find_Node(Item2));

                        _PickComponent.reset();
                    }
                }
            }
            ImGui::EndChild();
        }
    }

    if (ImGui::BeginPopupContextItem("SomeThing"))
    {
        ImGui::Text("Entity2d options");
        ImGui::Separator();
        if (ImGui::MenuItem("Inspect"))
        {
            auto inpswin = Get_App()->Get_Window<InspectWindow>();

            inpswin->Inspect(Inspect_Entity2d::Get(Item));
            SetScelected(Item);
        }
        if (ImGui::MenuItem("Copy"))
        {
            SetCopy(Item);
        }


        if (ImGui::MenuItem("Rename"))
        {
            SetScelected(Item);
            IsRenameing = true;
            WasSelectedObjectOpened = node_open;
            
        }
        if (ImGui::MenuItem("Duplicate"))
        {
            UCode::Scene2dData::CloneEntity(Item);
        }
        if (ImGui::MenuItem("Destroy"))
        {
            {
                UndoData undo;
                UCode::Scene2dData::Entity_Data V;
                UCode::Scene2dData::SaveEntityData(Item, V, USerializerType::Fastest);


                auto ScencPtr = Item->NativeScene()->Get_ManagedPtr();
                undo._UndoCallBack = [ScencPtr, V](UndoData& This)
                {
                    if (ScencPtr.Has_Value())
                    {
                        auto newentity = ScencPtr.Get_Value()->NewEntity()->NativeManagedPtr();
                        UCode::Scene2dData::LoadEntity(newentity.Get_Value(), V);


                        This._RedoCallBack = [newentity](UndoData& This)
                        {
                            if (newentity.Has_Value())
                            {
                                UCode::Entity::Destroy(newentity.Get_Value());
                            }
                        };
                    }
                };
                Get_App()->AddUndo(undo);
            }
            UCode::Entity::Destroy(Item);

        }

        if (ImGui::MenuItem("Set Scenc Cam To "))
        {
            _SceneCameraData._Pos = Item->worldposition();
        }



        if (ImGui::BeginMenu("Add Compoent"))
        {
            Inspect_Compoent2d::ShowAddCompoenList(Item);
            ImGui::EndMenu();
        }

        if (ImGui::MenuItem("Add Entity"))
        {
            auto E = Item->NativeAddEntity();
            E->NativeName() = UnNamedEntity;
            SetScelected(E);
        }

        ImGui::EndPopup();
    }
   

    if (node_open)
    {
        

        {
            auto& Entitys =Item->NativeGetEntitys();
            for (size_t i = 0; i < Entitys.size(); i++)
            {
                auto& Item2 = Entitys[i];
                if (Item2->Get_IsDestroyed()) { continue; }
                ShowEntityData(Item2.get());
            }
        }
        
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();

        {
            auto& Compoents = Item->NativeCompoents();

            for (size_t i = 0; i < Compoents.size(); i++)
            {
                auto& Item2 = Compoents[i];
                if (Item2->Get_IsDestroyed()) { continue; }

                Inspect_Compoent2d::DrawCompoentButtion(Item2.get(), Get_App());

            }
        }

        if (ShowingTree) {
            ImGui::TreePop();
        }
    }
   
    ImGui::PopID();
}
void GameEditorWindow::ShowlibraryBookData(UCode::System* Item)
{
    // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
    ImGui::PushID(Item);

    // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();

    String EnityName = "[libraryBook Name here]";
    bool node_open = ImGui::TreeNode("Object", "%s", EnityName.c_str());



    if (node_open)
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();

        if (ImGui::Button("Show In inspector"))
        {

        }

        ImGui::TreePop();
    }
    ImGui::PopID();
}
void GameEditorWindow::ShowRunTimeGameLibrary()
{
    ImGui::Text("Add Stuff here");

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));

    ImGuiTableFlags flags = ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable;
    auto Lib = Get_GameLib();
    auto& Books = Lib->Get_Books();

    if (ImGui::BeginTable("RunTimeGameLibrary", 1, flags))
    {
        for (size_t i = 0; i < Books.size(); i++)
        {
            auto& Item = Books[i];
            ShowlibraryBookData(Item.get());
        }

        ImGui::EndTable();
    }

    ImGui::PopStyleVar();
}
void GameEditorWindow::SaveScene()
{
    if (_SceneData) {
        auto SaveType = Get_ProjectData()->Get_ProjData()._SerializeType;
        UCode::Scene2dData::SaveScene(_SceneDataAsRunTiime, *_SceneData, SaveType);
        UCode::Scene2dData::ToFile(_UseingScenePath.value(), *_SceneData, SaveType);
    }
}
void GameEditorWindow::OpenScencAtPath(const Path&  Path)
{
    LoadRunTime();

    auto& Scenes = _GameRunTime->Get_Scenes();
    auto NewSceneData =std::make_unique<UCode::Scene2dData>();//Get from cash;
    UCode::AssetManager* Assets = UCode::AssetManager::Get(Get_GameLib());
    if (UCode::Scene2dData::FromFile(*NewSceneData, Path))
    {
        auto NewScene = UCode::Scene2dData::LoadScene(_GameRunTime.get(), *NewSceneData);
       // UCode::RunTimeScene::SetSceneMulti_Threaded(NewScene);
        if (Scenes.size() == 1)
        {
            if (_SceneData)
            {
                delete _SceneData;
                _SceneData = nullptr;
            }
            if (_SceneData == nullptr)
            {
                _SceneData = new UCode::Scene2dData();
            }

            _SceneData = NewSceneData.release();

            _UseingScenePath = Path;
            _SceneDataAsRunTiime = NewScene;
        }
       
    }
}
Vec2 GameEditorWindow::MousePosFromImage(const Vec2 CursorPos, const Vec2 ImageSize)
{
    auto tep =ImGui::GetMousePos();
    Vec2 Mpos = *(Vec2*)&tep;

    auto tep2 =ImGui::GetMousePos();
    auto  CursorPosScreen = CursorPos + *(Vec2*)&tep2;
    auto A = Mpos - CursorPos;
    
    //std::cout << Mpos.X << "," << Mpos.Y << ":" << CursorPosScreen.X << "," << CursorPosScreen.Y << '\n';
    //std::cout << A.X << "," << A.Y << '\n';
    return A;
}
void GameEditorWindow::GameTab()
{
    if (_GameRunTime == nullptr)
    {
        LoadRunTime();
        LoadRender(_WindowType == GameWindowType::ExternalWindow);
    }

    const char* WindowTypeText;
    switch (_WindowType)
    {
    case GameWindowType::EditorWindow:WindowTypeText = "EditorWindow"; break;
    case GameWindowType::ExternalWindow:WindowTypeText = "ExternalWindow"; break;
    default:UCodeLangUnreachable(); break;
    }

    

    bool PlayButtionDown = false;
    bool PausedButtionDown = false;
    bool WindowTypeButtionDown = false;
    auto winSize = ImGui::GetWindowSize();
    //ImGui::SetCursorPos({ (winSize.x) * 0.5f,ImGui::GetCursorPosY() });

    const ImVec2 ButtionSize = { 15,15 };
    PlayButtionDown = ImGuIHelper::ImageButton(&PlayButtionDown,
        _IsRuningGame ? AppFiles::sprite::Play_Blue : AppFiles::sprite::Play_Green,ButtionSize);
    
    ImGui::SameLine();
    
    PausedButtionDown = ImGuIHelper::ImageButton(&PausedButtionDown,
        _IsGamePaused ? AppFiles::sprite::Paused_Blue : AppFiles::sprite::Paused_Green, ButtionSize);
    
    float Sizey = ImGui::GetItemRectSize().y;
    ImGui::SameLine();
    
    WindowTypeButtionDown = ImGui::Button(WindowTypeText, {0,Sizey });

    ImGui::Separator();
   
    const char* CantPlayScenePopName = "Some Popup";

    if (WindowTypeButtionDown)
    {
       
        switch (_WindowType)
        {
        case GameWindowType::EditorWindow:_WindowType = GameWindowType::ExternalWindow; break;
        case GameWindowType::ExternalWindow:_WindowType = GameWindowType::EditorWindow; break;
        default:break;
        }


        if (_IsRuningGame)
        {
            UnLoadRender();
            LoadRender(_WindowType == GameWindowType::ExternalWindow);
        }
    }
    if (PausedButtionDown) { _IsGamePaused = !_IsGamePaused; }
    if (PlayButtionDown)
    {
        _IsRuningGame = !_IsRuningGame;

        if (_IsRuningGame)
        {
            if (!_UseingScenePath.has_value())
            {
                
                ImGui::OpenPopup(CantPlayScenePopName);

               

                _IsRuningGame = false;
            }
            else
            {
                OnPlayScene();
            }
        }
        else
        {
            OnStopPlaying();
        }
        
    } 
    
    if (ImGui::BeginPopup(CantPlayScenePopName))
    {
        ImGui::Text("Cant Play Scene Try Saveing the Scene To A File. then Opening it");
        ImGui::EndPopup();
    }

    UpdateAndShowGameImg();
    if (_WindowType == GameWindowType::ExternalWindow)
    {
        if (_IsRuningGame) {
            ImGui::Text("(Runing ExternalWindow) Open the Window Called ExternalWindow");
        }
        else
        {
            ImGui::Text("Press play for ExternalWindow To Open");
        }
    }
}

void GameEditorWindow::OnPlayScene()
{
    SaveScene();  
    
    UnLoadRender();
    LoadRender(_WindowType == GameWindowType::ExternalWindow);
}
void GameEditorWindow::OnStopPlaying()
{
    UnLoadRunTime(); LoadRunTime();
    _IsRuningGame = false;

    if (_SceneData) 
    {
        auto ShowScene = UCode::Scene2dData::LoadScene(_GameRunTime.get(), *_SceneData);
        _SceneDataAsRunTiime = ShowScene;
    }
    LoadRender(false);
}
void GameEditorWindow::UpdateAndShowGameImg()
{
    if (_GameRunTime && _GameRender)
    {
        UpdateGame();
    }
    ShowGameImage();
}

EditorWindowData GameEditorWindow::GetEditorData()
{
    return EditorWindowData("GameEditorWindow", MakeWin);
}
void GameEditorWindow::ShowGameImage()
{
    UCode::RenderRunTime2d* runtime = _GameRunTime ? UCode::RenderRunTime2d::GetRenderRunTime(_GameRunTime.get()) : nullptr;
   
    if (_WindowType == GameWindowType::EditorWindow)
    {
        LoadRender(false);
    }

    if (_WindowType != GameWindowType::ExternalWindow)
    { 
        ImVec2 ViewportPOs = ImGui::GetCursorPos();
        ImVec2 Viewportsize = ImGui::GetContentRegionAvail();

        ImgeVecPos = ViewportPOs;
        ImgeVecSize = Viewportsize;
        ImGuiFocusedFlags flags = ImGuiFocusedFlags_::ImGuiFocusedFlags_None;
        flags &= ImGuiFocusedFlags_::ImGuiFocusedFlags_AnyWindow;

        ImGuiIO& io = ImGui::GetIO();
        _IsGameWindowFocused = io.WantCaptureMouse;

        size_t textid;
        if (UCode::Camera2d* MainCam = UCode::Camera2d::FindMainCam(_GameRunTime.get()))
        {
            UpdateRunTimeWindowSize(Viewportsize, MainCam);
            textid = MainCam->Get_Buffer().Get_TextureId();
        }
        else
        {
            textid = 0;
        }

        ImGui::Image((ImTextureID)textid, Viewportsize, { 0,0 }, { 1,-1 });
    }
   
    
}
void GameEditorWindow::UpdateGame()
{

    ImVec2 EditorWindowsize = { _Size.X , _Size.Y };
    if (EditorWindowsize.x <= 0 && EditorWindowsize.y <= 0) { return; }
   
    
    if (_IsRuningGame && !_IsGamePaused) { _GameRunTime->GameUpdate(); }




    if (_GameRunTime)
    {
        _GameRunTime->DestroyNullScenes();
        _GameRender->UpdateFrame();

        InputEmuation();
     
        if (_IsRuningGame && !_GameRunTime->Get_IsGameRuning())
        {
            UnLoadRender();
            OnStopPlaying();
        }
    }

}
void GameEditorWindow::UnLoadRender()
{
    if (_GameRender)
    {
        _GameRender->EndRender();
        delete _GameRender.release();
        _GameRender = nullptr;
    }
}
void GameEditorWindow::UnLoadRunTime()
{
    if (_GameRunTime)
    {
        UnLoadSceneCamera();
        UnLoadRender();
     
        _GameRunTime->EndRunTime();
        delete _GameRunTime.release();
        _GameRunTime = nullptr;
    }
}
void GameEditorWindow::LoadRunTime()
{
   
    if (_GameRunTime == nullptr) 
    {
        _GameRunTime =std::make_unique<UCode::GameRunTime>(Get_GameLibRef());
        _GameRunTime->Set_GameTime_FramesToDestroy(2);//Because The Inspector be able to deselect RunTimeObjects instead of reading freed memory.
        _GameRunTime->Init();

        if (_SceneData && _SceneDataAsRunTiime == nullptr)
        {
            auto ShowScene = UCode::Scene2dData::LoadScene(_GameRunTime.get(), *_SceneData);
            _SceneDataAsRunTiime = ShowScene;
        }
    }
}
void GameEditorWindow::LoadRender(bool MakeWin)
{
    if (_GameRender == nullptr)
    {
        i32 Sx = (i32)_Size.X;
        i32 Sy = (i32)_Size.Y;
        if (Sx <= 0) { Sx = 640;}
        if (Sy <= 0) { Sy = 480; }

        _GameRender = std::make_unique <UCode::RenderAPI::Render>();
        auto Windata = UCode::RenderAPI::WindowData(Sx, Sy, "GameEditorWindow(ExternalWindow)");

        Windata.ImGui_Init = false;
        Windata.GenNewWindow = MakeWin;
        Windata.ImguiDrawIsMade = true;
        Windata.shared_window = glfwGetCurrentContext();

        _GameRender->Init(Windata, _GameRunTime.get());

       // auto& Tex = *AppFiles::GetTexture(AppFiles::texture::AppIcon);//TODO add call backs
       // while (Tex.) {}//wait for texture
       // _GameRender->Get_RenderAPI()->SetWindowIcon(Tex);
    }
}
void SetUCodeInput(ImGuiKey Item, UCode::InputManger* _Input)
{
    auto Ukey = UCode::RenderAPI::ImguiKeyToUCodeKey(Item);
    if (Ukey != UCode::InputKey::Null)
    {
        bool V = ImGui::IsKeyDown(Item);
        UCode::CallBackType C;

        if (V)
        {
            C = UCode::CallBackType::Press;
        }
        else
        {
            C = UCode::CallBackType::Release;
        }

        _Input->Input_API_SetInput(V, Ukey);
        _Input->Input_API_CallInputCallBack(Ukey, C);
    }
}
void GameEditorWindow::InputEmuation()
{
    if (_WindowType != GameWindowType::ExternalWindow)
    {
       
        if (_IsGameWindowFocused)
        {
            auto RunTime = _GameRunTime.get();
            auto Render = _GameRender.get();



            ImGuiContext& g = *GImGui;
            ImGuiWindow* window = g.CurrentWindow;
            ImGuiIO& io = ImGui::GetIO();

            Vec2 M_pos = MousePosFromImage(*(Vec2*)&ImgeVecPos, *(Vec2*)&ImgeVecSize);
            auto M_wheelX = io.MouseWheelH;
            auto M_wheelY = io.MouseWheel;

            auto _Input = UCode::InputManger::GetInput(RunTime);
            _Input->Input_API_Set_CursorPosition(M_pos.X, M_pos.Y);
            _Input->Input_API_Set_Scroll(M_wheelX, M_wheelY);
            //add mouse input

            for (size_t i = ImGuiKey::ImGuiKey_MouseLeft; i < (size_t)ImGuiKey::ImGuiKey_MouseRight; i++)
            {
                auto Item = (ImGuiKey)i;
                SetUCodeInput(Item, _Input);
            }


            for (size_t i = ImGuiKey::ImGuiKey_NamedKey_BEGIN; i < (size_t)ImGuiKey::ImGuiKey_NamedKey_END; i++)
            {
                auto Item = (ImGuiKey)i;
                SetUCodeInput(Item, _Input);

            }
        }
    }
}
EditorEnd


