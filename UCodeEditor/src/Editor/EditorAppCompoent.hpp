#pragma once
#include "EditorNamespace.hpp"

#include <UCodeRunTime/BasicTypes.hpp>
#include <UCodeRunTime/Core/GameRunTime.hpp>
#include "UCodeRunTime/includeForCompoent.hpp"
#include <UCodeRunTime/ULibrarys/Rendering/RenderRunTime2d.hpp>
#include <UCodeRunTime/ULibrarys/AssetManagement/AssetManager.hpp>

#include "ImGui.hpp"


#include  <UCodeRunTime/ULibrarys/Loger.hpp>

#include "ProjectManagement/ProjectManger.hpp"
#include "ProjectManagement/RunTimeProjectData.hpp"
#include "ProjectManagement/ProjectData.hpp"
#include "WindowPrefData.hpp"
#include "EditorWindows/EditorWindow.hpp"
#include "Typedefs.hpp"
#include "EditorAssetLoader.hpp"
#include "ProjectManagement/ProjectFiles.hpp"
EditorStart

struct UndoData
{
	Ref<Byte> TepData;


	Delegate<void, UndoData&> _UndoCallBack;
	Delegate<void, UndoData&> _RedoCallBack;
	Delegate<void, UndoData&> _UndoRemoved;
};
enum class KeyInputMode
{
	Normal,
	Window,
};

struct BlockCloseData
{
	String Popname;
	String Info;

	std::function<void()> OnSave;
	std::function<void()> OnDontSave;
};

using DontWaitInputKey = int;
using ULangAssemblyID = int;
using BlockCloseKey = int;

class EditorAppCompoent :public UCode::Renderer2d
{
public:
	EditorAppCompoent(UCode::Entity* entity);
	~EditorAppCompoent() override;

	EditorWindow* MakeNewWindow(const EditorWindowData& windata, NewEditorWindowData DataForWindow);


	UCODE_EDITOR_FORCEINLINE RunTimeProjectData* Get_RunTimeProjectData() { return &_RunTimeProjectData; }
	UCODE_EDITOR_FORCEINLINE UCode::Ref<UCode::Gamelibrary> Get_EditorLibraryRef() { return GetGameRunTime()->Get_LibraryRef(); }
	UCODE_EDITOR_FORCEINLINE UCode::Gamelibrary* Get_EditorLibrary() { return GetGameRunTime()->Get_Library_Edit(); }

	template<typename T> T* Get_Window()
	{
		auto Data = T::GetEditorData();

		for (auto& Item : _EditorWindows)
		{
			if (Item->WindowName == Data.GetName())
			{
				return (T*)Item.get();
			}
		}
		NewEditorWindowData _data(this);
		T* win = (T*)MakeNewWindow(Data, _data);
		return win;
	}
	template<typename T> T* Find_Window()
	{
		const auto& Data = T::GetEditorData();
		for (auto& Item : _EditorWindows)
		{
			if (Item->WindowName == Data.GetName())
			{
				return (T*)Item;
			}
		}
		return nullptr;
	}


	UCODE_EDITOR_FORCEINLINE UCode::AssetManager* Get_AssetManager() {
		return UCode::AssetManager::Get(Get_EditorLibrary());
	}
	UCODE_EDITOR_NODISCARD Result<bool, String> OpenProject(const Path& ProjectDataPath);
	void Init(const Path& propath);
	inline static const char* ImGUIDockName = "EditorApp";


	void AddUndo(const UndoData& V)
	{
		_Undos.push_back(V);
	}
	KeyInputMode GetInputMode() const
	{
		return _InputMode;
	}
	void SetToNormal()
	{
		_InputMode = KeyInputMode::Normal;
	}
	static auto GetCurrentEditorAppCompoent()
	{
		return _This;
	}

	ProjectFiles& GetPrjectFiles()
	{
		return _ProjectFiles;
	}

	void OnFilesDropedOnWindow(Vector<StringView> filespaths);

	std::function<void(bool)> SetWaitForInput;
	DontWaitInputKey AddDontWaitForInput();
	void RemoveWaitForInput(DontWaitInputKey key);

	//will change when ULang runtime reloads
	ULangAssemblyID GetULangAssemblyID()
	{
		return _AssemblyKey;
	}

	void UpdateUAssemblyKey()
	{
		_AssemblyKey++;
	}
	void SetProjectOpenError(const String& errorstring)
	{
		ProjectOpenError = errorstring;
	}
	EditorAssetLoader& Get_AssetLoader()
	{
		return *_Loader;
	}

	struct CloseRequestData
	{
		using OnClose = std::function<void()>;

		OnClose onclose;
	};
	bool CloseProjectRequest(CloseRequestData&& data);

	BlockCloseKey AddBlockClose(BlockCloseData&& data)
	{
		auto newkey = GetNextKey();
		List.AddValue(newkey, std::move(data));

		return newkey;
	}
	void RemoveBlockClose(BlockCloseKey key)
	{
		List.erase(key);
	}
private:
	DontWaitInputKey _NextDontWaitKey = {};
	Vector<DontWaitInputKey> _ListDontWaitKeys;
	ULangAssemblyID _AssemblyKey = {};
	//CompoentStuff
	void OnDraw() override;




	void EndDockSpace();
	void ShowEditiorWindows();
	void EndEditor();

	void OnProjectLoadedPreWindows();
	void OnProjectLoaded();
	UCODE_EDITOR_FORCEINLINE void StopEditor() { GetGameRunTime()->StopRunTime(); }

	void ReMoveEditorWindow(EditorWindow* window);
	int GetNewWindowId()
	{
		auto V = NextWindowId;

		NextWindowId++;
		return V;
	}


	void WaitForInput(bool Value)
	{
		SetWaitForInput(Value);
	}


	Vector<Unique_ptr<EditorWindow>> _EditorWindows;
	i8 NextWindowId;

	void ShowMainMenuBar();
	void BeginDockSpace(bool* p_open);
	struct DockSpaceData
	{
		bool opt_fullscreen = true;
		bool opt_padding = false;
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	};
	DockSpaceData _DockSpaceData;
	RunTimeProjectData _RunTimeProjectData;
	float _AutoSaveTimer = 0;
	Vector<UndoData> _Undos;
	Vector<UndoData> _Redos;
	KeyInputMode _InputMode = KeyInputMode::Normal;
	ProjectFiles _ProjectFiles;
	Optional<Vector<String>> _DropedFiles;
	Optional<std::ofstream> ActiveProjectLockFile;

	Optional<String> ProjectOpenError;
	void SaveApp();
	void OnAppEnded();
	void EndProject();
	void LoadWindowsPref();
	void SaveWindowsPref();
	Path GetWindows_prefPath();//sh

	static void OnFileUpdated(void* This, const Path& path, ChangedFileType Type);

	void Undo();
	void Redo();

	inline static EditorAppCompoent* _This = nullptr;

	Optional<CloseRequestData> requestdata;
	
	Vector<BlockCloseData> popupscloserequestlist;
	Unique_ptr<EditorAssetLoader> _Loader;

	BlockCloseKey Next = BlockCloseKey();
	Unordered_map<BlockCloseKey, BlockCloseData> List;
	inline BlockCloseKey GetNextKey()
	{
		auto r = Next;
		Next++;
		return r;
	}

	EditorIndex::ProjectInfo GetEditorProjectInfo();
};

EditorEnd

