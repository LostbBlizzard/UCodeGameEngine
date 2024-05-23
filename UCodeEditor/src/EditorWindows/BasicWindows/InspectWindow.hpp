#pragma once
#include <EditorWindows/EditorWindow.hpp>
#include <Typedefs.hpp>


#include <UCodeRunTime/ULibrarys/AssetManagement/AssetManager.hpp>
#include <UCodeRunTime/ULibrarys/AssetManagement/AssetRendering.hpp>
#include <Helper/ImGuIHelper_Asset.hpp>
EditorStart


class InspectWindow :public EditorWindow
{
public:
	struct InspectDrawer
	{
		friend InspectWindow;
	public:
		UCODE_EDITOR_FORCEINLINE void SetPtrNull() { win->_Inspect._Data = nullptr; }
		UCODE_EDITOR_FORCEINLINE AnyManagedPtr GetPtr() { return  win->_Inspect._Data; }

	private:
		InspectDrawer(InspectWindow* w) { win = w; }
		InspectWindow* win;
	};
	typedef void(*DrawData)(InspectDrawer& data);
	typedef void(*OnInspecChange)(void* _OldData);
	struct InspectData
	{
		
		AnyManagedPtr _Data;
		void* _Data2 = nullptr;
		DrawData _Draw;
		OnInspecChange _Change;
		InspectData(AnyManagedPtr Data, DrawData Draw, OnInspecChange Change =nullptr)
			: _Data(Data), _Draw(Draw), _Change(Change)
		{

		}
		InspectData(): _Data(), _Draw(nullptr), _Change(nullptr)
		{

		}
	};
	friend InspectDrawer;

	InspectWindow(const NewEditorWindowData& windowdata);
	~InspectWindow() override;
	void UpdateWindow() override;

	void Inspect(const InspectData& Data) { _Inspect = Data; _Updated = true; }
	static EditorWindowData GetEditorData();

private:
	static EditorWindow* MakeWin(const NewEditorWindowData& windowdata);
	InspectData _Inspect;
	InspectDrawer _InspectDrawer;
	bool _Updated = false;

};
class Inspect2Window :public EditorWindow
{
public:
	struct InspectDrawer
	{
		friend Inspect2Window;
	public:
		UCODE_EDITOR_FORCEINLINE void SetPtrNull() { win->_Inspect._Data = nullptr; }
		UCODE_EDITOR_FORCEINLINE AnyManagedPtr GetPtr() { return  win->_Inspect._Data; }

	private:
		InspectDrawer(Inspect2Window* w) { win = w; }
		Inspect2Window* win;
	};
	typedef void(*DrawData)(InspectDrawer& data);
	typedef void(*OnInspecChange)(void* _OldData);
	struct InspectData
	{
		
		AnyManagedPtr _Data;
		void* _Data2 = nullptr;
		DrawData _Draw;
		OnInspecChange _Change;
		InspectData(AnyManagedPtr Data, DrawData Draw, OnInspecChange Change =nullptr)
			: _Data(Data), _Draw(Draw), _Change(Change)
		{

		}
		InspectData(): _Data(), _Draw(nullptr), _Change(nullptr)
		{

		}
	};
	Inspect2Window(const NewEditorWindowData& windowdata);
	~Inspect2Window() override;
	void UpdateWindow() override;

	void Inspect(const InspectData& Data) { _Inspect = Data; _Updated = true; }
	static EditorWindowData GetEditorData();

private:
	static EditorWindow* MakeWin(const NewEditorWindowData& windowdata);
	InspectData _Inspect;
	InspectDrawer _InspectDrawer;
	bool _Updated = false;

};
EditorEnd 