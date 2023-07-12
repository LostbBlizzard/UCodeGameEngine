#pragma once

#include "UCodeRunTime/AppData.hpp"


#ifdef UCode_Build_Windows_OS
#include "WindowsOS/WinSeverSocket.hpp"
#endif 

CoreStart
#ifdef UCode_Build_Windows_OS
typedef WinSeverSocket SeverSocket;
#else
static_assert(false, "SeverSocket Has no implementation");
typedef void* SeverSocket;
#endif // win

CoreEnd
