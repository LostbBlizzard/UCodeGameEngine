#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/IntTypes.hpp>
#include <functional>
CoreStart

template<typename R, typename... Pars>
using Delegate = std::function<R(Pars...)>;

CoreEnd