#pragma once
#include "Editor/EditorNamespace.hpp"
#include <UCodeRunTime/CoreSystems/Threads.hpp>

EditorStart

template<typename T>
using AsynTask_t = UCode::AsynTask_t<T>;

using AsynTask = UCode::AsynTask;

EditorEnd