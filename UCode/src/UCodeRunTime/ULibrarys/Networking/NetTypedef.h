#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/String.hpp>
CoreStart


using Ip_t = String;
using Port_t = int;

constexpr Port_t default_port = 80;
inline static const char* default_address = "127.0.0.1";

CoreEnd