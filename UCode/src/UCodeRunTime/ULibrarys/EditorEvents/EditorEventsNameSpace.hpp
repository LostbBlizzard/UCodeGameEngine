#include "UCodeRunTime/Core/CoreNamespace.hpp"
#include "UCodeRunTime/ULibrarys/Loger.hpp"


CoreStart
using EditorEventID = int;
CoreEnd

#if UCodeGEDebugMode
#define UCodeEditorEventStart CoreStart
#define UCodeEditorEventEnd CoreEnd
#else 
#define UCodeEditorEventStart 
#define UCodeEditorEventEnd  
#endif
