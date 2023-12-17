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
		using ComponentAPI__entity_ptr = Entity*(*UCodeLangAPI)(uintptr_t); 
		using ComponentAPI__ientity_ptr = const Entity*(*UCodeLangAPI)(uintptr_t); 
		using Entity__Enable_ptr = void(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__Disable_ptr = void(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__active_ptr = bool*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__iactive_ptr = const bool*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__Destroy_ptr = void(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__name_ptr = const StringView(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__name0_ptr = void(*UCodeLangAPI)(UCode::Entity*,StringView); 
		using Entity__localposition_ptr = Vec3*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__ilocalposition_ptr = const Vec3*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__localposition2d_ptr = Vec2*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__ilocalposition2d_ptr = const Vec2*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__localrotation_ptr = Vec3*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__ilocalrotation_ptr = const Vec3*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__localrotation2d_ptr = Vec2*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__ilocalrotation2d_ptr = const Vec2*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__localscale_ptr = Vec3*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__ilocalscale_ptr = const Vec3*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__localscale2d_ptr = Vec2*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__ilocalscale2d_ptr = const Vec2*(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__worldposition_ptr = Vec3(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__worldposition2d_ptr = Vec2(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__worldrotation_ptr = Vec3(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__worldrotation2d_ptr = Vec2(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__worldscale_ptr = Vec3(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__worldscale2d_ptr = Vec2(*UCodeLangAPI)(UCode::Entity*); 
		using Entity__worldposition0_ptr = void(*UCodeLangAPI)(UCode::Entity*,const Vec3&); 
		using Entity__worldposition1_ptr = void(*UCodeLangAPI)(UCode::Entity*,const Vec2&); 
		using Entity__worldrotation0_ptr = void(*UCodeLangAPI)(UCode::Entity*,const Vec3&); 
		using Entity__worldrotation1_ptr = void(*UCodeLangAPI)(UCode::Entity*,const Vec2&); 
		using Entity__worldscale0_ptr = void(*UCodeLangAPI)(UCode::Entity*,const Vec3&); 
		using Entity__worldscale1_ptr = void(*UCodeLangAPI)(UCode::Entity*,const Vec2&); 
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
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:ComponentAPI:entity",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				uintptr_t Par0 = Input.GetParameter<uintptr_t>();
				
				Entity* Ret =&UCode::ComponentAPI::entity(Par0);
				
				Input.Set_Return<Entity*>(Ret);
			
			},(ComponentAPI__entity_ptr)UCode::ComponentAPI::entity); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:ComponentAPI:ientity",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				uintptr_t Par0 = Input.GetParameter<uintptr_t>();
				
				const Entity* Ret =&UCode::ComponentAPI::ientity(Par0);
				
				Input.Set_Return<const Entity*>(Ret);
			
			},(ComponentAPI__ientity_ptr)UCode::ComponentAPI::ientity); 
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
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:active",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				bool* Ret =&thisPar->active();
				
				Input.Set_Return<bool*>(Ret);
			
			},(Entity__active_ptr)[](UCode::Entity* thisPar) ->bool*
			{
				return &thisPar->active();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:iactive",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const bool* Ret =&thisPar->iactive();
				
				Input.Set_Return<const bool*>(Ret);
			
			},(Entity__iactive_ptr)[](UCode::Entity* thisPar) ->const bool*
			{
				return &thisPar->iactive();
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
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:name",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const StringView Ret =thisPar->name();
				
				Input.Set_Return<const StringView>(Ret);
			
			},(Entity__name_ptr)[](UCode::Entity* thisPar) ->const StringView
			{
				return thisPar->name();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Entity__name0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				StringView Par0 = Input.GetParameter<StringView>();
				
				thisPar->name(Par0);
				
				Input.Set_Return();
			
			},(Entity__name0_ptr)[](UCode::Entity* thisPar,StringView Par0) ->void
			{
				thisPar->name(Par0);
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:localposition",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec3* Ret =&thisPar->localposition();
				
				Input.Set_Return<Vec3*>(Ret);
			
			},(Entity__localposition_ptr)[](UCode::Entity* thisPar) ->Vec3*
			{
				return &thisPar->localposition();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:ilocalposition",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const Vec3* Ret =&thisPar->ilocalposition();
				
				Input.Set_Return<const Vec3*>(Ret);
			
			},(Entity__ilocalposition_ptr)[](UCode::Entity* thisPar) ->const Vec3*
			{
				return &thisPar->ilocalposition();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:localposition2d",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec2* Ret =&thisPar->localposition2d();
				
				Input.Set_Return<Vec2*>(Ret);
			
			},(Entity__localposition2d_ptr)[](UCode::Entity* thisPar) ->Vec2*
			{
				return &thisPar->localposition2d();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:ilocalposition2d",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const Vec2* Ret =&thisPar->ilocalposition2d();
				
				Input.Set_Return<const Vec2*>(Ret);
			
			},(Entity__ilocalposition2d_ptr)[](UCode::Entity* thisPar) ->const Vec2*
			{
				return &thisPar->ilocalposition2d();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:localrotation",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec3* Ret =&thisPar->localrotation();
				
				Input.Set_Return<Vec3*>(Ret);
			
			},(Entity__localrotation_ptr)[](UCode::Entity* thisPar) ->Vec3*
			{
				return &thisPar->localrotation();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:ilocalrotation",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const Vec3* Ret =&thisPar->ilocalrotation();
				
				Input.Set_Return<const Vec3*>(Ret);
			
			},(Entity__ilocalrotation_ptr)[](UCode::Entity* thisPar) ->const Vec3*
			{
				return &thisPar->ilocalrotation();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:localrotation2d",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec2* Ret =&thisPar->localrotation2d();
				
				Input.Set_Return<Vec2*>(Ret);
			
			},(Entity__localrotation2d_ptr)[](UCode::Entity* thisPar) ->Vec2*
			{
				return &thisPar->localrotation2d();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:ilocalrotation2d",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const Vec2* Ret =&thisPar->ilocalrotation2d();
				
				Input.Set_Return<const Vec2*>(Ret);
			
			},(Entity__ilocalrotation2d_ptr)[](UCode::Entity* thisPar) ->const Vec2*
			{
				return &thisPar->ilocalrotation2d();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:localscale",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec3* Ret =&thisPar->localscale();
				
				Input.Set_Return<Vec3*>(Ret);
			
			},(Entity__localscale_ptr)[](UCode::Entity* thisPar) ->Vec3*
			{
				return &thisPar->localscale();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:ilocalscale",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const Vec3* Ret =&thisPar->ilocalscale();
				
				Input.Set_Return<const Vec3*>(Ret);
			
			},(Entity__ilocalscale_ptr)[](UCode::Entity* thisPar) ->const Vec3*
			{
				return &thisPar->ilocalscale();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:localscale2d",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec2* Ret =&thisPar->localscale2d();
				
				Input.Set_Return<Vec2*>(Ret);
			
			},(Entity__localscale2d_ptr)[](UCode::Entity* thisPar) ->Vec2*
			{
				return &thisPar->localscale2d();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:ilocalscale2d",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const Vec2* Ret =&thisPar->ilocalscale2d();
				
				Input.Set_Return<const Vec2*>(Ret);
			
			},(Entity__ilocalscale2d_ptr)[](UCode::Entity* thisPar) ->const Vec2*
			{
				return &thisPar->ilocalscale2d();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:worldposition",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec3 Ret =thisPar->worldposition();
				
				Input.Set_Return<Vec3>(Ret);
			
			},(Entity__worldposition_ptr)[](UCode::Entity* thisPar) ->Vec3
			{
				return thisPar->worldposition();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:worldposition2d",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec2 Ret =thisPar->worldposition2d();
				
				Input.Set_Return<Vec2>(Ret);
			
			},(Entity__worldposition2d_ptr)[](UCode::Entity* thisPar) ->Vec2
			{
				return thisPar->worldposition2d();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:worldrotation",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec3 Ret =thisPar->worldrotation();
				
				Input.Set_Return<Vec3>(Ret);
			
			},(Entity__worldrotation_ptr)[](UCode::Entity* thisPar) ->Vec3
			{
				return thisPar->worldrotation();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:worldrotation2d",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec2 Ret =thisPar->worldrotation2d();
				
				Input.Set_Return<Vec2>(Ret);
			
			},(Entity__worldrotation2d_ptr)[](UCode::Entity* thisPar) ->Vec2
			{
				return thisPar->worldrotation2d();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:worldscale",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec3 Ret =thisPar->worldscale();
				
				Input.Set_Return<Vec3>(Ret);
			
			},(Entity__worldscale_ptr)[](UCode::Entity* thisPar) ->Vec3
			{
				return thisPar->worldscale();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Entity:worldscale2d",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				Vec2 Ret =thisPar->worldscale2d();
				
				Input.Set_Return<Vec2>(Ret);
			
			},(Entity__worldscale2d_ptr)[](UCode::Entity* thisPar) ->Vec2
			{
				return thisPar->worldscale2d();
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Entity__worldposition0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const Vec3* Par0 = Input.GetParameter<const Vec3*>();
				
				thisPar->worldposition(*Par0);
				
				Input.Set_Return();
			
			},(Entity__worldposition0_ptr)[](UCode::Entity* thisPar,const Vec3& Par0) ->void
			{
				thisPar->worldposition(Par0);
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Entity__worldposition1",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const Vec2* Par0 = Input.GetParameter<const Vec2*>();
				
				thisPar->worldposition(*Par0);
				
				Input.Set_Return();
			
			},(Entity__worldposition1_ptr)[](UCode::Entity* thisPar,const Vec2& Par0) ->void
			{
				thisPar->worldposition(Par0);
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Entity__worldrotation0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const Vec3* Par0 = Input.GetParameter<const Vec3*>();
				
				thisPar->worldrotation(*Par0);
				
				Input.Set_Return();
			
			},(Entity__worldrotation0_ptr)[](UCode::Entity* thisPar,const Vec3& Par0) ->void
			{
				thisPar->worldrotation(Par0);
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Entity__worldrotation1",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const Vec2* Par0 = Input.GetParameter<const Vec2*>();
				
				thisPar->worldrotation(*Par0);
				
				Input.Set_Return();
			
			},(Entity__worldrotation1_ptr)[](UCode::Entity* thisPar,const Vec2& Par0) ->void
			{
				thisPar->worldrotation(Par0);
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Entity__worldscale0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const Vec3* Par0 = Input.GetParameter<const Vec3*>();
				
				thisPar->worldscale(*Par0);
				
				Input.Set_Return();
			
			},(Entity__worldscale0_ptr)[](UCode::Entity* thisPar,const Vec3& Par0) ->void
			{
				thisPar->worldscale(Par0);
			}); 
		_UCodeAPILib.Add_CPPCall("UCodeGameEngine:Internal:Entity__worldscale1",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				UCode::Entity* thisPar = Input.GetParameter<UCode::Entity*>();
				
				const Vec2* Par0 = Input.GetParameter<const Vec2*>();
				
				thisPar->worldscale(*Par0);
				
				Input.Set_Return();
			
			},(Entity__worldscale1_ptr)[](UCode::Entity* thisPar,const Vec2& Par0) ->void
			{
				thisPar->worldscale(Par0);
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


