#include "API.hpp"
#include "UCodeRunTime/ULibrarys/Loger.hpp"
#include "UCodeRunTime/ULibrarys/InputManger/InputManger.hpp"
UCodeAPIStart

GameRunTime* GetRunTime()
{
	return nullptr;
}

UCode::LogType To(LogType Value)
{
	return (UCode::LogType)Value;
}
UCode::InputKey To(InputKey Value)
{
	return (UCode::InputKey)Value;
}
UCode::InputKey To(MouseButtion Value)
{
	return (UCode::InputKey)Value;
}


void Debug::LogEx(const StringView Msg, LogType logType)
{
	String NewMSG =String(Msg);

	//TODO add stack frames to MSG. 

	UCode::Loger::Log(NewMSG,To(logType));
	return UCodeLangExport void();
}
bool Input::IsKeyDown(InputKey Key)
{
	return InputManger::GetInput(GetRunTime())->GetInputKey(To(Key));
}
bool Input::IsMouseKeyDown(MouseButtion Key)
{
	return InputManger::GetInput(GetRunTime())->GetInputKey(To(Key));
}
Vec2 Input::GetMousePosition()
{
	return InputManger::GetInput(GetRunTime())->Get_CursorPosition();
}
UCodeAPIEnd


