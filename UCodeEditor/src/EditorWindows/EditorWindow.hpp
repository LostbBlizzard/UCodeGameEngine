#pragma once
#include  <UCodeRunTime/RunTimeBasicTypes/String.hpp>
#include "ProjectManagement/RunTimeProjectData.hpp"
#include <Editor/EditorNamespace.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/Vectors.hpp>
#include <UCodeRunTime/ULibrarys/Serialization/USerializer.hpp>
#include <UCodeRunTime/Core/GameRunTime.hpp>

#include "ImGui.hpp"
#include "Serialization.hpp"
EditorStart

class EditorAppCompoent;
struct NewEditorWindowData
{
	EditorAppCompoent* _App;
	NewEditorWindowData(EditorAppCompoent* e) : _App(e)
	{
		
	}
};

class EditorWindow
{
public:
	EditorWindow(const NewEditorWindowData& project) : _WindowData(project), IsOpen(true), Closed(false) 
	{
		flags = 0;
		flags = ImGuiWindowFlags_::ImGuiWindowFlags_None;


		//flags |= ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar;
		flags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar;
	}
	virtual ~EditorWindow() {};

	virtual void SetUp() {};
	bool _SetUp = false;
	void WindowStart();
	virtual void UpdateWindow() = 0;
	void WindowEnd();



	
	virtual void OnSaveWindow(USerializer& Input) {};
	virtual void OnLoadWindow(UDeserializer& JsonToOutof) {};


	String WindowName = "EditorWindow";
	int WindowId = 0;

	const std::string& Get_ImGuiName() {return _ImGuiName;}
	
	UCODE_EDITOR_FORCEINLINE bool Get_IsOpen() { return IsOpen; }
	UCODE_EDITOR_FORCEINLINE bool Get_IsClosed() { return Closed; }
	UCODE_EDITOR_FORCEINLINE void CloseEditorWindow() { Closed = true; };
	
	bool Init = false;
	UCode::Vec2 _Pos;
	UCode::Vec2 _Size;
	bool  _Collapsed = false;
	String _ImGuiName;
	ImGuiWindowFlags flags = 0;
	bool CallBegin = true;
public:
	UCode::Gamelibrary* Get_GameLib();
	UCode::Ref<UCode::Gamelibrary> Get_GameLibRef();
	RunTimeProjectData* Get_ProjectData();
	UCODE_EDITOR_FORCEINLINE EditorAppCompoent* Get_App() { return _WindowData._App; }
private:
	bool IsOpen;
	bool Closed;
	NewEditorWindowData _WindowData;
}; 
class EditorWindowData
{
	typedef EditorWindow* (*MakeWin)(const NewEditorWindowData&);

	String WindowName = "EditorWindow";
	MakeWin _MakeWindow;
	
public:
	const String&  GetName() const { return WindowName; }
	EditorWindowData(String wName, MakeWin MakeWin) : WindowName(wName),_MakeWindow(MakeWin)
	{

	}
	EditorWindow* MakeWindow(const NewEditorWindowData& data) const
	{
		if (_MakeWindow)
		{
			return _MakeWindow(data);
		}
		return nullptr;
	}
	
	template<typename T>
	static EditorWindowData Editor(MakeWin Make)
	{
		const std::type_info& TypeData = typeid(T);

		return EditorWindowData(TypeData.name,Make);
	}
};


EditorEnd