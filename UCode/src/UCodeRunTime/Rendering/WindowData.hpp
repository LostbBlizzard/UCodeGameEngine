#pragma once
#include "RenderAPINamespace.h"
#include <UCodeRunTime/BasicTypes.hpp>
RenderAPIStart

using OnFileDroped = void(*)(Vector<StringView>);
struct WindowData
{
public:
	i32 width, height;
	String WindowName;
	bool ImGui_Init = true;//Set To false if rendering context already been loaded on current thread
	bool GenNewWindow = true;//If false Input will not work you have to emulate it useing InputManger.
	bool ImguiDrawIsMade = false;//Set to True if DrawUpdate is Called on a Imgui Draw Call.
	void* shared_window =nullptr;
	bool SetUpInputs = true;
	bool UpdateAppIfInputUpdate =false;
	OnFileDroped _filedroped = nullptr;

	WindowData(i32 w, i32 h, String name) :width(w), height(h), WindowName(name)
	{

	}
	WindowData():width(1), height(1),WindowName("Window")
	{

	}
};

RenderAPIEnd