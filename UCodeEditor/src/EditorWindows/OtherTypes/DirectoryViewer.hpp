#pragma once

#include <Editor/EditorNamespace.hpp>
#include <Typedefs.hpp>
#include <filesystem>
EditorStart
class DirectoryViewer
{
public:
	using OpenPathCallBack = void (*)(DirectoryViewer& From, void* Ptr,const Path& Path);
	DirectoryViewer();
	~DirectoryViewer();

	void Draw();


	UCODE_EDITOR_FORCEINLINE void SetPathSelected(const String& Value) { PathSelected = Value; }
	UCODE_EDITOR_FORCEINLINE void SetPath(const Path& Value) { LookingAtPath = Value; }
	UCODE_EDITOR_FORCEINLINE void SetCallBackPtr(void* Ptr) { CallBackPtr = Ptr; }
	UCODE_EDITOR_FORCEINLINE void AddExt(const String& ExtwithDot) { OnlyExt.push_back(ExtwithDot); }
	UCODE_EDITOR_FORCEINLINE void SetOpenPathCallback(OpenPathCallBack Ptr) { OpenPathCallback = Ptr; }
	UCODE_EDITOR_FORCEINLINE void SetFindString(const String Value) { FindString = Value; }
	UCODE_EDITOR_FORCEINLINE void SetShowFindFiles(bool V) { _ShowFindFiles = V; }
private:
	Path LookingAtPath;
	void* CallBackPtr = nullptr;
	OpenPathCallBack OpenPathCallback = nullptr;
	Vector<String> OnlyExt;
	String FindString;
	String PathSelected;
	bool _ShowFindFiles =true;
	void DrawDir(const Path& Dir);
};
EditorEnd
