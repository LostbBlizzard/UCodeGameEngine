#include "ImGui.hpp"
#include "ProjectFilesWindow.hpp"
#include <fstream>
#include <Helper/FileHelper.hpp>

#include "Imgui/imgui_internal.h"
#include "Helper/ImGuIHelper.hpp"
#include <UCodeRunTime/ULibrarys/AssetManagement/SceneData.hpp>
#include <EditorWindows/DragAndDropNames.hpp>
#include <EditorWindows/OtherTypes/RawEntityData.hpp>
#include <EditorWindows/OtherTypes/ImageData.hpp>
#include "Plugin/UPlugin.hpp"
#include <EditorWindows/OtherTypes/ColorData.hpp>
#include <EditorWindows/EditorStrings.hpp>

#include <Editor/EditorAppCompoent.hpp>
#include <UCodeLang/LangCore/FileHelper.hpp>

#include "stb_image_write/stb_image_write.h"
#include <Helper/StringHelper.hpp>
#include "../../ULang/UCompiler.hpp"
#include "UCodeRunTime/ULibrarys/UCodeLang/ULangRunTime.hpp"
#include "UCodeRunTime/ULibrarys/UCodeLang/ScirptableObject.hpp"
#include "DrawMenu.hpp"
#include "Helper/Fuzzhelper.hpp"
#include "Helper/UserSettings.hpp"
EditorStart
namespace fs = std::filesystem;

void ProjectFilesWindow::DirectoryViewerOpenFile(DirectoryViewer& From, void* Ptr, const Path& Path)
{
    ProjectFilesWindow* ptr = (ProjectFilesWindow*)Ptr;
    ptr->_LookingAtDir = Path.parent_path().native() + Path::preferred_separator;
    ptr->UpdateDir();
}
ProjectFilesWindow::ProjectFilesWindow(const NewEditorWindowData& windowdata) :EditorWindow(windowdata)
{
    Viewer.SetCallBackPtr(this);
    Viewer.SetOpenPathCallback(DirectoryViewerOpenFile);
    Viewer.SetShowFindFiles(false);
}
ProjectFilesWindow::~ProjectFilesWindow()
{

}

#define ColorSpiteName "NewColorSprite"
static bool OpenPopUp = false;

#define ColorEdit "UpdateColor"
static bool ColorEditPopUp = false;
static String ColorEditFilePath;
static UCode::Color32* ColorObject = nullptr;

void ProjectFilesWindow::OnFileUpdate(const Path& path)
{
    bool islookatdir = false;

    Path newpath = "Assets";
    newpath /= path.native();

    if (newpath.native().size() > _LookingAtDirReadable.native().size())
    {
        islookatdir = UCode::StringHelper::StartWith<char>(newpath.generic_string(), _LookingAtDirReadable.generic_string());

    }
    if (islookatdir)
    {
        UpdateDir();
    }
}

Optional<Path> GetAssetMetaFileExt(const Path& Ext)
{

    auto Modules = UEditorModules::GetModules();


    for (size_t i = 0; i < Modules.Size(); i++)
    {
        auto& item = Modules[i];
        auto AssetDataList = item->GetAssetData();

        auto Info = item->GetAssetDataUsingExt(Ext);
        if (Info.has_value())
        {
            auto Data = AssetDataList[Info.value()];
            return Data->FileMetaExtWithDot;
        }
    }
    return {};
}
Optional<Path> GetAssetMetaFilePath(const Path& from)
{

    auto metaf = GetAssetMetaFileExt(from.extension());
    if (metaf.has_value())
    {
        Path filepath =Path(from).native() + PathString(metaf.value());
        if (std::filesystem::exists(filepath)) {
            return filepath;
        }
    }
    return {};
}
void RenameAssetFile(const Path& from, const Path& to)
{
    std::filesystem::rename(from, to);
    auto v = GetAssetMetaFilePath(from);
    if (v.has_value())
    {
        Path outpath = Path(to).native() + Path(v.value()).extension().native();
        std::filesystem::rename(v.value(),outpath);
    }
}

NullablePtr<ProjectFiles::AssetFile> ProjectFilesWindow::TryLoadAsset(Path fullpath)
{
    auto Modules = UEditorModules::GetModules();

    Path FileExt = fullpath.extension();
    bool ShowOthers = true;
    bool FoundIt = false;

    auto& ProjectFiles = Get_ProjectFiles();

    NullablePtr<ProjectFiles::AssetFile> r;
 
    for (size_t i = 0; i < Modules.Size(); i++)
    {
        auto& item = Modules[i];
        auto AssetDataList = item->GetAssetData();

        auto Info = item->GetAssetDataUsingExt(FileExt);
        if (Info.has_value())
        {
            auto Data = AssetDataList[Info.value()];
            FoundIt = true;
            if (Data->CanHaveLiveingAssets)
            {
                auto B = ProjectFiles.FindAssetFile(fullpath);

                if (!B.has_value())
                {
                    auto Ptr = Data->GetMakeNewAssetFile();
                    Ptr->FileFullPath = fullpath;
                    UEditorAssetFileInitContext InitContext;
                    Ptr->Init(InitContext);


                    ProjectFiles::AssetFile tep1;
                    tep1.Set(std::move(Ptr));
                    tep1.LastUsed = ProjectFiles::AssetFileMaxLastUsed;

                    ProjectFiles._AssetFiles.push_back(std::move(tep1));
                    r = &ProjectFiles._AssetFiles.back();
                }
                else
                {
                    r = &ProjectFiles._AssetFiles[B.value()];
                }
            }
        }
    }
    return r;
}
NullablePtr<UEditorAssetFileData> ProjectFilesWindow::GetAssetData(Path fullpath)
{
    auto Modules = UEditorModules::GetModules();
    Path FileExt = fullpath.extension();
    bool ShowOthers = true;
    bool FoundIt = false;

    auto& ProjectFiles = Get_ProjectFiles();

    NullablePtr<ProjectFiles::AssetFile> r;
 
    for (size_t i = 0; i < Modules.Size(); i++)
    {
        auto& item = Modules[i];
        auto AssetDataList = item->GetAssetData();

        auto Info = item->GetAssetDataUsingExt(FileExt);
        if (Info.has_value())
        {
            auto Data = AssetDataList[Info.value()];

            return Nullableptr(Data);
        }
    }
    return {};
}
void ProjectFilesWindow::UpdateWindow()
{
    if (!_LookingAtDir.has_value())
    {
        _LookingAtDir = Get_ProjectData()->GetAssetsDir();
        UpdateDir();
    }

    auto window = ImGui::GetCurrentWindow();

    auto& settings = UserSettings::GetSettings();
    /*
    if (Get_App()->GetInputMode() == KeyInputMode::Window)
    {
        if (settings.IsKeybindActive(KeyBindList::FilesWindow)) 
        {
            ImGui::SetWindowFocus();
			Get_App()->SetToNormal();
        }
    }
    */

    window->Flags |= ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar;


    if (_LookingAtDir.has_value())
    {
        static bool iswindowfocused = false;
        {
            const auto AssetsDir = Get_ProjectData()->GetAssetsDir();
            bool CantGoBack = _LookingAtDir == AssetsDir;



            ImGui::BeginDisabled(CantGoBack);
            if (ImGui::Button("Back") 
                || (iswindowfocused && settings.IsKeybindActive(KeyBindList::Alternative) && !CantGoBack))
            {
                if (_LookingAtAssetForSubAssets.has_value())
                {
                    _LookingAtAssetForSubAssets = {};
                    UpdateDir();
                }
                else
                {
                    Path oldpath = _LookingAtDir.value();
                    _LookingAtDir = oldpath.parent_path().parent_path().generic_u8string() + '/';
                    UpdateDir();
                }
            }
            ImGui::EndDisabled();

            ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_ItemSpacing, { 2,4 });

            ImGui::SameLine();

            iswindowfocused = ImGui::IsWindowFocused();

            ImGui::PushID(&_LookingAtDir);
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);

            bool v = ImGuIHelper::InputText("", _FindFileName);

            if (settings.IsKeybindActive(KeyBindList::Search))
            {
                ImGui::FocusItem();
            }

            ImGui::PopItemWidth();
            ImGui::PopID();

            if (v)
            {
                Viewer.SetFindString(_FindFileName);
                UpdateDir();
            }
            auto str = _LookingAtDirReadable.generic_string();

            static Vector<StringView> list;
            StringHelper::Split(str, "/", list);
            String itemstr;

            Optional<size_t> ClickedIndex;
            for (size_t i = 0; i < list.size(); i++)
            {
                auto& Item = list[i];
                itemstr = Item;

                ImGui::PushID(&Item);

                auto s = ImGui::CalcTextSize(itemstr.c_str());
                auto roundsize = 10;
                bool onclick = ImGui::Button(itemstr.c_str(), ImVec2(std::ceil(s.x / roundsize) * roundsize + 10, 0));
                ImGui::PopID();

                if (i + 1 < list.size())
                {
                    ImGui::SameLine();
                }

                if (onclick)
                {
                    ClickedIndex = i;
                }
            }
            if (_LookingAtAssetForSubAssets.has_value())
            {
                ImGui::SameLine();
                String itemstr2 = "~" + _LookingAtAssetForSubAssets.value().generic_string();
                auto s = ImGui::CalcTextSize(itemstr2.c_str());
                auto roundsize = 10;

                ImGui::PushID(&_LookingAtAssetForSubAssets.value());

                ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_::ImGuiCol_FrameBgActive]);
                bool onclick = ImGui::Button(itemstr2.c_str(), ImVec2(std::ceil(s.x / roundsize) * roundsize + 10, 0));
                ImGui::PopStyleColor();
                ImGui::PopID();
            }
            ImGui::PopStyleVar();

            if (ClickedIndex.has_value())
            {
                if (_LookingAtAssetForSubAssets.has_value())
                {
                    _LookingAtAssetForSubAssets = {};
                }
                Path oldpath = _LookingAtDir.value();
                for (size_t i = 0; i < (list.size() - ClickedIndex.value() - 1); i++)
                {
                    oldpath = oldpath.parent_path().parent_path().generic_u8string() + '/';
                }

                _LookingAtDir = oldpath;
                UpdateDir();
            }

            ImGui::Separator();
        }
        //ImGui::Columns(2);
        //ShowExlorer();

        ImGui::NextColumn();
        {
            auto ContentSize = ImGui::GetContentRegionAvail();
            auto Pos = ImGui::GetCursorPos();

            ImGui::SetCursorPos(Pos);



            ImGui::BeginChild(ImGui::GetID("FilesStuff"), ContentSize);

            ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0,0,0,0 });
            ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 0,0,0,0 });
            ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 1,1,1,0.1 });

            if (_LookingAtAssetForSubAssets.has_value())
            {
                auto& files = Get_ProjectFiles()._AssetFiles;
                auto& index = Get_App()->Get_RunTimeProjectData()->Get_AssetIndex();

                
                ProjectFiles::AssetFile* myassetfile = nullptr;
                static Path currentpath;
                currentpath = _LookingAtDir.value().native() + _LookingAtAssetForSubAssets.value().native();
                
                myassetfile = TryLoadAsset(currentpath).value_unchecked(); 
                

                if (myassetfile == nullptr)
                {
                    _LookingAtAssetForSubAssets = {};
                }
                else
                {

                    if (!myassetfile->_ManageFile.Get_Value()->HasSubAssets)
                    {

                        _LookingAtAssetForSubAssets = {};
                    }
                    else
                    {
                        auto val = myassetfile->_ManageFile.Get_Value();
                        auto buttioninfo = GetButtionSize();

                        ImVec2 CeSize = { buttioninfo.thumbnail.X,buttioninfo.thumbnail.Y };
                        float cellSize = buttioninfo.cellSize;

                        float panelWidth = ImGui::GetContentRegionAvail().x;
                        i32 columnCount = (i8)(panelWidth / cellSize);
                        if (columnCount < 1) {
                            columnCount = 1;
                        }

                        ImGui::Columns(columnCount, 0, false);



                        UEditorDrawSubAssetContext context;
                        context._ManageFile = AnyManagedPtr::As(myassetfile->_ManageFile);
                        context.ButtionSize = { CeSize.x,CeSize.y };
                        context.OnDoneDrawingAssetButton = [](UEditorDrawSubAssetContext::DoneDraw& context)
                            {

                                if (ImGui::IsItemFocused() && !ImGui::GetIO().WantTextInput)
                                {
                                    auto& settings = UserSettings::GetSettings();


                                    if (settings.IsKeybindActive(KeyBindList::Inspect))
                                    {

                                    }
                                    if (settings.IsKeybindActive(KeyBindList::Rename))
                                    {
                                        if (context.OnAssetRename.has_value())
                                        {

                                        }

                                    }
                                    if (settings.IsKeybindActive(KeyBindList::Copy))
                                    {
                                        Path subassetpath = currentpath.native() + Path(EditorIndex::SubAssetSeparator).native() + Path(context.AssetName).native();
                                        USerializer V(USerializerType::YAML);
                                        V.Write("UData", subassetpath);
                                        V.Write("UType", "AssetPath");

                                        auto copytext = V.Get_TextMaker().c_str();

                                        ImGui::SetClipboardText(copytext);

                                    }
                                    if (settings.IsKeybindActive(KeyBindList::Delete))
                                    {
                                        if (context.OnDestroy.has_value())
                                        {
                                            (*context.OnDestroy)();
                                        }

                                    }
                                }

                                if (ImGuIHelper::BeginPopupContextItem("Test"))
                                {
                                    ImGuIHelper::Text(StringView("File Options"));
                                    ImGui::Separator();

                                    auto& settings = UserSettings::GetSettings();

                                    auto str = settings.KeyBinds[(size_t)KeyBindList::Inspect].ToString();

                                    str = settings.KeyBinds[(size_t)KeyBindList::Inspect].ToString();
                                    if (ImGui::MenuItem("Inspect", str.c_str(), nullptr) || settings.IsKeybindActive(KeyBindList::Inspect))
                                    {


                                        ImGui::CloseCurrentPopup();
                                    }

                                    str = settings.KeyBinds[(size_t)KeyBindList::Rename].ToString();
                                    if (ImGui::MenuItem("Rename", str.c_str(), nullptr, context.OnAssetRename.has_value()) || settings.IsKeybindActive(KeyBindList::Inspect))
                                    {

                                        ImGui::CloseCurrentPopup();
                                    }

                                    str = settings.KeyBinds[(size_t)KeyBindList::Copy].ToString();
                                    if (ImGui::MenuItem("Copy", str.c_str(), nullptr, context.OnAssetRename.has_value()) || settings.IsKeybindActive(KeyBindList::Copy))
                                    {
                                        Path subassetpath = currentpath.native() + Path(EditorIndex::SubAssetSeparator).native() + Path(context.AssetName).native();
                                        USerializer V(USerializerType::YAML);
                                        V.Write("UData", subassetpath);
                                        V.Write("UType", "AssetPath");

                                        auto copytext = V.Get_TextMaker().c_str();

                                        ImGui::SetClipboardText(copytext);
                                        ImGui::CloseCurrentPopup();
                                    }

                                    str = settings.KeyBinds[(size_t)KeyBindList::Delete].ToString();
                                    if (ImGui::MenuItem("Delete", str.c_str(), nullptr, context.OnDestroy.has_value()) || settings.IsKeybindActive(KeyBindList::Inspect))
                                    {
                                        if (context.OnDestroy.has_value())
                                        {
                                            (*context.OnDestroy)();
                                        }

                                        ImGui::CloseCurrentPopup();
                                    }

                                    ImGui::EndPopup();
                                }

                               
                                auto App = EditorAppCompoent::GetCurrentEditorAppCompoent();
                                if (App->GetInputMode() == KeyInputMode::Window)
                                {
                                    if (UserSettings().GetSettings().IsKeybindActive(KeyBindList::FilesWindow))
                                    {
                                        ImGui::FocusItem();
                                        App->SetToNormal();
                                    }
                                }

                                if (false)
                                {

                                }
                                else
                                {
                                    ImGuIHelper::Text(context.AssetName);
                                }
                                ImGui::NextColumn();

                            };
                        val->DrawSubAssets(context);


                        ImGui::Columns(1);
                    }
                }
            }
            else 
            {
                ShowFileCells();
            }

            if (ImGui::IsWindowFocused())
            {
                iswindowfocused = true;
            }

            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::EndChild();
 
            ShowDirButtions();
            


            if (ImGui::BeginDragDropTarget())
            {
                ImGuiDragDropFlags target_flags = 0;
                target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DragAndDropType_Entity2dType, target_flags))
                {
                    const String _String = "Move Entity To " + _LookingAtDirReadable.generic_string();

                    ImGuIHelper::Text(_String);
                    const UCode::Entity* DropItem = *(UCode::Entity**)payload->Data;
                    if (payload->IsDelivery())
                    {
                        RawEntityData Data(DropItem);

                        const String EntityName = DropItem->NativeName().size() ? DropItem->NativeName() : UnNamedEntity;

                        Path path = FileHelper::GetNewFileName(Path(_LookingAtDir.value().native() + Path(EntityName).native()), Path(RawEntityData::FileExtDot));

                        RawEntityData::WriteToFile(path, Data);
                        UpdateDir();
                    }

                }

                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DragAndDropType_Scene2dType, target_flags))
                {
                    const String _String = "Save Scene To " + _LookingAtDirReadable.generic_string();

                    ImGuIHelper::Text(_String);
                    UCode::RunTimeScene* DropItem = *(UCode::RunTimeScene**)payload->Data;
                    if (payload->IsDelivery())
                    {
                        auto SaveType = Get_ProjectData()->Get_ProjData()._SerializeType;
                        UCode::Scene2dData Data;
                        UCode::Scene2dData::SaveScene(DropItem, Data, SaveType);

                        auto newuid = Get_ProjectData()->GetNewUID();
                        Data._UID = newuid;
                        DropItem->Get_UID() = newuid;

                        const String _SceneName = DropItem->Get_Name().size() ? DropItem->Get_Name() : UnNamedScene;


                        Path path = FileHelper::GetNewFileName(_LookingAtDir.value().native() + Path(_SceneName).native(), Path(UCode::Scene2dData::FileExtDot));
                        UCode::Scene2dData::ToFile(path, Data, SaveType);
                        UpdateDir();
                    }

                }
                ImGui::EndDragDropTarget();
            }
        }



    } //popups


    {
        static UCode::Color Color;
        static String FileName;
        if (OpenPopUp)
        {
            OpenPopUp = false;
            FileName = "New Color";
            ImGui::OpenPopup(ColorSpiteName);
        }
        if (ImGui::BeginPopup(ColorSpiteName))
        {

            ImGuIHelper::Text(StringView("Color Sprite"));
            ImGuIHelper::InputText("File Name", FileName);
            ImGui::ColorPicker4("Color", &Color.R);

            bool MakeColor = false;
            bool MakeSprite = false;
            if (ImGui::Button("Create as Color"))
            {
                MakeColor = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Create as Sprite"))
            {
                MakeSprite = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Create Sprite and Color"))
            {
                MakeSprite = true;
                MakeColor = true;
            }

            if (MakeSprite)
            {
                Path NewPath = FileHelper::GetNewFileName(_LookingAtDir.value().native() + Path(FileName).native(), ".png");
                UCode::Color32 Bytes = (UCode::Color32)Color;
                constexpr auto CHANNEL_NUM = 4;
                constexpr auto width = 1;
                constexpr auto height = 1;
                stbi_write_png(NewPath.generic_string().c_str(), width, height, CHANNEL_NUM, &Bytes, width * CHANNEL_NUM);
            }
            if (MakeColor)
            {
                ColorData Data;
                Data._Color = Color;

                Path NewPath = FileHelper::GetNewFileName(_LookingAtDir.value().native() + Path(FileName).native(), (UCode::String)ColorData::FileExtDot);
                ColorData::WriteToFile(NewPath, Data, Get_ProjectData()->Get_ProjData()._SerializeType);
            }
            if (MakeSprite || MakeColor)
            {
                UpdateDir();
                ImGui::CloseCurrentPopup();
            }


            ImGui::EndPopup();
        }
    }
    {

        static UCode::Color C;
        if (ColorEditPopUp)
        {
            ColorEditPopUp = false;
            C = (UCode::Color)*ColorObject;
            ImGui::OpenPopup(ColorEdit);
        }
        if (ImGui::BeginPopup(ColorEdit))
        {

            ImGui::ColorPicker4("Color", &C.R);


            if (ImGui::Button("Update Color"))
            {
                *ColorObject = (UCode::Color32)C;
                ColorData Data;
                Data._Color = C;
                ColorData::WriteToFile(ColorEditFilePath, Data, Get_ProjectData()->Get_ProjData()._SerializeType);

                UpdateDir();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
}
void ProjectFilesWindow::ShowFileCells()
{ 
    auto buttioninfo =GetButtionSize();

    ImVec2 CeSize = { buttioninfo.thumbnail.X,buttioninfo.thumbnail.Y };
    float cellSize = buttioninfo.cellSize;
    float panelWidth = ImGui::GetContentRegionAvail().x;
    i32 columnCount = (i8)(panelWidth / cellSize);
    if (columnCount < 1) {
        columnCount = 1;
    }

    ImGui::Columns(columnCount, 0, false);

    auto findname = _FindFileName;
    if (findname.size())
    {
        if (findname[0] == ':')
        {
            findname = findname.substr(1);
        }
    }
    for (auto& Item : _Files)
    {
        if (Item.FullFileName.extension() != Path(UEditorAssetFileData::DefaultMetaFileExtWithDot))
        {
            if (StringHelper::Fllter(findname, Item.FileName))
            {
                if (DrawFileItem(Item, CeSize)) { break; }
                if (Get_App()->GetInputMode() == KeyInputMode::Window)
                {
                    if (UserSettings().GetSettings().IsKeybindActive(KeyBindList::FilesWindow))
                    {
                        ImGui::FocusItem();
                        Get_App()->SetToNormal();
                    }
                }
                ImGui::NextColumn();
            }
        }
    }

    ImGui::Columns(1);

    auto ContentSize2 = ImGui::GetContentRegionAvail();
    if (ContentSize2.x > 0 && ContentSize2.y > 0) {
        //ImGui::InvisibleButton("##", ContentSize2);
    }
}
void ProjectFilesWindow::ShowExlorer()
{
    //Make Sure This and ShowEditor Use The Same Code.
    fs::directory_options opt = fs::directory_options::skip_permission_denied;
    auto path = Get_ProjectData()->GetAssetsDir();

    Viewer.SetPath(path);
    Viewer.Draw();
}

Optional<String> CanPasteShowDirButtionsPaste()
{
    auto clipboard = ImGui::GetClipboardText();

    Optional<String> PasteType;
    YAML::Node tep;
    if (clipboard)
    {
        bool ok = true;
        try
        {
            tep = YAML::Load(clipboard);
        }
        catch (YAML::ParserException ex)
        {
            ok = false;
        }
        if (ok && tep.IsMap())
        {
            bool isclipboardentity = (bool)tep["UType"];
            if (isclipboardentity)
            {
                if ((bool)tep["UData"])
                {
                    auto  t = tep["UType"].as<String>("");

                    if (t == "AssetPath")
                    {
                        PasteType = t;
                    }
                }
            }
        }
    }
 
    return PasteType;
}

void ProjectFilesWindow::ShowDirButtionsPaste()
{
    bool ctrl = ImGui::IsKeyDown(ImGuiMod_Ctrl);

    auto p = CanPasteShowDirButtionsPaste().value();

    if (p == "AssetPath")
    {
        auto clipboard = ImGui::GetClipboardText();
        YAML::Node tep = YAML::Load(clipboard);

        auto str = tep["UData"].as<Path>();

        if (std::filesystem::exists(str))
        {
            auto& dir = _LookingAtDir.value();
            auto OutPath =FileHelper::GetNewFileName(dir / Path(str).filename().replace_extension(""), str.extension());
            if (ctrl)
            {
                RenameAssetFile(str, OutPath);
            }
            else
            {
                std::filesystem::copy(str, OutPath);
            }
        }
    }
}
void ProjectFilesWindow::ShowDirButtions()
{

    if (Get_App()->GetInputMode() == KeyInputMode::Window)
    {
        if (UserSettings::GetSettings().IsKeybindActive(KeyBindList::FilesWindow))
        {
            ImGui::SetWindowFocus();
            Get_App()->SetToNormal();
        }
    }
    if (!ImGui::GetIO().WantTextInput && (ImGui::IsItemFocused() || (_Files.size() == 0 && ImGui::IsWindowFocused())))
    {
        auto& userseting = UserSettings::GetSettings();
        if (userseting.IsKeybindActive(KeyBindList::Paste))
        {

            Optional<String> PasteType = CanPasteShowDirButtionsPaste();
            if (PasteType.has_value())
            {
                ShowDirButtionsPaste();
            }
        }
        else if (userseting.IsKeybindActive(KeyBindList::New))
        {
            ImGui::OpenPopup("f");
        }

    }


    bool showcontextitem = false;
    const char* popupname = "f";
    if (ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_::ImGuiPopupFlags_AnyPopup))
    {
        if (ImGui::IsPopupOpen(popupname))
        {
            showcontextitem = true;
        }
    }
    else
    {
        showcontextitem = true;
    }
    if (showcontextitem) 
    {
        if (ImGuIHelper::BeginPopupContextItem(popupname))
        {
            if (ImGui::BeginMenu("Create"))
            {
                if (ImGui::BeginMenu("Files"))
                {
                    if (ImGui::MenuItem("Folder"))
                    {
                        Path NewFolderPath = FileHelper::GetNewFileName(_LookingAtDir.value().native() + Path("New folder").native()) / "";
                        if (fs::create_directory(NewFolderPath))
                        {
                            UpdateDir();
                        }

                    }

                    ImGui::EndMenu();
                }


                if (ImGui::BeginMenu("GameEngine"))
                {
                    if (ImGui::MenuItem("Scene"))
                    {
                        Path NewPath = FileHelper::GetNewFileName(_LookingAtDir.value().native() + Path("New 2DScene").native()
                            , Path("." + (UCode::String)UCode::Scene2dData::FileExt));
                        UCode::Scene2dData newScene = UCode::Scene2dData();
                        UCode::Scene2dData::ToFile(NewPath, newScene, Get_ProjectData()->Get_ProjData()._SerializeType);
                        UpdateDir();
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Script"))
                {
                    if (ImGui::MenuItem("File"))
                    {
                        Path NewPath = FileHelper::GetNewFileName(_LookingAtDir.value().native() + Path("File").native(),
                            Path("." + (UCode::String)UCodeLang::FileExt::SourceFile));

                        std::ofstream NewScirpt(NewPath);
                        NewScirpt << "use ULang;\n";
                        NewScirpt << "use UCodeGameEngine;\n\n";
                        NewScirpt.close();
                        //Go To TextEditor
                        UpdateDir();

                    }
                    if (ImGui::MenuItem("Component"))
                    {
                        Path NewPath = FileHelper::GetNewFileName(_LookingAtDir.value().native() + Path("New_Script").native(),
                            Path("." + (UCode::String)UCodeLang::FileExt::SourceFile));

                        std::ofstream NewScirpt(NewPath);
                        NewScirpt << UCompiler::NewComponentTemplate("Script");
                        NewScirpt.close();
                        UpdateDir();
                    }
                    if (ImGui::MenuItem("Asset"))
                    {
                        Path NewPath = FileHelper::GetNewFileName(_LookingAtDir.value().native() + Path("New_Asset_Script").native(),
                            Path("." + (UCode::String)UCodeLang::FileExt::SourceFile));

                        std::ofstream NewScirpt(NewPath);
                        NewScirpt << UCompiler::NewAssetTemplate("NewAsset");
                        NewScirpt.close();
                        //Go To TextEditor
                        UpdateDir();
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Art"))
                {
                    if (ImGui::MenuItem("Color"))
                    {
                        OpenPopUp = true;
                    }
                    ImGui::EndMenu();

                }

                auto currentulangkey = Get_App()->GetULangAssemblyID();
                auto ULang = UCode::ULangRunTime::Get(Get_App()->GetGameRunTime()->Get_Library_Edit());

                struct ScriptMenuInfo
                {
                    String MenuName;
                    const UCodeLang::AssemblyNode* node = nullptr;
                };

                thread_local Vector<ScriptMenuInfo> MenuInfo;
                thread_local Optional<ULangAssemblyID> Myulangkey;
                {
                    bool isoutofdate = Myulangkey.has_value() ? currentulangkey != Myulangkey.value() : true;
                    if (isoutofdate)
                    {
                        Myulangkey = currentulangkey;
                        const UCodeLang::ClassAssembly& CurrentAssembly = ULang->Get_Assembly();

                        for (auto& Item : CurrentAssembly.Classes)
                        {

                            Optional<const UCodeLang::UsedTags*> tagsop;

                            if (Item->Get_Type() == UCodeLang::ClassType::Class)
                            {
                                auto& nod = Item->Get_ClassData();

                                tagsop = &nod.Attributes;
                            }

                            if (tagsop.has_value())
                            {
                                const UCodeLang::UsedTags& tags = *tagsop.value();

                                for (auto& tag : tags.Attributes)
                                {
                                    const auto& nod = CurrentAssembly.Find_Node(tag.TypeID);
                                    if (nod) {
                                        if (StringHelper::StartsWith(nod->FullName, "UCodeGameEngine:MenuItem"))
                                        {
                                            StringView menuname = StringView((const char*)tag._Data.Get_Data(), tag._Data.Size);

                                            bool hasItem = false;

                                            for (auto& Item : MenuInfo)
                                            {
                                                if (Item.MenuName == menuname)
                                                {
                                                    hasItem = true;
                                                    break;
                                                }
                                            }

                                            if (!hasItem) {
                                                ScriptMenuInfo info;
                                                info.MenuName = menuname;
                                                info.node = Item.get();
                                                MenuInfo.push_back(info);
                                            }

                                        }
                                    }
                                }
                            }
                        }

                        //sort by name
                        std::sort(MenuInfo.begin(), MenuInfo.end(), [](ScriptMenuInfo& A, ScriptMenuInfo& B)
                        {
                                return A.MenuName.compare(B.MenuName) < 0;
                        });
                    }
                }

                ImGui::Separator();



                Optional<size_t> ClickedIndex = DrawMenu<ScriptMenuInfo>(spanof(MenuInfo), [](const ScriptMenuInfo& Item) -> StringView {return StringView(Item.MenuName); });
                if (ClickedIndex.has_value())
                {
                    auto& Script = MenuInfo[ClickedIndex.value()];
                    auto SaveType = Get_ProjectData()->Get_ProjData()._SerializeType;

                    UC::ScirptableObjectData data;
                    UC::ScirptableObject obj;

                    obj.LoadScript(Script.node);

                    obj.SaveTo(data, SaveType);

                    data._UID = Get_ProjectData()->GetNewUID();

                    Path pathout = FileHelper::GetNewFileName(_LookingAtDir.value() / Script.node->Name, UC::ScirptableObjectData::FileExtDot);

                    if (!UC::ScirptableObjectData::ToFile(pathout, data, SaveType))
                    {
                        UCodeGEError("Unable to save Asset at " << pathout);
                    }

                }

                ImGui::EndMenu();
            }


            {
                Optional<String> PasteType = CanPasteShowDirButtionsPaste();


                auto& userseting = UserSettings::GetSettings();
                auto str = userseting.KeyBinds[(size_t)KeyBindList::Paste].ToString();

                if (ImGui::MenuItem("Paste", str.c_str(), false, PasteType.has_value()) ||
                    (userseting.IsKeybindActive(KeyBindList::Paste) && PasteType.has_value()))
                {
                    ShowDirButtionsPaste();
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndPopup();
        }
    }
}

ProjectFiles& ProjectFilesWindow::Get_ProjectFiles()
{
    return this->Get_App()->GetPrjectFiles();
}

struct TepFilesToRemove
{
    std::set<Path> files;
    ~TepFilesToRemove()
    {
        for (auto& Item : files)
        {
            FileHelper::TrashFile(Item);
            auto v = GetAssetMetaFilePath(Item);
            if (v.has_value())
            {
                FileHelper::TrashFile(v.value());
            }
        }
    }
};
static TepFilesToRemove fileinteptoremove;

ProjectFilesWindow::ButtionSizeData ProjectFilesWindow::GetButtionSize()
{
    float padding = 32;
    float thumbnailSize = 64;
    //ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
    // ImGui::SliderFloat("Padding", &padding, 0, 32);
    float cellSize = thumbnailSize + padding;
    ButtionSizeData r;
    r.cellSize = cellSize;
    r.thumbnail = { thumbnailSize, thumbnailSize };
    return r;
}

bool ProjectFilesWindow::DrawFileItem(UCodeEditor::ProjectFilesWindow::FileData& Item, ImVec2& ButtionSize)
{

    Optional<std::function<void()>> OnInspect;
    Optional<std::function<void()>> OnShowSubAssets;
    if (Item.FileType == FileHelper::FileType::Dir)
    {
        if (ImGuIHelper::ImageButton(&Item, AppFiles::sprite::Dir_folder_image, ButtionSize))
        {
            _LookingAtDir = Item.FullFileName / "";
            UpdateDir();
            return true;
        }
    }
    else
    {
        bool FoundIt = false;
        auto v = TryLoadAsset(Item.FullFileName);

        if (v.has_value())
        {
            FoundIt = true;
            auto& assetfile = v.value();
            assetfile->LastUsed = ProjectFiles::AssetFileMaxLastUsed;

            OnInspect = [&]()
                {
                    InspectWindow::InspectData V;
                    V._Data = AnyManagedPtr::As(assetfile->_ManageFile);

                    static auto Func = Get_ProjectFiles()._newuid;
                    V._Draw = [](InspectWindow::InspectDrawer& data)
                        {
                            UEditorAssetDrawInspectContext Data;
                            Data.Drawer = &data;
                            Data._newuid = Func;

                            auto AssetFile = data.GetPtr().As_ptr<UEditorAssetFile>();
                            if (AssetFile.Has_Value())
                            {
                                AssetFile.Get_Value()->DrawInspect(Data);
                            }
                            else
                            {
                                data.SetPtrNull();
                            }
                        };


                    auto inpswin = Get_App()->Get_Window<InspectWindow>();
                    inpswin->Inspect(V);
                };

            if (assetfile->_File->HasSubAssets)
            {
                OnShowSubAssets = [&]()
                    {
                        _LookingAtAssetForSubAssets = assetfile->_File->FileFullPath.filename();
                    };
            }

            UEditorAssetDrawButtionContext Context;
            Context.ButtionSize = *(Vec2*)&ButtionSize;
            Context.ObjectPtr = &Item;

            auto& ProjectFiles = Get_ProjectFiles();
            Context._newuid = ProjectFiles._newuid;


            if (assetfile->_File->DrawButtion(Context))
            {
                (*OnInspect)();
            }
        }
        else
        {
            auto f = GetAssetData(Item.FullFileName);

            if (f.has_value())
            {
                auto& assetdata = f.value();
             
                auto& ProjectFiles = Get_ProjectFiles();
                
                UEditorAssetDataConext Conext;
                Conext.ButtionSize = *(Vec2*)&ButtionSize;
                Conext.ObjectPtr = &Item;
                Conext._newuid = ProjectFiles._newuid;

                assetdata->Draw(Conext, Item.FullFileName);

                FoundIt = true;
            }
        }
        

        if (FoundIt == false)
        {
            if (ImGuIHelper::ImageButton(&Item, AppFiles::sprite::File_image, ButtionSize))
            {
            }
        }

    }

    bool RetOutfunc = false;
    ImGui::PushID(&Item);
    if (ImGui::IsItemFocused() && !ImGui::GetIO().WantTextInput)
    {
        auto& settings = UserSettings::GetSettings();


        if (settings.IsKeybindActive(KeyBindList::Inspect))
        {
            if (ImGui::IsKeyDown(ImGuiMod_Ctrl))
            {
                if (OnShowSubAssets.has_value())
                {
                    (*OnShowSubAssets)();
                }
            }
            else
            {
                if (OnInspect.has_value())
                {
                    (*OnInspect)();
                }
            }
        }
        
        
        if (settings.IsKeybindActive(KeyBindList::Copy))
        {
            USerializer V(USerializerType::YAML);
            V.Write("UData", Item.FullFileName);
            V.Write("UType", "AssetPath");

            auto copytext = V.Get_TextMaker().c_str();

            ImGui::SetClipboardText(copytext);
        }

        if (settings.IsKeybindActive(KeyBindList::Rename))
        {
            RenameFile = Item.FullFileName;
        }

        if (settings.IsKeybindActive(KeyBindList::Delete))
        {

            bool allowforundo = true;

            RetOutfunc = true;
            if (allowforundo)
            {
                Path teppath = std::filesystem::temp_directory_path() / Path(Item.FileName);
                RenameAssetFile(Item.FullFileName, teppath);


                UndoData undodata;
                undodata._UndoCallBack = [teppath, oldpath = Item.FullFileName](UndoData& data)
                    {
                        fileinteptoremove.files.erase(teppath);
                        RenameAssetFile(teppath, oldpath);
                    };
                undodata._RedoCallBack = [teppath, oldpath = Item.FullFileName](UndoData& data)
                    {
                        RenameAssetFile(oldpath, teppath);
                    };
                undodata._UndoRemoved = [teppath](UndoData& data)
                    {
                        fileinteptoremove.files.erase(teppath);
                        FileHelper::TrashFile(teppath);
                        auto v = GetAssetMetaFilePath(teppath);
                        if (v.has_value())
                        {
                            FileHelper::TrashFile(v.value());
                        }
                    };
                Get_App()->AddUndo(undodata);
            }
            else
            {
                FileHelper::TrashFile(Item.FullFileName);
            }
        }
    }
    if (ImGuIHelper::BeginPopupContextItem("Test"))
    {
        ImGuIHelper::Text(StringView("Asset Options"));
        ImGui::Separator();

        auto& settings = UserSettings::GetSettings();

        auto str = settings.KeyBinds[(size_t)KeyBindList::Inspect].ToString();

        if (ImGui::MenuItem("Inspect", str.c_str(),nullptr, OnInspect.has_value()) || settings.IsKeybindActive(KeyBindList::Inspect))
        {
            if (OnInspect.has_value()) 
            {
                (*OnInspect)();
            }
            ImGui::CloseCurrentPopup();
        }
        
        str = "Ctrl+" + settings.KeyBinds[(size_t)KeyBindList::Inspect].ToString();
        if (ImGui::MenuItem("Show SubAssets", str.c_str(),nullptr,OnShowSubAssets.has_value()) || (ImGui::IsKeyDown(ImGuiMod_Ctrl) && settings.IsKeybindActive(KeyBindList::Inspect)))
        {
            if (OnShowSubAssets.has_value())
            {
                (*OnShowSubAssets)();
            }
            ImGui::CloseCurrentPopup();
        }

        //str = "Ctrl+" + settings.KeyBinds[(size_t)KeyBindList::Inspect].ToString();
        if (ImGui::MenuItem("Show in Files"))
        {
            FileHelper::OpenPathinFiles(_LookingAtDir.value());
            ImGui::CloseCurrentPopup();
        }

        str = settings.KeyBinds[(size_t)KeyBindList::Paste].ToString();
        if (ImGui::MenuItem("Copy File", str.c_str()) || settings.IsKeybindActive(KeyBindList::Copy))
        {
            USerializer V(USerializerType::YAML);
            V.Write("UData", Item.FullFileName);
            V.Write("UType", "AssetPath");

            auto copytext = V.Get_TextMaker().c_str();

            ImGui::SetClipboardText(copytext);
        }
    
        str = settings.KeyBinds[(size_t)KeyBindList::Rename].ToString();
        if (ImGui::MenuItem("Rename File", str.c_str()) || settings.IsKeybindActive(KeyBindList::Rename))
        {
            RenameFile = Item.FullFileName;
        }

        str = settings.KeyBinds[(size_t)KeyBindList::Delete].ToString();
        if (ImGui::MenuItem("Delete file", str.c_str()) || settings.IsKeybindActive(KeyBindList::Delete))
        {
            bool allowforundo = true;
            if (allowforundo)
            {
                Path teppath = std::filesystem::temp_directory_path() / Path(Item.FileName);
                RenameAssetFile(Item.FullFileName, teppath);


                UndoData undodata;
                undodata._UndoCallBack = [teppath, oldpath = Item.FullFileName](UndoData& data)
                    {
                        fileinteptoremove.files.erase(teppath);
                        RenameAssetFile(teppath, oldpath);
                    };
                undodata._RedoCallBack = [teppath, oldpath = Item.FullFileName](UndoData& data)
                    {
                        RenameAssetFile(oldpath, teppath);
                    };
                undodata._UndoRemoved = [teppath](UndoData& data)
                    {
                        fileinteptoremove.files.erase(teppath);
                        FileHelper::TrashFile(teppath);
                        auto v = GetAssetMetaFilePath(teppath);
                        if (v.has_value())
                        {
                            FileHelper::TrashFile(v.value());
                        }
                    };
                Get_App()->AddUndo(undodata);
            }
            else
            {
                FileHelper::TrashFile(Item.FullFileName);
            }
            UpdateDir();
            RetOutfunc = true;
            ImGui::CloseCurrentPopup();
        }



        ImGui::EndPopup();
    }

    ImGui::PopID();
    if (RetOutfunc)
    {
        return true;
    }

    auto style = ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_ChildBg);
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, style);

    Path Ext = Path(Item.FileName).extension();
    String NewName = Path(Item.FileName).replace_extension("").generic_string();
    ImGui::PushID(&Item);

    if (RenameFile.has_value() && RenameFile.value() == Item.FullFileName)
    {
        bool ReNameValue = true;
        if (ImGuIHelper::DrawRenameName(NewName, ReNameValue))
        {
            auto oldpath = Item.FullFileName;
            auto NewPath = Path(_LookingAtDir.value()) / Path(NewName).concat(Ext.generic_string());
            RenameAssetFile(Item.FullFileName, NewPath);
            Item.FullFileName = NewPath.generic_string();
            Item.FileName = NewName;

            UndoData undodata;
            undodata._UndoCallBack = [oldpath, NewPath](UndoData& data)
                {
                    RenameAssetFile(NewPath, oldpath);
                };
            undodata._RedoCallBack = [oldpath,NewPath](UndoData& data)
                {
                    RenameAssetFile(oldpath,NewPath);
                };
            Get_App()->AddUndo(undodata);
        }

        if (!ReNameValue)
        {
            RenameFile = {};
        }
    }
    else
    {
        ImGuIHelper::Text(NewName);
    }
    ImGui::PopID();
    ImGui::PopStyleColor();


    return false;
}
void ProjectFilesWindow::UpdateDir()
{

    Path path = _LookingAtDir.value();


    _LookingAtDirReadable = FileHelper::ToRelativePath(Get_ProjectData()->Get_ProjectDir(), _LookingAtDir.value());


    if (fs::exists(path))
    {
        fs::directory_options opt = fs::directory_options::skip_permission_denied;
        String FindText = _FindFileName;
        bool allfiles = false;

        if (FindText.size())
        {
            if (FindText[0] == ':')
            {
                allfiles = true;
                FindText = FindText.substr(1);
            }
        }

        _Files.clear();
        for (auto Item : fs::directory_iterator(allfiles ? Get_ProjectData()->GetAssetsDir() : path, opt))
        {
            FileData newdata = FileData();


            newdata.FileName = Item.path().filename().u8string();
            newdata.FullFileName = Item.path().u8string();
            const String Ext = Item.path().extension().u8string();
            if (Item.is_regular_file())
            {
                newdata.FileType = FileHelper::GetFileType(Ext);
            }
            else if (Item.is_directory())
            {
                if (allfiles)
                {
                    continue;
                }
                newdata.FileType = FileHelper::FileType::Dir;
            }
            if (newdata.FileType == FileHelper::FileType::Null)
            {
                continue;
            }
            _Files.push_back(newdata);
        }

        if (FindText.size())
        {
            Unordered_map<Path, float> ScoreMap;
            ScoreMap.reserve(_Files.size());

            auto assetdir = Get_ProjectData()->GetAssetsDir();
            auto assetdirstr = assetdir.generic_string();
            auto pathstr = path.generic_string();

            auto& basepath = allfiles ? assetdirstr : pathstr;
            for (auto& Item : _Files)
            {
                auto str = Item.FullFileName.generic_string();

                str = str.substr(basepath.size());

                ScoreMap.AddValue(Item.FullFileName, FuzzHelper::GetFuzzRotio(FindText, str));
            }

            std::sort(_Files.begin(), _Files.end(), [&ScoreMap](const FileData& A, const FileData& B)
                {
                    return ScoreMap.GetValue(A.FullFileName) < ScoreMap.GetValue(B.FullFileName);
                });
        }

        if (!allfiles)
        {
            //make directorys first

            std::sort(_Files.begin(), _Files.end(), [](const FileData& A, const FileData& B)
                {
                    return A.FileType == FileHelper::FileType::Dir &&
                        A.FileType != B.FileType;
                });
        }

    }

}


EditorWindowData ProjectFilesWindow::GetEditorData()
{
    return EditorWindowData("ProjectFiles", MakeWin);
}

void ProjectFilesWindow::OnSaveWindow(USerializer& SaveIn)
{
    auto Assespath = Get_App()->Get_RunTimeProjectData()->GetAssetsDir();

    auto PathString = FileHelper::ToRelativePath(Assespath, _LookingAtDir.value());


    SaveIn.Write("_LookingAtDir", PathString); 
    SaveIn.Write("_IsLookingAtAssetForSubAssets", _LookingAtAssetForSubAssets.has_value());

    if (_LookingAtAssetForSubAssets.has_value()) 
    {
        SaveIn.Write("_LookingAtAssetForSubAssets", _LookingAtAssetForSubAssets.value());
    }

    //
    //
}

void ProjectFilesWindow::OnLoadWindow(UDeserializer& Loadin)
{
    auto Assespath = Get_App()->Get_RunTimeProjectData()->GetAssetsDir();

    Path PathString = "";
    Loadin.ReadType("_LookingAtDir", PathString, PathString);

    if (PathString != "")
    {
        _LookingAtDir = Assespath.native() + PathString.native();
        if (!fs::is_directory(_LookingAtDir.value()))
        {
            _LookingAtDir = Assespath;
        }
    }
    else
    {
        _LookingAtDir = Assespath;
    }

    bool V = false;
    Loadin.ReadType("_IsLookingAtAssetForSubAssets", V, V);

    if (V) 
    {
        Path tep;
        Loadin.ReadType("_LookingAtAssetForSubAssets", tep, tep);
        _LookingAtAssetForSubAssets = tep;
    }
    UpdateDir();
}

EditorWindow* ProjectFilesWindow::MakeWin(const NewEditorWindowData& project)
{
    return new ProjectFilesWindow(project);
}

EditorEnd
