#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/ULibrarys/Loger.hpp>
CoreStart


inline thread_local bool HeadLessMode = false;

inline bool IsHeadlessMode()
{
	return HeadLessMode;
}

//This would be set during Exporting Scenes and RawEntity's
inline void SetEditor_HeadlessMode(bool value)
{
	HeadLessMode = value;
}

CoreEnd