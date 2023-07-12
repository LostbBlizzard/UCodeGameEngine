#pragma once

#include <UCodeRunTime/UDefs.hpp>
#include <UCodeRunTime/Core/CoreNamespace.hpp>

#ifdef UCode_Build_Windows_OS
#include "WindowsOS/WinClientSocket.hpp"
#endif // win

CoreStart

#ifdef UCode_Build_Windows_OS
typedef WinCilentSocket ClientSocket;
#else
static_assert(false, "ClientSocket Has no implementation");
typedef void* ClientSocket;
#endif // win

CoreEnd
