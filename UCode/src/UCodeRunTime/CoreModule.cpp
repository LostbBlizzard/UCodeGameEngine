#include "CoreModule.hpp"


#include "ULibrarys/Rendering/SpriteRenderer2d.hpp"
#include "ULibrarys/Rendering/Camera2d.hpp"
#include "ULibrarys/UCodeLang/ULangRunTime.hpp"
CoreStart
CoreModule::CoreModule()
{
	_Compoents[0] = UModuleComponentData(&SpriteRenderer::type_Data);
	_Compoents[1] = UModuleComponentData(&Camera2d::type_Data);
	_Compoents[2] = UModuleComponentData(&ULangScript::type_Data);
}
CoreModule::~CoreModule()
{

}
CoreEnd