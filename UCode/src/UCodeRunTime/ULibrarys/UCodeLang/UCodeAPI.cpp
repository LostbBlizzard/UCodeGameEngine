#include "UCodeAPI.hpp"
#include "ULangRunTime.hpp"
//include Made By UCodeAutoLink
#include "../../../UCodeRunTime/ULibrarys/InputManger/InputManger.hpp"
#include "../../../UCodeRunTime/ULibrarys/Loger.hpp"
//UCodeAutoLink include End
LangStart
bool UCodeAPI::_HasLib = false;
UCodeLang::RunTimeLib UCodeAPI::_UCodeAPILib = UCodeLang::RunTimeLib();

UCodeLang::RunTimeLib* UCodeAPI::Get()
{
	
	if (!_HasLib)
	{
		MakeNewLib();
	}
	return &_UCodeAPILib;
}
void UCodeAPI::MakeNewLib()
{
	_HasLib = true;
	using namespace UCode;

	UCodeLangAutoLink(_UCodeAPILib, UCode)

	{//Made by UCodeAutoLink 
		using Input__IsKeyDown_ptr = bool(*UCodeLangAPI)(InputKey); 
		using Input__GetMousePosition_ptr = Vec2(*UCodeLangAPI)(); 
		using Debug__Log_ptr = void(*UCodeLangAPI)(const String&); 
		using Debug__Log0_ptr = void(*UCodeLangAPI)(const StringView); 
		using Debug__LogWarning_ptr = void(*UCodeLangAPI)(const String&); 
		using Debug__LogWarning0_ptr = void(*UCodeLangAPI)(const StringView); 
		using Debug__LogError_ptr = void(*UCodeLangAPI)(const String&); 
		using Debug__LogError0_ptr = void(*UCodeLangAPI)(const StringView); 
		using Debug__LogFatalError_ptr = void(*UCodeLangAPI)(const String&); 
		using Debug__LogFatalError0_ptr = void(*UCodeLangAPI)(const StringView); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Input:IsKeyDown",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				InputKey Par0 = Input.GetParameter<InputKey>();
				
				bool Ret =UCode::Input::IsKeyDown(Par0);
				
				Input.Set_Return<bool>(Ret);
			
			},(Input__IsKeyDown_ptr)UCode::Input::IsKeyDown); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Input:GetMousePosition",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				Vec2 Ret =UCode::Input::GetMousePosition();
				
				Input.Set_Return<Vec2>(Ret);
			
			},(Input__GetMousePosition_ptr)UCode::Input::GetMousePosition); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Debug:Log",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const String* Par0 = Input.GetParameter<const String*>();
				
				UCode::Debug::Log(*Par0);
				
				Input.Set_Return();
			
			},(Debug__Log_ptr)UCode::Debug::Log); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Debug__Log0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				UCode::Debug::Log(Par0);
				
				Input.Set_Return();
			
			},(Debug__Log0_ptr)UCode::Debug::Log); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Debug:LogWarning",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const String* Par0 = Input.GetParameter<const String*>();
				
				UCode::Debug::LogWarning(*Par0);
				
				Input.Set_Return();
			
			},(Debug__LogWarning_ptr)UCode::Debug::LogWarning); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Debug__LogWarning0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				UCode::Debug::LogWarning(Par0);
				
				Input.Set_Return();
			
			},(Debug__LogWarning0_ptr)UCode::Debug::LogWarning); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Debug:LogError",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const String* Par0 = Input.GetParameter<const String*>();
				
				UCode::Debug::LogError(*Par0);
				
				Input.Set_Return();
			
			},(Debug__LogError_ptr)UCode::Debug::LogError); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Debug__LogError0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				UCode::Debug::LogError(Par0);
				
				Input.Set_Return();
			
			},(Debug__LogError0_ptr)UCode::Debug::LogError); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Debug:LogFatalError",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const String* Par0 = Input.GetParameter<const String*>();
				
				UCode::Debug::LogFatalError(*Par0);
				
				Input.Set_Return();
			
			},(Debug__LogFatalError_ptr)UCode::Debug::LogFatalError); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Debug__LogFatalError0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				UCode::Debug::LogFatalError(Par0);
				
				Input.Set_Return();
			
			},(Debug__LogFatalError0_ptr)UCode::Debug::LogFatalError); 
	}//Made by UCodeAutoLink End
}
LangEnd


