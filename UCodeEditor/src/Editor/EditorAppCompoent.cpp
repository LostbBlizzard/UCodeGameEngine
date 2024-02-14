#include "EditorAppCompoent.hpp"
#include <Imgui/imgui_internal.h>
#include <UCodeRunTime/CoreSystems/GameFiles.hpp>
#include <UCodeRunTime/ULibrarys/UCodeLang/ULangRunTime.hpp>
#include "EditorWindowsList.hpp"

#include <Helper/AppFiles.hpp>
#include <UCodeRunTime/ULibrarys/Serialization_Library.h>

#include <Helper/FileHelper.hpp>
#include <Helper/ImGuIHelper_Asset.hpp>
#include <EditorWindows/ProjectManagement/ExportProjectWindow.hpp>

#include "Serialization.hpp"

#include "UEditorModules/UEditorModule.hpp"
#include "Helper/Tasks.hpp"
#include "UEditorModules/Modules/CodeModule.hpp"
#include "Helper/StringHelper.hpp"
EditorStart


constexpr const float MaxAutoTimer = 30;
#define OpenProjectWindow EditorWindowsList::EditorWindows[1].GetType<EditorWindowData>()
EditorAppCompoent::EditorAppCompoent(UCode::Entity* entity) :
    UCode::Renderer2d(entity, nullptr),
    NextWindowId(0)
{
    AppFiles::Init(GetGameRunTime()->Get_Library_Edit()); 


    _This = this;
}

EditorAppCompoent::~EditorAppCompoent()
{
    EndEditor();
    _This = nullptr;
}

Optional< Unique_Bytes> bytes;

void EditorAppCompoent::Init(const Path& projDir)
{
    UEditorModules::Init();

    UCode::AssetManager* AssetS = Get_AssetManager();
    _Loader.reset(new EditorAssetLoader());
    AssetS->Set_AssetLoader(_Loader.get());

    _Loader->Init(&_RunTimeProjectData, AssetS);
    
    {
        auto FontBytes = AppFiles::ReadFileAsBytes("art/OpenSans-VariableFont_wdth,wght.ttf");
        auto& io = ImGui::GetIO();
        io.Fonts->AddFontFromMemoryTTF(FontBytes.Release(), FontBytes.Size(), 16.0f);
        io.FontDefault = io.Fonts->Fonts.back();
        io.Fonts->Build();
    }

    ImGuIHelper_Asset::AssetManager = AssetS;
    ImGuIHelper_Asset::ProjectData = Get_RunTimeProjectData(); 
   
    if (!std::filesystem::exists("imgui.ini"))
    {
        AppFiles::CopyFile("presets/imgui.ini", "imgui.ini");
        ImGui::LoadIniSettingsFromDisk("imgui.ini");
    }

    if (!OpenProject(projDir))
    {
        NewEditorWindowData DataForWindow(this);
        MakeNewWindow(OpenProjectWindow, DataForWindow);
    }
}
EditorWindow*  EditorAppCompoent::MakeNewWindow(const EditorWindowData& windata, NewEditorWindowData DataForWindow)
{
    EditorWindow* Game = windata.MakeWindow(DataForWindow);
    Game->WindowName = windata.GetName();
    Game->WindowId = GetNewWindowId();
    Game->_ImGuiName = windata.GetName() + (std::string)"##" + std::to_string(Game->WindowId);
    _EditorWindows.push_back(Unique_ptr<EditorWindow>(Game));
    return Game;
}
void  EditorAppCompoent::EndProject()
{
    SaveApp();
    _EditorWindows.clear();


    _RunTimeProjectData.SetProjectToNull();
   // ImGui::ClearIniSettings();
}

bool EditorAppCompoent::OpenProject(const Path& ProjectDir)
{
    bool IsOpenInProject = _RunTimeProjectData.Is_ProjLoaded();
    if (IsOpenInProject)
    {
        EndProject();
    }


    ProjectData Data;
    auto ProjectDataPath = ProjectManger::GetProjectDataPath(ProjectDir);
    if (ProjectData::ReadFromFile(ProjectDataPath, Data))
    {
        _RunTimeProjectData.SetProject(Data, ProjectDir,_ProjectFiles);

        OnProjectLoadedPreWindows();
       
        LoadWindowsPref();

        UCodeGELog("Project Loaded");
        //
        OnProjectLoaded();
        //
        return true;
    }
    else
    {
        return false;
    }
}
void EditorAppCompoent::OnProjectLoadedPreWindows()
{ auto GameLib = Get_EditorLibrary();
    UCode::GameFiles* gamefiles = gamefiles->Get(GameLib);
    
    
    //UCode::GameFilesData Newdata;
    //Newdata.SetRedirectDir(_RunTimeProjectData.GetGameLibDir());
    //gamefiles->ReInit(Newdata);

    UCode::ULangRunTime* SRuntime = UCode::ULangRunTime::Get(GameLib);

    SRuntime->OpenLibs(_RunTimeProjectData.GetULangOutDir());
}
void EditorAppCompoent::OnProjectLoaded()
{
    auto& Watcher = _RunTimeProjectData.Get_FileWatcher();
    Watcher.ThisPointer = this;
    Watcher.CallBack = OnFileUpdated;


    auto AssetDir = _RunTimeProjectData.GetAssetsDir();

    auto& Index = _RunTimeProjectData.Get_AssetIndex();
    auto List = Index.GetDiffFromDir(AssetDir);
    
    Unordered_map<PathString, Vector<EditorIndex::ChangedFile*>> ExtList;

    for (auto& Item : List)
    {
        auto Ext = Item.RelativePath.extension();
        ExtList[Ext].push_back(&Item);
    }


    for (auto& Item : ExtList) 
    {
        auto Info = UEditorModules::GetModuleFromFileExt(Item.first);

        if (Info.Index)
        {

            Vector<UEditorModule::FileUpdateData> ListToPush;
            for (auto& Item2 : Item.second)
            {
                UEditorModule::FileUpdateData Data;
                Data.path = AssetDir.native() + Item2->RelativePath.native();
                Data.type = Item2->Type;
            }
            Info.Ptr->FilesUpdated(ListToPush);
        }
    }
    CodeModule::BuildUCode(true);



    _AutoSaveTimer = MaxAutoTimer;
}
void  EditorAppCompoent::ShowMainMenuBar()
{

    if (ImGui::BeginMainMenuBar())
    {
        bool IsOpenInProject = _RunTimeProjectData.Is_ProjLoaded();
        ImVec2 ButtionSize = { 140,20 };
        
        {
            ImGuiIO& io = ImGui::GetIO();
            bool IsCtrlDown = io.KeyCtrl;
            if (io.KeyCtrl) 
            {
                if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Z))
                {
                    Undo();
                }
                if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Y))
                {
                    Redo();
                }
            }
        }

        if (ImGui::BeginMenu("File"))
        {
           

            const char* ProjText = IsOpenInProject ? "Project Stuff" : "No Project Is Loaded";

            ImGui::Text(ProjText);
            if (IsOpenInProject)
            {
                if (ImGui::MenuItem("Save Project"))
                {
                    SaveApp();
                }
                if (ImGui::MenuItem("Close Project"))
                {
                    EndProject();
                }
                if (ImGui::MenuItem("Export Project"))
                {
                    Get_Window<ExportProjectWindow>();
                }
            }

            ImGui::Separator();
            if (ImGui::MenuItem("Make New Project"))//Move This to OpenProjectWindow
            {
                NewEditorWindowData DataForWindow(this);
                MakeNewWindow(OpenProjectWindow, DataForWindow);
            }
            if (ImGui::MenuItem("Open Project"))
            {
                auto DirInfo = FileHelper::OpenDirFromUser();
                if (DirInfo.Result == FileHelper::OpenFileResult::OKAY)
                {
                    std::string ProjectPath = DirInfo.Path + '/';
                    if (!OpenProject(ProjectPath))
                    {
                        UCodeGELog("Failed To Open Project at " + ProjectPath);

                    }
                }

            }
          
            if (ImGui::MenuItem("Quit"))
            {
                StopEditor();
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit", IsOpenInProject))
        {
            if (ImGui::MenuItem("Undo","CTRL+Z",nullptr,_Undos.size()))
            {
                Undo();
            }
            if (ImGui::MenuItem("Redo","CTRL+Y",nullptr,_Redos.size()))
            {
                Redo();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Windows", IsOpenInProject))
        {
            if (ImGui::BeginMenu("Options"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Fullscreen", NULL, &_DockSpaceData.opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &_DockSpaceData.opt_padding);
                ImGui::Separator();

                if (ImGui::MenuItem("Flag: NoSplit", "", (_DockSpaceData.dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { _DockSpaceData.dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
                if (ImGui::MenuItem("Flag: NoResize", "", (_DockSpaceData.dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { _DockSpaceData.dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (_DockSpaceData.dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { _DockSpaceData.dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
                if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (_DockSpaceData.dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { _DockSpaceData.dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (_DockSpaceData.dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, _DockSpaceData.opt_fullscreen)) { _DockSpaceData.dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }



                //bool* p_open = nullptr;
                 //if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                   //  *p_open = false;

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Add New windows"))
            {

                String tep;
                Vector<bool> stack;
                for (size_t i = 0; i < EditorWindowsList::EditorWindows_Size; i++)
                {
                    auto& Item = EditorWindowsList::EditorWindows[i];
                   
                    if (auto item = Item.IfType<EditorWindowsList::NewMenu>())
                    {
                        tep = item->Name;
                        stack.push_back(ImGui::BeginMenu(tep.c_str()));
                    }
                    else if (Item.IsType<EditorWindowsList::EndMenu>())
                    {
                        if (stack.size() ? stack.back() == true : true) {
                            ImGui::EndMenu();
                        }
                    }
                    else if (auto val = Item.IfType<EditorWindowData>()) 
                    {
                        if (stack.size() ? stack.back() == true : true) 
                        {
                            if (ImGui::MenuItem(val->GetName().c_str()))
                            {
                                NewEditorWindowData Data = NewEditorWindowData(this);
                                MakeNewWindow(*val, Data);
                            }
                        }
                    }
                    else
                    {
                        UCodeGEUnreachable();
                    }
                }
                

                ImGui::EndMenu();
            }
            ImGui::Separator();
            for (auto& Window : _EditorWindows)
            {
                if (Window)
                {
                    if (ImGui::BeginMenu(Window->Get_ImGuiName().c_str()))
                    {
                        if (ImGui::MenuItem("Remove Window"))
                        {
                            ReMoveEditorWindow(Window.get());
                        }
                        ImGui::EndMenu();
                    }
                }
                
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}


void  EditorAppCompoent::BeginDockSpace(bool* p_open)
{
    // If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
    // In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
    // In this specific demo, we are not using DockSpaceOverViewport() because:
    // - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
    // - we allow the host window to have padding (when opt_padding == true)
    // - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
    // TL;DR; this demo is more complicated than what you would normally use.
    // If we removed all the options we are showcasing, this demo would become:
    //     void ShowExampleAppDockSpace()
    //     {
    //         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    //     }

    bool& opt_fullscreen = _DockSpaceData.opt_fullscreen;
    bool& opt_padding = _DockSpaceData.opt_padding;
    ImGuiDockNodeFlags& dockspace_flags = _DockSpaceData.dockspace_flags;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    //ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin(ImGUIDockName, p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID(ImGUIDockName);
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
}
void  EditorAppCompoent::SaveApp()
{
    if (_RunTimeProjectData.Is_ProjLoaded()) {
        SaveWindowsPref();
    }
}
void  EditorAppCompoent::OnAppEnded()
{

}


void  EditorAppCompoent::LoadWindowsPref()
{
    const Path WinPrePath = _RunTimeProjectData.Get_ProjectPrefsDir().concat(WindowPrefData::FileName);
    const Path IniPrePath = _RunTimeProjectData.Get_ProjectPrefsDir().concat("Ini").concat(WindowPrefData::FileName);



    WindowPrefData data;
    if (!WindowPrefData::ReadFromFile(WinPrePath, data))
    {
        if (!AppFiles::CopyFile("presets/WindowPrefData.Pref", WinPrePath))
        {
            UCodeGEError("fail to copying default window presets");
        }
        else
        {
            if (!WindowPrefData::ReadFromFile(WinPrePath, data))
            {
                UCodeGEError("default window presets is unable to be read from UFiles may be corrupted");
            }
        }

    }
    for (size_t i = 0; i < data._Windows.size(); i++)
    {
        const auto& Item = data._Windows[i];
        const EditorWindowData* window = nullptr;

        for (size_t i = 0; i < EditorWindowsList::EditorWindows_Size; i++)
        {
            if (EditorWindowsList::EditorWindows[i].IsType<EditorWindowData>()) 
            {
                auto& WItem = EditorWindowsList::EditorWindows[i].GetType<EditorWindowData>();

                if (WItem.GetName() == Item._Windowid)
                {
                    window = &WItem;
                    break;
                }
            }
        }
        if (window == nullptr) { continue; }

        UDeserializer Data;

        switch ((USerializerType)Item._WindowData[0])
        {
        case USerializerType::Bytes:
            Data.SetBytes(BytesView((Byte*)Item._WindowData.data() + 1, Item._WindowData.size() - 1));
            break;
        case USerializerType::YAML:
            Data.SetYAMLString(StringView((char*)Item._WindowData.data() + 1, Item._WindowData.size() - 1));
            break;
        default:
            UCodeGEUnreachable();
            break;
        }


        NewEditorWindowData NEData(this);
        auto* newwin = MakeNewWindow(*window, NEData);
        newwin->_ImGuiName = Item._ImguiName;
        newwin->OnLoadWindow(Data);
    }

    //ImGui::LoadIniSettingsFromDisk(IniPrePath);
}
void  EditorAppCompoent::SaveWindowsPref()
{
    const Path WinPrePath = _RunTimeProjectData.Get_ProjectPrefsDir().concat(WindowPrefData::FileName);
    const Path IniPrePath = _RunTimeProjectData.Get_ProjectPrefsDir().concat("Ini").concat(WindowPrefData::FileName);
   
    WindowPrefData data = WindowPrefData();

    auto TypeSerializer = USerializerType::Bytes;// _RunTimeProjectData.Get_ProjData()._SerializeType;
    //Text is too unstable
    USerializer SWindowData = USerializer(TypeSerializer);
        
    for (auto& Item : _EditorWindows)
    {
        WindowPrefData::WindowData windata;

        windata._Windowid = Item->WindowName;
        windata._ImguiName = Item->_ImGuiName;
        
        Item->OnSaveWindow(SWindowData);
        SWindowData.ToBytes(windata._WindowData,true);
        
        data._Windows.push_back(std::move(windata));

        
        SWindowData.Reset();
    }
    
    //ImGui::SaveIniSettingsToDisk(IniPrePath.c_str());
    if (!WindowPrefData::WriteToFile(WinPrePath, data, TypeSerializer))
    {
        UCodeGELog("Could not Save WindowPrefs");
    }
    
}
Path EditorAppCompoent::GetWindows_prefPath()
{
    return _RunTimeProjectData.Get_ProjectPrefsDir().concat("EditorWindows.").concat(FileExt::Prefs);
}
void EditorAppCompoent::OnFileUpdated(void* This, const Path& path, ChangedFileType Type)
{
    auto Info = UEditorModules::GetModuleFromFileExt(path.extension());

    if (Info.Index)
    {
        UEditorModule::FileUpdateData Data;
        Data.path = path;
        Data.type = Type;
        Info.Ptr->FileUpdate(Data);
    }

   
}
void EditorAppCompoent::Undo()
{
    if (_Undos.size()) {
        auto Undo = _Undos.back(); _Undos.pop_back();
        Undo._UndoCallBack(Undo);
        _Redos.push_back(Undo);
    }
}
void EditorAppCompoent::Redo()
{
    if (_Undos.size()) {
        auto Undo = _Redos.back();  _Redos.pop_back();
        Undo._RedoCallBack(Undo);
    }
}
void  EditorAppCompoent::EndDockSpace()
{
    ImGui::End();//EndDockSpace
}

void EditorAppCompoent::OnFilesDropedOnWindow(Vector<StringView> filespaths)
{
    if (_RunTimeProjectData.Is_ProjLoaded())
    {
        if (!_DropedFiles.has_value())
        {
            Vector<String> tep;
            tep.resize(filespaths.size());

            for (size_t i = 0; i < filespaths.size(); i++)
            {
                tep[i] = String(filespaths[i]);
            }

            _DropedFiles = std::move(tep);
        }
    }
}

DontWaitInputKey EditorAppCompoent::AddDontWaitForInput()
{
    auto newval = _NextDontWaitKey;
    _NextDontWaitKey++;

    if (_ListDontWaitKeys.size() == 0)
    {
        WaitForInput(false);
    }

    _ListDontWaitKeys.push_back(newval);



    return newval;
}

void EditorAppCompoent::RemoveWaitForInput(DontWaitInputKey key)
{
    for (size_t i = 0; i < _ListDontWaitKeys.size(); i++)
    {
        auto& Item = _ListDontWaitKeys[i];

        if (Item == key)
        {
            _ListDontWaitKeys.erase(_ListDontWaitKeys.begin() + i);
            
            if (_ListDontWaitKeys.size() == 0)
            {
                WaitForInput(true);
            }
            
            return;
        }
    }
    UCodeGEUnreachable();
}

void EditorAppCompoent::OnDraw()
{
    
    if (_RunTimeProjectData.Is_ProjLoaded()) 
    {
        if (_AutoSaveTimer <= 0.0f)
        {
            _AutoSaveTimer = MaxAutoTimer;
            SaveApp();
            UCodeGELog("Auto Saved");
        }
        else
        {
            _AutoSaveTimer -= GetGameRunTime()->Get_GameTime().UpateDelta;
        }
    }

    if (_RunTimeProjectData.Is_ProjLoaded())
    {
        _RunTimeProjectData.UpdateFileWatcher();
    }
    _ProjectFiles.Update(GetGameRunTime()->Get_GameTime().UpateDelta);

    //

    ShowMainMenuBar();

   

    bool Hi = true;
    BeginDockSpace(&Hi);

    if (_DropedFiles.has_value())
    {
        auto& files = _DropedFiles.value();

        bool open = true;
        const char* windowname = "droped files window";

        static bool OnDropedMovefile = false;
        static String Sreach = "";
        static String lastvalue = "";
        if (ImGui::BeginPopupModal(windowname,nullptr))
        {
            auto assets = _RunTimeProjectData.GetAssetsDir();
            Vector<String> filepaths;
            {
                
                std::filesystem::recursive_directory_iterator it(assets);

                // Iterate over all directories and print their names
                for (const auto& entry : it) {
                    if (entry.is_directory()) {
                       filepaths.push_back(entry.path().generic_string());
                    }
                }

                filepaths.push_back(assets.generic_string());
            }

            ImGuIHelper::InputText("Search", Sreach);
            ImGui::Separator();
            
            String comboname = "Assets" + FileHelper::ToRelativePath(assets.parent_path(),lastvalue).generic_string();
                    
            if (ImGui::BeginCombo("Select Directory", comboname.c_str(), ImGuiComboFlags_NoArrowButton))
            {
                for (size_t i = 0; i < filepaths.size(); i++)
                {
                    const auto& Item = filepaths[i];

                    String name = "Assets" + FileHelper::ToRelativePath(assets.parent_path(), (Path)Item).generic_string();
                    
                    if (StringHelper::Fllter(Sreach, name))
                    {
                    
                        bool is_selected = (Item == lastvalue);

                        if (ImGui::Selectable(name.c_str(), is_selected))
                        {
                            lastvalue = Item;
                        }
                        if (is_selected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::Separator();

            bool haspickeddir = lastvalue.size() != 0;

            ImGui::Checkbox("Move files",&OnDropedMovefile);
            ImGui::SameLine();

            ImGui::BeginDisabled(!haspickeddir);
            
            if (ImGui::Button("Select"))
            {
                for (auto& Item : files)
                {
                    Path outpath = FileHelper::GetNewFileName(assets.generic_string() + Path(Item).filename().generic_string());
                    if (OnDropedMovefile)
                    {
                        std::filesystem::rename(Item, outpath);
                    }
                    else
                    {
                        std::filesystem::copy_file(Item, outpath);
                    }
                }

                _DropedFiles = {};
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndDisabled();
            ImGui::SameLine();

            if (ImGui::Button("Close")) 
            {
                ImGui::CloseCurrentPopup();
            }


            ImGui::EndPopup();
        }
        else
        {
            ImGui::OpenPopup(windowname);
        }
    }


    {
        auto& Tasks = RuningTasksInfo::Get_Tasks();
        if (Tasks.size())
        {
            if (ImGui::Begin("Runing Tasks"))
            {
                ImGui::Text("Task Name 1");
                ImGui::Text("Task Name 2");
                ImGui::Text("Task Name 3");

                ImGui::End();
            }
        }
    }

    ShowEditiorWindows();


    EndDockSpace();


   
}
void  EditorAppCompoent::ShowEditiorWindows()
{
    for (size_t i = 0; i < _EditorWindows.size(); i++)
    {
        auto Item = _EditorWindows[i].get();
        if (!Item->_SetUp)
        {
            Item->_SetUp = true;
            Item->SetUp();
        }

        Item->WindowStart();



        if (Item->Get_IsOpen()) {
            Item->UpdateWindow();
        }
        Item->WindowEnd();


    }

    for (auto it = _EditorWindows.begin(); it != _EditorWindows.end();)
    {
        auto& Item = *it;
        if (Item->Get_IsClosed())
        {
            it = _EditorWindows.erase(it);
            continue;
        }
        it++;
    }
}

void  EditorAppCompoent::EndEditor()
{
    OnAppEnded();
    EndProject();
}

void  EditorAppCompoent::ReMoveEditorWindow(EditorWindow* window)
{
    for (auto it = _EditorWindows.begin(); it != _EditorWindows.end();)
    {
        auto& Item = *it;
        if (Item.get() == window)
        {
            it = _EditorWindows.erase(it);
            return;
        }


        it++;
    }
}

EditorEnd

