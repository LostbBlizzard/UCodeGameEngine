#pragma once

#define UCodeGEOpenGL 1
#define UCodeGEVulkan 0
#define UCodeGEDirect3D 0
#define UCodeGEMetal 0
#define UCodeGEWebGPU 0

#define UCodeGEDebug DEBUG

#define UCodeGEMajor 0
#define UCodeGEMinor 0
#define UCodeGEPatch 1

using VersionNumber_t = int;
#define UCodeGEVersion ((VersionNumber_t)((UCodeGEMajor << 16) | (UCodeGEMinor << 8) | UCodeGEPatch))

#if defined(_MSC_VER)
#define UCodeGEMSVC 1
#else
#define UCodeGEMSVC 0
#endif

#if defined(__GNUC__)
#define UCodeGEGNUC 1
#else
#define UCodeGEGNUC 0
#endif

#if defined(__clang__)
#define UCodeGEClang 1
#else
#define UCodeGEClang 0
#endif

#if defined(__EMSCRIPTEN__)
#define UCodeGEEmscripten 1
#else
#define UCodeGEEmscripten 0
#endif

#if _WIN64 || _WIN32
#define UCodeGEWindows 1
#else
#define UCodeGEWindows 0
#endif // _WIN64 || _WIN32

#if __gnu_linux__ || __linux__
#define UCodeGELinux 1
#include <endian.h> //For UCodeLang_CPUBIs_BigEndian
#else
#define UCodeGELinux 0
#endif // linux

#if __APPLE__ && __MACH__
#define UCodeGE 1
#else
#define UCodeGEMacOS 0
#endif // MacOs

#if __APPLE__
#include "TargetConditionals.h"

#if TARGET_OS_IPHONE
#define UCodeGEIphone 1
#else
#define UCodeGEIphone 0
#endif

#else
#define UCodeGEIphone 0
#endif // MacOs

#if __ANDROID__
#define UCodeGEAndroid 1
#else
#define UCodeGEAndroid 0
#endif // ANDROID

#if defined(__wasm32__) || defined(__wasm64__)
#define UCodeGEWasm 1
#else
#define UCodeGEWasm 0
#endif // Wasm

#if defined(__wasm32__)
#define UCodeGEWasm32 1
#else
#define UCodeGEWasm32 0
#endif // wasm32

#if defined(__wasm64__)
#define UCodeGEWasm64 1
#else
#define UCodeGEWasm64 0
#endif // wasm64

#if defined(__unix__) || (__APPLE__ && __MACH__)
#define UCodeGEPosix 1
#else
#define UCodeGEPosix 0
#endif // MacOs

#ifdef UCodeGEDebug
#define UCodeGEForceinlne inline
#else
#define UCodeGEForceinlne __forceinline
#endif // DEBUG

#ifdef UCodeGEDebug
#define UCodeGameEngineNoDiscard
#else
#define UCodeGameEngineNoDiscard [[nodiscard]]
#endif // DEBUG

#define UCodeGESignature "LBGameEngine"

#if UCodeGEDebug
#define UCODE_VS_FULLPATH "../"
#define UCode_VS_PROJECTPATH UCODE_VS_FULLPATH "UCode/"
#define UCodeEditor_VS_PROJECTPATH UCODE_VS_FULLPATH "UCodeEditor/"
#endif // DEBUG

#if UCodeGEMSVC
#include <vcruntime_exception.h>
#endif

#if UCodeGEPosix
#include <csignal>
#endif

UCodeGEForceinlne void __DebugBreak()
{
#if UCodeGEDebug

#if UCodeGEMSVC
	__debugbreak();
#elif UCodeGEPosix
	raise(SIGTRAP);
#endif

#endif // DEBUG
}

#if UCodeGEDebug
#define UCodeGEAssert(x) \
	if (!x)              \
	{                    \
		__DebugBreak();  \
	}
#else
#define UCodeGEAssert(x)
#endif

#if UCodeGEPosix
#define UCodeGEThrow(x) throw std::runtime_error(x);
#else
#define UCodeGEThrow(x) throw std::exception(x);
#endif

#if UCodeGEDebug
#define UCodeGEUnreachable() UCodeGEThrow("reached unreachable path")
#else
#if UCodeGEMSVC
#define UCodeGEUnreachable() __assume(0);
#elif UCodeGEGNUC
#define UCodeGEUnreachable() __builtin_unreachable()
#else
#define UCodeGEUnreachable()
#endif

#endif

#define UCODE_ENGINE_CURRENT_FUNCTION __func__

#define UCodeGEToDo UCodeGameEngineSTATIC_ASSERT("CURRENT_FUNCTION Is Not implemented");
