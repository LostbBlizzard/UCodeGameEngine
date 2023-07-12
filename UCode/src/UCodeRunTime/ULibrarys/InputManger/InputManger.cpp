#include "InputManger.hpp"
InputStart

UComponentData InputManger::type_Data = { "InputManger",nullptr};
InputManger::InputManger(Entity* p) : Compoent(p,&type_Data)
{
	constexpr size_t V = (size_t)InputKey::MaxSize;
	_InputBuff = std::make_unique<bool[]>(V);
	for (size_t i = 0; i < V; i++)
	{
		_InputBuff[i] = false;
	}

	_NextCallBackID = 0;
}

InputManger::~InputManger()
{
	GetGameRunTime()->ReMoveStaticComponent((size_t)StaticRuntimeComponentsIndex::InputManger);
}

InputManger* InputManger::GetInput(GameRunTime* e)
{
	const auto index = (size_t)StaticRuntimeComponentsIndex::InputManger;
	auto item = e->Get_StaticComponent(index);
	if (item == nullptr)
	{
		Entity* newe = e->NewEntityOnRunTimeScene();
		auto r = new InputManger(newe);

		newe->MoveCompoent(r);
		e->SetStaticComponent(index, r);
		return r;
	}
	return (InputManger*)item;
}

InputManger* InputManger::FindInput(const GameRunTime* e)
{
	const auto index = (size_t)StaticRuntimeComponentsIndex::InputManger;
	auto item = e->Get_StaticComponent(index);
	return (InputManger*)item;
}
	



float32 InputManger::GetInputKey_Float(InputKey input) const
{
	switch (input)
	{
	case InputKey::MOUSE_SCROLL_UP:return _ScrollPosition.Y;
	case InputKey::MOUSE_SCROLL_Down:return _ScrollPosition.Y;

	default:return 0.0f;
	}
}

InputCallBackKey InputManger::SetCallBack(const InputCallBack& callback, InputKey Inputkey, CallBackType type)
{
	auto R = _NextCallBackID;
	CallBackData data = CallBackData(Inputkey, type, R);
	data.SetUpCall(callback);

	_CallBacks.push_back(data);
	_NextCallBackID++;
	return R;
}
void InputManger::ReMoveCallBack(InputCallBackKey id)
{
	auto& All_Renders = _CallBacks;
	for (auto it = All_Renders.begin(); it != All_Renders.end();)
	{
		auto& Item = *it;
		if (Item.Get_ID() == id)
		{
			All_Renders.erase(it);
			return;
		}
	}
}

void InputManger::Input_API_CallInputCallBack(InputKey Inputkey, CallBackType type)
{
	for (size_t i = 0; i < _CallBacks.size(); i++)
	{
 		auto& item = _CallBacks[i];
 		if (item.Get_Key() == Inputkey && item.Get_Type() == type)
		{
			item.Call();
		}
	}
}

void InputManger::Input_API_Set_Scroll(float32 x, float32 y)
{
	_ScrollPosition = { x,y };

	bool Vx = x != 0;
	bool Vy = y != 0;

	if (Vx)
	{

	}
	if (Vy)
	{
		bool Upbool = x > 0;

		Input_API_SetInput(Upbool, InputKey::MOUSE_SCROLL_UP);
		Input_API_SetInput(!Upbool, InputKey::MOUSE_SCROLL_Down);


		if (Upbool) {
			Input_API_CallInputCallBack(InputKey::MOUSE_SCROLL_UP, CallBackType::Press);
		}
		else
		{
			Input_API_CallInputCallBack(InputKey::MOUSE_SCROLL_Down, CallBackType::Press);
		}
	}
	else
	{
		Input_API_SetInput(false, InputKey::MOUSE_SCROLL_UP);
		Input_API_SetInput(false, InputKey::MOUSE_SCROLL_Down);


		Input_API_CallInputCallBack(InputKey::MOUSE_SCROLL_UP, CallBackType::Release);
		Input_API_CallInputCallBack(InputKey::MOUSE_SCROLL_Down, CallBackType::Release);
	}
}


InputEnd