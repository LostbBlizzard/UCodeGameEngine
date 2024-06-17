#include "OpenProjectWindow.hpp"
#include <Imgui/imgui.h>
#include <Imgui/imgui_internal.h>

#include <Helper/FileHelper.hpp>
#include <Editor/EditorApp.hpp>
#include <filesystem>

#include <UCodeRunTime/ULibrarys/Serialization_Library.h>
#include "../../EditorWindows/BasicWindows/UserSettingsWindow.hpp"
namespace YAML 
{
    template<> struct convert<UCodeEditor::OpenProjectWindow::ProjectPath> {
        static Node encode(const UCodeEditor::OpenProjectWindow::ProjectPath& rhs) {
            Node node;
            node.push_back(rhs._Name);
            node.push_back(rhs._Path.generic_string());
            return node;
        }

        static bool decode(const Node& node, UCodeEditor::OpenProjectWindow::ProjectPath& rhs) {
            if (!node.IsSequence() || node.size() != 2) {
                return false;
            }

            rhs._Name = node[0].as<UCode::String>();
            rhs._Path = node[1].as<UCode::String>();
            return true;
        }
    };

}
EditorStart


EditorWindow* OpenProjectWindow::MakeWin(const NewEditorWindowData& windowdata)
{
    return new OpenProjectWindow(windowdata);
}
OpenProjectWindow::OpenProjectWindow(const NewEditorWindowData& project) :EditorWindow(project), _State(State::ProjectList)
{

    auto* ProjData = GetProjects();
    ProjectDir = ProjData->ProjectDir;
}
OpenProjectWindow::~OpenProjectWindow()
{
}
Path GetProjectsPaths_Path()
{
    return FileHelper::Get_PersistentDataPath() / Path("Projects.data");
}
void OpenProjectWindow::SetUp()
{
    ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
    ImGuiID dockspaceID = ImGui::GetID(EditorAppCompoent::ImGUIDockName);

  
}
void OpenProjectWindow::UpdateWindow()
{
    if (_State == State::ProjectList)
    {
        ImGui::Columns(2,"columns",false);
        ImGui::SetColumnOffset(0, 0.0f);
        ImGui::SetColumnOffset(1, 500.0f);
        {
            ImGuIHelper::Text(StringView("Projects List"));
            ImGui::Separator();
            auto& Projects = GetProjects()->_Projects;
            for (auto it = Projects.begin(); it != Projects.end();)
            {
                const auto& Item = *it;

                ImGuIHelper::ItemLabel(StringView(Item._Name), ImGuIHelper::Left); ImGui::SameLine();
	
                const float square_sz = ImGui::GetFrameHeight();
                float fullWidth = ImGui::GetContentRegionAvail().x;
                ImVec2 ButtonSize = { (fullWidth / 2) - ImGui::GetStyle().ItemSpacing.x,square_sz };
                
                ImGui::PushID(&Item);
                bool a = ImGui::Button("Open Project", ButtonSize);
                ImGui::PopID();
                if (a)
                {
                    auto v = Get_App()->OpenProject(Item._Path / "");
                    if (v.IsValue())
                    {
                        CloseEditorWindow();
                    }
                    else
                    {
                        Get_App()->SetProjectOpenError(v.GetError());
                    }
                }

                ImGui::PushID((uintptr_t(&Item)+2));
                if (ImGuIHelper::BeginPopupContextItem("Open Project Popup"))
                {
                    if (ImGui::MenuItem("Open in Safe Mode"))
                    {

                    }
                    if (ImGui::MenuItem("Show in Files"))
                    {
                        FileHelper::OpenPathinFiles(Item._Path);
                    }
                    ImGui::EndPopup();
                }
                ImGui::PopID();

                ImGui::SameLine();


                ImGui::PushID((uintptr_t(&Item)+1));
                bool c = ImGui::Button("Remove from Project List", ButtonSize);
                ImGui::PopID();
                if (c)
                {
                    GetProjects()->_Projects.erase(it);
                    SaveProjects();
                    break;
                }

                ++it;
            }
        }
        ImGui::NextColumn();
        {
            const float square_sz = ImGui::GetFrameHeight();
            float fullWidth = ImGui::GetContentRegionAvail().x;
            ImVec2 ButtonSize = { (fullWidth / 2) - ImGui::GetStyle().ItemSpacing.x,square_sz };


            if (ImGui::Button("Make New Project", ButtonSize))
            {
                _State = State::MakeNewProject;
            }
            ImGui::SameLine();
            if (ImGui::Button("Add Project", ButtonSize))
            {
                FileHelper::OpenFileData data = FileHelper::OpenDirFromUser(ProjectDir);
                if (data.Result == FileHelper::OpenFileResult::OKAY)
                {
                    auto* ProjData = GetProjects();
                    String ProjectName = std::filesystem::path(data.Path).filename().generic_string();

                    ProjData->_Projects.push_back({ ProjectName ,data.Path });
                    SaveProjects();
                }
            }

            if (ImGui::Button("Open UserSettings", ButtonSize))
            {
                Get_App()->Get_Window<UserSettingsWindow>();

            }
            ImGui::SameLine();
            if (ImGui::Button("Credits", ButtonSize))
            {
                _State = State::Credits;
            }
        }
    }
    else if (_State == State::MakeNewProject)
    {
        auto tep = ProjectDir.generic_string();

        if (ImGuIHelper::InputPath("Project folder", tep, 0, ImGuIHelper::InputPathType::Dir)) 
        {
            ProjectDir = tep;
        }

        ImGuIHelper::InputText("New Project Name:", NewProjectName);

        thread_local Vector<ImGuIHelper::EnumValue<ProjectTemplateInfo>> Val;
        {
            thread_local bool v = false;
            if (v == false)
            {
                v = true;

                ProjectTemplateInfo info2;
                info2._ProjectType = ProjectManger::ProjectType::Empty;
                Val.push_back(ImGuIHelper::EnumValue<ProjectTemplateInfo>("Basic", std::move(info2)));

                info2 = ProjectTemplateInfo();
                info2._ProjectType = ProjectManger::ProjectType::PlugIn;
                Val.push_back(ImGuIHelper::EnumValue<ProjectTemplateInfo>("Plugin", std::move(info2)));

                //TODO show TemplateProject Info
            }
        }
        ImGuIHelper::EnumField("Project Template:", templateinfo, Val);

        const float square_sz = ImGui::GetFrameHeight();
        float fullWidth = ImGui::GetContentRegionAvail().x;
        ImVec2 ButtonSize = { (fullWidth / 2) - ImGui::GetStyle().ItemSpacing.x,square_sz };


        if (ImGui::Button("Back", ButtonSize))
        {
            _State = State::ProjectList;
        }
        ImGui::SameLine();
        if (ImGui::Button("Create Project", ButtonSize))
        {
            if (ProjectManger::MakeNewProject(ProjectDir, NewProjectName, templateinfo._ProjectType,
                templateinfo._ProjectType == ProjectManger::ProjectType::ExternalProjectType ? templateinfo.ExternalProjectID : Optional<String>()
            ))
            {
                auto* ProjData = GetProjects();

                ProjData->ProjectDir = ProjectDir;
                ProjData->_Projects.push_back({ NewProjectName ,ProjectDir / Path(NewProjectName)});
                SaveProjects();

                _State = State::ProjectList;
            }
        }
    }
    else if (_State == State::Credits)
    {
        const float square_sz = ImGui::GetFrameHeight();
        float fullWidth = ImGui::GetContentRegionAvail().x;

        {
            ImGuIHelper::ItemLabel(StringView("Main Developer: LostBlizzard"), ImGuIHelper::Left);

            float fullWidth = ImGui::GetContentRegionAvail().x;
            ImGui::SameLine();
            if (ImGui::Button("GitHub Page",{(fullWidth/2) - ImGui::GetStyle().ItemSpacing.x * 2,square_sz,}))
            {
                const char* link = "https://github.com/LostbBlizzard";
                FileHelper::OpenWebLink(link);
            }
            ImGui::SameLine();
            if (ImGui::Button("Itch.io Page",{(fullWidth/2) - ImGui::GetStyle().ItemSpacing.x * 2,square_sz}))
            {
                const char* link = "https://lost-blizzard.itch.io";
                FileHelper::OpenWebLink(link);
            }
        }
        auto buttionsize = ImVec2{ 180,square_sz  };
        {
            ImGuIHelper::Text(StringView("Tools"));
            ImGui::Indent();

            ImGuIHelper::ItemLabel(StringView("Programming language:C++"),ImGuIHelper::Left);

            if (ImGui::Button("Open Page", buttionsize))
            {
                FileHelper::OpenWebLink("https://en.wikipedia.org/wiki/C%2B%2B");
            }
            ImGuIHelper::ItemLabel(StringView("BuildSytem:Premake"),ImGuIHelper::Left);

            ImGui::PushID(1);
            if (ImGui::Button("Open Page", buttionsize))
            {
                FileHelper::OpenWebLink("https://premake.github.io");
            }
            ImGui::PopID();
            ImGui::Unindent();
        }
        {
            ImGuIHelper::Text(StringView("Project Librarys"));

            ImGui::Indent();

            struct Pages
            {
                StringView Name;
                StringView Url;
                Pages(StringView name, StringView url)
                    :Name(name),Url(url)
                {

                }
            };
            static Array<Pages, 10> PagesList = {
                Pages( StringView("ImGui"),StringView("https://github.com/ocornut/imgui")),
                {"UCodeLang","https://github.com/LostbBlizzard/UCodeLang"},
                {"Yaml-Cpp","https://github.com/jbeder/yaml-cpp"},
               
                {"Box2D","https://github.com/erincatto/box2d"},
                {"SoLoad","https://solhsa.com/soloud/"},
               
                {"Rttr","https://github.com/rttrorg/rttr"},
                {"MinimalSocket","https://github.com/andreacasalino/Minimal-Socket"},
                
                {"Stb_image","https://github.com/nothings/stb/blob/master/stb_image.h"},
                {"Stb_image_write","https://github.com/nothings/stb/blob/master/stb_image_write.h"},
                {"SimpleFileWatcher","https://github.com/apetrone/simplefilewatcher/tree/master"},

                //ToDo add others
            };

            for (auto& Item : PagesList)
            {
                ImGuIHelper::ItemLabel(Item.Name, ImGuIHelper::Left);

                ImGui::PushID(&Item);
                if (ImGui::Button("Open Page", buttionsize))
                {
                    FileHelper::OpenWebLink(String(Item.Url));
                }
                ImGui::PopID();
            }

            ImGui::Unindent();
        }

        ImGui::GetFrameHeight();
        if (ImGui::Button("Back", { fullWidth / 2,square_sz }))
        {
            _State = State::ProjectList;
        }
        ImGui::SameLine();
        if (ImGui::Button("Show Licenses", { fullWidth / 2,square_sz }))
        {
            FileHelper::OpenPathinFiles(std::filesystem::absolute(AppFiles::GetFilePathByMove("licenses")));
        }
    }
}
EditorWindowData OpenProjectWindow::GetEditorData()
{
    return EditorWindowData("OpenProjectWindow",MakeWin);
}

OpenProjectWindow::ProjectsPath* OpenProjectWindow::GetProjects()
{
    if (_ThisProjects == nullptr)
    {
        _ThisProjects =std::make_unique<ProjectsPath>();
        
        ProjectsPath::ReadToFile(GetProjectsPaths_Path(), *_ThisProjects);
    }
    return _ThisProjects.get();
}
void OpenProjectWindow::SaveProjects()
{
    ProjectsPath::WriteToFile(GetProjectsPaths_Path(),*_ThisProjects);
}
void OpenProjectWindow::ProjectsPath::ReadToFile(const Path&  Path, ProjectsPath& out)
{
    auto File = std::ifstream(Path);


    if (File.is_open())
    {
        YAML::Node node(YAML::Load(File));
        out.ProjectDir = node["_ProjectDir"].as<UCode::String>();

        out._Projects = node["_Projects"].as<UCode::Vector<ProjectPath>>();
     
        File.close();
    }
}

void OpenProjectWindow::ProjectsPath::WriteToFile(const Path&  Path, const ProjectsPath& out)
{
    auto File = std::ofstream(Path);
    if (File.is_open())
    {
        YAML::Emitter Node(File);
        Node << YAML::BeginMap;

        Node << YAML::Key << "_ProjectDir" << YAML::Value << out.ProjectDir.generic_string();

        Node << YAML::Key << "_Projects" << YAML::Value << out._Projects;

        Node << YAML::EndSeq;

        File.close();
       
    }
}
EditorEnd


