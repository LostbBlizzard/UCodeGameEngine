#pragma once

#include <UCodeRunTime/ULibrarys/Rendering/RenderRunTime2d.hpp>
#include <UCodeRunTime/Rendering/Render.hpp>

#include <EditorWindows/EditorWindow.hpp>
#include <Editor/EditorNamespace.hpp>

#include <UCodeRunTime/ULibrarys/Rendering/Camera2d.hpp>
#include <UCodeRunTime/ULibrarys/AssetManagement/SceneData.hpp>
#include "ImGui.hpp"
#include <UCodeRunTime/ULibrarys/UCodeLang/ULangRunTime.hpp>
#include "Editor/EditorApp.hpp"
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

	
	struct PrefabModeData
	{
		std::function<bool(UC::Entity* entity)> WasUpdated;
		std::function<void(UC::Entity* entity)> OnSave;
	};
	void SetPrefabMode(UC::EntityPtr prefab,PrefabModeData&& data);
	static UC::EntityPtr GetCurrentSeclectedEntity();

	void OpenScencAtPathRemoveOthers(const UID& Path);
	void OpenScencAtPath(const UID& Path);
	void OpenScencAtPath(const Path& Path);
	
	inline Optional<Vec2> Get_EditorSceneMIn3d()
	{
		return EditorSceneMIn3d;
	}
	 Optional<std::function<void(UC::RenderRunTime2d::DrawData& Data)>> PreDraw;
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
	struct SceneEditorTabData
	{
		Unique_ptr<UCode::GameRunTime> _GameRunTime;
		Unique_ptr<UCode::RenderAPI::Render> _GameRender;
		UCode::GameRunTime* _GameRuntimeRef = nullptr;
		UCode::Camera2d* _SceneCamera = nullptr;
		SceneCameraData _SceneCameraData;

		UCode::GameRunTime* GetGameRuntime()
		{
			if (_GameRunTime.get())
			{
				return _GameRunTime.get();
			}
			else
			{
				return _GameRuntimeRef;
			}
		}
	};
	bool _IsRuningGame = false;
	bool _IsGamePaused = false;
	
	GameWindowType _WindowType = GameWindowType::EditorWindow;
	bool _IsGameWindowFocused = false, _ShowingGameStats =false;
	int _ShowingGameStatslocation = 0;
	Optional<int> _DontWaitInputKey;	
	Optional<int> _UCodeLangReloadKey;	
	
	Optional<UID> _UseingSceneAsset;
	UCode::Scene2dData* _SceneData = nullptr;
	UCode::RunTimeScene* _SceneDataAsRunTiime = nullptr;
	ToolBarType _ToolBar = ToolBarType::Select;
	
	ImVec2 ImgeVecPos;
	ImVec2 ImgeVecSize;
	Optional<Vec2> EditorSceneMIn3d = {};

	bool WasSelectedObjectOpened = false;
	bool IsRenameing = false;
	UCode::EntityPtr SelectedObject = nullptr;
	UCode::RunTimeScenePtr SelectedScene = nullptr;
	Optional<Vector<UCodeLang::ReflectionCustomTypeID>> _PickComponent;

	UC::EntityPtr _PrefabMode;
	PrefabModeData _PrefabModeData;

	SceneEditorTabData MainSceneData;
	SceneEditorTabData PrefabSceneData;

	Optional<BlockCloseKey> BlockKey;
	String sceneassaved;
	bool ScencIsDiffent();
	
	void SetCopy(const UCode::Scene2dData::Entity_Data Entity);
	void SetCopy(const UCode::Entity* Entity,bool CopyRef);
	void SetCopy(const UCode::RunTimeScene* Entity);

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
	


	void SceneCameraGetInputs(SceneEditorTabData& data);
	void SceneEditorTab();

	
	void SceneEditor(SceneEditorTabData& data);	
	void SceneEditorBar(SceneEditorTabData& data);
	static void UpdateRunTimeWindowSize(ImVec2& Viewportsize, UCode::Camera2d* runtime);
	void UnLoadSceneCamera(SceneEditorTabData& data);
	void LoadSceneCamera(SceneEditorTabData& data);
	void ShowSceneDataAddNewScene();
	void ShowSceneData();
	void DropSceneFromPath();
	void PasteInScene(UCode::RunTimeScene* Item);
	void PasteInEntity(UCode::Entity* Item);
	void ShowScene(SceneEditorTabData& data,UCode::RunTimeScene* Item);

	void EntityDestroy(UCode::Entity* Item);
	void EntityAdd(UCode::Entity* Item, bool AddToChild);
	void ShowEntityData(SceneEditorTabData& data,UCode::Entity* Item);
	void ShowlibraryBookData(UCode::System* Item);
	void Scenehierarchy();
	void InputEmuation();
	void LoadRunTime();
	void UnLoadRender(SceneEditorTabData& data);
	
	void LoadRender(SceneEditorTabData& data,bool MakeWin);
	void UnLoadRunTime();
	
	void UpdateGame();
	
	void GameTab();
	void OnStopPlaying();
	void OnPlayScene();
	void UpdateAndShowGameImg();
	void ShowGameImage();
	void ShowRunTimeGameLibrary();
	void SaveScene();
	static Vec2 MousePosFromImage(const Vec2 CursorPos, const Vec2 ImageSize);
	void SetSeneAsSaved();
	void OnULangReload();
};
EditorEnd


