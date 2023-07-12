#pragma once
#include "UCodeRunTime/RunTimeBasicTypes/IntTypes.hpp"
#include "UCodeRunTime/ULibrarys/Loger.hpp"


#ifdef DEBUG
#define UCODE_ENGINE_FORCE_INLINE inline
#else 
#define UCODE_ENGINE_FORCE_INLINE __forceinline 
#endif // DEBUG

#ifdef DEBUG
#define UCODE_ENGINE_NODISCARD
#else 
#define UCODE_ENGINE_NODISCARD [[nodiscard]] 
#endif // DEBUG

#ifdef DEBUG
#define UCODE_ENGINE_NULLSTRING "nullstr"
#else
#define UCODE_ENGINE_NULLSTRING ""
#endif // DEBUG

using VersionNumber_t =unsigned long long;

#define UCODE_ENGINE_FILE_SIGNATURE "UCodeGameEngine_Lost_blizzard"
#define UCODE_ENGINE_VERSION_NUMBER ((VersionNumber_t)0)

#ifdef DEBUG
#define UCODE_VS_FULLPATH "../"
#define UCode_VS_PROJECTPATH UCODE_VS_FULLPATH "UCode/"
#define UCodeEditor_VS_PROJECTPATH UCODE_VS_FULLPATH "UCodeEditor/"
#endif // DEBUG

#include <vcruntime_exception.h>

UCODE_ENGINE_FORCE_INLINE  void __DebugBreak()
{
	#ifdef DEBUG 
	__debugbreak(); 
	#endif // DEBUG 
}

#define UCODE_ENGINE_ASSERT UCODE_ENGINE_LOG("breakpoint Called") __DebugBreak();
#define UCODE_ENGINE_ASSERT_IF(x) if(x){UCODE_ENGINE_ASSERT }
#define UCODE_ENGINE_STATIC_ASSERT(x) static_assert(false, x);
#define UCODE_ENGINE_THROWERROR(x)  UCODE_ENGINE_ERROR(x);  UCODE_ENGINE_ASSERT;
#define UCODE_ENGINE_THROWEXCEPTION(x) UCODE_ENGINE_THROWERROR(x);throw std::exception("Exception: "x);

#define UCODE_ENGINE_BADPATH_ASSERT UCODE_ENGINE_THROWEXCEPTION("reached unreachable path")

#define UCODE_ENGINE_CURRENT_FUNCTION __func__

#define UCODE_ENGINE_THROWNOTIMPlEMENTED UCODE_ENGINE_STATIC_ASSERT(UCODE_ENGINE_CURRENT_FUNCTION  " Is Not implemented");



#ifdef DEBUG
#define UCODE_ENGINE_IMPlEMENTED_LATER UCODE_ENGINE_THROWEXCEPTION("CURRENT Is Not implemented");
#else
#define UCODE_ENGINE_IMPlEMENTED_LATER UCODE_ENGINE_THROWNOTIMPlEMENTED
#endif // DEBUG