#pragma once
#include "Imgui/imgui.h"
#include "RenderAPINamespace.h"
#include "UCodeRunTime/ULibrarys/InputManger/InputManger.hpp"
RenderAPIStart
InputKey ImguiKeyToUCodeKey(ImGuiKey key);
String UCodeKeyToString(InputKey key);
InputKey ToStringToKey(StringView str);
RenderAPIEnd