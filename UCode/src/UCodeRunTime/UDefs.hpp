#pragma once


#define UCodeGameEngineOpenGL 1
#define UCodeGameEngineVulkan 0
#define UCodeGameEngineDirect3D 0
#define UCodeGameEngineMetal 0
#define UCodeGameEngineWebGPU 0


#define UCodeGameEngineDEBUG DEBUG


#define UCodeGameEngineMajorVersion 0
#define UCodeGameEngineMinorVersion 0
#define UCodeGameEnginePatchVersion 1

using VersionNumber_t = int;
#define UCodeGameEngineVersionNumber ((VersionNumber_t)(UCodeGameEnginePatchVersion))


#if defined(_MSC_VER)
#define UCodeGameEngineMSVC 1
#else
#define UCodeGameEngineMSVC 0
#endif

#if defined(__GNUC__)
#define UCodeGameEngineGNUC 1
#else
#define UCodeGameEngineGNUC 0
#endif

#if defined(__clang__)
#define UCodeGameEngineClang 1
#else
#define UCodeGameEngineClang 0
#endif


#if defined(__EMSCRIPTEN__)
#define UCodeGameEngineEmscripten 1
#else
#define UCodeGameEngineEmscripten 0
#endif



#if _WIN64 || _WIN32
#define UCodeGameEnginePlatformWindows 1
#else
#define UCodeGameEnginePlatformWindows 0
#endif // _WIN64 || _WIN32

#if __gnu_linux__ || __linux__
#define UCodeGameEnginePlatformLinux 1
#include <endian.h> //For UCodeLang_CPUBIs_BigEndian
#else
#define UCodeGameEnginePlatformLinux 0
#endif // linux 

#if __APPLE__ && __MACH__
#define UCodeGameEnginePlatformMacOS 1
#else
#define UCodeGameEnginePlatformMacOS 0
#endif // MacOs


#if __APPLE__ 
#include "TargetConditionals.h"

#if TARGET_OS_IPHONE
#define UCodeGameEnginePlatformIPHONE 1
#else 
#define UCodeGameEnginePlatformIPHONE 0
#endif 

#else
#define UCodeGameEnginePlatformIPHONE 0
#endif // MacOs

#if __ANDROID__
#define UCodeLangPlatformANDROID 1
#else
#define UCodeLangPlatformANDROID 0
#endif // ANDROID

#if  defined(__wasm32__) || defined(__wasm64__)
#define UCodeLangPlatformWasm 1
#else
#define UCodeLangPlatformWasm 0
#endif // Wasm 

#if defined(__wasm32__)
#define UCodeLangPlatformWasm32 1
#else
#define UCodeLangPlatformWasm32 0
#endif // wasm32

#if defined(__wasm64__)
#define UCodeGameEnginePlatformWasm64 1
#else
#define UCodeGameEnginePlatformWasm64 0
#endif // wasm64


#if defined(__unix__) || (__APPLE__ && __MACH__)
#define UCodeGameEnginePlatformPosix 1
#else
#define UCodeGameEnginePlatformPosix 0
#endif // MacOs

#ifdef UCodeGameEngineDEBUG
#define UCodeGameEngineForceinlne inline
#else 
#define UCodeGameEngineForceinlne __forceinline 
#endif // DEBUG

#ifdef UCodeGameEngineDEBUG
#define UCodeGameEngineNoDiscard
#else 
#define UCodeGameEngineNoDiscard [[nodiscard]] 
#endif // DEBUG

#define UCodeGameEngineFileSignature "LBGameEngine"


#if UCodeGameEngineDEBUG
#define UCODE_VS_FULLPATH "../"
#define UCode_VS_PROJECTPATH UCODE_VS_FULLPATH "UCode/"
#define UCodeEditor_VS_PROJECTPATH UCODE_VS_FULLPATH "UCodeEditor/"
#endif // DEBUG

#if UCodeGameEngineMSVC
#include <vcruntime_exception.h>
#endif



UCodeGameEngineForceinlne void __DebugBreak()
{
	#if UCodeGameEngineDEBUG

	#if UCodeGameEngineMSVC
	__debugbreak(); 
	#elif UCodeGameEnginePlatformPosix
	raise(SIGTRAP);
	#endif
	
	#endif // DEBUG 
}

//#define UCodeGameEngineAssert(x) if(x){UCODE_ENGINE_LOG("breakpoint Called") __DebugBreak(); }
#define UCodeGameEngineAssert(x) if(x){__DebugBreak(); }
#define UCodeGameEngineThrowException(x) throw std::exception(x);


#if UCodeGameEngineDEBUG
#define UCodeGameEngineUnreachable() UCodeGameEngineThrowException("reached unreachable path")
#else
#if UCodeGameEngineMSVC
#define UCodeGameEngineUnreachable() __assume(0);
#elif UCodeGameEngineGNUC
#define UCodeGameEngineUnreachable() __builtin_unreachable()
#else
#define UCodeLangUnreachable()
#endif

#endif

#define UCODE_ENGINE_CURRENT_FUNCTION __func__

#define UCODE_ENGINE_THROWNOTIMPlEMENTED UCodeGameEngineSTATIC_ASSERT("CURRENT_FUNCTION Is Not implemented");



#if UCodeGameEngineDEBUG
#define UCODE_ENGINE_IMPlEMENTED_LATER UCodeGameEngineThrowException("CURRENT Is Not implemented");
#else
#define UCODE_ENGINE_IMPlEMENTED_LATER UCODE_ENGINE_THROWNOTIMPlEMENTED
#endif // DEBUG