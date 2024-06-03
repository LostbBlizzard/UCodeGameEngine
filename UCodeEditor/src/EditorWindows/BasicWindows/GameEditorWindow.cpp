#include "GameEditorWindow.hpp"


#include "ImGui.hpp"
#include "Imgui/imgui_internal.h"
#include <UCodeRunTime/ULibrarys/Input_Library.hpp>
#include <UCodeRunTime/Rendering/InputHelper.hpp>
#include <Helper/FileHelper.hpp>
#include "Helper/ImGuIHelper.hpp"
#include "EditorWindows/InspectTypes/Inspect_Entity2d.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/EntityPlaceHolder.hpp"
#include <EditorWindows/DragAndDropNames.hpp>

#include <EditorWindows/EditorStrings.hpp>
#include <ULang/UCompiler.hpp>

#include <OtherLibrarys/ImGuizmo/ImGuizmo.h>
#include "Helper/UserSettings.hpp"
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
    UnLoadRender(MainSceneData);
    UnLoadRunTime();
    if (_SceneData) {
        delete _SceneData;
    }


    if (_DontWaitInputKey.has_value())
    {
        Get_App()->RemoveWaitForInput(_DontWaitInputKey.value());
        _DontWaitInputKey.reset();
    } 
    if (BlockKey.has_value())
    {
        Get_App()->RemoveBlockClose(BlockKey.value());
        BlockKey = {};
    }
}
void GameEditorWindow::UpdateWindow()
{
    ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, { 0.0f,0.0f });


    bool editorwindow = false;
    bool gamewindow = false;
    bool hierarchuwindow = false;
    if (Get_App()->GetInputMode() == KeyInputMode::Window)
    {
        auto& settings = UserSettings::GetSettings();
        editorwindow = settings.IsKeybindActive(KeyBindList::EditorWindow);
        gamewindow = settings.IsKeybindActive(KeyBindList::GameWindow);
        hierarchuwindow = settings.IsKeybindActive(KeyBindList::HierarchyWindow);

        if (hierarchuwindow || gamewindow || editorwindow)
        {
            Get_App()->SetToNormal();
        }
    }
    if (!_UCodeLangReloadKey.has_value())
    {
        _UCodeLangReloadKey = Get_App()->GetULangAssemblyID();
    }
    else  
    {
        if (Get_App()->GetULangAssemblyID() != _UCodeLangReloadKey.value())
        {
            _UCodeLangReloadKey = Get_App()->GetULangAssemblyID();
            OnULangReload();
        }
    }

    if (editorwindow)
    {
        ImGui::SetNextWindowFocus();
    }
    if (ImGui::Begin(Get_ImGuiName().c_str()))
    {
        SceneEditorTab();

    } ImGui::End();


    ImGui::PopStyleVar();


    if (hierarchuwindow)
    {
        ImGui::SetNextWindowFocus();
    }
    if (ImGui::Begin("Hierarchy"))
    {
        Scenehierarchy();

    }ImGui::End();


    ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, { 0.0f,0.0f });

    if (gamewindow)
    {
        ImGui::SetNextWindowFocus();
    }

    if (ImGui::Begin("Game"))
    {
        //    UnLoadSceneCamera();
        GameTab();

    } ImGui::End();

    ImGui::PopStyleVar();

    if (_UseingScenePath.has_value() && _SceneDataAsRunTiime) 
    {
        if (BlockKey.has_value())
        {
            if (!ScencIsDiffent())
            {
                Get_App()->RemoveBlockClose(BlockKey.value());
                BlockKey = {};
            }
        }
        else
        {
            if (ScencIsDiffent())
            {
                BlockCloseData data;
                data.Popname = "Save " + _UseingScenePath.value().filename().generic_string();
                data.OnDontSave = [this]()
                    {
                        BlockKey = {};
                    };
                data.OnSave = [this]()
                    {
                        BlockKey = {};
                        SaveScene();
                    };
                BlockKey = Get_App()->AddBlockClose(std::move(data));
            }
        }
    }

    auto _GameRunTime = MainSceneData.GetGameRuntime();
    if (!_IsRuningGame)
    {
        _GameRunTime->DestroyNullScenes();
    }
}

void GameEditorWindow::OnSaveWindow(USerializer& JsonToSaveIn)
{
    
    auto Assespath = Get_App()->Get_RunTimeProjectData()->GetAssetsDir();
    if (_SceneData && _UseingScenePath.has_value() && !_IsRuningGame)
    {
        auto PathString = FileHelper::ToRelativePath(Assespath, _UseingScenePath.value());


        JsonToSaveIn.Write("_ScenePath", PathString);

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
    ShowSceneData();
    /*
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
    */
}


bool GameEditorWindow::ScencIsDiffent()
{
    if (_SceneDataAsRunTiime) 
    {
        String asstr;
        {
            UCode::Scene2dData data;
            UCode::Scene2dData::SaveScene(_SceneDataAsRunTiime, data,USerializerType::Bytes);
            UCode::USerializer serializer(USerializerType::Bytes);
            data.PushData(serializer);
            serializer.ToString(asstr, false);
        }

        return asstr != sceneassaved;
    }
    return false;
}

void GameEditorWindow::SetCopy(const UCode::Scene2dData::Entity_Data Entity)
{
    UserSettings::SetCopyBufferAsValue("Entity", Entity);
}

void GameEditorWindow::SetCopy(const UCode::Entity* Entity, bool CopyRef)
{
    if (CopyRef)
    {
        auto v = Entity->NativeManagedPtr();
        UserSettings::SetCopyManagedRef(AnyManagedPtr::As(v));
        UserSettings::SetCopyBufferAsValue("ManagedRef", "Entity");
    }
    else
    {
        UCode::Scene2dData::Entity_Data v;
        UCode::Scene2dData::SaveEntityData(Entity, v, USerializerType::YAML);
        SetCopy(v);
    }
}
void GameEditorWindow::SetCopy(const UCode::RunTimeScene* Entity)
{
    UCode::Scene2dData v;
    UCode::Scene2dData::SaveScene(Entity, v, USerializerType::YAML);

    UC::USerializer s(USerializerType::YAML);
    v.PushData(s);
    UserSettings::SetCopyBufferAsValue<String>("Scene", s.Get_TextMaker().c_str());
}

void GameEditorWindow::SceneCameraGetInputs(SceneEditorTabData& data)
{
    auto& _GameRender = data._GameRender;
    auto& _SceneCameraData = data._SceneCameraData;
    auto& _SceneCamera = data._SceneCamera;

    const float TimeFromlastFrame = Get_App()->GetGameRunTime()->Get_GameTime().UpateDelta;

    UCode::Vec2& CamPos = *(UCode::Vec2*)&_SceneCameraData._Pos.X;


    float CamSpeed = 2.0f;
    float ScrollSizeSpeed = 2.5f;

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImGuiIO& io = ImGui::GetIO();

    auto M_pos = g.MouseLastValidPos;
    auto M_wheelX = io.MouseWheelH;
    auto M_wheelY = io.MouseWheel;




    UCode::Vec2 CamVelocity = { 0,0 };

    auto& userset = UserSettings::GetSettings();
    bool UpV = userset.IsKeybindActive(KeyBindList::Up);
    bool DownV = userset.IsKeybindActive(KeyBindList::Down);
    bool liftV = userset.IsKeybindActive(KeyBindList::Left);
    bool rightV = userset.IsKeybindActive(KeyBindList::Right);


    if (!ImGui::IsKeyDown(ImGuiMod_Ctrl))
    {
        if (UpV) { CamVelocity.Y += 1; }
        if (DownV) { CamVelocity.Y -= 1; }
        if (liftV) { CamVelocity.X -= 1; }
        if (rightV) { CamVelocity.X += 1; }
    }
    else
    {
        auto moveval = 0.1;
        if (UpV) { _SceneCameraData.Orth_Size += moveval; }
        if (DownV) { _SceneCameraData.Orth_Size -= moveval; }
    }


    static Optional<DontWaitInputKey> DontWait;
    bool hasanydown = UpV || DownV || liftV || rightV;
    if (hasanydown)
    {
        if (!DontWait.has_value())
        {
            DontWait = Get_App()->AddDontWaitForInput();
        }
    }
    else
    {
        if (DontWait.has_value())
        {
            Get_App()->RemoveWaitForInput(DontWait.value());
            DontWait = {};
        }
    }



    float NewCamSpeed = CamSpeed * (_SceneCameraData.Orth_Size / 7) + 10;

    CamPos += CamVelocity.Get_Normalize() * NewCamSpeed * TimeFromlastFrame;
    auto NewSize = _SceneCameraData.Orth_Size - M_wheelY * ScrollSizeSpeed;

    if (NewSize > 0)
    {
        _SceneCameraData.Orth_Size = NewSize;
    }

    {
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
    }

    //

    auto& Pos = _SceneCamera->NativeEntity()->NativeLocalPosition();
    Pos = CamPos;



    _SceneCamera->Set_Ortho_size(_SceneCameraData.Orth_Size);
}
void GameEditorWindow::SetPrefabMode(UC::EntityPtr prefab, PrefabModeData&& data)
{
    _PrefabMode = prefab;
    _PrefabModeData = std::move(data);
    PrefabSceneData._SceneCameraData._Pos = prefab.Get_Value()->NativeLocalPosition();
    PrefabSceneData._GameRuntimeRef = prefab.Get_Value()->NativeScene()->Get_RunTime();
}
UC::EntityPtr GameEditorWindow::GetCurrentSeclectedEntity()
{
    auto win = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_Window<GameEditorWindow>();

    return win->SelectedObject;
}
void GameEditorWindow::SceneEditorTab()
{


    if (_PrefabMode.Has_Value())
    {
        auto entity = _PrefabMode.Get_Value();
        auto scene = entity->NativeScene();
        auto prefabruntime = scene->Get_RunTime();

        SceneEditorBar(PrefabSceneData);
        SceneEditor(PrefabSceneData);
    }
    else
    {
        SceneEditorBar(MainSceneData);
        SceneEditor(MainSceneData);
    }


}

void GameEditorWindow::SceneEditor(SceneEditorTabData& data)
{
    auto _GameRunTime = data.GetGameRuntime();
    auto& _GameRender = data._GameRender;
    if (_GameRunTime == nullptr)
    {
        LoadRunTime(); LoadRender(data, false);
    }


    auto Viewportsize = ImGui::GetContentRegionAvail();
    auto p = ImGui::GetCursorPos();

    if (_WindowType != GameWindowType::EditorWindow)
    {
        UpdateAndShowGameImg();
    }
    if (_GameRender == nullptr) { LoadRender(data, false); }
    LoadSceneCamera(data);
    auto runtime = UCode::RenderRunTime2d::GetRenderRunTime(_GameRunTime);



    UpdateRunTimeWindowSize(Viewportsize, data._SceneCamera);
    runtime->UpdateDrawData();



    {
        if (SelectedObject.Has_Value())
        {
            auto Entity = SelectedObject.Get_Value();

            auto pos = Entity->WorldPosition();
            auto rot = Entity->WorldRotation();
            auto scl = Entity->WorldScale();


            UCode::Mat4 matrix;

            float* matrixfptr = (float*)&matrix;

            ImGuizmo::RecomposeMatrixFromComponents(
                &pos.X,
                &rot.X,
                &scl.X, matrixfptr);


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


            ImGuizmo::SetOrthographic(data._SceneCamera->Get_CamType() == UCode::Camera2d::CamType::Orthographic);
            ImGuizmo::BeginFrame();

            auto tep = ImGui::GetCursorPos();

            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
            auto viewportOffset = ImGui::GetWindowPos();

            ImVec2 view = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
            ImVec2 view2 = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };



            ImGuiIO& io = ImGui::GetIO();
            ImGuizmo::SetRect(view.x, view.y, view2.x - view.x, view2.y - view.y);;

            auto vm = data._SceneCamera->Get_ViewMatrix();
            auto pm = data._SceneCamera->Get_ProjectionMatrix();

            bool updated = ImGuizmo::Manipulate(
             (float*)&vm,
             (float*)&pm
                , mode, ImGuizmo::MODE::WORLD, matrixfptr);


            if (updated)
            {
                ImGuizmo::DecomposeMatrixToComponents(matrixfptr, &pos.X, &rot.X, &scl.X);
                Entity->WorldPosition(pos);
                Entity->WorldRotation(rot);
                Entity->WorldScale(scl);
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
                    UCode::RenderRunTime2d::DrawQuad2dData data = { Item->WorldPosition2D(),{1,1},{0,90} };
                    data.color.A = 0.05f;
                    // data.Spr = AppFiles::GetSprite(AppFiles::sprite::Scene2dData);
                    runtime->DrawQuad2d(data);
                }
            }
        }
    }
    _GameRender->Draw(runtime->Get_DrawData(), data._SceneCamera);

    size_t SceneTex = data._SceneCamera->Get_Buffer().Get_TextureId();


    auto tep = ImGui::GetCursorPos();

    Vec2 V = *(Vec2*)&tep;
    ImGui::Image((ImTextureID)SceneTex, Viewportsize, { 0,0 }, { 1,-1 });
    bool IsOverImage = ImGui::IsItemHovered();
    if (IsOverImage)
    {
        Vec2 Mpos = MousePosFromImage(V, *(Vec2*)&Viewportsize);

    }


    
    ImGuiIO& io = ImGui::GetIO();
    bool IsFocused = ImGui::IsItemHovered() || ImGui::IsWindowFocused();
    if (IsFocused)
    {
        SceneCameraGetInputs(data);
    }
}
void GameEditorWindow::SceneEditorBar(SceneEditorTabData& data)
{
    auto& _SceneCamera = data._SceneCamera;

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
void GameEditorWindow::UnLoadSceneCamera(SceneEditorTabData& data)
{
    if (data._SceneCamera)
    {
        UCode::Entity::Destroy(data._SceneCamera->NativeEntity());
        data._SceneCamera->UnSetAsMainCam();
        data._SceneCamera = nullptr;

    }
}
void GameEditorWindow::LoadSceneCamera(SceneEditorTabData& data)
{
    if (data._SceneCamera == nullptr)
    {
        auto Entity = data.GetGameRuntime()->NewEntityOnRunTimeScene();
        data._SceneCamera = Entity->AddCompoent<UCode::Camera2d>();
        data._SceneCamera->UnSetAsMainCam();
    }
}

void GameEditorWindow::ShowSceneDataAddNewScene()
{
    auto* e = MainSceneData._GameRunTime->Add_NewScene();
    e->Get_Name() = UnNamedScene;
    if (MainSceneData._GameRunTime->Get_Scenes().size() == 1)
    {
        if (_SceneData == nullptr)
        {
            _SceneData = new  UCode::Scene2dData();

            UCode::Scene2dData::SaveScene(e, *_SceneData, USerializerType::Fastest);
        }


        _SceneDataAsRunTiime = e;
    }

}
void GameEditorWindow::ShowSceneData()
{
    static bool showsavepopup = false;

    if (_PrefabMode.Has_Value())
    {
        String name = "Save " + _PrefabMode.Get_Value()->NativeName() + " Prefab";
        if (ImGui::BeginPopupModal(name.c_str()))
        {
            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Yes"))
            {
                _PrefabModeData.OnSave(_PrefabMode.Get_Value());
                _PrefabMode = {};
                _PrefabModeData = {};
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("No"))
            {
                _PrefabMode = {};
                _PrefabModeData = {};
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

    }

    if (_PrefabMode.Has_Value())
    {
        auto imageh = ImGui::GetFrameHeight();
        bool onback = ImGui::Button("<", { imageh,imageh });

        ImGui::SameLine();
        ImGuIHelper::Image(AppFiles::sprite::RawEntityData, { imageh,imageh });

        ImGui::SameLine();
        ImGuIHelper::Text(_PrefabMode.Get_Value()->NativeName());
        if (onback)
        {
            auto e = _PrefabMode.Get_Value();
            bool wasupdated = _PrefabModeData.WasUpdated(e);
            if (wasupdated)
            {
                String name = "Save " + _PrefabMode.Get_Value()->NativeName() + " Prefab";
                ImGui::OpenPopup(name.c_str());
                showsavepopup = true;
            }
            else
            {
                _PrefabMode = {};
            }
        }
    }

    bool HasSceneRightPrivilege = true;//add this to stop updateing the same scene data file.

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));

    ImGuiTableFlags flags = ImGuiTableFlags_Resizable;
    if (_PrefabMode.Has_Value())
    {
        if (ImGui::BeginTable("split", 1, flags))
        {
            ShowEntityData(PrefabSceneData, _PrefabMode.Get_Value());
            ImGui::EndTable();
        }
    }
    else if (MainSceneData._GameRunTime)
    {
        if (ImGui::BeginTable("split", 1, flags))
        {
            auto& Scenes = MainSceneData._GameRunTime->Get_Scenes();



            for (size_t i = 0; i < Scenes.size(); i++)
            {
                auto& Item = Scenes[i];

                ShowScene(MainSceneData, Item.get());
            }

            ImGui::EndTable();
        }

    }

    ImGui::PopStyleVar();


    auto Size = ImGui::GetContentRegionAvail();
    if (Size.x == 0 || Size.y == 0) { Size = { 1,1 }; }
    ImGui::InvisibleButton("Data", Size);
    DropSceneFromPath();
    if (ImGui::IsItemFocused())
    {
        auto& settings = UserSettings::GetSettings();

        if (settings.IsKeybindActive(KeyBindList::New))
        {
            ShowSceneDataAddNewScene();
        }
        if (settings.IsKeybindActive(KeyBindList::Paste))
        {
            auto v = UserSettings::ReadCopyBufferAs<Path>("AssetPath");

            if (v.has_value())
            {
                auto& assetpath = *v;
                OpenScencAtPath(assetpath);
            }
        }
    }
    if (ImGuIHelper::BeginPopupContextItem())
    {
        ImGuIHelper::Text(StringView("RunTime Options"));
        ImGui::Separator();

        auto& settings = UserSettings::GetSettings();
        auto keybindstring = settings.KeyBinds[(size_t)KeyBindList::New].ToString();
        if (ImGui::MenuItem("Add New Scene", keybindstring.c_str()) || settings.IsKeybindActive(KeyBindList::New))
        {
            ShowSceneDataAddNewScene();
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::MenuItem("Paste Scene", keybindstring.c_str()) || settings.IsKeybindActive(KeyBindList::Paste))
        {
            auto v = UserSettings::ReadCopyBufferAs<Path>("AssetPath");

            if (v.has_value())
            {
                auto& assetpath = *v;
                OpenScencAtPath(assetpath);
            }

            ImGui::CloseCurrentPopup();
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
        target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;
        target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DragAndDropType_AssetPath, target_flags))
        {
            const Path* fullpath = *(Path**)payload->Data;


            auto ext = Path(*fullpath).extension().generic_string();
            bool canbedroped = ext == UCode::Scene2dData::FileExtDot;
            if (payload->IsDelivery())
            {
                if (canbedroped)
                {
                    OpenScencAtPath(*fullpath);
                }
            }

            if (canbedroped)
            {
                ImGuiContext& g = *GImGui;
                ImGui::RenderDragDropTargetRect(g.DragDropTargetRect, g.DragDropTargetClipRect);
            }
        }


        ImGui::EndDragDropTarget();
    }
}
bool CanPasteScene()
{
    bool r = false;
    r = UserSettings::CanReadCopyBufferAs("Entity");

    if (r == false)
    {
        auto v = UserSettings::ReadCopyBufferAs<Path>("AssetPath");

        if (v.has_value())
        {
            auto p = v.value();
            auto  ext = p.extension();

            if (ext == Path(UC::RawEntityData::FileExtDot))
            {
                r = true;
            }

        }
    }
    return r;
}
void GameEditorWindow::PasteInScene(UCode::RunTimeScene* Item)
{
    auto copyop1 = UserSettings::ReadCopyBufferAs<UCode::Scene2dData::Entity_Data>("Entity");
    if (copyop1.has_value())
    {
        UCode::Scene2dData::Entity_Data _CopyedEntity = copyop1.value();
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
        return;
    }

    auto copyop2 = UserSettings::ReadCopyBufferAs<Path>("AssetPath");
    if (copyop2.has_value())
    {
        auto& assetpath = copyop2.value();

        auto  ext = assetpath.extension();

        auto app = EditorAppCompoent::GetCurrentEditorAppCompoent();
        auto& loader = app->Get_AssetLoader();
        auto& index = loader.RunTimeProject->Get_AssetIndex();
        auto assetdir = app->Get_RunTimeProjectData()->GetAssetsDir();

        auto relassetpath = Path(assetpath.native().substr(assetdir.native().size())).generic_string();

        auto assetindex = index.FindFileRelativeAssetName(relassetpath);
        if (assetindex.has_value() && assetindex.value().UserID.has_value())
        {
            UID _id = assetindex.value().UserID.value();
            auto assetop = loader.LoadAssetPtr(_id);

            if (assetop.has_value())
            {
                auto asset = assetop.value();
                if (ext == Path(UC::RawEntityData::FileExtDot))
                {
                    auto typedassetop = asset->GetAssetAs<UC::RawEntityDataAsset>();

                    if (typedassetop.has_value())
                    {
                        auto typed = typedassetop.value();

                        UCode::Entity* e = Item->NewEntity();
                        UCode::Scene2dData::LoadEntity(e, typed.value()->_Base.Get_Value());

                        auto f = e->AddCompoent<UC::EntityPlaceHolder>();
                        f->_id = _id;
                    }
                }
            }
        }
    }
}
void GameEditorWindow::PasteInEntity(UCode::Entity* Item)
{
    auto copyop1 = UserSettings::ReadCopyBufferAs<UCode::Scene2dData::Entity_Data>("Entity");
    if (copyop1.has_value())
    {
        UCode::Scene2dData::Entity_Data _CopyedEntity = copyop1.value();
        auto pastedentity = Item->NativeAddEntity();

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
        return;
    }

    auto copyop2 = UserSettings::ReadCopyBufferAs<Path>("AssetPath");
    if (copyop2.has_value())
    {
        auto& assetpath = copyop2.value();

        auto  ext = assetpath.extension();

        auto app = EditorAppCompoent::GetCurrentEditorAppCompoent();
        auto& loader = app->Get_AssetLoader();
        auto& index = loader.RunTimeProject->Get_AssetIndex();
        auto assetdir = app->Get_RunTimeProjectData()->GetAssetsDir();

        auto relassetpath = Path(assetpath.native().substr(assetdir.native().size())).generic_string();

        auto assetindex = index.FindFileRelativeAssetName(relassetpath);
        if (assetindex.has_value() && assetindex.value().UserID.has_value())
        {
            UID _id = assetindex.value().UserID.value();
            auto assetop = loader.LoadAssetPtr(_id);

            if (assetop.has_value())
            {
                auto asset = assetop.value();
                if (ext == Path(UC::RawEntityData::FileExtDot))
                {
                    auto typedassetop = asset->GetAssetAs<UC::RawEntityDataAsset>();

                    if (typedassetop.has_value())
                    {
                        auto typed = typedassetop.value();

                        UCode::Entity* e = Item->NativeAddEntity();
                        UCode::Scene2dData::LoadEntity(e, typed.value()->_Base.Get_Value());

                        auto f = e->AddCompoent<UC::EntityPlaceHolder>();
                        f->_id = _id;
                    }
                }
            }
        }
    }
}
void GameEditorWindow::ShowScene(SceneEditorTabData& data, UCode::RunTimeScene* Item)
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

    bool ischanged = false;
    if (Item == _SceneDataAsRunTiime)
    {
        ischanged = ScencIsDiffent();
    }

    if (IsRenameing && IsSelected(Item))
    {
        node_open = WasSelectedObjectOpened;
        ImGuIHelper::DrawRenameTree(Item->Get_Name(), node_open, IsRenameing, AppFiles::sprite::Scene2dData);
        ImGui::Indent();
    }
    else
    {
        String str = SceneName;
        if (ischanged)
        {
            str += "*";
        }
        auto Data = ImGuIHelper::TreeNode(Item, str.c_str(), AppFiles::sprite::Scene2dData);
        node_open = Data;
        ShowingTree = true;
    }
    ImGui::PopStyleColor();


    if (ImGui::IsItemFocused() && !ImGui::GetIO().WantTextInput)
    {
        auto& settings = UserSettings::GetSettings();

        if (settings.IsKeybindActive(KeyBindList::Rename))
        {
            SetScelected(Item);
            IsRenameing = true;
            WasSelectedObjectOpened = node_open;

        }

        if (settings.IsKeybindActive(KeyBindList::New))
        {
            if (node_open)
            {
                UCode::Entity* e = Item->NewEntity();
                e->NativeName() = UnNamedEntity;
                e->WorldPosition(data._SceneCameraData._Pos);
            }
            else
            {
                ShowSceneDataAddNewScene();
            }
        }

        if (settings.IsKeybindActive(KeyBindList::Paste))
        {
            if (CanPasteScene())
            {
                PasteInScene(Item);
            }
        }


        if (settings.IsKeybindActive(KeyBindList::Copy))
        {
            SetCopy(Item);
        }

        if (settings.IsKeybindActive(KeyBindList::Delete))
        {
            UCode::RunTimeScene::Destroy(Item);
        }

    }

    if (ImGuIHelper::BeginPopupContextItem())
    {
        ImGuIHelper::Text(StringView("Scene Options"));
        ImGui::Separator();

        auto& settings = UserSettings::GetSettings();
        auto keybindstring = settings.KeyBinds[(size_t)KeyBindList::Rename].ToString();

        if (ImGui::MenuItem("Rename", keybindstring.c_str()) || settings.IsKeybindActive(KeyBindList::Rename))
        {
            SetScelected(Item);
            IsRenameing = true;
            WasSelectedObjectOpened = node_open;

            ImGui::CloseCurrentPopup();
        }


        if (ImGui::MenuItem("Save"))
        {
            SaveScene();
            ImGui::CloseCurrentPopup();
        }
        keybindstring = settings.KeyBinds[(size_t)KeyBindList::New].ToString();

        if (ImGui::MenuItem("Add Entity") || settings.IsKeybindActive(KeyBindList::New))
        {
            if (node_open)
            {
                UCode::Entity* e = Item->NewEntity();
                e->NativeName() = UnNamedEntity;
                e->WorldPosition(data._SceneCameraData._Pos);
            }
            else
            {
                ShowSceneDataAddNewScene();
            }

            ImGui::CloseCurrentPopup();
        }

        keybindstring = settings.KeyBinds[(size_t)KeyBindList::Copy].ToString();
        if (ImGui::MenuItem("Copy", keybindstring.c_str(), nullptr) || settings.IsKeybindActive(KeyBindList::Paste))
        {
            SetCopy(Item);
            ImGui::CloseCurrentPopup();
        }


        keybindstring = settings.KeyBinds[(size_t)KeyBindList::Paste].ToString();
        if (ImGui::MenuItem("Paste", keybindstring.c_str(), nullptr, CanPasteScene()) || settings.IsKeybindActive(KeyBindList::Paste))
        {
            PasteInScene(Item);
            ImGui::CloseCurrentPopup();
        }
        ImGui::Separator();


        keybindstring = settings.KeyBinds[(size_t)KeyBindList::Delete].ToString();
        if (ImGui::MenuItem("Destroy", keybindstring.c_str()) || settings.IsKeybindActive(KeyBindList::Delete))
        {
            UCode::RunTimeScene::Destroy(Item);
            ImGui::CloseCurrentPopup();
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
                const UID* DropItem = *(UID**)payload->Data;

                auto& loader = Get_App()->Get_AssetLoader();
                auto opasset = loader.LoadAssetPtr(*DropItem);

                if (opasset.has_value())
                {
                    auto asset = opasset.value();
                    auto a = asset->GetAssetAs<UC::RawEntityDataAsset>();

                    if (a.has_value())
                    {
                        UCode::Entity* e = Item->NewEntity();
                        UCode::Scene2dData::LoadEntity(e, a.value()->_Base.Get_Value());

                        auto f = e->AddCompoent<UC::EntityPlaceHolder>();
                        f->_id = *DropItem;
                        f->OnUpdatedID();
                    }
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



        ImGui::EndDragDropTarget();
    }

    if (ImGui::BeginDragDropSource())
    {

        bool OnDropable = ImGui::SetDragDropPayload(DragAndDropType_Scene2dType, &Item, sizeof(UCode::RunTimeScene*));
        if (OnDropable)
        {
            String Text = "Drop " + SceneName + " Here?";
            ImGuIHelper::Text(Text);
        }
        else
        {
            String Text = "Draging " + SceneName;
            ImGuIHelper::Text(Text);
        }

        ImGui::EndDragDropSource();
    }



    if (node_open)
    {

        auto& Enitys = Item->Get_Entitys();
        for (size_t i2 = 0; i2 < Enitys.size(); i2++)
        {
            auto& Enity = Enitys[i2];

            ShowEntityData(data, Enity.get());
        }
        if (IsRenameing && IsSelected(Item))
        {
            ImGui::Unindent();
        }

        if (ShowingTree) {
            ImGui::TreePop();
        }
    }
    ImGui::PopID();
}

void GameEditorWindow::EntityDestroy(UCode::Entity* Item)
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

void GameEditorWindow::EntityAdd(UCode::Entity* Item, bool AddToChild)
{
    if (AddToChild)
    {
        auto E = Item->NativeAddEntity();
        E->NativeName() = UnNamedEntity;
        SetScelected(E);
    }
    else
    {
        if (Item->NativeParent())
        {
            auto E = Item->NativeParent()->NativeAddEntity();
            E->NativeName() = UnNamedEntity;
            SetScelected(E);
        }
        else
        {
            auto E = Item->NativeScene()->NewEntity();
            E->NativeName() = UnNamedEntity;
            SetScelected(E);
        }
    }
}
void GameEditorWindow::ShowEntityData(SceneEditorTabData& data, UCode::Entity* Item)
{
    if (Item->Get_IsDestroyed()) { return; }

    ImGui::PushID(Item);

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();



    //
    AppFiles::sprite _SprToShow = AppFiles::sprite::Entity;
    auto& Co = Item->NativeCompoents();
    if (Item->GetCompent<UC::EntityPlaceHolder>())
    {
        _SprToShow = AppFiles::sprite::RawEntityData;
    }
    //
    auto& g = *GImGui;

    //
    String& EntityName = Item->NativeName();
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
            ImGuIHelper::DrawRenameTree(Item->NativeName(), node_open, IsRenameing, AppFiles::sprite::Entity);
            ImGui::Indent();
        }
        else
        {
            ImGuiTreeNodeFlags TreeFlags = IsSelectedEntity ? ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected : 0;
            TreeFlags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow;
            TreeFlags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_SpanAvailWidth;
            node_open = ImGuIHelper::TreeNode(Item, Item->NativeName().c_str(), AppFiles::sprite::Entity,TreeFlags);
            ShowingTree = true;
        }


        if (IsSelectedEntity == false)
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
                const UID* DropItem = *(UID**)payload->Data;
                UC::RawEntityData Data;

                auto runtimeprj = Get_App()->Get_RunTimeProjectData();
                auto& index = runtimeprj->Get_AssetIndex();

                auto file = index.FindFileUsingID(*DropItem);

                if (file.has_value())
                {
                    Path Path = runtimeprj->GetAssetsDir() / file.value().RelativePath;
                    if (UC::RawEntityData::ReadFromFile(Path, Data))
                    {
                        UCode::Entity* e = Item->NativeAddEntity();
                        UCode::Scene2dData::LoadEntity(e, Data.Get_Value());

                        auto f = e->AddCompoent<UC::EntityPlaceHolder>();
                        f->_id = *DropItem;
                        f->OnUpdatedID();
                    }
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
                            auto& Assembly = UCode::ULangRunTime::Get(Get_GameLib())->Get_Assembly();
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
            ImGuIHelper::Text(Text);
        }
        else
        {
            String Text = "Draging " + EntityName;
            ImGuIHelper::Text(Text);
        }

        ImGui::EndDragDropSource();
    }



    if (_PickComponent.has_value() && IsSelected(Item))
    {
        if (ImGui::BeginChild("PickComponent"))
        {
            ImGuIHelper::Text((StringView("Pick Component")));
            auto& Assembly = UCode::ULangRunTime::Get(Get_GameLib())->Get_Assembly();

            for (auto& Item2 : _PickComponent.value())
            {
                auto Node = Assembly.Find_Node(Item2);
                if (Node)
                {
                    bool Selected = ImGui::Button(Node->FullName.c_str());

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
    static bool showlist = false;
    static bool focusenext = false;
    const char* EntityOptions = "SomeThing";
    if (focusenext)
    {
        ImGui::FocusItem();
        focusenext = false;
    }

    if (ImGui::IsItemFocused() && !ImGui::GetIO().WantTextInput)
    {
        auto& settings = UserSettings::GetSettings();


        if (settings.IsKeybindActive(KeyBindList::Inspect))
        {
            if (ImGui::IsKeyDown(ImGuiKey::ImGuiMod_Ctrl))
            {
                data._SceneCameraData._Pos = Item->WorldPosition();
            }
            else
            {
                auto inpswin = Get_App()->Get_Window<InspectWindow>();
                inpswin->Inspect(Inspect_Entity2d::Get(Item));
                SetScelected(Item);
            }
        }

        if (settings.IsKeybindActive(KeyBindList::Rename))
        {
            SetScelected(Item);
            IsRenameing = true;
            WasSelectedObjectOpened = node_open;
        }

        if (settings.IsKeybindActive(KeyBindList::New))
        {
            if (ImGui::IsKeyDown(ImGuiKey::ImGuiMod_Ctrl))
            {
                ImGui::OpenPopup(EntityOptions);
                showlist = true;
            }
            else
            {
                EntityAdd(Item, node_open);
            }
        }

        if (settings.IsKeybindActive(KeyBindList::Paste))
        {
            PasteInEntity(Item);
        }


        if (settings.IsKeybindActive(KeyBindList::Copy))
        {
            SetCopy(Item, ImGui::IsKeyDown(ImGuiKey::ImGuiMod_Ctrl));
        }

        if (settings.IsKeybindActive(KeyBindList::Delete))
        {
            if (ImGui::IsKeyDown(ImGuiKey::ImGuiMod_Ctrl))
            {
                UCode::Scene2dData::CloneEntity(Item);
            }
            else
            {
                EntityDestroy(Item);
                focusenext = true;
            }
        }

    }
    if (ImGuIHelper::BeginPopupContextItem(EntityOptions))
    {
        ImGuIHelper::Text(StringView("Entity Options"));
        ImGui::Separator();

        auto& settings = UserSettings::GetSettings();
        auto keybindstring = settings.KeyBinds[(size_t)KeyBindList::Inspect].ToString();

        if (ImGui::MenuItem("Inspect", keybindstring.c_str()) || settings.IsKeybindActive(KeyBindList::Inspect))
        {
            auto inpswin = Get_App()->Get_Window<InspectWindow>();

            inpswin->Inspect(Inspect_Entity2d::Get(Item));
            SetScelected(Item);
        }
        keybindstring = "Ctrl+" + settings.KeyBinds[(size_t)KeyBindList::Inspect].ToString();
        if (ImGui::MenuItem("Set Scene Camera To", keybindstring.c_str()) || (ImGui::IsKeyDown(ImGuiKey::ImGuiMod_Ctrl) && settings.IsKeybindActive(KeyBindList::Inspect)))
        {
            data._SceneCameraData._Pos = Item->WorldPosition();
            ImGui::CloseCurrentPopup();
        }


        keybindstring = settings.KeyBinds[(size_t)KeyBindList::Copy].ToString();
        if (ImGui::MenuItem("Copy", keybindstring.c_str()) || settings.IsKeybindActive(KeyBindList::Copy))
        {
            SetCopy(Item, ImGui::IsKeyDown(ImGuiKey::ImGuiMod_Ctrl));
            ImGui::CloseCurrentPopup();
        }
        keybindstring = "Ctrl+" + settings.KeyBinds[(size_t)KeyBindList::Copy].ToString();
        if (ImGui::MenuItem("Copy Ref", keybindstring.c_str()) || settings.IsKeybindActive(KeyBindList::Copy))
        {
            SetCopy(Item, ImGui::IsKeyDown(ImGuiKey::ImGuiMod_Ctrl));
            ImGui::CloseCurrentPopup();
        }

        keybindstring = settings.KeyBinds[(size_t)KeyBindList::Paste].ToString();
        if (ImGui::MenuItem("Paste", keybindstring.c_str()) || settings.IsKeybindActive(KeyBindList::Paste))
        {
            PasteInEntity(Item);
            ImGui::CloseCurrentPopup();
        }

        keybindstring = settings.KeyBinds[(size_t)KeyBindList::Rename].ToString();
        if (ImGui::MenuItem("Rename", keybindstring.c_str()) || settings.IsKeybindActive(KeyBindList::Rename))
        {
            SetScelected(Item);
            IsRenameing = true;
            WasSelectedObjectOpened = node_open;
            ImGui::CloseCurrentPopup();
        }


        keybindstring = settings.KeyBinds[(size_t)KeyBindList::New].ToString();
        if (!showlist)
        {
            if (ImGui::MenuItem("Add Entity", keybindstring.c_str()) || settings.IsKeybindActive(KeyBindList::New))
            {
                EntityAdd(Item, node_open);
                ImGui::CloseCurrentPopup();
            }
        }

        if (ImGui::BeginMenu("Add Compoent"))
        {
            if (Inspect_Compoent2d::ShowAddCompoenList(Item))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndMenu();
        }
        keybindstring = "Ctrl+" + settings.KeyBinds[(size_t)KeyBindList::New].ToString();
        if (showlist)
        {
            showlist = false;
            ImGui::FocusItem();
        }

        keybindstring = "Ctrl+" + settings.KeyBinds[(size_t)KeyBindList::Delete].ToString();
        if (ImGui::MenuItem("Duplicate", keybindstring.c_str()) || (ImGui::IsKeyDown(ImGuiKey::ImGuiMod_Ctrl) && settings.IsKeybindActive(KeyBindList::Inspect)))
        {
            UCode::Scene2dData::CloneEntity(Item);
            ImGui::CloseCurrentPopup();
        }

        keybindstring = settings.KeyBinds[(size_t)KeyBindList::Delete].ToString();
        if (ImGui::MenuItem("Destroy", keybindstring.c_str()))
        {
            EntityDestroy(Item);
            focusenext = true;
        }

        ImGui::EndPopup();
    }


    if (node_open)
    {


        {
            auto& Entitys = Item->NativeGetEntitys();
            for (size_t i = 0; i < Entitys.size(); i++)
            {
                auto& Item2 = Entitys[i];
                if (Item2->Get_IsDestroyed()) { continue; }
                ShowEntityData(data, Item2.get());
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

                if (Item2->Get_CompoentTypeData() == &UC::EntityPlaceHolder::type_Data)
                {
                    continue;
                }
                Inspect_Compoent2d::DrawCompoentButtion(Item2.get(), Get_App());

            }
        }
        if (IsRenameing && IsSelectedEntity)
        {
            ImGui::Unindent();
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
    ImGuIHelper::Text(StringView("Add Stuff here"));

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
    if (_SceneData && _UseingScenePath.has_value())
    {
        auto SaveType = Get_ProjectData()->Get_ProjData()._SerializeType;
        UCode::Scene2dData::SaveScene(_SceneDataAsRunTiime, *_SceneData, SaveType);

        auto runprojectdata = Get_ProjectData();
        auto p = _UseingScenePath.value().generic_string();
        auto& scenefilepath = _UseingScenePath.value();

        if (UCode::Scene2dData::ToFile(scenefilepath, *_SceneData, SaveType))
        {
            auto& index = runprojectdata->Get_AssetIndex();

            auto relpath = p.substr(runprojectdata->GetAssetsDir().generic_string().size());
            auto op = index.FindFileRelativeAssetName(relpath);

            if (op.has_value())
            {
                auto& ind = op.value();
                ind.FileLastUpdatedTime = std::filesystem::last_write_time(scenefilepath).time_since_epoch().count();
                ind.FileSize = std::filesystem::file_size(scenefilepath);
                SetSeneAsSaved();
            }
        }
        else
        {
            auto relpath = p.substr(runprojectdata->GetAssetsDir().generic_string().size());
            UCodeGEError("Saveing Asset for " << relpath << " Failed");
        }
    }
}
void GameEditorWindow::OpenScencAtPath(const Path& Path)
{
    LoadRunTime();

    auto& Scenes = MainSceneData._GameRunTime->Get_Scenes();
    auto NewSceneData = std::make_unique<UCode::Scene2dData>();//Get from cash;
    UCode::AssetManager* Assets = UCode::AssetManager::Get(Get_GameLib());
    if (UCode::Scene2dData::FromFile(*NewSceneData, Path))
    {
        if (BlockKey.has_value())
        {
            Get_App()->RemoveBlockClose(BlockKey.value());
            BlockKey = {};
        }
           

        auto NewScene = UCode::Scene2dData::LoadScene(MainSceneData._GameRunTime.get(), *NewSceneData);
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
            SelectedScene = NewScene->Get_ManagedPtr();
            _UseingScenePath = Path;
            _SceneDataAsRunTiime = NewScene;
            SetSeneAsSaved();
        }

    }
}
Vec2 GameEditorWindow::MousePosFromImage(const Vec2 CursorPos, const Vec2 ImageSize)
{
    auto tep = ImGui::GetMousePos();
    Vec2 Mpos = *(Vec2*)&tep;

    auto tep2 = ImGui::GetMousePos();
    auto  CursorPosScreen = CursorPos + *(Vec2*)&tep2;
    auto A = Mpos - CursorPos;

    //std::cout << Mpos.X << "," << Mpos.Y << ":" << CursorPosScreen.X << "," << CursorPosScreen.Y << '\n';
    //std::cout << A.X << "," << A.Y << '\n';
    return A;
}
void GameEditorWindow::OnULangReload()
{
    if (_UseingScenePath.has_value())
    {
        auto NewSceneData = std::make_unique<UCode::Scene2dData>();//Get from cash;
        if (UCode::Scene2dData::FromFile(*NewSceneData, _UseingScenePath.value()))
        {
            auto NewScene = UCode::Scene2dData::LoadScene(MainSceneData._GameRunTime.get(), *NewSceneData);

            UCode::Scene2dData data;
            UCode::Scene2dData::SaveScene(NewScene, data, USerializerType::Bytes);
            UCode::USerializer serializer(USerializerType::Bytes);
            data.PushData(serializer);
            serializer.ToString(sceneassaved, false);

            UCode::RunTimeScene::Destroy(NewScene);
        }
    }
}
void GameEditorWindow::SetSeneAsSaved()
{
    UCode::Scene2dData data;
    UCode::Scene2dData::SaveScene(_SceneDataAsRunTiime, data, USerializerType::Bytes);
    UCode::USerializer serializer(USerializerType::Bytes);
    data.PushData(serializer);
    serializer.ToString(sceneassaved, false);
}
void GameEditorWindow::GameTab()
{
    if (MainSceneData._GameRunTime == nullptr)
    {
        LoadRunTime();
        LoadRender(MainSceneData, _WindowType == GameWindowType::ExternalWindow);
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
        _IsRuningGame ? AppFiles::sprite::Play_Blue : AppFiles::sprite::Play_Green, ButtionSize);

    ImGui::SameLine();

    PausedButtionDown = ImGuIHelper::ImageButton(&PausedButtionDown,
        _IsGamePaused ? AppFiles::sprite::Paused_Blue : AppFiles::sprite::Paused_Green, ButtionSize);

    float Sizey = ImGui::GetItemRectSize().y;
    ImGui::SameLine();

    WindowTypeButtionDown = ImGui::Button(WindowTypeText, { 0,ButtionSize.y + 6 });

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
            UnLoadRender(MainSceneData);
            LoadRender(MainSceneData, _WindowType == GameWindowType::ExternalWindow);
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
        ImGuIHelper::Text(StringView("Cant Play Scene Try Saveing the Scene To A File. then Opening it"));
        ImGui::EndPopup();
    }

    UpdateAndShowGameImg();
    if (_WindowType == GameWindowType::ExternalWindow)
    {
        if (_IsRuningGame) {
            ImGuIHelper::Text(StringView("(Runing ExternalWindow) Open the Window Called ExternalWindow"));
        }
        else
        {
            ImGuIHelper::Text(StringView("Press play for ExternalWindow To Open"));
        }
    }
}

void GameEditorWindow::OnPlayScene()
{
    SaveScene();

    UnLoadRender(MainSceneData);
    LoadRender(MainSceneData, _WindowType == GameWindowType::ExternalWindow);


    if (!_DontWaitInputKey.has_value())
    {
        _DontWaitInputKey = Get_App()->AddDontWaitForInput();
    }
}
void GameEditorWindow::OnStopPlaying()
{
    UnLoadRunTime(); LoadRunTime();
    _IsRuningGame = false;

    if (_SceneData)
    {
        auto ShowScene = UCode::Scene2dData::LoadScene(MainSceneData._GameRunTime.get(), *_SceneData);
        _SceneDataAsRunTiime = ShowScene;
    }
    LoadRender(MainSceneData, false);

    if (_DontWaitInputKey.has_value())
    {
        Get_App()->RemoveWaitForInput(_DontWaitInputKey.value());
        _DontWaitInputKey.reset();
    }
}
void GameEditorWindow::UpdateAndShowGameImg()
{
    if (MainSceneData._GameRunTime && MainSceneData._GameRender)
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
    UCode::RenderRunTime2d* runtime = MainSceneData._GameRunTime ? UCode::RenderRunTime2d::GetRenderRunTime(MainSceneData._GameRunTime.get()) : nullptr;

    auto& _GameRunTime = MainSceneData._GameRunTime;

    if (_WindowType == GameWindowType::EditorWindow)
    {
        LoadRender(MainSceneData, false);
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
    UCodeGEStackFrame("Scene:Update");

    ImVec2 EditorWindowsize = { _Size.X , _Size.Y };
    if (EditorWindowsize.x <= 0 && EditorWindowsize.y <= 0) { return; }


    if (_IsRuningGame && !_IsGamePaused) { MainSceneData._GameRunTime->GameUpdate(); }




    if (MainSceneData._GameRunTime)
    {
        MainSceneData._GameRunTime->DestroyNullScenes();
        MainSceneData._GameRender->UpdateFrame();

        InputEmuation();

        if (_IsRuningGame && !MainSceneData._GameRunTime->Get_IsGameRuning())
        {
            UnLoadRender(MainSceneData);
            OnStopPlaying();
        }
    }

}
void GameEditorWindow::UnLoadRender(SceneEditorTabData& data)
{
    auto& _GameRender = data._GameRender;
    if (_GameRender)
    {
        _GameRender->EndRender();
        delete _GameRender.release();
        _GameRender = nullptr;
    }
}
void GameEditorWindow::UnLoadRunTime()
{
    auto& _GameRunTime = MainSceneData._GameRunTime;
    if (_GameRunTime)
    {
        UnLoadSceneCamera(MainSceneData);
        UnLoadRender(MainSceneData);

        _GameRunTime->EndRunTime();
        delete _GameRunTime.release();
        _GameRunTime = nullptr;
    }
}
void GameEditorWindow::LoadRunTime()
{
    auto& _GameRunTime = MainSceneData._GameRunTime;
    if (_GameRunTime == nullptr)
    {
        _GameRunTime = std::make_unique<UCode::GameRunTime>(Get_GameLibRef());
        _GameRunTime->Set_GameTime_FramesToDestroy(2);//Because The Inspector be able to deselect RunTimeObjects instead of reading freed memory.
        _GameRunTime->Init();

        if (_SceneData && _SceneDataAsRunTiime == nullptr)
        {
            auto ShowScene = UCode::Scene2dData::LoadScene(_GameRunTime.get(), *_SceneData);
            _SceneDataAsRunTiime = ShowScene;
        }
    }
}
void GameEditorWindow::LoadRender(SceneEditorTabData& data, bool MakeWin)
{
    auto _GameRunTime = data.GetGameRuntime();
    auto& _GameRender = data._GameRender;
    if (_GameRender == nullptr)
    {
        i32 Sx = (i32)_Size.X;
        i32 Sy = (i32)_Size.Y;
        if (Sx <= 0) { Sx = 640; }
        if (Sy <= 0) { Sy = 480; }

        _GameRender = std::make_unique <UCode::RenderAPI::Render>();
        auto Windata = UCode::RenderAPI::WindowData(Sx, Sy, "GameEditorWindow(ExternalWindow)");

        Windata.ImGui_Init = false;
        Windata.GenNewWindow = MakeWin;
        Windata.ImguiDrawIsMade = true;
        Windata.shared_window = glfwGetCurrentContext();

        _GameRender->Init(Windata, _GameRunTime);

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
    auto& _GameRunTime = MainSceneData._GameRunTime;
    auto& _GameRender = MainSceneData._GameRender;
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


