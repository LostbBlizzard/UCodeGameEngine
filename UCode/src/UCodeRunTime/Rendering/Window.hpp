#pragma once
#include "RenderAPINamespace.h"
#include "RenderAPI.hpp"
#include <UCodeRunTime/RunTimeBasicTypes/String.hpp>
RenderAPIStart

struct Window
{
public:
	Window()
	{

	}
	~Window()
	{

	}
	SInt32 _width = 1, _height =1;
	String Name ="Window";
	void Init();
		
private:
	enum class State :unsigned char
	{
		 Null,
		 Init,
	};
	void* handle =nullptr;
	State state = State::Null;
};

RenderAPIEnd