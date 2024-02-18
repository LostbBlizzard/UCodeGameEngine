#pragma once

#include <UCodeRunTime/ULibrarys/Rendering/RenderRunTime2d.hpp>
#include <UCodeRunTime/Rendering/Render.hpp>

#include <EditorWindows/EditorWindow.hpp>
#include <Editor/EditorNamespace.hpp>

#include <UCodeRunTime/ULibrarys/Rendering/Camera2d.hpp>
#include <UCodeRunTime/ULibrarys/AssetManagement/SceneData.hpp>
#include "ImGui.hpp"
#include <UCodeRunTime/ULibrarys/UCodeLang/ULangRunTime.hpp>
EditorStart


class GameEditorWindow :public EditorWindow
{
public:
	GameEditorWindow(const NewEditorWindowData& windowdata);
	~GameEditorWindow() override;
	void UpdateWindow() override;
	void OnSaveWindow(USerializer& JsonToSaveIn)  override;
	void OnLoadWindow(UDeserializer& JsonToOutof)  override;
	

	static EditorWindowData GetEditorData();

	

private:
	static EditorWindow* MakeWin(const NewEditorWindowData& windowdata);

	enum class ToolBarType :u16
	{
		Select,
		Move,
		Scale,
		Rotate,
	};
	enum class GameWindowType :u16
	{
		EditorWindow,
		ExternalWindow,
	};
	
	struct SceneCameraData
	{
		UCode::Vec3 _Pos;//Vec 3 Because We need to Both 2D and 3D.
		float Orth_Size =10;
	};
	bool _IsRuningGame = false;
	bool _IsGamePaused = false;
	
	GameWindowType _WindowType = GameWindowType::EditorWindow;
	Unique_ptr<UCode::GameRunTime> _GameRunTime;
	Unique_ptr<UCode::RenderAPI::Render> _GameRender;
	bool _IsGameWindowFocused = false, _ShowingGameStats =false;
	int _ShowingGameStatslocation = 0;
	Optional<int> _DontWaitInputKey;
	

	//
	Optional<Path> _UseingScenePath;
	UCode::Scene2dData* _SceneData = nullptr;
	UCode::RunTimeScene* _SceneDataAsRunTiime = nullptr;
	UCode::Camera2d* _SceneCamera = nullptr;
	SceneCameraData _SceneCameraData;
	ToolBarType _ToolBar = ToolBarType::Select;
	
	ImVec2 ImgeVecPos;
	ImVec2 ImgeVecSize;

	
	void SetCopy(const UCode::Scene2dData::Entity_Data Entity);
	void SetCopy(const UCode::Entity* Entity);

	
	

	bool WasSelectedObjectOpened = false;
	bool IsRenameing = false;
	UCode::EntityPtr SelectedObject = nullptr;
	UCode::RunTimeScenePtr SelectedScene = nullptr;
	Optional<Vector<UCodeLang::ReflectionCustomTypeID>> _PickComponent;
	
	bool IsSelected(UCode::Entity* Object)
	{
		return SelectedObject.Get_Value() == Object;
	}
	bool IsSelected(UCode::RunTimeScene* Object)
	{
		return SelectedScene.Get_Value() == Object;
	}

	void SetScelected(UCode::Entity* Object)
	{
		SelectedObject = Object->NativeManagedPtr();
		SelectedScene = nullptr;
		
		IsRenameing = false;
	}
	void SetScelected(UCode::RunTimeScene* Object)
	{
		SelectedObject = nullptr;
		SelectedScene = Object->Get_ManagedPtr();


		IsRenameing = false;
	}

	void SceneCameraGetInputs();
	void SceneEditorTab();
	void SceneEditorBar();
	static void UpdateRunTimeWindowSize(ImVec2& Viewportsize, UCode::Camera2d* runtime);
	void UnLoadSceneCamera();
	void LoadSceneCamera();
	void ShowSceneData();
	void DropSceneFromPath();
	void ShowScene(UCode::RunTimeScene* Item);
	void ShowEntityData(UCode::Entity* Item);
	void ShowlibraryBookData(UCode::System* Item);
	void Scenehierarchy();
	void InputEmuation();
	void LoadRunTime();
	void UnLoadRender();
	
	void LoadRender(bool MakeWin);
	void UnLoadRunTime();
	
	void UpdateGame();
	
	void GameTab();
	void OnStopPlaying();
	void OnPlayScene();
	void UpdateAndShowGameImg();
	void ShowGameImage();
	void ShowRunTimeGameLibrary();
	void SaveScene();
	void OpenScencAtPath(const Path& Path);
	static Vec2 MousePosFromImage(const Vec2 CursorPos, const Vec2 ImageSize);
};
EditorEnd


