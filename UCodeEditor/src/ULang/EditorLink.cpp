#include "EditorLink.hpp"
//include Made By UCodeAutoLink
#include "API/Exporter.hpp"
#include "API/OpenedProject.hpp"
#include "API/Window.hpp"
//UCodeAutoLink include End
EditorStart
void EditorLink::Link(UCodeLang::RunTimeLib& lib)
{
	UCodeLangAutoLink(lib, ULangAPI)

	{//Made by UCodeAutoLink 
		using AssetPath__ID_ptr = uint64_t(*UCodeLangAPI)(ULangAPI::AssetPath*); 
		using OpenedProject__GetProjectDirPath_ptr = Path(*UCodeLangAPI)(); 
		using WindowDrawAPI__Field_ptr = bool(*UCodeLangAPI)(const StringView,uint8_t&); 
		using WindowDrawAPI__Field0_ptr = bool(*UCodeLangAPI)(const StringView,uint16_t&); 
		using WindowDrawAPI__Field1_ptr = bool(*UCodeLangAPI)(const StringView,uint32_t&); 
		using WindowDrawAPI__Field2_ptr = bool(*UCodeLangAPI)(const StringView,uint64_t&); 
		using WindowDrawAPI__Field3_ptr = bool(*UCodeLangAPI)(const StringView,int8_t&); 
		using WindowDrawAPI__Field4_ptr = bool(*UCodeLangAPI)(const StringView,int16_t&); 
		using WindowDrawAPI__Field5_ptr = bool(*UCodeLangAPI)(const StringView,int32_t&); 
		using WindowDrawAPI__Field6_ptr = bool(*UCodeLangAPI)(const StringView,int64_t&); 
		using WindowDrawAPI__Fieldptr_ptr = bool(*UCodeLangAPI)(const StringView,uintptr_t&); 
		using WindowDrawAPI__Fieldptr0_ptr = bool(*UCodeLangAPI)(const StringView,intptr_t&); 
		using WindowDrawAPI__Field7_ptr = bool(*UCodeLangAPI)(const StringView,bool&); 
		using WindowDrawAPI__Field8_ptr = bool(*UCodeLangAPI)(const StringView,float&); 
		using WindowDrawAPI__Field9_ptr = bool(*UCodeLangAPI)(const StringView,double&); 
		using WindowDrawAPI__Field10_ptr = bool(*UCodeLangAPI)(const StringView,const uint8_t&); 
		using WindowDrawAPI__Field11_ptr = bool(*UCodeLangAPI)(const StringView,const uint16_t&); 
		using WindowDrawAPI__Field12_ptr = bool(*UCodeLangAPI)(const StringView,const uint32_t&); 
		using WindowDrawAPI__Field13_ptr = bool(*UCodeLangAPI)(const StringView,const uint64_t&); 
		using WindowDrawAPI__Field14_ptr = bool(*UCodeLangAPI)(const StringView,const int8_t&); 
		using WindowDrawAPI__Field15_ptr = bool(*UCodeLangAPI)(const StringView,const int16_t&); 
		using WindowDrawAPI__Field16_ptr = bool(*UCodeLangAPI)(const StringView,const int32_t&); 
		using WindowDrawAPI__Field17_ptr = bool(*UCodeLangAPI)(const StringView,const int64_t&); 
		using WindowDrawAPI__Fieldptr1_ptr = bool(*UCodeLangAPI)(const StringView,const uintptr_t&); 
		using WindowDrawAPI__Fieldptr2_ptr = bool(*UCodeLangAPI)(const StringView,const intptr_t&); 
		using WindowDrawAPI__Field18_ptr = bool(*UCodeLangAPI)(const StringView,const bool&); 
		using WindowDrawAPI__Field19_ptr = bool(*UCodeLangAPI)(const StringView,const float&); 
		using WindowDrawAPI__Field20_ptr = bool(*UCodeLangAPI)(const StringView,const double&); 
		using WindowDrawAPI__BeginReadOnly_ptr = void(*UCodeLangAPI)(); 
		using WindowDrawAPI__EndReadOnly_ptr = void(*UCodeLangAPI)(); 
		lib.Add_CPPCall("UCodeGameEngine:AssetPath:ID",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ULangAPI::AssetPath* thisPar = Input.GetParameter<ULangAPI::AssetPath*>();
				
				uint64_t Ret =thisPar->ID();
				
				Input.Set_Return<uint64_t>(Ret);
			
			},(AssetPath__ID_ptr)[](ULangAPI::AssetPath* thisPar) ->uint64_t
			{
				return thisPar->ID();
			}); 
		lib.Add_CPPCall("UCodeGameEngine:OpenedProject:GetProjectDirPath",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				Path Ret =ULangAPI::OpenedProject::GetProjectDirPath();
				
				Input.Set_Return<Path>(Ret);
			
			},(OpenedProject__GetProjectDirPath_ptr)ULangAPI::OpenedProject::GetProjectDirPath); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:WindowDrawAPI:Field",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				uint8_t* Par1 = Input.GetParameter<uint8_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				uint16_t* Par1 = Input.GetParameter<uint16_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field0_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field1",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				uint32_t* Par1 = Input.GetParameter<uint32_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field1_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field2",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				uint64_t* Par1 = Input.GetParameter<uint64_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field2_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field3",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				int8_t* Par1 = Input.GetParameter<int8_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field3_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field4",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				int16_t* Par1 = Input.GetParameter<int16_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field4_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field5",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				int32_t* Par1 = Input.GetParameter<int32_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field5_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field6",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				int64_t* Par1 = Input.GetParameter<int64_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field6_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:WindowDrawAPI:Fieldptr",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				uintptr_t* Par1 = Input.GetParameter<uintptr_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Fieldptr(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Fieldptr_ptr)ULangAPI::WindowDrawAPI::Fieldptr); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Fieldptr0",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				intptr_t* Par1 = Input.GetParameter<intptr_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Fieldptr(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Fieldptr0_ptr)ULangAPI::WindowDrawAPI::Fieldptr); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field7",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				bool* Par1 = Input.GetParameter<bool*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field7_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field8",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				float* Par1 = Input.GetParameter<float*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field8_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field9",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				double* Par1 = Input.GetParameter<double*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field9_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field10",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				const uint8_t* Par1 = Input.GetParameter<const uint8_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field10_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field11",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				const uint16_t* Par1 = Input.GetParameter<const uint16_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field11_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field12",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				const uint32_t* Par1 = Input.GetParameter<const uint32_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field12_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field13",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				const uint64_t* Par1 = Input.GetParameter<const uint64_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field13_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field14",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				const int8_t* Par1 = Input.GetParameter<const int8_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field14_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field15",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				const int16_t* Par1 = Input.GetParameter<const int16_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field15_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field16",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				const int32_t* Par1 = Input.GetParameter<const int32_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field16_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field17",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				const int64_t* Par1 = Input.GetParameter<const int64_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field17_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Fieldptr1",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				const uintptr_t* Par1 = Input.GetParameter<const uintptr_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Fieldptr(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Fieldptr1_ptr)ULangAPI::WindowDrawAPI::Fieldptr); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Fieldptr2",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				const intptr_t* Par1 = Input.GetParameter<const intptr_t*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Fieldptr(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Fieldptr2_ptr)ULangAPI::WindowDrawAPI::Fieldptr); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field18",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				const bool* Par1 = Input.GetParameter<const bool*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field18_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field19",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				const float* Par1 = Input.GetParameter<const float*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field19_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:Internal:WindowDrawAPI__Field20",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				const StringView Par0 = Input.GetParameter<const StringView>();
				
				const double* Par1 = Input.GetParameter<const double*>();
				
				bool Ret =ULangAPI::WindowDrawAPI::Field(Par0,*Par1);
				
				Input.Set_Return<bool>(Ret);
			
			},(WindowDrawAPI__Field20_ptr)ULangAPI::WindowDrawAPI::Field); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:WindowDrawAPI:BeginReadOnly",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ULangAPI::WindowDrawAPI::BeginReadOnly();
				
				Input.Set_Return();
			
			},(WindowDrawAPI__BeginReadOnly_ptr)ULangAPI::WindowDrawAPI::BeginReadOnly); 
		lib.Add_CPPCall("UCodeGameEngineEditorInternal:WindowDrawAPI:EndReadOnly",[](UCodeLang::InterpreterCPPinterface& Input) -> void
			{
			
				ULangAPI::WindowDrawAPI::EndReadOnly();
				
				Input.Set_Return();
			
			},(WindowDrawAPI__EndReadOnly_ptr)ULangAPI::WindowDrawAPI::EndReadOnly); 
	}//Made by UCodeAutoLink End
}
EditorEnd
