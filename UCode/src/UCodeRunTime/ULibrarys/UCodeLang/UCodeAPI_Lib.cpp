#include "UCodeAPI_Lib.hpp"
#include "ULangRunTime.hpp"

LangStart
bool UCodeAPI_Lib::_HasLib = false;
UCodeLang::RunTimeLib UCodeAPI_Lib::_UCodeAPILib = UCodeLang::RunTimeLib();

#define AddAPI(x) _UCodeAPILib.Add_CPPCall(GetUAPIFuncNameS(x), &UCodeAPI_Lib:: GetUCPPName(x));

#define LibName  APILibName".lib"

#define TestHelloCPPCall "Hello"
UCodeLang::RunTimeLib* UCodeAPI_Lib::Get()
{
	if (!_HasLib)
	{
		MakeNewLib();
	}
	return &_UCodeAPILib;
}
StringView UCodeAPI_Lib::GetULibString()
{
	if (!_ULIbString.size())
	{
		String& Out = _ULIbString;
		Out += "[DLL]|Hello[];";
	}
	return  _ULIbString;
}
void UCodeAPI_Lib::MakeNewLib()
{
	_HasLib = true;
	

	_UCodeAPILib.Add_CPPCall(TestHelloCPPCall, DLLTest_Hello);
}

void UCodeAPI_Lib::DLLTest_Hello(CPPParameters Pars)
{
	//UCODE_ENGINE_LOG("UCall was Called");
}

LangEnd


