#include "App.hpp"
CoreStart
UApp::UApp() :_run(nullptr), _render(nullptr)
{

}
void UApp::Init(RenderAPI::WindowData  windowdata, const  GameFilesData& dats)
{
	_run =std::make_unique<GameRunTime>();
	_run->Init();

	auto GameFiles = GameFiles::Init(_run->Get_Library_Edit(),dats);//This Can be Moved 

	_render = std::make_unique<RenderAPI::Render>();
	_render->PreInit();
	_render->Init(windowdata,_run.get());
}

void UApp::Run()
{
	while (_run->Get_IsGameRuning())
	{
		_run->GameUpdate();
		_render->UpdateFrame();
	}
	_run->EndRunTime();
	End();
}
void UApp::End()
{

}

USeverApp::USeverApp() :_run(nullptr)
{
}
void USeverApp::Init()
{
	_run = std::make_unique<GameRunTime>();
	_run->Init();
}
void USeverApp::Run()
{
	while (_run->Get_IsGameRuning())
	{
		_run->GameUpdate();
	}
	End();
}
void USeverApp::End()
{
	
}
CoreEnd


