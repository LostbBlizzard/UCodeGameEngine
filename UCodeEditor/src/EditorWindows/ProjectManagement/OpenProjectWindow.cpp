#include "OpenProjectWindow.hpp"
#include <Imgui/imgui.h>
#include <Imgui/imgui_internal.h>

#include <ProjectManagement/ProjectManger.hpp>
#include <Helper/FileHelper.hpp>
#include <Editor/EditorApp.hpp>
#include <filesystem>

#include <UCodeRunTime/ULibrarys/Serialization_Library.h>

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
    constexpr ImVec2 ButtonSize = { 150,20 };
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
                
                ImGui::PushID(&Item);
                bool a = ImGui::Button("OpenProject", ButtonSize);
                ImGui::PopID();
                if (a)
                {
                    if (Get_App()->OpenProject(Item._Path / ""))
                    {
                        CloseEditorWindow();
                    }
                    else
                    {
                        //ShowPopUp
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
           
            if (ImGui::Button("Make New Project", ButtonSize))
            {
                _State = State::MakeNewProject;
            }
            if (ImGui::Button("Add Project", ButtonSize))
            {
                FileHelper::OpenFileData data = FileHelper::OpenDirFromUser(ProjectDir);
                if (data.Result == FileHelper::OpenFileResult::OKAY)
                {
                    auto* ProjData = GetProjects();
                    String ProjectName =std::filesystem::path(data.Path).filename().generic_string();

                    ProjData->_Projects.push_back({ ProjectName ,data.Path });
                    SaveProjects();
                }
            }
        }
    }
    else
    {
        auto tep = ProjectDir.generic_string();
        ImGuIHelper::InputText("Project folder",tep); 
        ImGui::SameLine();
        if (ImGui::ArrowButton("More", ImGuiDir_::ImGuiDir_Up))
        {
            FileHelper::OpenFileData data = FileHelper::OpenDirFromUser(ProjectDir);
            if (data.Result == FileHelper::OpenFileResult::OKAY)
            {
                ProjectDir = data.Path + '/';
            }
        }

        ImGuIHelper::InputText("New Project Name:", NewProjectName);

        if (ImGui::Button("Back", ButtonSize))
        {
            _State = State::ProjectList;
        }  
        ImGui::SameLine();
        if (ImGui::Button("Create Project", ButtonSize))
        {
            if (ProjectManger::MakeNewProject(ProjectDir, NewProjectName))
            {
                auto* ProjData = GetProjects();
                
                ProjData->ProjectDir = ProjectDir;
                ProjData->_Projects.push_back({ NewProjectName ,ProjectDir.native() + Path(NewProjectName).native() });
                SaveProjects();

                _State = State::ProjectList;
            }
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


