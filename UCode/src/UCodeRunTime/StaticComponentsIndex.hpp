#pragma once
#include "UCodeRunTime/Core/CoreNamespace.hpp"

CoreStart
typedef size_t StaticBooksIndex_t;
typedef size_t StaticRunTimeComponentsIndex_t;
typedef size_t StaticSceneComponentsIndex_t;

enum class StaticBooksIndex : StaticBooksIndex_t
{
	GameFiles,
	Dictionary,
	BookOfThreads,
	AssetManager,
	ULangRunTime,
	BookOfDebuging,
	AudioSystem,
	MaxValue,
};
enum class StaticRuntimeComponentsIndex : StaticRunTimeComponentsIndex_t
{
	PhysicsRunTime = 0,
	InputManger,
	RenderRunTime,
	Camera2d,
	StaticComManger,
	UIRunTIme,
	MaxValue,
};

enum class StaticSceneComponentsIndex : StaticSceneComponentsIndex_t
{
	SceneName,
	MaxValue,
};
CoreEnd
