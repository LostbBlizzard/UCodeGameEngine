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
#include <EditorWindows/ProjectManagement/ProjectFilesWindow.hpp>

#include "Serialization.hpp"

#include "UEditorModules/UEditorModule.hpp"
#include "Helper/Tasks.hpp"
#include "UEditorModules/Modules/CodeModule.hpp"
#include "Helper/StringHelper.hpp"
#include "Helper/UserSettings.hpp"
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
Optional<String> ChangeLogtext;

void EditorAppCompoent::Init(const Path& projDir)
{
    UEditorModules::Init();

    UCode::AssetManager* AssetS = Get_AssetManager();
    _Loader.reset(new EditorAssetLoader());
    AssetS->Set_AssetLoader(_Loader.get());

    _Loader->Init(&_ProjectFiles, &_RunTimeProjectData, AssetS);

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

    if (std::filesystem::exists("changelog.md"))
    {
        ChangeLogtext = UCode::GameFiles::ReadFileAsString("changelog.md");

        std::filesystem::remove("changelog.md");
    }

    if (OpenProject(projDir).IsError())
    {
        NewEditorWindowData DataForWindow(this);
        MakeNewWindow(OpenProjectWindow, DataForWindow);
    }
}
EditorWindow* EditorAppCompoent::MakeNewWindow(const EditorWindowData& windata, NewEditorWindowData DataForWindow)
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
    UCodeGEStackFrame("EditorApp::CloseProject");
    SaveApp();
    _EditorWindows.clear();


    if (ActiveProjectLockFile.has_value())
    {
        ActiveProjectLockFile.value().close();
        ActiveProjectLockFile = {};
        std::filesystem::remove(ProjectManger::GetProjectLockPath(_RunTimeProjectData.Get_ProjectDir()));
    }

    _RunTimeProjectData.SetProjectToNull();

    // ImGui::ClearIniSettings();
}

Result<bool, String> EditorAppCompoent::OpenProject(const Path& ProjectDir)
{
    UCodeGEStackFrame("EditorApp::OpenProject");
    bool IsOpenInProject = _RunTimeProjectData.Is_ProjLoaded();
    if (IsOpenInProject)
    {
        EndProject();
    }


    ProjectData Data;
    auto ProjectDataPath = ProjectManger::GetProjectDataPath(ProjectDir);
    if (ProjectData::ReadFromFile(ProjectDataPath, Data))
    {
        auto lockpath = ProjectManger::GetProjectLockPath(ProjectDir);
        if (std::filesystem::exists(lockpath))
        {
            std::error_code errorcode = std::error_code();
            if (std::filesystem::remove(lockpath, errorcode) == false)
            {
                String error = "Project Is already Opened";
                return error;
            }
        }

        ActiveProjectLockFile = std::ofstream(lockpath);
        _RunTimeProjectData.SetProject(Data, ProjectDir, _ProjectFiles);

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
        String error = "Unable ToRead/Parse UProjectData.data";
        return error;
    }
}
void EditorAppCompoent::OnProjectLoadedPreWindows()
{
    auto GameLib = Get_EditorLibrary();
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
        ExtList.GetOrAdd(Ext, {}).push_back(&Item);
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

                ListToPush.push_back(std::move(Data));
            }
            Info.Ptr->FilesUpdated(ListToPush);
        }
        else
        {

        }
    }


    auto Modules = UEditorModules::GetModules();

    for (auto& fileitem : List)
    {
        if (fileitem.Type == ChangedFileType::FileRemoved)
        {

            for (size_t i = 0; i < Modules.Size(); i++)
            {
                auto& item = Modules[i];
                auto AssetDataList = item->GetAssetData();


                auto Info = item->GetAssetDataUsingExt(Path(fileitem.RelativePath).extension());
                if (Info.has_value())
                {
                    auto Data = AssetDataList[Info.value()];

                    auto fullpath = AssetDir.native() + fileitem.RelativePath.native();
                    Data->OnFileRemoved(fullpath);
                }

                auto v = fileitem.RelativePath.generic_string();
                Index.RemoveIndexFilesRelativePath(v);
            }
        }
        else if (fileitem.Type == ChangedFileType::FileAdded)
        {
            auto path = AssetDir.native() + fileitem.RelativePath.native();
            EditorIndex::IndexFile newfileIndex;

            Vector<GetSubAssetData> subassets;
            EditorIndex::UpdateFile(newfileIndex, path, fileitem.RelativePath.generic_string(), subassets);

            for (auto& Item : subassets)
            {
                EditorIndex::IndexFile newsubfileIndex;
                newsubfileIndex.RelativePath = newfileIndex.RelativePath;
                newsubfileIndex.RelativeAssetName = newfileIndex.RelativePath + EditorIndex::SubAssetSeparator + Item._SubAssetName;
                newsubfileIndex.UserID = Item._ID;

                Index._Files.push_back(std::move(newsubfileIndex));
            }
            Index._Files.push_back(std::move(newfileIndex));
        }
        else if (fileitem.Type == ChangedFileType::FileUpdated)
        {
            auto path = AssetDir.native() + fileitem.RelativePath.native();
            auto r = fileitem.RelativePath.generic_string();
            auto old = Index.FindFileRelativeAssetName(r);

            UCodeGEAssert(old.has_value());
            auto& oldv = old.value();

            Vector<GetSubAssetData> subassets;
            EditorIndex::UpdateFile(oldv, path, r, subassets);

            for (auto& Item : subassets)
            {
                auto find = Index.FindFileUsingID(Item._ID);
                if (find.has_value())
                {
                    auto& foundfile = find.value();
                    foundfile.RelativeAssetName = oldv.RelativePath + EditorIndex::SubAssetSeparator + Item._SubAssetName;
                }
            }

        }
    }



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
                if (UserSettings::GetSettings().IsKeybindActive(KeyBindList::Undo))
                {
                    Undo();
                }
                if (UserSettings::GetSettings().IsKeybindActive(KeyBindList::Redo))
                {
                    Redo();
                }
            }


        }
        if (GetInputMode() == KeyInputMode::Window && UserSettings::GetSettings().IsKeybindActive(KeyBindList::MenuBar)) {
            ImGui::FocusItem();
            SetToNormal();
        }
        if (ImGui::BeginMenu("File"))
        {

            StringView ProjText = IsOpenInProject ? StringView("Project Stuff") : StringView("No Project Is Loaded");

            ImGuIHelper::Text(ProjText);
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

                    auto v = OpenProject(ProjectPath);

                    if (v.IsError())
                    {
                        ProjectOpenError = v.GetError();
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
            auto str = "Ctrl+" + UserSettings::GetSettings().KeyBinds[(size_t)KeyBindList::Undo].ToString();
            if (ImGui::MenuItem("Undo", str.c_str(), nullptr, _Undos.size()))
            {
                Undo();
            }

            str = "Ctrl+" + UserSettings::GetSettings().KeyBinds[(size_t)KeyBindList::Redo].ToString();
            if (ImGui::MenuItem("Redo", str.c_str(), nullptr, _Redos.size()))
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

        if (ImGui::BeginMenu("Project", IsOpenInProject))
        {
            if (ImGui::BeginMenu("Assets"))
            {
                if (ImGui::MenuItem("Clear Cache"))
                {
                    std::filesystem::remove_all(_RunTimeProjectData.GetCachedDir());
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("UCodeLang"))
            {
                if (ImGui::MenuItem("Rebuild"))
                {
                    std::filesystem::remove_all(_RunTimeProjectData.GetULangIntDir());
                    std::filesystem::remove_all(_RunTimeProjectData.GetULangOutDir());

                    auto assetsdir = _RunTimeProjectData.GetAssetsDir();
                    auto tepfile = assetsdir / "ULangModule.ucm";

                    if (std::filesystem::exists(tepfile))
                    {
                        OnFileUpdated(this, "ULangModule.ucm", ChangedFileType::FileUpdated);
                    }
                }
                if (ImGui::MenuItem("Clear Cache"))
                {
                    std::filesystem::remove_all(_RunTimeProjectData.GetULangIntDir());
                }
                if (ImGui::MenuItem("Dump"))
                {

                }
                if (ImGui::MenuItem("Dump as IR"))
                {

                }
                if (ImGui::MenuItem("Dump as C"))
                {

                }
                if (ImGui::MenuItem("Type was Renamed"))
                {

                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Geting Started"))
            {

            }

            if (ImGui::MenuItem("Documentation"))
            {

            }

            if (ImGui::MenuItem("Forums"))
            {

            }

            if (ImGui::MenuItem("Bugreport/Issue"))
            {

            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("|", false))
        {

            ImGui::EndMenu();
        }

        String msg = "InputMode:";
        switch (_InputMode)
        {
        case UCodeEditor::KeyInputMode::Normal:
            msg += "Normal";
            break;
        case UCodeEditor::KeyInputMode::Window:
            msg += "Window";
            break;
        default:
            UCodeGEUnreachable();
            break;
        }

        if (ImGui::BeginMenu(msg.c_str(), false))
        {

            ImGui::EndMenu();
        }
        {
            auto& Tasks = RuningTasksInfo::Get_Tasks();
            if (Tasks.size())
            {

                for (auto& Item : Tasks)
                {
                    String TaskStr = "Task Name 1";
                    if (ImGui::BeginMenu(msg.c_str(), false))
                    {

                        ImGui::EndMenu();
                    }

                }

            }
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
    if (_RunTimeProjectData.Is_ProjLoaded())
    {
        SaveWindowsPref();

        for (auto& Item : _ProjectFiles._AssetFiles)
        {
            UEditorAssetFileSaveFileContext Context;
            Item._File->SaveFile(Context);
        }
        EditorIndex::ToFile(_RunTimeProjectData.Get_AssetIndex(), _RunTimeProjectData.GetAssetIndexPath());
    }
}
void EditorAppCompoent::OnAppEnded()
{

}


void EditorAppCompoent::LoadWindowsPref()
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

}
void EditorAppCompoent::SaveWindowsPref()
{
    const Path WinPrePath = _RunTimeProjectData.Get_ProjectPrefsDir().concat(WindowPrefData::FileName);
    const Path IniPrePath = _RunTimeProjectData.Get_ProjectPrefsDir().concat("Ini").concat(WindowPrefData::FileName);

    WindowPrefData data = WindowPrefData();

    auto TypeSerializer = USerializerType::YAML;
    USerializer SWindowData = USerializer(TypeSerializer);

    for (auto& Item : _EditorWindows)
    {
        WindowDataForWindowPrefData windata;

        windata._Windowid = Item->WindowName;
        windata._ImguiName = Item->_ImGuiName;

        Item->OnSaveWindow(SWindowData);
        SWindowData.ToString(windata._WindowData, true);

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
    UCodeGEStackFrame("EditorApp::FileUpdate");
    auto Info = UEditorModules::GetModuleFromFileExt(path.extension());

    auto Modules = UEditorModules::GetModules();

    if (Type == ChangedFileType::FileAdded)
    {
        auto AssetDir = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData()->GetAssetsDir();
        auto fullpath = AssetDir.native() + path.native();
        auto& Indexlist = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData()->Get_AssetIndex();

        EditorIndex::IndexFile Index;
        Vector<GetSubAssetData> subassets;
        EditorIndex::UpdateFile(Index, fullpath, path.generic_string(), subassets);

        for (auto& Item : subassets)
        {
            EditorIndex::IndexFile newsubfileIndex;
            newsubfileIndex.RelativePath = path.generic_string();
            newsubfileIndex.RelativeAssetName = newsubfileIndex.RelativePath + EditorIndex::SubAssetSeparator + Item._SubAssetName;
            newsubfileIndex.UserID = Item._ID;

            Indexlist._Files.push_back(std::move(newsubfileIndex));
        }
        Indexlist._Files.push_back(std::move(Index));
    }
    else if (Type == ChangedFileType::FileRemoved)
    {
        auto AssetDir = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData()->GetAssetsDir();
        auto relpath = path.generic_string();
        auto fullpath = AssetDir.native() + path.native();

        for (size_t i = 0; i < Modules.Size(); i++)
        {
            auto& item = Modules[i];
            auto AssetDataList = item->GetAssetData();

            auto Info = item->GetAssetDataUsingExt(path.extension());
            if (Info.has_value())
            {
                auto Data = AssetDataList[Info.value()];

                Data->OnFileRemoved(fullpath);
            }
        }


        EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData()->Get_AssetIndex().RemoveIndexFilesRelativePath(relpath);
    }

    if (Info.Index)
    {
        UEditorModule::FileUpdateData Data;
        Data.path = path;
        Data.type = Type;
        Info.Ptr->FileUpdate(Data);


        if (Type == ChangedFileType::FileUpdated)
        {
            auto& Files = EditorAppCompoent::GetCurrentEditorAppCompoent()->GetPrjectFiles();
            auto AssetDir = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData()->GetAssetsDir();
            auto runtime = EditorAppCompoent::GetCurrentEditorAppCompoent()->Get_RunTimeProjectData();

            for (auto& Item : Info.Ptr->GetAssetData())
            {

                if (Item->FileExtWithDot == path.extension().generic_string()
                    && Item->CallLiveingAssetsWhenUpdated)
                {
                    auto v = Files.FindAssetFile(AssetDir / path);
                    if (v.has_value())
                    {
                        auto& ItemV = Files._AssetFiles[v.value()];

                        ItemV._File->FileUpdated();
                    }
                }
            }

            auto vstr = path.generic_string();
            auto v = runtime->Get_AssetIndex().FindFileRelativeAssetName(vstr);

            Vector<GetSubAssetData> subassets;
            EditorIndex::UpdateFile(v.value(), AssetDir / path, vstr, subassets);

            for (auto& Item : subassets)
            {
                auto find = runtime->Get_AssetIndex().FindFileUsingID(Item._ID);
                if (find.has_value())
                {
                    auto& foundfile = find.value();
                    foundfile.RelativeAssetName = vstr + EditorIndex::SubAssetSeparator + Item._SubAssetName;
                }
            }


        }

    }


    for (auto& Item : EditorAppCompoent::GetCurrentEditorAppCompoent()->_EditorWindows)
    {
        auto v = dynamic_cast<ProjectFilesWindow*>(Item.get());
        if (v)
        {
            v->OnFileUpdate(path);
        }
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
    if (_Redos.size()) {
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
    UCode::UCodeRunTimeState::Set_Current(UCode::ULangRunTime::Get(Get_EditorLibrary()));

    if (_RunTimeProjectData.Is_ProjLoaded())
    {
        if (_AutoSaveTimer <= 0.0f)
        {
            _AutoSaveTimer = MaxAutoTimer;

            UCodeGEStackFrame("EditorApp::AutoSave");
            SaveApp();
            UCodeGELog("[ConsoleWindowSkip]:Auto Saved");
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

    if (ImGui::BeginPopupModal("ChangeLog"))
    {
        auto& changelog = ChangeLogtext.value();
        ImGuIHelper::Text(changelog);
        ImGui::EndPopup();
    }
    else
    {
        if (ChangeLogtext.has_value())
        {
            ImGui::OpenPopup("ChangeLog");
        }
    }
    {
        static bool isopen = false;

        bool v = true;
        if (ImGui::BeginPopupModal("Open Project Error", &v))
        {
            auto& changelog = ProjectOpenError.value();
            ImGuIHelper::Text(StringView(changelog));
            ImGui::EndPopup();

        }
        else
        {
            if (isopen)
            {
                ProjectOpenError = {};
                isopen = false;
            }
            if (ProjectOpenError.has_value())
            {
                ImGui::OpenPopup("Open Project Error");
                isopen = true;
            }
        }

    }

    if (_DropedFiles.has_value())
    {
        auto& files = _DropedFiles.value();

        bool open = true;
        const char* windowname = "droped files window";

        static bool OnDropedMovefile = false;
        static String Sreach = "";
        static String lastvalue = "";
        if (ImGui::BeginPopupModal(windowname, nullptr))
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

            String comboname = "Assets" + FileHelper::ToRelativePath(assets.parent_path(), lastvalue).generic_string();

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

            ImGui::Checkbox("Move files", &OnDropedMovefile);
            ImGui::SameLine();

            ImGui::BeginDisabled(!haspickeddir);

            if (ImGui::Button("Select"))
            {
                for (auto& Item : files)
                {
                    Path outpath = FileHelper::GetNewFileName(Path(lastvalue) / Path(Item).filename().generic_string());
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

    if (!ImGui::GetIO().WantTextInput)
    {
        if (UserSettings::GetSettings().IsKeybindActive(KeyBindList::Special))
        {
            _InputMode = KeyInputMode::Window;
        }
    }
    else
    {
        _InputMode = KeyInputMode::Normal;
    }


    UCodeGEStackFrame("EditorApp::Update");
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

