#pragma once
#include <EditorWindows/BasicWindows/GameEditorWindow.hpp>

#include <Helper/ImGuIHelper.hpp>
#include <ProjectManagement/ProjectManger.hpp>
EditorStart
class OpenProjectWindow :public EditorWindow
{
public:
	struct ProjectPath
	{
		String _Name;
		Path _Path;
	};
	struct ProjectsPath
	{
		Path ProjectDir;
		UCode::Vector<ProjectPath> _Projects;
		static void ReadToFile(const Path&  Path,ProjectsPath& out);
		static void WriteToFile(const Path&  Path, const ProjectsPath& out);
	};
	

	OpenProjectWindow(const NewEditorWindowData& project);
	~OpenProjectWindow() override;

	void SetUp() override;

	void UpdateWindow() override;

	static EditorWindowData GetEditorData();
	static ProjectsPath* GetProjects();
	static void SaveProjects();
	
	enum class State : unsigned char
	{
		ProjectList,
		MakeNewProject,
		Credits,
	};
	State _State;
	Path ProjectDir;
	String NewProjectName;
	
	struct ProjectTemplateInfo
	{
		ProjectManger::ProjectType _ProjectType =ProjectManger::ProjectType::Empty;
		String ExternalProjectID;

	};
	ProjectTemplateInfo templateinfo;
private:
	inline static Unique_ptr<ProjectsPath> _ThisProjects;
	
	static  EditorWindow* MakeWin(const NewEditorWindowData& windowdata);
};

EditorEnd