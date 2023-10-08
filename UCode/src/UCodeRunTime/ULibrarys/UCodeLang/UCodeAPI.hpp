#pragma once

#include "LangNamespace.hpp"
#include <UCodeLang/UCodeLang.hpp>
#include "UCodeRunTime/CoreRunTime.hpp"
LangStart

#define GetUAPIFuncName(x) Core ##_ULangAPI##x 

#define ToCString(x) #x

#define GetUAPIFuncNameS(x) "UCode.lib/" ToCString(x)

#define GetUCPPName(x) ULangAPI_##x

#define UAPIStatic(Ret,NewFuncName,FuncName,...) \
inline static const char* GetUAPIFuncName(NewFuncName) = #NewFuncName;  \
inline static Ret GetUCPPName(NewFuncName) (CPPParameters parameters) \
{ \
 ##__VA_ARGS__ Pars = parameters.GetParameters<##__VA_ARGS__>(); \
 ##FuncName(Pars); \
} \


#define UFunc_Entity_Create  Entity_Create
#define UFunc_Entity_Destroy Entity_Destroy


#define APILibName "UCodeGameEngine"

class UCodeAPI
{
public:
	typedef UCodeLang::InterpreterCPPinterface& CPPParameters;
	static UCodeLang::RunTimeLib* Get();
private:
	static void MakeNewLib();
	static bool _HasLib;
	static UCodeLang::RunTimeLib _UCodeAPILib;
	inline static String _ULIbString;
};
LangEnd
