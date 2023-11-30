#include "UCodeAPI.hpp"
#include "ULangRunTime.hpp"
//include Made By UCodeAutoLink
#include "../../../UCodeRunTime/Core/Entity.hpp"
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
		using Entity__Enable_ptr = void(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__Disable_ptr = void(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__Active_ptr = bool*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__Active0_ptr = const bool*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__Destroy_ptr = void(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__Name_ptr = const StringView(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__Name0_ptr = void(*UCodeLangAPI)(UCode::Entity*,StringView); 
		using Entity__LocalPosition_ptr = Vec3*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__LocalPosition0_ptr = const Vec3*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__LocalPosition2D_ptr = Vec2*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__LocalPosition2D0_ptr = const Vec2*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__LocalRotation_ptr = Vec3*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__LocalRotation0_ptr = const Vec3*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__LocalRotation2D_ptr = Vec2*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__LocalRotation2D0_ptr = Vec2*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__LocalScale_ptr = Vec3*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__LocalScale0_ptr = const Vec3*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__LocalScale2D_ptr = Vec2*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__LocalScale2D0_ptr = Vec2*(*UCodeLangAPI)(UCode::Entity*); 
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
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:Enable",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				thisPar->Enable();
				
				Input.Set_Return();
			
			},(Entity__Enable_ptr)[](UCode::Entity* thisPar) ->void
			{
				thisPar->Enable();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:Disable",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				thisPar->Disable();
				
				Input.Set_Return();
			
			},(Entity__Disable_ptr)[](UCode::Entity* thisPar) ->void
			{
				thisPar->Disable();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:Active",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				bool* Ret =&thisPar->Active();
				
				Input.Set_Return<bool*>(Ret);
			
			},(Entity__Active_ptr)[](UCode::Entity* thisPar) ->bool*
			{
				return &thisPar->Active();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Entity__Active0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const bool* Ret =&thisPar->Active();
				
				Input.Set_Return<const bool*>(Ret);
			
			},(Entity__Active0_ptr)[](UCode::Entity* thisPar) ->const bool*
			{
				return &thisPar->Active();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:Destroy",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				thisPar->Destroy();
				
				Input.Set_Return();
			
			},(Entity__Destroy_ptr)[](UCode::Entity* thisPar) ->void
			{
				thisPar->Destroy();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:Name",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const StringView Ret =thisPar->Name();
				
				Input.Set_Return<const StringView>(Ret);
			
			},(Entity__Name_ptr)[](UCode::Entity* thisPar) ->const StringView
			{
				return thisPar->Name();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Entity__Name0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				StringView Par0 = Input.GetParameter<StringView>();
				
				thisPar->Name(Par0);
				
				Input.Set_Return();
			
			},(Entity__Name0_ptr)[](UCode::Entity* thisPar,StringView Par0) ->void
			{
				thisPar->Name(Par0);
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:LocalPosition",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec3* Ret =&thisPar->LocalPosition();
				
				Input.Set_Return<Vec3*>(Ret);
			
			},(Entity__LocalPosition_ptr)[](UCode::Entity* thisPar) ->Vec3*
			{
				return &thisPar->LocalPosition();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Entity__LocalPosition0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const Vec3* Ret =&thisPar->LocalPosition();
				
				Input.Set_Return<const Vec3*>(Ret);
			
			},(Entity__LocalPosition0_ptr)[](UCode::Entity* thisPar) ->const Vec3*
			{
				return &thisPar->LocalPosition();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:LocalPosition2D",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec2* Ret =&thisPar->LocalPosition2D();
				
				Input.Set_Return<Vec2*>(Ret);
			
			},(Entity__LocalPosition2D_ptr)[](UCode::Entity* thisPar) ->Vec2*
			{
				return &thisPar->LocalPosition2D();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Entity__LocalPosition2D0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const Vec2* Ret =&thisPar->LocalPosition2D();
				
				Input.Set_Return<const Vec2*>(Ret);
			
			},(Entity__LocalPosition2D0_ptr)[](UCode::Entity* thisPar) ->const Vec2*
			{
				return &thisPar->LocalPosition2D();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:LocalRotation",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec3* Ret =&thisPar->LocalRotation();
				
				Input.Set_Return<Vec3*>(Ret);
			
			},(Entity__LocalRotation_ptr)[](UCode::Entity* thisPar) ->Vec3*
			{
				return &thisPar->LocalRotation();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Entity__LocalRotation0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const Vec3* Ret =&thisPar->LocalRotation();
				
				Input.Set_Return<const Vec3*>(Ret);
			
			},(Entity__LocalRotation0_ptr)[](UCode::Entity* thisPar) ->const Vec3*
			{
				return &thisPar->LocalRotation();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:LocalRotation2D",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec2* Ret =&thisPar->LocalRotation2D();
				
				Input.Set_Return<Vec2*>(Ret);
			
			},(Entity__LocalRotation2D_ptr)[](UCode::Entity* thisPar) ->Vec2*
			{
				return &thisPar->LocalRotation2D();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Entity__LocalRotation2D0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec2* Ret =&thisPar->LocalRotation2D();
				
				Input.Set_Return<Vec2*>(Ret);
			
			},(Entity__LocalRotation2D0_ptr)[](UCode::Entity* thisPar) ->Vec2*
			{
				return &thisPar->LocalRotation2D();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:LocalScale",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec3* Ret =&thisPar->LocalScale();
				
				Input.Set_Return<Vec3*>(Ret);
			
			},(Entity__LocalScale_ptr)[](UCode::Entity* thisPar) ->Vec3*
			{
				return &thisPar->LocalScale();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Entity__LocalScale0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const Vec3* Ret =&thisPar->LocalScale();
				
				Input.Set_Return<const Vec3*>(Ret);
			
			},(Entity__LocalScale0_ptr)[](UCode::Entity* thisPar) ->const Vec3*
			{
				return &thisPar->LocalScale();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:LocalScale2D",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec2* Ret =&thisPar->LocalScale2D();
				
				Input.Set_Return<Vec2*>(Ret);
			
			},(Entity__LocalScale2D_ptr)[](UCode::Entity* thisPar) ->Vec2*
			{
				return &thisPar->LocalScale2D();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Entity__LocalScale2D0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec2* Ret =&thisPar->LocalScale2D();
				
				Input.Set_Return<Vec2*>(Ret);
			
			},(Entity__LocalScale2D0_ptr)[](UCode::Entity* thisPar) ->Vec2*
			{
				return &thisPar->LocalScale2D();
			}); 
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


