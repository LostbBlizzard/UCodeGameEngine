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
EditorStart
namespace fs = std::filesystem;

void ProjectFilesWindow::DirectoryViewerOpenFile(DirectoryViewer& From, void* Ptr, const Path& Path)
{
    ProjectFilesWindow* ptr = (ProjectFilesWindow*)Ptr;
    ptr->_LookingAtDir = Path.parent_path().native() + Path::preferred_separator;
    ptr->UpdateDir();
}
ProjectFilesWindow::ProjectFilesWindow(const NewEditorWindowData& windowdata):EditorWindow(windowdata)
{
    Viewer.SetCallBackPtr(this);
    Viewer.SetOpenPathCallback(DirectoryViewerOpenFile);
    Viewer.SetShowFindFiles(false);
}
ProjectFilesWindow::~ProjectFilesWindow()
{
   
}

#define ColorSpiteName "NewColorSprite"
static bool OpenPopUp =false;

#define ColorEdit "UpdateColor"
static bool ColorEditPopUp = false;
static String ColorEditFilePath;
static UCode::Color32* ColorObject =nullptr;

void ProjectFilesWindow::OnFileUpdate(const Path& path)
{
    bool islookatdir = false;
   
    Path newpath = "Assets";
    newpath /= path.native();

    if (newpath.native().size() > _LookingAtDirReadable.native().size())
    {
        islookatdir = UCode::StringHelper::StartWith<PathChar>(newpath.native(),_LookingAtDirReadable.native());

    }
    if (islookatdir)
    {
        UpdateDir();
    }
}
void ProjectFilesWindow::UpdateWindow()
{
    if (!_LookingAtDir.has_value())
    {
        _LookingAtDir = Get_ProjectData()->GetAssetsDir();
        UpdateDir();
    }
    
    auto window = ImGui::GetCurrentWindow();

    window->Flags |= ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar;

   
    if (_LookingAtDir.has_value())
    {
        {
            const auto AssetsDir = Get_ProjectData()->GetAssetsDir();
            bool CantGoBack = _LookingAtDir == AssetsDir;

            ImGui::BeginDisabled(CantGoBack);
            if (ImGui::Button("Back"))
            {

                Path oldpath = _LookingAtDir.value();
                _LookingAtDir = oldpath.parent_path().parent_path().generic_u8string() + '/';
                UpdateDir();
            }
            ImGui::EndDisabled();

            ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_ItemSpacing, { 2,4});

            ImGui::SameLine();

          
            ImGui::PushID(&_LookingAtDir);
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            bool v = ImGuIHelper::InputText("", _FindFileName);
            ImGui::PopItemWidth();
            ImGui::PopID();

           if (v)
            {
                Viewer.SetFindString(_FindFileName);
                UpdateDir();
            }
            auto str = _LookingAtDirReadable.generic_string();

            static Vector<StringView> list;
            StringHelper::Split(str, "/",list);
            String itemstr;

            Optional<size_t> ClickedIndex;
            for (size_t i = 0; i < list.size(); i++)
            {
                auto& Item = list[i];
                itemstr = Item;
                
                ImGui::PushID(&Item);

                auto s = ImGui::CalcTextSize(itemstr.c_str());
                auto roundsize = 10;
                bool onclick = ImGui::Button(itemstr.c_str(),{ std::ceilf(s.x / roundsize) * roundsize + 10,0});
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
            ImGui::PopStyleVar();
         
            if (ClickedIndex.has_value())
            {
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

            ShowFileCells();

            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::EndChild();




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

                        Path path = FileHelper::GetNewFileName(Path(_LookingAtDir.value().native() + Path(EntityName).native()),Path(RawEntityData::FileExtDot));

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


                        Path path = FileHelper::GetNewFileName(_LookingAtDir.value().native() +  Path(_SceneName).native(),Path(UCode::Scene2dData::FileExtDot));
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
            ImGuIHelper::InputText("File Name",FileName);
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
                Path NewPath = FileHelper::GetNewFileName(_LookingAtDir.value().native() + Path(FileName).native(),".png");
                UCode::Color32 Bytes = (UCode::Color32)Color;
                constexpr auto CHANNEL_NUM = 4;
                constexpr auto width = 1;
                constexpr auto height = 1;            
                stbi_write_png(NewPath.generic_string().c_str(), width, height, CHANNEL_NUM, &Bytes, width* CHANNEL_NUM);
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
    static float padding = 32;
    static float thumbnailSize = 64;
    //ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
    // ImGui::SliderFloat("Padding", &padding, 0, 32);
    float cellSize = thumbnailSize + padding;
    ImVec2 CeSize = { thumbnailSize, thumbnailSize };
    float panelWidth = ImGui::GetContentRegionAvail().x;
    i32 columnCount = (i8)(panelWidth / cellSize);
    if (columnCount < 1) {
        columnCount = 1;
    }

    ImGui::Columns(columnCount, 0, false);

    for (auto& Item : _Files)
    {
        if (Item.FullFileName.extension() != Path(UEditorAssetFileData::DefaultMetaFileExtWithDot))
        {
            if (StringHelper::Fllter(_FindFileName, Item.FileName))
            {
                if (DrawFileItem(Item, CeSize)) { break; }
                ImGui::NextColumn();
            }
        }
    }

    ImGui::Columns(1);

    auto ContentSize2 = ImGui::GetContentRegionAvail();
    if (ContentSize2.x > 0 && ContentSize2.y > 0) {
        ImGui::InvisibleButton("##", ContentSize2);
        ShowDirButtions();
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
void ProjectFilesWindow::ShowDirButtions()
{
    if (ImGui::BeginPopupContextItem("f"))
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
                
                obj.SaveTo(data,SaveType);

                data._UID = Get_ProjectData()->GetNewUID();

                Path pathout = FileHelper::GetNewFileName(_LookingAtDir.value() / Script.node->Name,UC::ScirptableObjectData::FileExtDot);

                if (!UC::ScirptableObjectData::ToFile(pathout, data, SaveType))
                {
                    UCodeGEError("Unable to save Asset at " << pathout);
                }
               
            }

            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }
}

ProjectFiles& ProjectFilesWindow::Get_ProjectFiles()
{
    return this->Get_App()->GetPrjectFiles();
}
bool ProjectFilesWindow::DrawFileItem(UCodeEditor::ProjectFilesWindow::FileData& Item, ImVec2& ButtionSize)
{

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
        auto Modules = UEditorModules::GetModules();

        Path FileExt = Item.FullFileName.extension();
        bool ShowOthers = true;
        bool FoundIt = false;

        auto& ProjectFiles = Get_ProjectFiles();
        
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
                    auto B =ProjectFiles.FindAssetFile(Item.FullFileName);
                    
                    UEditorAssetDrawButtionContext Context;
                    Context.ButtionSize = *(Vec2*)&ButtionSize;
                    Context.ObjectPtr = &Item;
                    Context._newuid = ProjectFiles._newuid;

                    ProjectFiles::AssetFile* File;
                    if (!B.has_value())
                    {
                        auto Ptr = Data->GetMakeNewAssetFile();
                        Ptr->FileFullPath = Item.FullFileName;
                        UEditorAssetFileInitContext InitContext;
                        Ptr->Init(InitContext);


                        ProjectFiles::AssetFile tep1;
                        tep1.Set(std::move(Ptr));
                        tep1.LastUsed = ProjectFiles::AssetFileMaxLastUsed;

                        ProjectFiles._AssetFiles.push_back(std::move(tep1));
                        File = &ProjectFiles._AssetFiles.back();
                    }
                    else
                    {
                        auto& HG = ProjectFiles._AssetFiles[B.value()];
                        File = &HG;
                        HG.LastUsed = ProjectFiles.AssetFileMaxLastUsed;
                    }
                    
                    
                    if (File->_File->DrawButtion(Context))
                    {
                        InspectWindow::InspectData V;
                        V._Data =AnyManagedPtr::As(File->_ManageFile);

                        static auto Func = Get_ProjectFiles()._newuid;
                        V._Draw = [](InspectWindow::InspectDrawer& data)
                        {
                            UEditorAssetDrawInspectContext Data;
                            Data.Drawer = &data;
                            Data._newuid =Func;

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
                    }
                }
                else
                {
                    UEditorAssetDataConext Conext;
                    Conext.ButtionSize = *(Vec2*)&ButtionSize;
                    Conext.ObjectPtr = &Item;
                    Conext._newuid = ProjectFiles._newuid;

                    ShowOthers = Data->Draw(Conext, Item.FullFileName);
                }

                break;
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
    if (ImGui::BeginPopupContextItem("Test"))
    {
       
        ImGuIHelper::Text(StringView("File options"));
        ImGui::Separator();
    
        if (ImGui::MenuItem("delete file"))
        {
           FileHelper::TrashFile(Item.FullFileName);
            UpdateDir();
            RetOutfunc = true;
        }
        if (ImGui::MenuItem("Show in Files"))
        {
            FileHelper::OpenPathinFiles(_LookingAtDir.value());
        }  
        if (ImGui::MenuItem("Rename File"))
        {
            RenameFile = Item.FullFileName;
        }
        if (ImGui::MenuItem("Open File"))
        {
            FileHelper::OpenPathinFiles(Item.FullFileName);
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
            auto NewPath = Path(_LookingAtDir.value()) / Path(NewName).concat(Ext.generic_string());
            FileHelper::RenameFile(Item.FullFileName, NewPath);
            Item.FullFileName = NewPath.generic_string();
            Item.FileName = NewName;
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
        
        _Files.clear();
        for (auto Item : fs::directory_iterator(path,opt))
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
                newdata.FileType = FileHelper::FileType::Dir;
            }
            if (newdata.FileType == FileHelper::FileType::Null)
            {
                continue;
            }
            _Files.push_back(newdata);
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


    SaveIn.Write("_LookingAtDir",PathString);



    //
    //
}

void ProjectFilesWindow::OnLoadWindow(UDeserializer& Loadin)
{
    auto Assespath = Get_App()->Get_RunTimeProjectData()->GetAssetsDir();

    Path PathString= "";
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
    UpdateDir();
}

EditorWindow* ProjectFilesWindow::MakeWin(const NewEditorWindowData& project)
{
    return new ProjectFilesWindow(project);
}

EditorEnd
