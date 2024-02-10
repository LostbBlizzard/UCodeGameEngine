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
	}//Made by UCodeAutoLink End
}
EditorEnd
