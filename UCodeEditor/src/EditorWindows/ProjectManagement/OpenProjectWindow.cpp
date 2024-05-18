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
    return FileHelper::Get_PersistentDataPath().native() + Path("Projects.data").native();
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
        ImGui::Columns(2);
        {
            ImGuIHelper::Text(StringView("Projects List"));
            auto& Projects = GetProjects()->_Projects;
            for (auto it = Projects.begin(); it != Projects.end();)
            {
                const auto& Item = *it;

                ImGui::Separator();
                ImGuIHelper::Text(StringView(Item._Name)); ImGui::SameLine();
                
	
                const float square_sz = ImGui::GetFrameHeight();
                float fullWidth = ImGui::GetContentRegionAvail().x;
                ImVec2 ButtonSize = { (fullWidth / 2) - ImGui::GetStyle().ItemSpacing.x,square_sz };
                
                ImGui::PushID(&Item);
                bool a = ImGui::Button("OpenProject", ButtonSize);
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
                }ImGui::SameLine();


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
        ImGuIHelper::InputPath("Project folder", tep, 0, ImGuIHelper::InputPathType::Dir);

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
                ProjData->_Projects.push_back({ NewProjectName ,ProjectDir.native() + Path(NewProjectName).native() });
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
            }
            ImGui::SameLine();
            if (ImGui::Button("Itch.io Page",{(fullWidth/2) - ImGui::GetStyle().ItemSpacing.x * 2,square_sz}))
            {
                const char* link = "https://lost-blizzard.itch.io/";

            }
        }
        auto buttionsize = ImVec2{ 180,square_sz  };
        {
            #define SPACEING "   "
            ImGuIHelper::Text(StringView("Tools"));
            ImGuIHelper::ItemLabel(StringView(SPACEING "Programming language:C++"),ImGuIHelper::Left);

            if (ImGui::Button("Open Page", buttionsize))
            {

            }
            ImGuIHelper::ItemLabel(StringView(SPACEING  "BuildSytem:Premake"),ImGuIHelper::Left);
            if (ImGui::Button("Open Page", buttionsize))
            {

            }
        }
        {
            ImGuIHelper::Text(StringView("Project Librarys"));
            ImGuIHelper::ItemLabel(StringView(SPACEING  "ImGui"),ImGuIHelper::Left);
            if (ImGui::Button("Open Page", buttionsize))
            {

            }

            ImGuIHelper::ItemLabel(StringView(SPACEING  "UCodeLang"),ImGuIHelper::Left);
            if (ImGui::Button("Open Page", buttionsize))
            {

            }
        }

        ImGui::GetFrameHeight();
        if (ImGui::Button("Back", { fullWidth / 2,square_sz }))
        {
            _State == State::ProjectList;
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


