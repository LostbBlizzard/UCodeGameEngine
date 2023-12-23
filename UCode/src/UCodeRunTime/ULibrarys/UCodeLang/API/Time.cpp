#include "Time.hpp"
#include "UCodeRunTime/Core/GameRunTime.hpp"
LangStart

float Time::DeltaTime()
{
	return GameRunTime::Current()->Get_GameTime().UpateDelta;
}

LangEnd


