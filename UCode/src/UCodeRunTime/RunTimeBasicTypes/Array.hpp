#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <array>
CoreStart
template<typename T,size_t Size>
using Array = std::array<T, Size>;
CoreEnd